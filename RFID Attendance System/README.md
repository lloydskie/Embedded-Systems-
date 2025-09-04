# RFID Attendance System

This folder contains multiple projects for RFID-based attendance tracking, including card reading, LCD display integration, and student data management. Several experiments and wiring diagrams are included.

## Features
- RFID card reading and authentication
- LCD/I2C display for user feedback
- Student data management and logging
- Multiple experimental versions and wiring diagrams

## Hardware Requirements
- Arduino Uno or compatible board
- MFRC522 RFID module
- LCD/I2C display
- Jumper wires, breadboard
- RFID cards/tags

## Folder Details
- `attendance-system/`: Basic RFID attendance system
- `card-holder-name/`: Card holder name display and reader
- `I2CLCDsketch_mar5a/`, `i2clcdsketch_mar6a/`: LCD/I2C display integration
- `LCDI2C_RFID_ARDUINOR3sketch_mar14b/`: Combined LCD and RFID system
- `MFRC522_ARDUINOR3_sketch_mar14a/`: MFRC522 RFID module integration
- `rfid_attendance3/`, `rfid-attendance3/`, `rfid-attendance4/`: Various versions of RFID attendance systems
- `rfid-based-automatic-door-system/`: RFID-based door automation
- `student-data/`, `student-data4/`: Student data management sketches
- `write-data/`: Writing data to storage

## Setup Instructions
1. Open the relevant `.ino` file in Arduino IDE.
2. Connect hardware as per wiring diagrams (see images in folders).
3. Upload the sketch to your Arduino board.
4. Test with RFID cards and observe LCD feedback.

## Usage
- Use each sketch for its intended hardware and experiment.
- Refer to code comments and wiring diagrams for setup and troubleshooting.

## License
Educational use only.
