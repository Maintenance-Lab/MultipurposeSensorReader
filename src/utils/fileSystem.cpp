#include "fileSystem.h"

bool FileSystem::exists(){
  return SD.cardType() == CARD_SD || SD.cardType() == CARD_SDHC;
}

String FileSystem::createFileName(uint32_t index) {
  return "/data_" + String(index) + ".txt";
}

File FileSystem::openFile(fs::FS &fs, const char *path, bool appendMode) {
  File file = appendMode ? fs.open(path, FILE_APPEND) : fs.open(path, FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file");
    return File(); // Return an empty File object
  }
  if (file.isDirectory()) {
    Serial.println("Path is a directory");
    return File(); // Return an empty File object
  }
  return file;
}

void FileSystem::createFile(fs::FS &fs, const char *path, const char* columnNames) {
  File file = openFile(fs, path);
  if (!file) return;
  file.print(columnNames);
  file.close();
}

void FileSystem::appendFile(int index, const String &accumulatedData, const char* columnNames) {
  String path = createFileName(index);
  if (!SD.exists(path.c_str())) {
    createFile(SD, path.c_str(), columnNames);
  }

  File file = openFile(SD, path.c_str(), true);
  if (!file) return;
  file.print(accumulatedData);
  file.close();
}

uint32_t FileSystem::countNumberOfFiles() {
  uint32_t fileCount = 0;
  File root = SD.open("/");
  while (File entry = root.openNextFile()) {
    if (!entry.isDirectory()) {
      fileCount++;
    }
    entry.close();
  }
  return fileCount;
}

String FileSystem::newFilename() {
  return createFileName(countNumberOfFiles());
}
