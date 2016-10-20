#include "eeprom_config.h"

const uint16_t EEPROM_Config::Address = constants::DefaultConfigEEPROMAddress;
const uint16_t EEPROM_Config::NotSetFlagValue = 255;

EEPROM_Config::EEPROM_Config() { }


EEPROM_Data EEPROM_Config::get()
{
    EEPROM_Data eeprom_data;
    EEPROM.get(Address,eeprom_data);
    return eeprom_data;
}


void EEPROM_Config::put(EEPROM_Data eeprom_data)
{
    eeprom_data.flag = 0;
    EEPROM.put(Address,eeprom_data);
}

EEPROM_Data EEPROM_Config::get_defaults()
{
    EEPROM_Data eeprom_data;
    for (int i=0; i<constants::NumStepper; i++)
    {
        eeprom_data.stepper_homing_direction[i] = constants::DefaultStepperHomingDirection[i];
        eeprom_data.stepper_minimum_position[i] = constants::DefaultStepperMinimumPosition[i];
        eeprom_data.stepper_maximum_position[i] = constants::DefaultStepperMaximumPosition[i];
        eeprom_data.stepper_maximum_speed[i] = constants::DefaultStepperMaximumSpeed[i];
        eeprom_data.stepper_maximum_accel[i] = constants::DefaultStepperMaximumAccel[i];
    }
    return eeprom_data;
}

void EEPROM_Config::clear()
{
    EEPROM_Data eeprom_data = get_defaults();
    eeprom_data.flag = 255;
    EEPROM.put(Address,eeprom_data);
}

void EEPROM_Config::set_to_defaults()
{
    EEPROM_Data eeprom_data = get_defaults();
    put(eeprom_data);
}
