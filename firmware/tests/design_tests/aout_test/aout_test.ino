const uint16_t AOUT_LIM= 4096;

void setup()
{
    analogWriteResolution(12);
    analogReference(INTERNAL);
    analogWrite(A14, 0);  //Set the DAC output to 0.
}

void loop()
{
    static bool increasing = true;
    static uint16_t value = 0;

    analogWrite(A14,value);

    if (true)
    {
        if (increasing)
        {
            if (value == AOUT_LIM-1)
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
            if (value == 0)
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
    delayMicroseconds(100);
}
