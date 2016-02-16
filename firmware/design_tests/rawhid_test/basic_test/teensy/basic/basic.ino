
IntervalTimer timer;
bool timerFlag = false;
void onTimerCallback()
{
    timerFlag = true;
}

void setup() {
    //Serial.begin(9600);
    //Serial.println(F("RawHID Example"));
    timer.priority(1);
    timer.begin(onTimerCallback,10000);
}

// RawHID packets are always 64 bytes
byte buffer[64];

void loop() {
    static uint8_t cnt = 0;
    static unsigned long last = micros();
    static unsigned long curr = micros();
    int n = 0;

    if (timerFlag)
    {
        timerFlag = false;
        //last = curr;
        //curr = micros();
        unsigned long diff = curr - last;

        for (int i=0; i<64; i++)
        {
            buffer[i] = 0;
        }
        for (int i=0; i<4; i++)
        {
            buffer[i] = (diff >> (8*i)) & 0b11111111; 
        }
        //Serial.println(diff);

        n = RawHID.send(buffer, 100);
        if (n <= 0) {
            //Serial.println(F("no transmit"));
        }

        n = RawHID.recv(buffer, 0); // 0 timeout = do not wait
        if (n>0)
        {
            bool ok = true;
            for (int i=0; i<64; i++)
            {
                if (buffer[i] != i)
                {
                    ok = false;
                }
            }
            if (!ok)
            {
                //Serial.print(F("ok = "));
                //Serial.println(ok);
            }
        }
        else
        {
            //Serial.println(F("no data"));
        }

        last = curr;
        curr = micros();
    }
    cnt++;
}
