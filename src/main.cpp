#include "main.h"
#include "AXP192.h"
#include "esp32_digital_led_lib.h"
#include "logging.h"
#include "UIRenderer.h"
#include "sensors/sensorFactory.h"
#include "sensors/motion.h"
#include "sensors/ultrasonic.h"
#include "utils/sdWrapper.h"
#include <M5Core2.h>

constexpr uint32_t DISPLAY_UPDATE_INTERVAL = 1000;

static AppState s_app;

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
    sess.m_isRunning = !sess.m_isRunning;
    sess.m_index = SDWrapper::countFiles();

    if (sess.m_isRunning) {
        startMeasurement();
    } else {
        sess.m_data.reset();
        sess.m_logger.close();
        sess.m_needsRender = true;
    }
}

void handleButtonEvents() {
    M5.update();

    if (s_app.m_sdCardPresent && M5.BtnA.wasPressed()) {
        toggleRunState();
    }

    if (M5.BtnB.wasPressed() && !s_app.m_session.m_isRunning) {
        static const int rates[] = { 1, 10, 25, 50, 100, 200, 250, 500 };
        int current = s_app.m_session.m_config.m_sampleRateHz;
        int idx = 0;
        for (int i = 0; i < (int)(sizeof(rates)/sizeof(rates[0])); i++) {
            if (rates[i] == current) {
                idx = i;
                break;
            }
        }
        idx = (idx + 1) % (sizeof(rates)/sizeof(rates[0]));
        s_app.m_session.m_config.m_sampleRateHz = rates[idx];
        s_app.m_session.m_needsRender = true;
    }

    if (M5.BtnC.wasPressed()) {
        auto next = (uint8_t)s_app.m_currentSensor + 1;
        if (next >= (uint8_t)SensorType::Count)
            next = 0;
        s_app.m_currentSensor = static_cast<SensorType>(next);
        s_app.m_activeSensor = SensorFactory::Create(s_app.m_currentSensor);
        s_app.m_session.m_needsRender = true;
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
    if (!s_app.m_session.m_needsRender)
        return;

    clearScreen();
    if (s_app.m_session.m_isRunning) {
        RenderUI::renderStatistics(s_app.m_session.m_data, s_app.m_session.m_config, s_app.m_activeSensor->getName());
    } else {
        RenderUI::renderStartScreen(s_app.m_session.m_index, s_app.m_activeSensor->getName(), s_app.m_sdCardPresent, s_app.m_session.m_config.m_sampleRateHz);
    }
    s_app.m_session.m_needsRender = false;
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

    if (s_app.m_session.m_isRunning) {
        performMeasurementStep();
        maybeRefreshStats();
    }

    renderIfNeeded();
    s_app.m_session.m_logger.update();
}