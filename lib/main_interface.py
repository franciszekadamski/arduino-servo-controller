from queue import deque
from statistics import median
import os
import threading
import cv2
import numpy as np
import time
from datetime import datetime
import zmq

from lib.servo_board import ServoBoard


class MainInterface:
    def __init__(
            self,
            data_dir='./data/src/',
            camera_index=1,
            model_path='./models/mobilenet_best.keras',
            port=5556
        ):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        # self.socket.connect(f"tcp://localhost:{port}")
        
        self.recent_data = deque(maxlen=155)
        self.tresholds = {
            "air_humidity": 85
        }

        self.running = True
        self._data_dir = data_dir
        self._camera_index = camera_index
        self._capture = cv2.VideoCapture(self._camera_index)
        self.ret = False
        self.text = "Classifying..."
        self._gui_thread = threading.Thread(target=self._main_loop)
        self._gui_thread.start()

        self.board = ServoBoard(verbose=True)
        self.angle_deg = 95
        self.current_pin = 13
        
        self._decider_min_frequency_hz = 1
        self._decider_thread = threading.Thread(target=self._continuous_decision_making)
        self._decider_thread.start()

    def _main_loop(self):
        while self.running:
            self.ret, self.frame = self._capture.read()
            if not self.ret:
                self.frame = np.zeros(shape=(200, 500, 3), dtype=np.uint8)
            cv2.putText(
                self.frame,
                self.text,
                (50, 50),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (0, 0, 0),
                2,
                cv2.LINE_AA
            )
            cv2.imshow(f'Camera {self._camera_index}', self.frame)

            # self.socket.send(bytes(f"{self.state}", "utf-8"))
            
            key = cv2.waitKey(1) & 0xFF
            if key == ord('c'):
                self._save_picture(self.frame)
            elif key == ord('p'):
                self._press_once(10)
                self._press_once(11)
            elif key == ord('q'):
                self._terminate()

    def get_medians(self):
        if not self.recent_data:  # Handle empty case
            return {"air_mean": 0, "soil_mean": 0, "temperature_mean": 0}

        air_mean = median(d["air_humidity_percent"] for d in self.recent_data)
        soil_mean = median(d["soil_humidity_percent"] for d in self.recent_data)
        temperature_mean = median(d["air_temperature_celsius"] for d in self.recent_data)

        return {"air_mean": air_mean, "soil_mean": soil_mean, "temperature_mean": temperature_mean}

    def _continuous_decision_making(self):
        while self.running:            
            # message = ""
            # message = self.socket.recv()
            # message = str(message)
            # if message == "screenshot":
            #     self._save_picture(self.frame)
            # elif message == "press-once":
            #     self._press_once()
            try:
                air, soil, temperature = self.board.read().split('-')
                if air == '':
                    air = 0
                if soil == '':
                    soil = 0
                if temperature == '':
                    temperature = 0
            except:
                air = 0
                soil = 0
                temperature = 0
            data = {
                "air_humidity_percent": 1.9 * int(air) / 3.3,
                "soil_humidity_percent": 100 - (int(soil) / 7) + 4,
                "air_temperature_celsius": int(float(temperature) - 6)
            }

            self.recent_data.append(data)
            air_p, soil_p, temperature_c = self.get_medians().values()

            air_humidity_percent = round(air_p, 1)
            soil_humidity_percent = round(soil_p, 1)
            air_temperature_celsius = round(temperature_c, 1)

            self.text = f"Ah:{air_humidity_percent}% Sh:{soil_humidity_percent}% At:{air_temperature_celsius}C"

            # if air_humidity_percent < self.tresholds["air_humidity"]:
            # if air_humidity_percent < 85:
            #     self._press_once(10)
            #     self._press_once(11)
            #     time.sleep(2)

            # time.sleep(1)


    def _press_once(self, pin):
            self.board.run_sequence(pin, [50, 95])

    def _save_picture(self, frame):
        print("Saving")
        if frame is not None:
            filename = os.path.join(
                self._data_dir,
                f"{datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg"
            )
            cv2.imwrite(filename, frame)
            print("Saved")

    def _terminate(self):
        self.running = False
        self._capture.release()
        cv2.destroyAllWindows()

    def __del__(self):
        self._terminate()
