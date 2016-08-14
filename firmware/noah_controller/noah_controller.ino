#include "motion_controller.h"

void setup()
{
    motion_controller.initialize();
}


void loop()
{
    motion_controller.update();
}

