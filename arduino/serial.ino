String str;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        str = Serial.readStringUntil('\n');
        Serial.println(str);
    }
}