#pragma once
#include "sensor.h"
#include <memory>

struct SensorFactory {
    static std::unique_ptr<Sensor> Create(SensorType type);
};