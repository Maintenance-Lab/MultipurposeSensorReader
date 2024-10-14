#pragma once

#define LOG
#define UART_BAUD 115200

#ifdef LOG
#include <M5Core2.h>
// Remove the initial LOG definition or rename it to avoid redefinition.
#undef LOG
#define INITLOG()                                                                                                      \
    {                                                                                                                  \
        Serial.begin(UART_BAUD);                                                                                       \
        Serial.println("Logging enabled. Warning: This will affect the performance of the device.");                   \
    }
#define LOGF(...) Serial.printf(__VA_ARGS__);
#define LOG(...) Serial.print(__VA_ARGS__);W@
#define LOGLN(...) Serial.println(__VA_ARGS__);
#else
#define INITLOG()
#define LOGF(...)
#define LOG(...)
#define LOGLN(...)
#endif
