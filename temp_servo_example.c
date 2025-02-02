#include <Servo.h>

volatile int servo_angle = 90;
volatile int servo_pin = 9;
char inputBuffer[10]; // Buffer for incoming serial data
int bufferIndex = 0;

Servo myServo; // Servo object

void setup() {
  Serial.begin(9600);
  myServo.attach(servo_pin); // Attach to initial pin

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  int data_a0 = analogRead(A0);
  int data_a1 = analogRead(A1);
  int data_a2 = analogRead(A2);
  Serial.print(data_a0);
  Serial.print('-');
  Serial.print(data_a1);
  Serial.print('-');
  Serial.println(data_a2);

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') { // End of message
      inputBuffer[bufferIndex] = '\0'; // Null-terminate string
      if (bufferIndex >= 2) {
        int new_servo_pin = atoi(inputBuffer); // Extract pin
        int new_servo_angle = atoi(inputBuffer + 2); // Extract angle

        if (new_servo_pin != servo_pin) { 
          myServo.detach(); // Detach old pin
          myServo.attach(new_servo_pin); // Attach to new pin
          servo_pin = new_servo_pin; 
        }

        servo_angle = new_servo_angle;
        myServo.write(servo_angle); // Move servo
      }
      bufferIndex = 0; // Reset buffer
    } 
    else if (bufferIndex < sizeof(inputBuffer) - 1) {
      inputBuffer[bufferIndex++] = c;
    }
  }

  Serial.println("DONE");
}
