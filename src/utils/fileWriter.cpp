#include "fileWriter.h"
#include "SD_controller.h"

void FileWriter::open(String header) {
    if(m_isOpen){
        close();
    }
    m_fileName = SD_controller::newFilename();
    SD_controller::createFile(SD, m_fileName.c_str(), header.c_str());
    m_file = SD.open(m_fileName, FILE_APPEND);
    m_isOpen = true;
}

void FileWriter::write(float value, int digits) {
    if (!m_isOpen) {
        return;
    }
    m_file.print(value, digits);
}

void FileWriter::write(char value){
    if (!m_isOpen) {
        return;
    }
    m_file.print(value);
}

void FileWriter::write(String value) {
    if (!m_isOpen) {
        return;
    }
    m_file.print(value);
}

void FileWriter::close() {
    m_file.close();
    m_isOpen = false;
}