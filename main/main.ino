void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(13, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:

    Serial.print("cool");

    digitalWrite(12, HIGH);
    delay(3000);
    digitalWrite(12, LOW);

}
