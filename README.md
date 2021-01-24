# IOT device with ethernet connection

## Hardware

- Arduino Uno board
- Ethernet W5500 module connects to Arduino Uno board through a SPI connection (pin 10, 11, 12, 13 on Arduino Uno board)
- Temperature sensor
- GPS NEO 6M (ublox)

##Firmware features
- DHCP client for automatic acquiring IP address
- TCP server that can accept n clients at a same time.
- Multi sensors and modules.
