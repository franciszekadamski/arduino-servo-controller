#!/usr/bin/python3

from servo_board import ServoBoard

def main():
    board = ServoBoard(verbose=True)
    for pin in range(12, 14):
        for angle in [0, 45, 90, 135, 180, 90, 0]:
            board.set_position(pin, angle)

if __name__ == "__main__":
    main()
