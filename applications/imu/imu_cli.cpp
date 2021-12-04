#include <furi-hal.h>
#include <furi.h>

#include <lib/toolbox/args.h>
#include <cli/cli.h>

#include "mpu6050/MPU6050.h"

MPU6050 mpu;

static void imu_cli(Cli* cli, string_t args, void* context) {
    printf("Hello, IMU!\r\n");

    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        printf("IMU NOT OK");
        return;
    }
    printf("IMU OK\r\n");

    auto acc = mpu.readScaledAccel();
    printf("acc x: %.6f\r\n", acc.XAxis);
    printf("acc y: %.6f\r\n", acc.YAxis);
    printf("acc z: %.6f\r\n", acc.ZAxis);

    mpu.end();
}

extern "C" void imu_cli_init() {
    Cli* cli = (Cli*)furi_record_open("cli");
    cli_add_command(cli, "imu", CliCommandFlagDefault, imu_cli, NULL);
    furi_record_close("cli");
}
