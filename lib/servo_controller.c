// const int LED_PIN = 13;
volatile int servo_angle = 90;
volatile int servo_pin = 9;
String inputString = "";

void write_servo(int pin, int angle) {
  int pulse_width = map(angle, 0, 180, 480, 2400);

  digitalWrite(pin, HIGH);
  delayMicroseconds(pulse_width);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - pulse_width);
}

void setup() {
  Serial.begin(9600);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

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
    String in_message = Serial.readStringUntil('\n');
    if(in_message.length() > 0) {
      String in_message_pin = in_message.substring(0, 2);
      String in_message_angle = in_message.substring(2);

      servo_pin = in_message_pin.toInt();
      servo_angle = in_message_angle.toInt();
    }
  }

  write_servo(servo_pin, servo_angle);
}
