#!/usr/bin/python3

import sys
sys.path.append("../")
from lib.main_interface import MainInterface

def main():
    interface = MainInterface(camera_index=2)
    

if __name__ == "__main__":
    main()
