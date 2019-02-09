# VFD-Clock
Firmware files for a VFD Tube Clock

I2C:
- [ ] Send Data
- [ ] Receive Data
- [ ] Sample and Record Temperature
- [ ] 10 second timer to trigger temp update

Switches and Buttons:
- [ ] Interrupts when buttons are pressed
- [ ] Interrupts when switches are moved

Shift Registers:
- [ ] Verify correct data is sent to shift reg's
- Verify correct sequence is shown on LEDs
  - [ ] Does sequence [0xFF, 0xAA, 0x00, 0xAA]
  
Power Management:
- Goes to sleep:
  - [ ] inbetween RTC updates
  - [ ] inbetween temp sensor reads
- [ ] Wakes up on any interrupt

UART and SPI:
- SPI is nice to have, optional
- Can send and receive data
  - [x] UART
  - [ ] SPI
- Commands/Messages:
  - [ ] SET_TIME:hh:mm:ss
  - [ ] TEMP_MODE
  - [ ] CLOCK_MODE
  - [ ] ACK
  - [ ] NACK
