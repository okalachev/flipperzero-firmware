#include <furi-hal.h>
#include <furi.h>

#include "mpu6050.h"

static uint8_t address;

bool writeRegister8(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    return furi_hal_i2c_tx(&furi_hal_i2c_handle_external, address, buffer, 2, 999);
}

uint8_t fastRegister8(uint8_t reg)
{
    uint8_t value;

    bool success = furi_hal_i2c_trx(&furi_hal_i2c_handle_external,
        address,
        &reg, 1,
        &value, 1,
        999);

    printf("reading success: %d\r\n", success);

    return value;
}

bool init_mpu6050() {
    furi_hal_i2c_init(); 

    address = MPU6050_ADDRESS << 1;

    furi_hal_i2c_acquire(&furi_hal_i2c_handle_external);

    writeRegister8(MPU6050_REG_PWR_MGMT_1, 0);

    // Check MPU6050 Who Am I Register
    uint8_t whoami = fastRegister8(MPU6050_REG_WHO_AM_I);
    if (whoami != 0x68)
    {
        printf("whoami: %d\r\n", whoami);
        return false;
    }

    return true;
}

void release_mpu6050() {
    furi_hal_i2c_release(&furi_hal_i2c_handle_external);
}
