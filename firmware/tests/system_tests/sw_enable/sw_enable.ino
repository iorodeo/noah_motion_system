const int SWEnablePin = 32;
const int DisableAin = A20;
bool enabled = false;


void setup()
{
    Serial.begin(115200);
    pinMode(SWEnablePin,OUTPUT);
    digitalWrite(SWEnablePin, LOW);
}

void loop()
{
    while (Serial.available() > 0)
    {
        char cmd = Serial.read();

        switch (cmd)
        {
            case 'e':
                digitalWrite(SWEnablePin,HIGH);
                Serial.println("Enable Drives");
                enabled = true;
                delay(100);
                break;

            case 'd':
                digitalWrite(SWEnablePin,LOW);
                Serial.println("Disable Drives");
                break;

            default:    
                break;
        }
    }

    int disableValue = analogRead(DisableAin);
    if (enabled && (disableValue > 200))
    {
        Serial.println("Disable Event");
        enabled = false;
    }
}

