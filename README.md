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
    git clone https://github.com/yourusername/iLabs_PMC.git
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

Example: Set an LED and put the device to sleep for 10 seconds

```cpp
PMC.setLed(LED1, true);         // Turn on LED1
PMC.setSleepTimer(10);          // Set sleep timer for 10 seconds
PMC.command(CMD_SLEEP_2);       // Send sleep command
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

- `void command(uint8_t command);`  
  Send a command to the PMC device.

- `uint8_t configurePmc(uint8_t divider, enum pit_ctrl_clk clk);`  
  Configure the timer system.

- `uint8_t setSleepTimer(uint16_t sleep_timer);`  
  Set the sleep timer.

- `void readNVram(uint8_t *array, uint16_t offset, uint16_t len);`  
  Read data from NVM.

- `uint8_t writeNVram(uint8_t *array, uint16_t offset, int len);`  
  Write data to NVM.

## Documentation

Full API documentation is available in the source code via Doxygen comments.

## License

This library is released under the MIT License. See [LICENSE](LICENSE) for details.

## Author

Pontus  
2025

---

*For questions, issues, or contributions, please open an issue or pull request on GitHub.*

