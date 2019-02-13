# VFD-Clock
Firmware files for a VFD Tube Clock

Clean up branches from comments and formatting:
- [x] Power Management
- [x] Shift Reg
- [x] Switchs/Buttons
- [ ] I2C
- [ ] UART and SPI


Power Management:
- Goes to sleep:
  - [x] inbetween RTC updates
  - [ ] inbetween temp sensor reads
- [x] Wakes up on interrupt

Shift Registers:
- [x] Verify correct data is sent to shift reg's
- Verify correct sequence is shown on LEDs
  - [x] Does sequence [0xFF, 0xAA, 0x00, 0xAA]
  
Switches and Buttons:
- [x] Interrupts when buttons are pressed
- [x] Interrupts when switches are moved
- [x] 5s timer
- [x] 50ms timer
  
I2C:
- [ ] Send Data
- [ ] Receive Data
- [ ] Sample and Record Temperature
- [x] 10 second timer to trigger temp update

UART and SPI:
- SPI is nice to have, optional
- Can send and receive data
  - [x] UART
  - [ ] SPI
- Commands/Messages:
  - [ ] Set Time -> 0x11 followed by 0xHHMMSS
  - [ ] Reqest Temp Mode -> 0x22
  - [ ] request Clock Mode -> 0x33
  - [ ] ACK -> 0x44
  - [ ] NACK -> 0x55

Power Management -> Shift Regs -> Switches and Buttons -> I2C -> UART and SPI 
