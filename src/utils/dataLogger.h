#pragma once

#include "FS.h"
#include "SD.h"
#include "periodicExecutor.h"
#include <M5Core2.h>
#include "../logging.h"

class DataLogger {
  public:
    DataLogger(uint32_t flushInterval) : m_flusher(flushInterval, [this]() { flush(); }) {}

    void open(String header);
    void close();

    template <typename T>
    void write(T value) {
        if (!m_isOpen) {
            LOGLN("DataLogger is not open!");
            return;
        }
        m_file.print(value);
    }

    template <typename T>
    void write(T value, int digits) {
        if (!m_isOpen) {
            LOGLN("DataLogger is not open!");
            return;
        }
        m_file.print(value, digits);
    }

    uint32_t getNewFileNameIndex();
    void update();

  private:
    PeriodicExecutor m_flusher;
    String m_fileName;
    File m_file;
    bool m_isOpen = false;

    void flush();
};