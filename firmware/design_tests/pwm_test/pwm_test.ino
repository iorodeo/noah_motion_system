const uint16_t PwmPin = 20;
const uint16_t PwmFreq = 50;
const uint16_t PwmRes = 12;
const uint16_t PwmMaxVal = 4096;

const uint16_t MotionMinVal = 200;
const uint16_t MotionMaxVal = 430;

void setup()
{
    Serial.begin(115200);
    pinMode(PwmPin,OUTPUT);
    analogWriteFrequency(PwmPin,PwmFreq);
    analogWriteResolution(PwmRes);
}

void loop()
{

    static bool increasing = true;
    static uint16_t value = MotionMinVal;

    analogWrite(PwmPin,value);

    if (true)
    {
        if (increasing)
        {
            if (value >= MotionMaxVal)
            {
                increasing = false;
                value -= 1;
            }
            else
            {
                value += 1;
            }
        }
        else
        {
            if (value <= MotionMinVal)
            {
                increasing = true;
                value += 1;
            }
            else
            {
                value -= 1;
            }
        }
    }
    Serial.println(value);
    delay(20);
}


