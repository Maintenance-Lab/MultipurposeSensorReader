#pragma once
#include "../main.h"
#include "../utils/dataLogger.h"
#include <M5Core2.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NUM_SENSORS 8

enum class sensingType : int {
    noSensor = 0,
    accelerometer = 1,
    gyro,
    temperature,
    humidity,
    pressure,
    altitude,
    motion,
    distance,
};

// Accelerometer is shortened is it would not fit in the menu
static char sensorNames[][20] = {"Acceler..", "Gyroscope", "Temperature", "Humidity", "Pressure", "Altitude", "Motion", "Distance"};
static char sensorUnits[][20] = {"accX,accY,accZ", "gyroX,gyroY,gyroZ", "temp", "humidity", "pressure", "altitude", "motion", "distance"};

struct sensorConfig {
    sensingType type;
    bool isActive;
};

class Sensor {
  public:
    String getColumnNames(); // Returns the column names for the sensor data
    virtual String getName() = 0;
    virtual void begin(MeasurementConfig& config) = 0;
    virtual void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) = 0;
    const char* getSensorList(int index) const;
    void toggleSensorState(int index);
    int getNumberOfSensors() const;
    bool isSensorActive(int index) const;

    sensorConfig* getSensorConfig() { return sensorList; }

  protected:
    // list of sensor to render for toggling readout
    int numberOfSensors = 0;
    sensorConfig sensorList[MAX_NUM_SENSORS] = {{sensingType::noSensor, false}};
    bool beginCalled = false;
};