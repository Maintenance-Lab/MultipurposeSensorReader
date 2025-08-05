#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "main.h"
#include "AXP192.h"
#include "UIRenderer.h"
#include "esp32_digital_led_lib.h"
#include "logging.h"
#include "sensors/allSensorHeaders.h"
#include "utils/sdWrapper.h"
#include <M5Core2.h>

constexpr uint32_t DISPLAY_UPDATE_INTERVAL = 1000;

static AppState s_app;
static int idx = 0;
static const int rates[] = {1, 10, 25, 50, 100, 200, 250, 500};

void initializeDisplay() {
    M5.Lcd.setTextSize(2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Axp.SetLcdVoltage(2700);
}

void startMeasurement() {
    auto& sess = s_app.m_session;
    sess.m_startMS = millis();
    sess.m_data.reset();
    sess.m_logger.open(s_app.m_activeSensor->getColumnNames().c_str());
    s_app.m_activeSensor->begin(sess.m_config);
}

void toggleRunState() {
    auto& sess = s_app.m_session;
    if (sess.m_uiState != UIState::Measuring) {
        sess.m_uiState = UIState::Measuring;
    } else {
        sess.m_uiState = UIState::StartScreen;
    }
    sess.m_index = SDWrapper::countFiles();

    if (sess.m_uiState == UIState::Measuring) {
        startMeasurement();
    } else {
        sess.m_data.reset();
        sess.m_logger.close();
        sess.m_needsRender = true;
        sess.m_uiState = UIState::StartScreen;
    }
}

void detectSensor() {
    int i2cAddresses[2] = {0};
    auto& currentSensor = s_app.m_currentSensor;

    Wire.begin(32, 33);
    // find all i2c devices
    for (int i = 0; i < 127; i++) {
        Wire.beginTransmission(i);
        if (!Wire.endTransmission()) {
            if (i2cAddresses[0]) {
                i2cAddresses[1] = i;
            } else {
                i2cAddresses[0] = i;
            }
        }
    }

    // match the devices
    if (i2cAddresses[0] == I2C_MPU6886_DEFAULT_ADDRESS) {
        if (currentSensor != SensorType::IMUExternal) {
            currentSensor = SensorType::IMUExternal;
            s_app.m_activeSensor = SensorFactory::Create(currentSensor);
        }
    } else if (i2cAddresses[0] == SHT3X_I2C_ADDR && i2cAddresses[1] == QMP6988_SLAVE_ADDRESS_L) {
        if (currentSensor != SensorType::ENV3) {
            currentSensor = SensorType::ENV3;
            s_app.m_activeSensor = SensorFactory::Create(currentSensor);
        }
    } else if (i2cAddresses[0] == SHT4x_DEFAULT_ADDR && i2cAddresses[1] == BMP280_I2C_ADDR) {
        if (currentSensor != SensorType::ENV4) {
            currentSensor = SensorType::ENV4;
            s_app.m_activeSensor = SensorFactory::Create(currentSensor);
        }
    } else if (i2cAddresses[0] == SHT4x_DEFAULT_ADDR && i2cAddresses[1] == BMP280_I2C_ADDR) {
        if (currentSensor != SensorType::Ultrasonic) {
            currentSensor = SensorType::Ultrasonic;
            s_app.m_activeSensor = SensorFactory::Create(currentSensor);
        }
    } else {
        // currentSensor = SensorType::IMUInternal;
        // s_app.m_activeSensor = SensorFactory::Create(currentSensor);
        auto& sess = s_app.m_session;
        sess.m_uiState = UIState::Settings;
        sess.m_subUiState = subUIState::DetectSensor;
    }

    Wire.end();
    s_app.m_session.m_needsRender = true;
}

void handleButtonEvents() {
    M5.update();
    auto& sess = s_app.m_session;

    if (M5.BtnA.wasPressed()) {
        if (s_app.m_sdCardPresent && (sess.m_uiState == UIState::Measuring || sess.m_uiState == UIState::StartScreen)) {
            toggleRunState();
        } else if (sess.m_uiState == UIState::Settings) {
            if (sess.m_subUiState == subUIState::DetectSensor) {
                sess.m_uiState = UIState::StartScreen;
                sess.m_subUiState = subUIState::None;
                if (idx == 0) {
                    s_app.m_currentSensor = SensorType::IMUInternal;
                    s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
                } else if (idx == 1) {
                    s_app.m_currentSensor = SensorType::Motion;
                    s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
                } else if (idx == 2) {
                    s_app.m_currentSensor = SensorType::Ultrasonic;
                    s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
                }
            }
            // idx  0 is for going back to the previous menu
            if (idx == 0) {
                if (sess.m_subUiState == subUIState::None) {
                    sess.m_uiState = UIState::StartScreen;
                } else {
                    sess.m_subUiState = subUIState::None;
                }
                sess.m_needsRender = true;
            } else {
                // update the setting for each sub state
                if (sess.m_subUiState == subUIState::None) {
                    if (idx == 1) {
                        sess.m_subUiState = subUIState::SampleRate;
                    } else if (idx == 2) {
                        sess.m_subUiState = subUIState::SensorList;
                    }
                    idx = 0;
                    sess.m_needsRender = true;
                } else {
                    if (sess.m_subUiState == subUIState::SampleRate) {
                        // Update the sample rate based on the current index
                        sess.m_config.m_sampleRateHz = rates[idx - 1]; //  as idx 0 is "back"

                        // return to the main settings menu
                        sess.m_subUiState = subUIState::None;
                    } else if (sess.m_subUiState == subUIState::SensorList) {
                        auto& sensor = s_app.m_activeSensor;
                        sensor->toggleSensorState(idx - 1); // idx 0 is "back", so we start from 1
                    }

                    sess.m_needsRender = true;
                }
            }
        }
    }

    if (M5.BtnB.wasPressed()) {
        if (sess.m_uiState == UIState::StartScreen) {
            sess.m_uiState = UIState::Settings;
            sess.m_needsRender = true;
            idx = 0; // Reset index for settings menu
        } else if (sess.m_uiState == UIState::Settings) {
            idx--;
            sess.m_needsRender = true;
        }
    }

    if (M5.BtnC.wasPressed()) {
        if (sess.m_uiState == UIState::StartScreen) {
            // auto next = (uint8_t)s_app.m_currentSensor + 1;
            // if (next >= (uint8_t)SensorType::Count)
            //     next = 0;
            // s_app.m_currentSensor = static_cast<SensorType>(next);
            // s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
            // sess.m_needsRender = true;
            detectSensor();
        } else if (sess.m_uiState == UIState::Settings) {
            idx++;
            sess.m_needsRender = true;
        }
    }
}

void handleSettingsState() {
    auto& sess = s_app.m_session;
    if (sess.m_subUiState == subUIState::None) {
        char menuItems[][MENU_STRING_LENGTH] = {"Start Screen", "sample rate", "sensor list"};
        size_t length = sizeof(menuItems) / sizeof(menuItems[0]);

        // do not show the sensor when there is only one sensor
        if (s_app.m_activeSensor->getNumberOfSensors() <= 1) {
            length -= 1;
        }

        // under and over index handling
        if (idx < 0) {
            idx = length - 1;
        } else if (idx >= length) {
            idx = 0;
        }

        RenderUI::renderSettings(menuItems, idx, length);
    } else {
        if (sess.m_subUiState == subUIState::SampleRate) {
            char menuItems[][MENU_STRING_LENGTH] = {"Setting Screen", "1 Hz",   "10 Hz",  "25 Hz", "50 Hz",
                                                    "100 Hz",         "200 Hz", "250 Hz", "500 Hz"};
            size_t length = sizeof(menuItems) / sizeof(menuItems[0]);

            if (idx < 0) {
                idx = length;
            } else if (idx >= length) {
                idx = 0;
            }

            RenderUI::renderSettings(menuItems, idx, length);
        } else if (sess.m_subUiState == subUIState::SensorList) {
            auto& sensor = s_app.m_activeSensor;
            const int numberOfSensors = sensor->getNumberOfSensors();
            Serial.printf("number of sensors: %d\n", numberOfSensors);
            char sensorList[numberOfSensors + 1][MENU_STRING_LENGTH] = {0};
            size_t length = sizeof(sensorList) / sizeof(sensorList[0]);
            strncpy(sensorList[0], "Setting Screen", MENU_STRING_LENGTH - 1);

            if (idx < 0) {
                idx = length - 1;
            } else if (idx >= length) {
                idx = 0;
            }

            for (int i = 0; i < numberOfSensors; i++) {
                char buffer[MENU_STRING_LENGTH] = {0};
                const char* sensorName = sensor->getSensorList(i);
                snprintf(buffer, sizeof(buffer), "%s %s", sensorName,
                         sensor->isSensorActive(i) ? "(active)" : "(inactive)");
                strncpy(sensorList[i + 1], buffer, MENU_STRING_LENGTH - 1);
            }
            RenderUI::renderSettings(sensorList, idx, length);
        } else if (sess.m_subUiState == subUIState::DetectSensor) {
            char sensors[][MENU_STRING_LENGTH] = {"Internal IMU", "Motion", "Distance"};
            size_t length = sizeof(sensors) / sizeof(sensors[0]);

                        if (idx < 0) {
                idx = length - 1;
            } else if (idx >= length) {
                idx = 0;
            }

            RenderUI::renderSettings(sensors, idx, length);
        }
    }
}

void updateSDCardPresence() {
    bool now_present = SDWrapper::exists();
    if (now_present != s_app.m_sdCardPresent) {
        s_app.m_sdCardPresent = now_present;
        s_app.m_session.m_needsRender = true;
    }
}

void performMeasurementStep() {
    uint64_t current_ms = millis();
    uint64_t elapsed = current_ms - s_app.m_session.m_startMS;
    uint32_t sampleRate = s_app.m_session.m_config.m_sampleRateHz;
    uint32_t interval_ms = (sampleRate > 0) ? (1000u / sampleRate) : 1000u;

    static uint64_t last_sample_ms = 0;
    if (elapsed - last_sample_ms >= interval_ms) {
        last_sample_ms = elapsed;
        s_app.m_activeSensor->gatherAndAccumulate(s_app.m_session.m_logger, s_app.m_session.m_data, elapsed);
        s_app.m_session.m_data.m_totalSamples++;
        s_app.m_session.m_data.m_samplesThisSecond++;
    }
}

void clearScreen() { M5.Lcd.fillRect(0, 0, 320, 240, BLACK); }

void maybeRefreshStats() {
    uint64_t now_ms = millis();
    if (now_ms - s_app.m_session.m_lastUpdateMS >= DISPLAY_UPDATE_INTERVAL) {
        clearScreen();
        RenderUI::renderStatistics(s_app.m_session.m_data, s_app.m_session.m_config, s_app.m_activeSensor->getName());
        s_app.m_session.m_data.m_samplesThisSecond = 0;
        s_app.m_session.m_data.m_elapsedSeconds++;
        s_app.m_session.m_lastUpdateMS = now_ms;
    }
}

void renderIfNeeded() {
    auto& sess = s_app.m_session;
    if (!sess.m_needsRender)
        return;

    clearScreen();
    if (sess.m_uiState == UIState::Measuring) {
        RenderUI::renderStatistics(sess.m_data, sess.m_config, s_app.m_activeSensor->getName());
    } else if (sess.m_uiState == UIState::StartScreen) {
        RenderUI::renderStartScreen(sess.m_index, s_app.m_activeSensor->getName(), s_app.m_sdCardPresent,
                                    sess.m_config.m_sampleRateHz);
    } else if (sess.m_uiState == UIState::Settings) {
        handleSettingsState();
    }
    sess.m_needsRender = false;
}

void setup() {
    M5.begin();
    INITLOG();

    s_app.m_sdCardPresent = SDWrapper::exists();
    s_app.m_session.m_index = SDWrapper::countFiles();
    s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
    initializeDisplay();
    renderIfNeeded();
}

void loop() {
    // UpdateSdCardPresence(); This does not work, needs a different approach
    handleButtonEvents();

    if (s_app.m_session.m_uiState == UIState::Measuring) {
        performMeasurementStep();
        maybeRefreshStats();
    }

    renderIfNeeded();
    s_app.m_session.m_logger.update();
}