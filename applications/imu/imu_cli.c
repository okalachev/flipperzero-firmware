#include <furi-hal.h>
#include <furi.h>

#include <lib/toolbox/args.h>
#include <cli/cli.h>

#include "mpu6050.h"

void imu_cli(Cli* cli, string_t args, void* context) {
    printf("Hello, IMU!\r\n");
    if(init_mpu6050()) {
        printf("IMU OK\r\n");
    } else {
        printf("IMU NOT OK\r\n");
    }
    release_mpu6050();
}

void imu_cli_init() {
    Cli* cli = furi_record_open("cli");
    cli_add_command(cli, "imu", CliCommandFlagDefault, imu_cli, NULL);
    furi_record_close("cli");
}
