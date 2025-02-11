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

int water_valve_angle_open = 90;
int water_valve_angle_close = 45;

int light_switch_angle_open = 90;
int light_switch_angle_close = 45;

unsigned long int light_switch_open_time_ms = 1000 * 60 * 60 * 16; // 57600 000 ms
unsigned long int light_switch_period_time_ms = 1000 * 60 * 60 * 24; // 86400 000 ms

int watering_state = 0; // 0: no watering; 1: watering;
int lighting_state = 0; // 0: light off; 1: light on

int air_humidity_treshold = 85;
int soil_humidit_treshold = 50;
unsigned long int post_watering_wait_time_ms = 1000 * 60 * 10; // 600 000 ms

void setup() {
  Serial.begin(9600);

  water_valve_servo.attach(water_valve_servo_pin);
  light_switch_servo.attach(light_switch_servo_pin);

  pinMode(air_humidity_sensor_pin, INPUT);
  pinMode(soil_humidity_sensor_pin, INPUT);
  pinMode(air_temperature_sensor_pin, INPUT);
}

void loop() {
  double air_humidity = analogRead(air_humidity_sensor_pin) * 1.9 / 3.3;
  double soil_humidity = 104 - (analogRead(soil_humidity_sensor_pin) / 7);
  int air_temperature = analogRead(air_temperature_sensor_pin);

  Serial.print(air_humidity);
  Serial.print('-');
  Serial.print(soil_humidity);
  Serial.print('-');
  Serial.println(air_temperature);

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

  if (air_humidity < air_humidity_treshold) {
    watering_state = 1;
  }
  delay(100);
}

