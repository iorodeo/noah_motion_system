

void setup()
{
    Serial.begin(115200);
    analogReadResolution(16);
    analogReadAveraging(32);
    analogReference(INTERNAL);
}


void loop()
{
    static bool running = false;
    if (Serial.available() > 0)
    {
        char cmd = Serial.read();
        switch (cmd)
        {
            case 'r':
                running = true;
                break;

            case 's':
                running = false;
                break;

            default:
                break;

        }
    }
    if (running)
    {
        uint16_t value = analogRead(A0);
        Serial.println(value);
    }
    delay(5);
}

