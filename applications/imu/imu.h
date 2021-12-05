#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector
{
    float XAxis;
    float YAxis;
    float ZAxis;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool imu_init();

void imu_release();

void imu_calibrate();

struct Vector imu_get_acc();

struct Vector imu_get_gyro();

struct Vector imu_get_attitude();

float imu_get_temp();

#ifdef __cplusplus
}
#endif
