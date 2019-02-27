# VFD-Clock
Firmware files for a VFD Tube Clock

Clean up branches from comments and formatting:
- [x] Power Management
- [x] Shift Reg
- [x] Switches/Buttons
- [x] I2C
- [x] UART


Power Management:
- Goes to sleep:
  - [x] inbetween RTC updates
  - [x] inbetween temp sensor reads
- [x] Wakes up on interrupt

Shift Registers:
- [x] Verify correct data is sent to shift reg's
- Verify correct sequence is shown on LEDs
  - [x] Display sequence [0xFF, 0xAA, 0x00, 0xAA]
  
Switches and Buttons:
- [x] Interrupts when buttons are pressed
- [x] Interrupts when switches are moved
- [x] 5s timer
- [x] 50ms timer
  
I2C:
- [x] Send Data
- [x] Receive Data
- [x] Sample and Record Temperature
- [x] 10 second timer to trigger temperature update
- [x] operate at 400kHz

UART:
- Can send and receive data using:
  - [x] UART
- Commands/Messages:
  - [x] Set Time -> 0x11 followed by 0xHHMMSS
  - [x] Request Temp Mode -> 0x22
  - [x] Request Clock Mode -> 0x33
  
                         Message Structure
 [ START_MSG | START_MSG | FUNCTION | DATA ... DATA | SIZE | END_MSG ]
 
                   Time Setting Message
 [ START_MSG | START_MSG | SET_TIME | HOURS | MINUTES | END_MSG ]
 
                Temp. Mode Request Message
 [ START_MSG | START_MSG | TEMP_MODE | END_MSG ]
 
              Clock Mode Request Message
 [ START_MSG | START_MSG | CLOCK_MODE | END_MSG ]

