import os
import threading
import cv2
import numpy as np
import time
from datetime import datetime

from lib.servo_board import ServoBoard


class MainInterface:
    def __init__(
            self,
            data_dir='./data/src/',
            camera_index=1,
            model_path='./models/mobilenet_best.keras'
        ):
        self.running = True
        self._data_dir = data_dir
        self._camera_index = camera_index
        self._capture = cv2.VideoCapture(self._camera_index)
        self.ret = False
        self.text = "Classifying..."
        self._gui_thread = threading.Thread(target=self._main_loop)
        self._gui_thread.start()

        self.board = ServoBoard()
        self.angle_deg = 95
        self.current_pin = 13
        
        self._decider_min_frequency_hz = 1
        self._decider_thread = threading.Thread(target=self._continuous_decision_making)
        self._decider_thread.start()

    def _main_loop(self):
        while self.running:
            self.ret, self.frame = self._capture.read()
            if not self.ret:
                self.frame = np.zeros(shape=(200, 200, 3), dtype=np.uint8)
            cv2.putText(
                self.frame,
                self.text,
                (50, 50),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (255, 0, 0),
                2,
                cv2.LINE_AA
            )
            cv2.imshow(f'Camera {self._camera_index}', self.frame)
            key = cv2.waitKey(1) & 0xFF
            if key == ord('c'):
                self._save_picture(self.frame)
            elif key == ord('p'):
                self._press_once()
            elif key == ord('q'):
                self._terminate()

    def _continuous_decision_making(self):
        while self.running:
            if self.ret:
                conclusion = 'low_humidity'
                if conclusion == 'low_humidity':
                    self.text = "Too low humidity"
                elif prediction == 'high_humidity':
                    self.text = "Too high humidity"
                else:
                    self.text = "Who knows what is that..."
            time.sleep(1 / self._decider_min_frequency_hz) 

    def _press_once(self):
        self.board.run_sequence(self.current_pin, [50, 95])

    def _save_picture(self, frame):
        if frame is not None:
            filename = os.path.join(
                self._data_dir,
                f"{datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg"
            )
            cv2.imwrite(filename, frame)

    def _terminate(self):
        self.running = False
        self._capture.release()
        cv2.destroyAllWindows()

    def __del__(self):
        self._terminate()
