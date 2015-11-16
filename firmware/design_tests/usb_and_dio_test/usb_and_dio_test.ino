#include <Streaming.h>

const long BaudRate = 115200;
const int NumDataPts = 5;

const int NumberOfDio = 12;
const int DioBegin = 2;
const int DioEnd = DioBegin + NumberOfDio;

IntervalTimer dioTimer;


void timerCallback()
{
    static bool state = false;

    int dioValue;
    if (state)
    {
        dioValue = LOW;
    }
    else
    {
        dioValue = HIGH;
    }
    state = !state;

    for (int i=DioBegin; i<DioEnd; i++)
    {
        digitalWriteFast(i,dioValue);
    }
}

void setup()
{
    Serial.begin(BaudRate); 

    for (int i=DioBegin; i<DioEnd; i++)
    {
        pinMode(i,OUTPUT);
        digitalWrite(i,LOW);
    }
    dioTimer.priority(1);
    dioTimer.begin(timerCallback, 5);
}


void loop()
{
    static unsigned int cnt = 0;

    while (Serial.available() > 0)
    {
        char cmd = Serial.read();
        unsigned long time = micros();
        switch (cmd)
        {
            case 'd':
                for (int i=0; i<NumDataPts; i++)
                {
                    Serial << cnt;
                    if (i<NumDataPts)
                    {
                        Serial << " ";
                    }
                }
                Serial << endl;
                cnt++;
                break;

            case 't':
                Serial << time << endl;
                break;

            default:
                break;
                
        }
    }
}
