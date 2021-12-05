#include <math.h>
#include "mpu6050/mpu6050.h"

static MPU6050 mpu;

#ifdef __cplusplus
extern "C" {
#endif

bool imu_init() {
    return mpu.begin(MPU6050_SCALE_250DPS, MPU6050_RANGE_2G);
}

void imu_release() {
    mpu.end();
}

void imu_calibrate() {
    mpu.calibrateGyro();
}

Vector imu_get_acc() {
    return mpu.readScaledAccel();
}

Vector imu_get_gyro() {
    return mpu.readNormalizeGyro();
}

// Return Euler angles
Vector imu_get_attitude() {
    Vector att;
    Vector acc = mpu.readNormalizeAccel();

    int pitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
    int roll = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;

    att.XAxis = roll;
    att.YAxis = pitch;

    return att;
}

float imu_get_temp() {
    return mpu.readTemperature();
}

#ifdef __cplusplus
}
#endif
