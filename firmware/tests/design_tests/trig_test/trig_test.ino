

const int Trig0Pin = 26;
const int Trig1Pin = 27;

void setup()
{
    Serial.begin(115200);
    pinMode(Trig0Pin,OUTPUT);
    pinMode(Trig1Pin,OUTPUT);
    digitalWrite(Trig0Pin,LOW);
    digitalWrite(Trig1Pin,LOW);
}


void loop()
{
    static bool state = false;

    if (state)
    {
        state = false;
        digitalWrite(Trig0Pin,LOW);
        digitalWrite(Trig1Pin,HIGH);
        Serial.println(state);
    }
    else
    {
        state = true;
        digitalWrite(Trig0Pin,HIGH);
        digitalWrite(Trig1Pin,LOW);
        Serial.println(state);
    } 
    delay(100);
}
