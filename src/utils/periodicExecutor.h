#pragma once

#include <Arduino.h>
#include <functional>

// Executes a function at the given interval
class PeriodicExecutor {
  public:
    PeriodicExecutor(unsigned long interval, std::function<void()> function)
        : m_interval(interval), m_lastTime(0), m_function(function) {}

    void update() {
        unsigned long currentTime = millis();
        if (currentTime - m_lastTime >= m_interval) {
            m_lastTime = currentTime;
            m_function();
        }
    }

  private:
    unsigned long m_interval;
    unsigned long m_lastTime;
    std::function<void()> m_function;
};