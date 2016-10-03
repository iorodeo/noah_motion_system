#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H
#include <Arduino.h>

// EEPROM parameters
struct EEPROM_Int32_Data
{
    uint8_t  flag;
    uint32_t value;
};

struct EEPROM_Int8_Data
{
    uint8_t flag;
    int8_t  value;
};

#endif 
