#include "dataLogger.h"
#include "sdWrapper.h"
#include <cstdio>
#include <cstdarg>

bool DataLogger::open(String header) {
    if (m_isOpen) {
        close();
    }
    m_fileName = SDWrapper::newFilename();
    SDWrapper::createFile(SD, m_fileName.c_str(), header.c_str());
    m_file = SD.open(m_fileName, FILE_APPEND);
    m_isOpen = true;
    if (!m_file) {
        m_isOpen = false;
    }
    return m_isOpen;
}

void DataLogger::close() {
    m_file.close();
    m_isOpen = false;
}

void DataLogger::update() { m_flusher.update(); }

void DataLogger::flush() {
    if (m_isOpen) {
        m_file.flush();
    }
}

void DataLogger::printf(const char* format, ...) {
    constexpr size_t BufferSize = 256;
    char buffer[BufferSize];

    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, BufferSize, format, args);
    va_end(args);

    m_file.print(buffer);
}