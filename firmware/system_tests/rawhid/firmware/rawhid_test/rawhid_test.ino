const int DevToHostTimeout = 50;
const int HostToDevTimeout = 50;
const unsigned long UpdateTimerPeriod = 50ul;
const unsigned long MessagePeriod = 5000ul;
const unsigned long NewMessageCount = MessagePeriod/UpdateTimerPeriod;

const int NumStepper = 5;
const int NumAnalogInput = 10;
const int NumPwm = 5;

const int AnalogInputPin[NumAnalogInput] = {A0,A1,A2,A3,A4,A5,A10,A11,A12,A13};

IntervalTimer update_timer;
bool new_msg_flag = false;


struct __attribute__((packed)) DevToHostMsg 
{
    uint16_t status;
    uint64_t time_us;
    uint16_t stepper_position[NumStepper];
    uint16_t stepper_velocity[NumStepper];
    uint16_t analog_input[NumAnalogInput];
    uint16_t pwm_position[NumPwm];
    uint16_t trigger_prescalar;
    uint8_t trigger_enable;
    uint8_t digital_output;
};


struct __attribute__((packed)) HostToDevMsg 
{
    uint8_t data[64];
};


void update_timer_callback()
{
    static uint32_t cnt = 0;
    if (cnt%NewMessageCount == 0)
    {
        new_msg_flag = true;
    }
    cnt++;
}


void setup()
{
    //Serial.begin(115200);
    update_timer.priority(1);
    update_timer.begin(update_timer_callback,UpdateTimerPeriod);

    analogReadResolution(16);
    analogReadAveraging(32);
    analogReference(INTERNAL);

}


void loop()
{
    int num_bytes;
    static uint32_t micros_last = 0;
    static uint64_t time_us = 0;

    // Send Message to host
    if (new_msg_flag)
    {
        new_msg_flag = false;
        DevToHostMsg dev_to_host_msg;

        // Get time
        uint32_t micros_curr = micros();
        uint32_t micros_dt = micros_curr - micros_last;
        micros_last = micros_curr;
        time_us += uint64_t(micros_dt);
        dev_to_host_msg.time_us = time_us;

        // Get analog inputs
        for (int i=0; i<NumAnalogInput; i++)
        {
            dev_to_host_msg.analog_input[i] = analogRead(AnalogInputPin[i]);
        }

        num_bytes = RawHID.send(&dev_to_host_msg,DevToHostTimeout);
        if (num_bytes != sizeof(DevToHostMsg))
        {
            time_us = 0;
        }
        else
        {
            // Get response back from host
            HostToDevMsg host_to_dev_msg;
            num_bytes = RawHID.recv(&host_to_dev_msg,HostToDevTimeout);
            if (num_bytes == sizeof(HostToDevMsg))
            {
                // Response is OK
            }
        }
    }
}
