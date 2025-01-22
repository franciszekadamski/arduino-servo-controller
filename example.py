#!/usr/bin/python3

from servo_board import ServoBoard

def main():
    board = ServoBoard()
    for pin in range(0, 14):
        for angle in [0, 45, 90, 135, 180, 90, 0]:
            board.send(pin, angle)


if __name__ == "__main__":
    main()
