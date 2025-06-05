#include "sensorFactory.h"
#include "env3.h"
#include "env4.h"
#include "imu_ext.h"
#include "imu_int.h"
#include "motion.h"
#include "ultrasonic.h"

std::unique_ptr<Sensor> SensorFactory::Create(SensorType type) {
    switch (type) {
    case SensorType::IMUInternal:
        return std::unique_ptr<Sensor>(new IMUInternalWrapper());
    case SensorType::IMUExternal:
        return std::unique_ptr<Sensor>(new IMUExternalWrapper());
    case SensorType::Ultrasonic:
        return std::unique_ptr<Sensor>(new UltrasonicWrapper());
    case SensorType::Motion:
        return std::unique_ptr<Sensor>(new MotionWrapper());
    case SensorType::ENV3:
        return std::unique_ptr<Sensor>(new ENV3Wrapper());
    case SensorType::ENV4:
        return std::unique_ptr<Sensor>(new ENV4Wrapper());
    default:
        return nullptr;
    }
}