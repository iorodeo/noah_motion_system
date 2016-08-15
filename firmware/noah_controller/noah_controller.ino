#include "system_state.h"

void setup()
{
    system_state.initialize();
}


void loop()
{
    system_state.send_and_recv();
}

