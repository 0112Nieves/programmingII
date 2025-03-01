#include <stdio.h>
#include <stdint.h>

uint16_t decimalToHex(uint16_t decimal) {
    return decimal & 0xFFFF;
}

int main() {
    uint16_t decimalNumber = 255;
    uint16_t hexValue = decimalToHex(decimalNumber);

    printf("Decimal: %u\nHexadecimal: 0x%X\n", decimalNumber, hexValue);

    return 0;
}
