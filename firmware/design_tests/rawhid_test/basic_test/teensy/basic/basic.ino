/*
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
