#include <Servo.h>


Servo water_valve_servo;
Servo light_switch_servo;

int water_valve_servo_pin = 10;
int light_switch_servo_pin = 11;

int air_humidity_sensor_pin = A0;
int soil_humidity_sensor_pin = A1;
int air_temperature_sensor_pin = A2;

String in_message;
String in_message_pin;
String in_message_angle;

int servo_pin;
int servo_angle;

// settings
int mode = 1;
int send_settings = 0;
int send_sensor_data = 0;

int water_valve_angle_open = 90;
int water_valve_angle_close = 45;

int light_switch_angle_open = 90;
int light_switch_angle_close = 45;

unsigned long int light_switch_open_time_ms = 1000 * 60 * 60 * 16; // 57600 000 ms
unsigned long int light_switch_period_time_ms = 1000 * 60 * 60 * 24; // 86400 000 ms
unsigned long int light_switch_close_time_ms = light_switch_period_time_ms - light_switch_open_time_ms;

unsigned long int water_valve_open_time_ms = 1000 * 5;
unsigned long int water_valve_closed_time_ms = 1000 * 60 * 10;

int watering_state = 0; // 0: no watering; 1: watering;
int lighting_state = 0; // 0: light off; 1: light on

int watering_state_changed = 0;
int lighting_state_changed = 0;

int air_humidity_treshold = 120;
int soil_humidit_treshold = 50;
unsigned long int post_watering_wait_time_ms = 1000 * 60 * 10; // 600 000 ms
unsigned long int watering_time_ms = 30 * 1000;

unsigned long int watering_start_time_ms = 0;
unsigned long int lighting_start_time_ms = 0;
unsigned long int watering_stop_time_ms = 0;
unsigned long int lighting_stop_time_ms = 0;

unsigned long int up_time_ms = 0;

String water_valve_servo_pin_string;
String light_switch_servo_pin_string;
String water_valve_angle_open_string;
String water_valve_angle_close_string;
String light_switch_angle_open_string;
String light_switch_angle_close_string;
String light_switch_open_time_ms_string;
String light_switch_period_time_ms_string;
String water_valve_open_time_ms_string;
String water_valve_closed_time_ms_string;
String air_humidity_treshold_string;
String soil_humidit_treshold_string;
String post_watering_wait_time_ms_string;
String watering_time_ms_string;


void setup() {
  Serial.begin(9600);

  water_valve_servo.attach(water_valve_servo_pin);
  light_switch_servo.attach(light_switch_servo_pin);

  pinMode(air_humidity_sensor_pin, INPUT);
  pinMode(soil_humidity_sensor_pin, INPUT);
  pinMode(air_temperature_sensor_pin, INPUT);
}


void sendSensorData(
  double air_humidity,
  double soil_humidity,
  int air_temperature
) {
  if (send_sensor_data) {
    Serial.print(air_humidity);
    Serial.print('-');
    Serial.print(soil_humidity);
    Serial.print('-');
    Serial.println(air_temperature);

    send_sensor_data = 0;
  }
}


void receiveControlMessage() {
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
}


void receiveMessage() {
  while (Serial.available() > 0) {
    in_message = Serial.readStringUntil('\n');
    if (in_message.length() > 0 && in_message == "GET_SETTINGS") {
      send_settings = 1;
    } else if (in_message.length() > 0 && in_message == "GET_SENSOR_DATA") {
      send_sensor_data = 1;
    } else if (in_message.length() > 0 && in_message.substring(0, 12) == "SET_SETTINGS") {
      setSettings(in_message);
    }
  }
}


void setSettings(String in_message) {
    // example message: SET_SETTINGS1011090045090045576000008640000000005000006000001200500060000030000 
    water_valve_servo_pin_string = in_message.substring(12, 14); // int 2 digit long
    light_switch_servo_pin_string = in_message.substring(14, 16); // int 2 digit long
    water_valve_angle_open_string = in_message.substring(16, 19); // int 3 digit long
    water_valve_angle_close_string = in_message.substring(19, 22); // int 3 digit long
    light_switch_angle_open_string = in_message.substring(22, 25); // int 3 digit long
    light_switch_angle_close_string = in_message.substring(25, 28); // int 3 digit long
    light_switch_open_time_ms_string = in_message.substring(28, 36); // unsigned long int 8 digit long
    light_switch_period_time_ms_string = in_message.substring(36, 44); // unsigned long int 8 digit long
    water_valve_open_time_ms_string = in_message.substring(44, 52); // unsigned long int 8 digit long
    water_valve_closed_time_ms_string = in_message.substring(52, 60); // unsigned long int 8 digit long
    air_humidity_treshold_string = in_message.substring(60, 63); // int digit 3 long
    soil_humidit_treshold_string = in_message.substring(63, 66); // int digit 3 long
    post_watering_wait_time_ms_string = in_message.substring(66, 74); // unsigned long int 8 digit long
    watering_time_ms_string = in_message.substring(74, 82); // unsigned long int 8 digit long

    water_valve_servo_pin = water_valve_servo_pin_string.toInt();
    light_switch_servo_pin = light_switch_servo_pin_string.toInt();
    water_valve_angle_open = water_valve_angle_open_string.toInt();
    water_valve_angle_close = water_valve_angle_close_string.toInt();
    light_switch_angle_open = light_switch_angle_open_string.toInt();
    light_switch_angle_close = light_switch_angle_close_string.toInt();
    light_switch_open_time_ms = light_switch_open_time_ms_string.toInt();
    light_switch_period_time_ms = light_switch_period_time_ms_string.toInt();
    water_valve_open_time_ms = water_valve_open_time_ms_string.toInt();
    water_valve_closed_time_ms = water_valve_closed_time_ms_string.toInt();
    air_humidity_treshold = air_humidity_treshold_string.toInt();
    soil_humidit_treshold = soil_humidit_treshold_string.toInt();
    post_watering_wait_time_ms = post_watering_wait_time_ms_string.toInt();
    watering_time_ms = watering_time_ms_string.toInt();    
}


void sendSettings() {
  if (send_settings) {
    Serial.print(water_valve_servo_pin);
    Serial.print('-');
    Serial.print(light_switch_servo_pin);
    Serial.print('-');
    Serial.print(water_valve_angle_open);
    Serial.print('-');
    Serial.print(water_valve_angle_close);
    Serial.print('-');
    Serial.print(light_switch_angle_open);
    Serial.print('-');
    Serial.print(light_switch_angle_close);
    Serial.print('-');
    Serial.print(light_switch_open_time_ms);
    Serial.print('-');
    Serial.print(light_switch_period_time_ms);
    Serial.print('-');
    Serial.print(water_valve_open_time_ms);
    Serial.print('-');
    Serial.print(water_valve_closed_time_ms);
    Serial.print('-');
    Serial.print(air_humidity_treshold);
    Serial.print('-');
    Serial.print(soil_humidit_treshold);
    Serial.print('-');
    Serial.print(post_watering_wait_time_ms);
    Serial.print('-');
    Serial.println(watering_time_ms);

    send_settings = 0;
  }
}

void handleLightingStateBySchedule() {
  unsigned long currentMillis = millis();
  if (lighting_state == 0 &&
      currentMillis - lighting_stop_time_ms > light_switch_close_time_ms
  ) {
    lighting_state = 1;
    lighting_start_time_ms = currentMillis;
    lighting_state_changed = 1;
  } else if (lighting_state == 1 &&
      currentMillis - lighting_start_time_ms > light_switch_open_time_ms
  ) {
    lighting_state = 0;
    lighting_stop_time_ms = currentMillis;
    lighting_state_changed = 1;
  }
}


void handleWateringStateByValue(double air_humidity) {
  unsigned long int currentMillis = millis();
  if (watering_state == 0 &&
      air_humidity < air_humidity_treshold &&
      currentMillis - watering_stop_time_ms > water_valve_closed_time_ms
  ) {
    watering_state = 1;
    watering_start_time_ms = currentMillis;
    watering_state_changed = 1;
  } if (watering_state == 1 &&
      currentMillis - watering_start_time_ms > water_valve_open_time_ms
  ) {
    watering_state = 0;
    watering_stop_time_ms = currentMillis;
    watering_state_changed = 1;
  }
}


void adjustServos() {
  if (lighting_state_changed == 1) {
    light_switch_servo.write(
      lighting_state == 0 ? light_switch_angle_close : light_switch_angle_open
    );
    water_valve_servo.write(
      watering_state == 0 ? water_valve_angle_close : water_valve_angle_open
    );
    lighting_state_changed = 0;
  }
}


void loop() {
  // read sensors
  double air_humidity = analogRead(air_humidity_sensor_pin) * 1.9 / 3.3;
  double soil_humidity = 104 - (analogRead(soil_humidity_sensor_pin) / 7);
  int air_temperature = analogRead(air_temperature_sensor_pin);

  switch (mode) {
    case 0:
      receiveControlMessage();
      break;
    case 1:
      receiveMessage();
      handleLightingStateBySchedule();
      handleWateringStateByValue(air_humidity);
      adjustServos();
      break;
    default:
      receiveMessage();
      handleLightingStateBySchedule();
      handleWateringStateByValue(air_humidity);
      adjustServos();
      break;
  }
  sendSensorData(air_humidity, soil_humidity, air_temperature);
  sendSettings();  
  delay(100);
}
