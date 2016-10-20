#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H
#include <Arduino.h>
#include <EEPROM.h>
#include "dev_constants.h"
#include "eeprom_data.h"

class EEPROM_Config
{
    public:

        static const uint16_t Address;
        static const uint16_t NotSetFlagValue;

        EEPROM_Config();

        void put(EEPROM_Data eeprom_data);
        EEPROM_Data get();

        void clear();
        void set_to_defaults();
        EEPROM_Data get_defaults();


};

#endif
