#pragma once

#include "FS.h"
#include "SD.h"
#include <M5Core2.h>

class FileWriter {
  public:
    void open(String header);
    void close();
    void write(float value, int digits);
    void write(char value);
    void write(String value);
    uint32_t getNewFileNameIndex();

  private:
    String m_fileName;
    File m_file;
    bool m_isOpen = false;
};