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

// settings
int water_valve_angle_open = 90;
int water_valve_angle_close = 45;

int light_switch_angle_open = 90;
int light_switch_angle_close = 45;

unsigned long int light_switch_open_time_ms = 1000 * 60 * 60 * 16; // 57600 000 ms
unsigned long int light_switch_period_time_ms = 1000 * 60 * 60 * 24; // 86400 000 ms
unsigned long int light_switch_close_time_ms = light_switch_period_time_ms - ligth_switch_open_time_ms;

int watering_state = 0; // 0: no watering; 1: watering;
int lighting_state = 0; // 0: light off; 1: light on

int watering_state_changed = 0;
int lighting_state_changed = 0;

int air_humidity_treshold = 85;
int soil_humidit_treshold = 50;
unsigned long int post_watering_wait_time_ms = 1000 * 60 * 10; // 600 000 ms
unsigned long int watering_time_ms = 30 * 1000;

unsigned long int watering_start_time_ms = 0;
unsigned long int lighting_start_time_ms = 0;
unsigned long int watering_stop_time_ms = 0;
unsigned long int lighting_stop_time_ms = 0;

unsigned long int up_time_ms = 0;

void setup() {
  Serial.begin(9600);

  water_valve_servo.attach(water_valve_servo_pin);
  light_switch_servo.attach(light_switch_servo_pin);

  pinMode(air_humidity_sensor_pin, INPUT);
  pinMode(soil_humidity_sensor_pin, INPUT);
  pinMode(air_temperature_sensor_pin, INPUT);
}


void loop() {
  // read sensors
  double air_humidity = analogRead(air_humidity_sensor_pin) * 1.9 / 3.3;
  double soil_humidity = 104 - (analogRead(soil_humidity_sensor_pin) / 7);
  int air_temperature = analogRead(air_temperature_sensor_pin);

  // send data
  Serial.print(air_humidity);
  Serial.print('-');
  Serial.print(soil_humidity);
  Serial.print('-');
  Serial.println(air_temperature);

  // receive control signals
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

  // check conditions and modify state
  lighting_state_changed = 0;
  if (lighting_state == 0) {
    if (milis() - lighting_stop_time_ms > ligth_switch_close_time_ms) {
      lighting_state = 1;
      lighting_start_time_ms = milis();
      lighting_state_changed = 1;
    }
  } else if (lighting_state == 1) {
    if (milis() - lighting_start_time_ms > light_switch_open_time_ms) {
      lighting_state = 0;
      lighting_stop_time_ms = milis();
      lighting_state_changed = 1;
    }
  }

  // adjust servos to the current state if changed
  if (lighting_state_changed == 1) {
    if (lighting_state == 0) {
      light_switch_servo.write(light_switch_angle_close)
    } else if (lighting_state == 1) {
      light_switch_servo.write(light_switch_angle_open)
    }

    if (watering_state == 0) {
      water_valve_servo.write(water_valve_angle_close)
    } else if (watering_state == 1) {
      water_valve_servo.write(water_valve_angle_open)
    }
  }

  delay(100);
}

// // Function to handle lighting state changes
// void handleLightingState() {
//   unsigned long currentMillis = milis();

//   // Check if lighting needs to be turned on (state 0 -> 1)
//   if (lighting_state == 0 && currentMillis - lighting_stop_time_ms > ligth_switch_close_time_ms) {
//     lighting_state = 1;
//     lighting_start_time_ms = currentMillis;
//     lighting_state_changed = 1;
//   }
//   // Check if lighting needs to be turned off (state 1 -> 0)
//   else if (lighting_state == 1 && currentMillis - lighting_start_time_ms > light_switch_open_time_ms) {
//     lighting_state = 0;
//     lighting_stop_time_ms = currentMillis;
//     lighting_state_changed = 1;
//   }
// }

// // Function to adjust servos based on state
// void adjustServos() {
//   if (lighting_state_changed == 1) {
//     // Adjust lighting servo based on state
//     light_switch_servo.write(lighting_state == 0 ? light_switch_angle_close : light_switch_angle_open);

//     // Adjust watering valve servo based on watering state
//     water_valve_servo.write(watering_state == 0 ? water_valve_angle_close : water_valve_angle_open);

//     // Reset the state change flag after adjustment
//     lighting_state_changed = 0;
//   }
// }

// void loop() {
//   // Handle lighting state changes
//   handleLightingState();

//   // Adjust servos if any state change occurred
//   adjustServos();
// }
