#include <AccelStepper.h>

const int NumMotor = 8;
const int MsgSize = 64;
const int MsgTimeout = 50;
const unsigned long TimerPeriod = 5000;

struct CmdMsg
{
    uint16_t id;
    float velocity[NumMotor];
    uint8_t padding[26];
};

struct DevMsg
{
    uint64_t time;
    int32_t position[NumMotor];
    uint8_t padding[24];
};

AccelStepper stepper;

IntervalTimer intervalTimer;
bool intervalTimerFlag = false;
void on_intervalTimer_callback();


void setup() 
{

    //Serial.begin(115200);
    stepper = AccelStepper(AccelStepper::DRIVER,1,2);
    stepper.setMaxSpeed(20000.0);
    stepper.setSpeed(0.0);
    intervalTimer.priority(1);
    intervalTimer.begin(on_intervalTimer_callback,TimerPeriod);

}


void loop() 
{
    static int cnt = 0;
    static uint64_t time_us = 0;
    static uint32_t micros_last = micros();

    if (intervalTimerFlag)
    {

        int num_bytes;
        DevMsg dev_msg;
        CmdMsg cmd_msg;

        // Compute time using difference
        uint32_t micros_curr = micros();
        uint32_t micros_dt = micros_curr - micros_last;
        micros_last = micros_curr;
        time_us += uint64_t(micros_dt);


        // Send current position
        dev_msg.time = time_us;
        for (int i=0; i<NumMotor; i++)
        {
            dev_msg.position[i] = 0;
        }
        dev_msg.position[0] = stepper.currentPosition();
        num_bytes = RawHID.send(&dev_msg,MsgTimeout);

        // Recive velocity command
        num_bytes = RawHID.recv(&cmd_msg,MsgTimeout);
        if (num_bytes == MsgSize) 
        {
            //Serial.println(cmd_msg.velocity[0]);
            stepper.setSpeed(cmd_msg.velocity[0]);
        }
        cnt++;
        intervalTimerFlag = false;
    }

    stepper.runSpeed();
}

void on_intervalTimer_callback()
{
    intervalTimerFlag = true;
}

    




