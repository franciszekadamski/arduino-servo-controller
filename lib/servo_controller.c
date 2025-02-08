#include <Servo.h>

Servo water_valve_servo;
Servo light_switch_servo;

int water_valve_angle = 0;
int light_switcher_angle = 0;

const int water_valve_servo_pin = 10;
const int light_switch_servo_pin = 11;

const int air_humidity_sensor_pin = A0;
const int soil_humidity_sensor_pin = A1;
const int air_temperature_sensor_pin = A2;

String in_message;
String in_message_pin;
String in_message_angle;

int servo_pin;
int servo_angle;

void setup() {
  Serial.begin(9600);

  water_valve_servo.attach(water_valve_servo_pin);
  light_switch_servo.attach(light_switch_servo_pin);

  pinMode(air_humidity_sensor_pin, INPUT);
  pinMode(soil_humidity_sensor_pin, INPUT);
  pinMode(air_temperature_sensor_pin, INPUT);
}

void loop() {
  int air_humidity_read = analogRead(air_humidity_sensor_pin);
  int soil_humidity_read = analogRead(soil_humidity_sensor_pin);
  int air_temperature_read = analogRead(air_temperature_sensor_pin);
  Serial.print(air_humidity_read);
  Serial.print('-');
  Serial.print(soil_humidity_read);
  Serial.print('-');
  Serial.println(air_temperature_read);

  while (Serial.available() > 0) {
    in_message = Serial.readStringUntil('\n');
    if(in_message.length() > 0) {
      in_message_pin = in_message.substring(0, 2);
      in_message_angle = in_message.substring(2);

      servo_pin = in_message_pin.toInt();
      servo_angle = in_message_angle.toInt();

      if (servo_pin == water_valve_servo_pin) {
        water_valve_servo.write(servo_angle);
      } else if (servo_pin == light_switch_servo_pin) {
        light_switch_servo.write(servo_angle);
      }
    }
  }
  delay(100);
}

