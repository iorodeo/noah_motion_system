#include "stepper.h"
#include "motor_pin.h"
#include "msg_types.h"
#include "constants.h"
#include "motion_controller.h"

MotionController motionController;

void setup()
{
    //Serial.begin(115200);
    motionController.initialize();

}

void loop()
{
    motionController.update();
}


    




