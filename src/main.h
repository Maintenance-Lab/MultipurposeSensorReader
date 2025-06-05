#pragma once
#include <cstdint>
#include "utils/dataLogger.h"

// Change this value to ajust how often the data logger flushes its data to the SD card.
constexpr uint32_t FLUSH_INTERVAL_MS = 1000;

// Define new sensor types here, and in the SensorFactory.
enum class SensorType : uint8_t {
    IMUInternal = 0,
    IMUExternal,
    Ultrasonic,
    Motion,
    ENV3,
    ENV4,
    Count
};

class Sensor;

struct MeasurementConfig {
    uint32_t m_sampleRateHz = 500;          // This is the frequency at which the sensor should be sampled, in Hz. This does not guarantee that the sensor will be sampled at this frequency, but it is a target.
};

struct MeasurementData {
    uint32_t m_samplesThisSecond = 0;       // This is the number of samples taken in the current second.
    uint32_t m_elapsedSeconds = 0;          // This is the number of seconds that have passed since the measurement started.
    uint32_t m_totalSamples = 0;            // This is the iterator for the current measurement, it is incremented every time a new measurement is taken.

    void reset() { m_samplesThisSecond = 0; m_elapsedSeconds = 0; m_totalSamples = 0; }
};

struct MeasurementSession {
    uint32_t       m_index          = 0;
    uint64_t       m_startMS        = 0;
    uint64_t       m_lastUpdateMS   = 0;
    bool           m_isRunning      = false;
    bool           m_needsRender    = true;

    MeasurementData   m_data{};
    MeasurementConfig m_config{};
    DataLogger        m_logger{ FLUSH_INTERVAL_MS };
};

struct AppState {
    bool              m_sdCardPresent   = false;
    SensorType        m_currentSensor   = SensorType::IMUInternal;
    std::unique_ptr<Sensor> m_activeSensor;
    MeasurementSession m_session;
};