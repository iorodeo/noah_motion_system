#include "system_state.h"

void setup()
{
    system_state.initialize();
}


void loop()
{
    system_state.update_on_loop();
}


