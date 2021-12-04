#include "mpu6050/MPU6050.h"

static MPU6050 mpu;

#ifdef __cplusplus
extern "C" {
#endif

void imu_init() {
    mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
}

void imu_release() {
    mpu.end();
}

Vector imu_get_acc() {
    return mpu.readScaledAccel();
}

Vector imu_get_gyro() {
    return mpu.readNormalizeGyro();
}

#ifdef __cplusplus
}
#endif
