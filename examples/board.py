#!/usr/bin/python3

import sys
sys.path.append("../")
from lib.servo_board import ServoBoard

def main():
    board = ServoBoard(verbose=True)
    for pin in range(13, 14):
        for angle in [95, 50, 95, 50, 95, 50, 95]:
            board.set_position(pin, angle)

if __name__ == "__main__":
    main()
