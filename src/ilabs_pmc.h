/*
 * iLabs Power Management Controller (PMC) I2C register definitions
 * This section was cut from the firmware file Challenger_2040_LoRa_ZP.ino
 * Remembed to update this section if the firmware is updated!
 * addr: 0 (Write)  - PIT Control register (PIT_CTRL_REG)

   Control register for the Periodic Interrupt Timer (PIT).
   The PIT is responsible for keeping the time while the system is sleeping
   in low power mode. As described in the data sheet "The timing of the
   first PIT interrupt and the first RTC count tick will be unknown (anytime
   between enabling and a full period)". To work around this problem the
   interrupt period is exposed here which allows the system designer to
   select an appropriate period for generating the delay.

   If CYC4 is selected the resolution of the interrupts will be 1/32768 * 4
   seconds = 122uS and if CYC32768 is selected the resolution will be 1/32768
   32768 = 1Second.

   The selected tick period is also used to increment the 16-bit sleep timer
   which gives the system designer freedom to optimize for time between woken
   periods and current consumption.

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- PERIDO[0]
       | | | | | | +--- PERIOD[1]
       | | | | | +----- PERIOD[2]
       | | | | +------- PERIOD[3]
       | | | +--------- Not implemented
       | | +----------- Not implemented
       | +------------- Not implemented
       +--------------- 32768Hz/1024Hz

*/
#define PIT_CTRL_REG        0
#define PIT_CTRL_OFF        0
#define PIT_CTRL_CYC4       1
#define PIT_CTRL_CYC8       2
#define PIT_CTRL_CYC16      3
#define PIT_CTRL_CYC32      4
#define PIT_CTRL_CYC64      5
#define PIT_CTRL_CYC128     6
#define PIT_CTRL_CYC256     7
#define PIT_CTRL_CYC512     8
#define PIT_CTRL_CYC1024    9
#define PIT_CTRL_CYC2048    10
#define PIT_CTRL_CYC4096    11
#define PIT_CTRL_CYC8192    12
#define PIT_CTRL_CYC16384   13 /* Default (0.5S) */
#define PIT_CTRL_CYC32768   14
enum pit_ctrl_clk {
  PIT_CTRL_CLK_32768 = 0,
  PIT_CTRL_CLK_1024 = 1
};
/*
   Register map of the I2C registers.

   addr: 1 (Write)  - Control register (CTRL_REG0)

   Controls interrups and reset sources.

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Interrupt enable
       | | | | | | +--- Enable sleep mode
       | | | | | +----- Reset device
       | | | | +------- Enable WU1
       | | | +--------- Enable WU2
       | | +----------- Enable WU3
       | +------------- Enable RXD wakeup
       +--------------- Enable D5 (DI0) wakeup

   Bit 0 : Interrupt enable (Default : 0)
     Setting this bit high will enable interrupts when a wakeup condition
     occurs. The interrupt comes out on CHGR_INT pin and is push/pull active low.

   Bit 1 : Enable sleep mode (Default : 0)
     This bit enables/disables the deep sleep mode functionality of the PMD.
     Whenever a deep sleep mode condition is detected it will force the CHGR_EN
     pin low, shutting down the Challenger board and stay low for the duration
     of the deep sleep period.

   Bit 2 : Reset device (Default : 0)
     Setting this bit to 1 will reset the device into a known state.

   Bit 3 : Enable wake up source 1 (Default : 0)
     Setting this bit high will enable wakeup source 1

   Bit 4 : Enable wake up source 2 (Default : 0)
     Setting this bit high will enable wakeup source 2

   Bit 5 : Enable wake up source 3 (Default : 0)
     Setting this bit high will enable wakeup source 3

   Bit 6 : Enable wake up source RXD (Default : 0)
     Setting this bit high will enable wakeup source RXD

   Bit 7 : Enable wake up source D5/DIO0 (Default : 0)
     Setting this bit high will enable wakeup source D5/DIO0

*/
#define CTRL_REG0                   1
#define CTRL_REG0_INTERRUPT_ENABLE  0x01
#define CTRL_REG0_SLEEP_ENABLE      0x02
#define CTRL_REG0_RESET             0x04
#define CTRL_REG0_ENABLE_WU1        0x08
#define CTRL_REG0_ENABLE_WU2        0x10
#define CTRL_REG0_ENABLE_WU3        0x20
#define CTRL_REG0_ENABLE_WU_RXD     0x40
#define CTRL_REG0_ENABLE_WU_DIO0    0x80

/* addr: 2 (Write)  - Control register (CTRL_REG1)

   Expansion of control register 0 for more complex designs

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Not implemented
       | | | | | | +--- Not implemented
       | | | | | +----- Not implemented
       | | | | +------- Not implemented
       | | | +--------- Not implemented
       | | +----------- Not implemented
       | +------------- Not implemented
       +--------------- Not implemented

*/
#define CTRL_REG1       2

/*
   addr: 3 (Write)  - Polarity register (POL_REG0)

   Polarity control register.

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Interrupt polarity
       | | | | | | +---
       | | | | | +-----
       | | | | +------- Polarity WU1
       | | | +--------- Polarity WU2
       | | +----------- Polarity WU3
       | +------------- Polarity RXD wakeup
       +--------------- Polarity D5 (DI0) wakeup

   Bit 0 : Interrupt enable (Default : 0)
     Setting this bit high will invert the function of the interrupt pin. By
     default interrupts are active low, setting this bit to "1" will make the
     interrupt output active high.

   Bit 3 : Polarity of wake up source 1 (Default : 0)
     Setting this bit high will invert the polarity of wakeup source 1. By
     default (bit set to "0") this is a positive wakeup source i.e. the
     system will wake up on a positive signal change. By setting thus bit
     to "1" the system will now wake up on a negative signal change.

   Bit 4 : Polarity of wake up source 2 (Default : 0)
     Setting this bit high will invert the polarity of wakeup source 1. By
     default (bit set to "0") this is a positive wakeup source i.e. the
     system will wake up on a positive signal change. By setting thus bit
     to "1" the system will now wake up on a negative signal change.

   Bit 5 : Polarity of wake up source 3 (Default : 0)
     Setting this bit high will invert the polarity of wakeup source 1. By
     default (bit set to "0") this is a positive wakeup source i.e. the
     system will wake up on a positive signal change. By setting thus bit
     to "1" the system will now wake up on a negative signal change.

   Bit 6 : Polarity of wake up source RXD (Default : 0)
     Setting this bit high will invert the polarity of wakeup source 1. By
     default (bit set to "0") this is a positive wakeup source i.e. the
     system will wake up on a positive signal change. By setting thus bit
     to "1" the system will now wake up on a negative signal change.

   Bit 7 : Polarity of wake up source D5/DIO0 (Default : 0)
     Setting this bit high will invert the polarity of wakeup source 1. By
     default (bit set to "0") this is a positive wakeup source i.e. the
     system will wake up on a positive signal change. By setting thus bit
     to "1" the system will now wake up on a negative signal change.

*/
#define POL_REG0                  3
#define POL_REG0_INTERRUPT     0x01
#define POL_REG0_WU1           0x08
#define POL_REG0_WU2           0x10
#define POL_REG0_WU3           0x20
#define POL_REG0_WU_RXD        0x40
#define POL_REG0_WU_DIO0       0x80

/* addr: 4 (Write)  - Polarity register (POL_REG1)

   Expansion of polarity register 0 for more complex designs

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Not implemented
       | | | | | | +--- Not implemented
       | | | | | +----- Not implemented
       | | | | +------- Not implemented
       | | | +--------- Not implemented
       | | +----------- Not implemented
       | +------------- Not implemented
       +--------------- Not implemented

*/
#define POL_REG1       4

/* addr: 5 (Write)   - Sleep timer (SLEEP_TMR_LO)

   The least significant byte of the 16-bit sleep timer.
   Sleep time is measured in PIT ticks, whose length is set in PIT_CTRL_REG
   (0.5 seconds by default). With the default tick the PMC can force the
   connected challenger board into sleep between 0.5 seconds and 65535 ticks,
   about 9 hours.


      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Bit 0
       | | | | | | +--- Bit 1
       | | | | | +----- Bit 2
       | | | | +------- Bit 3
       | | | +--------- Bit 4
       | | +------------Bit 5
       | +------------- Bit 6
       +--------------- Bit 7
*/
#define SLEEP_TMR_LO    5

/* addr: 6 (Write)   - Sleep timer (SLEEP_TMR_HI)

   The most significant byte of the 16-bit sleep timer.


      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Bit 8
       | | | | | | +--- Bit 9
       | | | | | +----- Bit 10
       | | | | +------- Bit 11
       | | | +--------- Bit 12
       | | +------------Bit 13
       | +------------- Bit 14
       +--------------- Bit 15
*/
#define SLEEP_TMR_HI    6

/* addr: 7 (Write)   - Led control register (LED_REG)

   This controls the LED's on the board. Pins are configured
   as outputs and set to 0 at reset.


      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- LED 1 (Green)
       | | | | | | +--- LED 2 (Red)
       | | | | | +----- Bit 10
       | | | | +------- Bit 11
       | | | +--------- Bit 12
       | | +------------Bit 13
       | +------------- Bit 14
       +--------------- Bit 15
*/
#define LED_REG                7
#define LED_REG_LED1_GREEN  0x01
#define LED_REG_LED2_RED    0x02

/* addr: 8 (Write)   - Command register (CMD_REG)

   This register accepts a number of different commands that
   are used to place the system in different states. Currently
   the following commands are defined and implemented.

   CMD_SYS_RESET    0x01 - Resets the system by pulling the reset
                           line of the system low.
   CMD_SLEEP_1      0x02 - Sets the system in sleep mode and wakes
                           up after the time defined in the sleep
                           timer.
   CMD_SLEEP_2      0x03 - Enters sleep and only wakes up on any of
                           the enabled wake up sources defined in
                           CTRL_REG.
   CMD_SLEEP_3      0x04 - Enters sleep and wakes up either after
                           the defined time in the sleep timer or
                           when any of the wake up sources are
                           triggered.
   CMD_CLEAR_NVRAM  0x05 - Clears the NVRAM and updates the CRC

*/
#define CMD_REG         8
#define CMD_SYS_RESET   0x01
#define CMD_SLEEP_1     0x02
#define CMD_SLEEP_2     0x03
#define CMD_SLEEP_3     0x04
#define CMD_CLEAR_NVRAM 0x05

/* addr: 8 (Read)   - Status Register (CMD_STAT)

   Holds the wake up reason and the NVRAM status.

      +-+-+-+-+-+-+-+-+
      |7|6|5|4|3|2|1|0|
      +-+-+-+-+-+-+-+-+
       | | | | | | | |
       | | | | | | | +- Bit 0 \
       | | | | | | +--- Bit 1 -- wakeup pin field
       | | | | | +----- Bit 2 /
       | | | | +------- Bit 3 - Not used, always 0
       | | | +--------- Bit 4 - Wake up reason 0
       | | +------------Bit 5 - Wake up reason 1
       | +------------- NVRAM Dirty bit, set on system power on indicating that the NVRAM is dirty.
       +--------------- Checks the CRC of the NVRAM, 1 = CRC Pass, 0 = CRC check did not pass

   The wakeup pin field holds the number of the wake up pin that ended the
   last sleep. It is only valid when the wake up reason is WUP_WAKE_PIN.
*/
#define CMD_STAT        CMD_REG
#define NVRAM_CRC       0x80
#define NVRAM_DIRTY     0x40
#define WUP_REASON1     0x20
#define WUP_REASON0     0x10
#define WUP_REASON_MASK 0x30
#define WUP_REASON_SHFT 0x04
#define WUP_PORT        0x08
#define WUP_PIN2        0x04
#define WUP_PIN1        0x02
#define WUP_PIN0        0x01
#define WUP_PIN_MASK    0x07

#define WUP_POWER_ON    0x00
#define WUP_SLEEP_TIMER 0x01
#define WUP_WAKE_PIN    0x02
#define WUP_RXD         0x03

/* addr: 0x0A and 0x0B (Read) - Battery voltage (BAT_VOLT_REG)

   This register returns the last current voltage of the connected battery.
   The returned value is of uint16_t type.
   
*/
#define BAT_VOLT_REG    0x0A
#define BAT_VOLT_REG_LO 0x0A
#define BAT_VOLT_REG_HI 0x0B

/* addr: 0x0A and 0x0B (Write) - Battery limit (BAT_LIM_REG)

   This register sets the lower limit of the accepted battery voltage.
   When this limit is reached the PMC will either wake the main MCU up
   to inform that the battery is running out, or optionally if the
   system is already up and running it will generate an interrupt to
   the MCU.

   This function is enabled by writing the desired level to this register
   and can be disabled by writing 0xFFFF here.
*/
#define BAT_LIM_REG     0x0A
#define BAT_LIM_REG_LO  0x0A
#define BAT_LIM_REG_HI  0x0B


/* addr: 0x80/0xC0 (Write/Read) - Read and Write to the NVM memory bank

   These registers are used to pass data to and from the NVM data bank.
   The lower 2 bits holds address bits 0 and 1 and then it needs to be
   followed by the high byte of the high address bits into the NVM memory
   bank (address bits 2 to 9).

   Register 0x80 is used to write data into the memory bank. The address
   bytes are followed by the data bytes to write.

   Register 0xC0 is used to read data from the bank. The address bytes are
   followed by the number of bytes to read, low byte first. The next I2C
   read then returns that many bytes.

   Examples

   Write 4 bytes to nvm bank example at position 514 (0x202):
   0x82 0x80 - 0x01 0x02 0x03 0x04

   Read 4 bytes from position 514 (0x202):
   0xC2 0x80 0x04 0x00, then read 4 bytes

*/

#ifndef ILABS_PMC_H
#define ILABS_PMC_H

#include <Arduino.h>
#include <Wire.h>

class PMCClass {
public:
    PMCClass();
    PMCClass(uint8_t address) : i2c_address(address) {};

    uint8_t write_reg(uint8_t, uint8_t);
    uint8_t read_reg(uint8_t); 
    bool begin();
    void setLed(uint8_t, bool);
    uint8_t readStatus();
    uint8_t command(uint8_t, bool);
    uint8_t configurePmc(uint8_t, enum pit_ctrl_clk);
    uint8_t setWakeupPins(uint16_t);
    uint8_t setSleepTimer(uint16_t);
    uint8_t sleep(double);
    uint8_t getWakeupReason();
    uint8_t getWakeupPin();
    uint16_t getBatteryVoltage();
    uint8_t writeNVram32(uint8_t *, uint16_t, uint8_t);
    uint8_t writeNVram(uint8_t *, uint16_t, int);
    void readNVram32(uint8_t *, uint16_t, uint8_t);
    void readNVram(uint8_t *, uint16_t, uint16_t);

private:
    uint8_t i2c_address;
    uint8_t led_reg_shadow = 0;
    int wakeupReason = -1;
    uint8_t g_divider = PIT_CTRL_CYC16384;
    enum pit_ctrl_clk g_clk = PIT_CTRL_CLK_32768;
};

extern PMCClass PMC;

#endif // ILABS_PMC_H