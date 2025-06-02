#ifndef SD_H
#define SD_H

#include "FS.h"
#include "SD.h"
#include <M5Core2.h>

class SDWrapper {
  public:
    static bool exists();

    // Creates a filename for a given index
    static String createFileName(uint32_t index);

    // Opens a file with the given path in either append or write mode
    static File openFile(fs::FS& fs, const char* const path, bool appendMode = true);

    // Creates a new file and writes a message to it
    static void createFile(fs::FS& fs, const char* const path, const char* columnNames);

    // Appends data to a file, creating the file if it doesn't exist
    static void appendFile(int index, const String& message, const char* columnNames);

    // Counts the number of files in the root directory
    static uint32_t countFiles();

    // Generates a new filename based on the count of existing files
    static String newFilename();
};

#endif // SD_H