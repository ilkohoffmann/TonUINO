#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <Arduino.h>
#include <MemoryFree.h>

class MemoryUtils {
   public:
    static void printFreeMemory() {
        int freeMemBytes = freeMemory();

        Serial.print(F("Free memory: "));

        if (freeMemBytes < 1024) {
            Serial.print(freeMemBytes);
            Serial.println(F(" bytes"));
        } else if (freeMemBytes < 1048576) {
            float freeMemKB = freeMemBytes / 1024.0;
            Serial.print(freeMemKB, 2);
            Serial.println(F(" KB"));
        } else {
            float freeMemMB = freeMemBytes / 1048576.0;
            Serial.print(freeMemMB, 2);
            Serial.println(F(" MB"));
        }
    }
};

#endif