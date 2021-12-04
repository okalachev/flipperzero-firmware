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

void imu_init();

void imu_release();

struct Vector imu_get_acc();

struct Vector imu_get_gyro();

struct Vector imu_get_attitude();

#ifdef __cplusplus
}
#endif
