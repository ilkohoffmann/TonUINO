#include "Utils.h" 

/**
 * Utility function that loops through a
 * byte array and prints each value in hexadecimal format.
 * 
 * @param buffer The byte array to print
 * @param bufferSize The number of bytes in the array
 */
void Utils::dump_byte_array(byte* buffer, byte bufferSize) {
    // Loop through each byte in the array
    for (byte i = 0; i < bufferSize; i++) {
        // Print a leading zero if the value is less than 0x10 (to keep formatting consistent)
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        // Print the byte value in hexadecimal format
        Serial.print(buffer[i], HEX);
    }
    // Print a new line character at the end to make output easier to read
    Serial.println();
}

bool Utils::isKnownCard(NfcCard* nfcCard) {
    return nfcCard->cookie == CARD_COOKIE;
}