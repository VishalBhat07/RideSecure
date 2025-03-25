void setup() {
    Serial.begin(9600);  // Initialize Serial communication
    pinMode(2, OUTPUT);  // ESP32 built-in LED pin
}

void loop() {
    if (Serial.available() > 0) {  // Check if data is received
        char command = Serial.read();
        if (command == 'B') {  // If LPC2148 sends 'B'
            digitalWrite(2, HIGH);
            delay(500);
            digitalWrite(2, LOW);
            delay(500);
        }
    }
}
