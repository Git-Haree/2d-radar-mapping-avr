#include <ESP32Servo.h>

#define TRIG_PIN  5
#define ECHO_PIN  18
#define SERVO_PIN 13

Servo radarServo;

int angle    = 0;
int step_dir = 1;

long measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    return duration * 0.034 / 2;
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    radarServo.attach(SERVO_PIN);
    radarServo.write(0);
    delay(500);
}

void loop() {
    radarServo.write(angle);
    delay(50);

    long dist = measureDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.println(dist);

    angle += step_dir;
    if (angle >= 180 || angle <= 0) {
        step_dir = -step_dir;
    }
}
