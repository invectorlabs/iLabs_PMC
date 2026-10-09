# iLabs Power Management Controller (PMC) Arduino Library

The **iLabs PMC Arduino Library** provides an easy-to-use interface for communicating with the iLabs Power Management Controller (PMC) over I2C. This library enables advanced power management features for your Arduino projects, including sleep modes, LED control, timer configuration, and non-volatile memory access.

## Features

- **I2C Communication** with the iLabs PMC device
- **Register Read/Write** utilities
- **LED Control** for status indication
- **Sleep Timer** configuration and management
- **Peripheral Interval Timer (PIT)** setup
- **Non-Volatile Memory (NVM) Read/Write** (with chunked transfer support)
- **Command Interface** for advanced device control

## Getting Started

### Requirements

- Arduino IDE (1.8.x or newer)
- Compatible Arduino board with I2C support
- iLabs PMC hardware

### Installation

1. Clone or download this repository into your Arduino `libraries` folder:
    ```sh
    git clone https://github.com/invectorlabs/iLabs_PMC.git
    ```
2. Restart the Arduino IDE.

### Usage

Include the library in your sketch:

```cpp
#include <ilabs_pmc.h>
```

Initialize and begin communication with the PMC:

```cpp
PMC.begin();
```

Example: Blink the green LED, then power the board off for 10 seconds

```cpp
PMC.setLed(LED_REG_LED1_GREEN, true);    // Turn on the green LED
delay(500);
PMC.setLed(LED_REG_LED1_GREEN, false);   // and off again
PMC.sleep(10);                           // Power off for 10 seconds, does not return
```

`sleep()` converts seconds to sleep timer ticks for you. The same sleep with the
lower level calls sets the timer in ticks, which are 0.5 seconds each by default:

```cpp
PMC.setSleepTimer(20);                   // 20 ticks of 0.5 s = 10 seconds
PMC.command(CMD_SLEEP_2, false);         // Send the sleep command
```

### API Overview

- `bool begin();`  
  Initialize communication with the PMC device.

- `void setLed(uint8_t led, bool state);`  
  Control the state of LEDs.

- `uint8_t read_reg(uint8_t reg);`  
  Read a register value.

- `uint8_t write_reg(uint8_t reg, uint8_t data);`  
  Write a value to a register.

- `uint8_t command(uint8_t command, bool wait);`  
  Send a command to the PMC device.

- `uint8_t configurePmc(uint8_t divider, enum pit_ctrl_clk clk);`  
  Configure the timer system.

- `uint8_t setSleepTimer(uint16_t sleep_timer);`  
  Set the sleep timer.

- `uint8_t sleep(double seconds);`  
  Set the sleep timer from a time in seconds and power the board off. Does not return.
  The time must be at least one timer tick (0.5 s by default).

- `uint8_t setWakeupPins(uint16_t pins);`  
  Enable wake up pins, one bit per pin (bit 0 is wake up pin 0). The board then wakes
  on a rising edge on any enabled pin or when the sleep timer expires.

- `uint8_t getWakeupReason();`  
  Why the board was powered on: `WUP_POWER_ON`, `WUP_SLEEP_TIMER` or `WUP_WAKE_PIN`.

- `uint8_t getWakeupPin();`  
  Which wake up pin woke the board, numbered as in `setWakeupPins()`. Only valid when
  `getWakeupReason()` returns `WUP_WAKE_PIN`.

- `uint16_t getBatteryVoltage();`  
  Measure the battery voltage, in millivolts.

- `uint8_t readStatus();`  
  Read the raw status register (`NVRAM_CRC`, `NVRAM_DIRTY` and wake up reason bits).

- `void readNVram(uint8_t *array, uint16_t offset, uint16_t len);`  
  Read data from NVM.

- `uint8_t writeNVram(uint8_t *array, uint16_t offset, int len);`  
  Write data to NVM.

- `void readNVram32(uint8_t *array, uint16_t offset, uint8_t len);`  
  `uint8_t writeNVram32(uint8_t *array, uint16_t offset, uint8_t len);`  
  Single transfer versions of the above, limited to 32 bytes per read and 30 bytes per write.

## Documentation

Full API documentation is available in the source code via Doxygen comments.

## License

This library is released under the MIT License. See [LICENSE](LICENSE) for details.

## Author

Pontus  
2025

---

*For questions, issues, or contributions, please open an issue or pull request on GitHub.*

