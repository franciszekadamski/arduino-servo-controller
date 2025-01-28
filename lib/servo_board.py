import serial
import time
import threading


class ServoBoard:
    def __init__(
            self,
            verbose=False,
            baud_rate=9600,
            serial_port='/dev/ttyACM0',
            timeout=1
        ):
        self.verbose = verbose
        try:
            self.ser = serial.Serial(
                serial_port,
                baud_rate,
                timeout=timeout
            )
        except serial.SerialException as e:
            print(f"Exception when initializing serial: {e}")
            raise
        self.previous_angle = 0

    def set_position(self, pin, angle):
        try:
            if pin < 10:
                pin_string = f"0{pin}"
            else:
                pin_string = f"{pin}"

            if angle < 10:
                angle_string = f"00{angle}"
            elif 10 < angle < 100:
                angle_string = f"0{angle}"
            else:
                angle_string = f"{angle}"

            message = f"{pin_string}{angle_string}\n"
            self.ser.write(message.encode())
            if self.verbose:
                print(f'Sending: {message}')
        except serial.SerialException as e:
            print(f"Serial Exception: {e}")

    def run_sequence(self, pin, sequence):
        for angle in sequence:
            self.set_position(pin, angle)
            sleeping_time = round(abs(angle - self.previous_angle) / 350, 4)
            time.sleep(sleeping_time)
            self.previous_angle = angle

    def read(self):
        return self.ser.readline()

    def __del__(self):
        self.ser.close()
