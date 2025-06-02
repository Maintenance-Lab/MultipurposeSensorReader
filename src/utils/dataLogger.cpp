#include "dataLogger.h"
#include "sdWrapper.h"

void DataLogger::open(String header) {
    if(m_isOpen){
        close();
    }
    m_fileName = SDWrapper::newFilename();
    SDWrapper::createFile(SD, m_fileName.c_str(), header.c_str());
    m_file = SD.open(m_fileName, FILE_APPEND);
    m_isOpen = true;
}

void DataLogger::close() {
    m_file.close();
    m_isOpen = false;
}

void DataLogger::update(){
    m_flusher.update();
}

void DataLogger::flush(){
    if(m_isOpen){
        m_file.flush();
    }
}