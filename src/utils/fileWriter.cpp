#include "fileWriter.h"
#include "fileSystem.h"

void FileWriter::open(String header) {
    if(m_isOpen){
        close();
    }
    m_fileName = FileSystem::newFilename();
    FileSystem::createFile(SD, m_fileName.c_str(), header.c_str());
    m_file = SD.open(m_fileName, FILE_APPEND);
    m_isOpen = true;
}

void FileWriter::close() {
    m_file.close();
    m_isOpen = false;
}

void FileWriter::update(){
    m_flusher.update();
}

void FileWriter::flush(){
    if(m_isOpen){
        m_file.flush();
    }
}