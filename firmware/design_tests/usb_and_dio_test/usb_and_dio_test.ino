#include <Streaming.h>

const long BaudRate = 115200;
const int NumDataPts = 5;

const int NumberOfDio = 12;
const int DioBegin = 2;
const int DioEnd = DioBegin + NumberOfDio;
const int DioTimerPeriod = 5;

const bool UseUSBTimer = false;
const int USBTimerPeriod = 5000;
unsigned long lastUSBTime = 0;
bool usbSendFlag = false;

IntervalTimer dioTimer;
IntervalTimer usbTimer;


void dioTimerCallback()
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


void usbTimerCallback()
{
    usbSendFlag = true;
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
    dioTimer.begin(dioTimerCallback, DioTimerPeriod);

    if (UseUSBTimer)
    {
        usbTimer.priority(2);
        usbTimer.begin(usbTimerCallback, USBTimerPeriod);
    }
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
                if (UseUSBTimer)
                {
                    lastUSBTime = time;
                }
                else
                {
                    Serial << time << endl;
                }
                break;

            default:
                break;
                
        }
    }

    if (usbSendFlag)
    {
        usbSendFlag = false;
        Serial << micros() << " " << lastUSBTime << endl;
    }

}
