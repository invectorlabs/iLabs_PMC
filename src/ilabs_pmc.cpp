/**
 * @file ilabs_pmc.cpp
 * @brief Implementation file for the iLabs PMC (Power Management Controller) Arduino library.
 *
 * This file contains the implementation of the PMCClass, which provides methods to communicate
 * with the iLabs PMC device over I2C. The class allows reading and writing registers, controlling LEDs,
 * and initializing communication with the device.
 *
 * @author Pontus
 * @date 2025-10-09
 */

#include "ilabs_pmc.h"

// Constructor
PMCClass::PMCClass() {
    i2c_address = I2C_DEFAULT_ADDRESS;
}

/**
 * @brief Write a value to a register on the PMC device.
 * 
 * @param reg  The register address to write to.
 * @param data The data byte to write.
 */
uint8_t PMCClass::write_reg(uint8_t reg, uint8_t data) {
    Wire.beginTransmission(0x18);
    Wire.write(reg);
    Wire.write(data);
    return Wire.endTransmission();
}

/**
 * @brief Read a value from a register on the PMC device.
 * 
 * @param reg The register address to read from.
 * @return uint8_t The value read from the register.
 */
uint8_t PMCClass::read_reg(uint8_t reg) {
    Wire.beginTransmission(i2c_address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, (uint8_t)1);
    return Wire.read();
}

/**
 * @brief Set the state of an LED on the PMC device.
 * 
 * @param led   The LED bitmask to set or clear.
 * @param state True to turn the LED on, false to turn it off.
 */
void PMCClass::setLed(uint8_t led, bool state) {
    if (state) {
        led_reg_shadow |= led;
        write_reg(LED_REG, led_reg_shadow);
    } else {
        led_reg_shadow &= ~led;
        write_reg(LED_REG, led_reg_shadow);
    }
}

/**
 * @brief Initialize communication with the PMC device.
 * 
 * @return true  If the device was found and communication was established.
 * @return false If the device was not found or communication failed.
 */
bool PMCClass::begin() {
  Wire.begin();
  Wire.beginTransmission(i2c_address);
  int error = Wire.endTransmission();

  if (error)
    return false;
  return true;
}

/**
 * @brief Read the status from the PMC device.
 * 
 * This method is currently not implemented.
 */
uint8_t PMCClass::readStatus() {
    return PMC.read_reg(CMD_STAT);
}

/**
 * @brief Send a command to the PMC device.
 *
 * This method writes a command byte to the PMC device using the appropriate register.
 *
 * @param command The command byte to send.
 * @return true if the command was sent successfully, false otherwise.
 */
uint8_t PMCClass::command(uint8_t command, bool wait) {
    uint8_t error = write_reg(CMD_REG, command);
    if (wait)
        delay(100);
    return error;
}

/**
 * @brief Configure the PMC device's timer system.
 *
 * This method sets up the Peripheral Interval Timer (PIT) with the specified
 * divider and clock source.This time base is used to internally run a 16 bit timer
 * that determines the amount of time a system should sleep for. The PIT can be
 * configured to run at either 32.768KHz or 1024Hz and have a prescaler that 
 * creates the time base. This prescaler (divider) can be configured in 14 steps
 * as follows:
 *
 *   CLK / 4
 *   CLK / 8
 *   CLK / 16
 *   CLK / 32
 *   CLK / 64
 *   CLK / 128
 *   CLK / 256
 *   CLK / 512
 *   CLK / 1024
 *   CLK / 2048
 *   CLK / 4096
 *   CLK / 8192
 *   CLK / 16384
 *   CLK / 32768
 *
 * The sleep time can be derived from the following formula
 *
 *   t = sleep_timer / (freq / divider)
 *
 *       freq={1024 | 32768}

 *
 * @param divider The prescaler divider value.
 * @param clk The clock source for the timer (1024Hz or 32768Hz).
 * @return true if the command was sent successfully, false otherwise.
 */
uint8_t PMCClass::configurePmc(uint8_t divider, enum pit_ctrl_clk clk) {
    if (divider < PIT_CTRL_CYC4 || divider > PIT_CTRL_CYC32768)
      return false;

    // Update instance settings
    g_divider = divider;
    g_clk = clk;

    uint8_t val = divider;
    if (clk == PIT_CTRL_CLK_1024)
      val |= 0x80;

    Wire.beginTransmission(0x18);
    Wire.write(PIT_CTRL_REG);
    Wire.write(val);
    return Wire.endTransmission();
}

/**
 * @brief Set the sleep timer on the PMC device.
 *
 * This method sets the 16-bit sleep timer value on the PMC device. The sleep timer
 * determines how long the system will remain in sleep mode before waking up.
 *
 * @param sleep_timer The 16-bit value to set the sleep timer to.
 * @return true if the command was sent successfully, false otherwise.
 */
uint8_t PMCClass::setSleepTimer(uint16_t sleep_timer) {
    Wire.beginTransmission(0x18);
    Wire.write(SLEEP_TMR_LO);
    Wire.write(sleep_timer & 0xff);
    Wire.write(sleep_timer >> 8);
    return Wire.endTransmission();
}

/**
 * @brief Put the PMC device to sleep for a specified duration.
 *
 * This method configures the sleep timer and sends the sleep command to the PMC device.
 * The system will enter sleep mode and will only wake up based on the configured
 * wake-up sources or when the sleep timer expires.
 *
 * @param seconds The duration in seconds for which the system should sleep.
 * @return This function does not return as the system enters sleep mode.
 */
uint8_t PMCClass::sleep(double seconds) {
    uint16_t timer_val;
    int divtab[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

    //Serial.printf("Selected clock %s\r\n", (g_clk ==  PIT_CTRL_CLK_1024 ? "1024" : "32768"));
    //Serial.printf("Used divider: %d\r\n", divtab[g_divider]);
    timer_val = ((g_clk ==  PIT_CTRL_CLK_1024 ? 1024 : 32768) / divtab[g_divider]) * seconds;
    //Serial.printf("Time out in %d seconds\r\n", timer_val);
    setSleepTimer(timer_val);
    command(CMD_SLEEP_2, false);

    // We will not return from this call.
    while (true);
}

/**
 * @brief Write data to the non-volatile memory (NVM) of the PMC device.
 *
 * This function writes a specified number of bytes to the NVM starting from a given offset.
 * The data is sent in chunks to accommodate the I2C buffer size limitations.
 *
 * @param array Pointer to the data array to be written to NVM.
 * @param offset The starting address in NVM where the data should be written.
 * @param len The number of bytes to write to NVM.
 */
uint8_t PMCClass::writeNVram32(uint8_t *array, uint16_t offset, uint8_t len) {
    //Serial.printf("Writing %d bytes from %08x to NVRAM at %d !\r\n", len, array, offset);
    uint8_t cmd = 0x80 | (offset & 3);
    uint8_t addrhi = offset >> 2;
    //Serial.printf("addrhi = %d\r\n", addrhi);
    Wire.beginTransmission(0x18);
    Wire.write(cmd);  // Write NVRAM command + low address bits
    Wire.write(addrhi);  // High address bits
    while(len--) {
      Wire.write(*array++);
    }
    uint8_t error = Wire.endTransmission();
    delay(100);
    //Serial.println("Done!");
    return error;
}

/**
 * @brief Write data to the non-volatile memory (NVM) of the PMC device.
 *
 * This function writes a specified number of bytes to the NVM starting from a given offset.
 * The data is sent in chunks to accommodate the I2C buffer size limitations.
 * The slave can receive up to 32bytes max due to its internal buffer size.
 * These 32 bytes include the command byte and the following high byte of the 
 * address. This means we can only send 30 bytes of actual data in one frame.
 * Anything more needs to be chunked up.
 *
 * @param array Pointer to the data array to be written to NVM.
 * @param offset The starting address in NVM where the data should be written.
 * @param len The number of bytes to write to NVM.
 */
#define I2C_CHUNK_SIZE    28
uint8_t PMCClass::writeNVram(uint8_t *array, uint16_t offset, int len) {
  // Calculate the number of chunks we need for this operation
  uint8_t whole_chunks = len / I2C_CHUNK_SIZE;
  uint8_t bytes_in_last_chunk = len - (whole_chunks * I2C_CHUNK_SIZE);
  uint8_t error = 0;

  //Serial.printf("Whole chunks to send = %d, bytes in last chunk = %d\r\n", whole_chunks, bytes_in_last_chunk);

  while (whole_chunks--) {
    error += writeNVram32(array, offset, I2C_CHUNK_SIZE);
    array += I2C_CHUNK_SIZE;
    offset += I2C_CHUNK_SIZE;
  }

  if (bytes_in_last_chunk)
    error += writeNVram32(array, offset, bytes_in_last_chunk);

  return error;
}

/**
 * @brief Read 32 bytes of data from the non-volatile memory (NVM) of the PMC device.
 *
 * This function reads up to 32 bytes of data from the NVM starting from a given offset.
 * The data is read in a single I2C transaction.
 *
 * @param array Pointer to the data array where the read data will be stored.
 * @param offset The starting address in NVM from where the data should be read.
 * @param len The number of bytes to read from NVM (maximum 32).
 */
void PMCClass::readNVram32(uint8_t *array, uint16_t offset, uint8_t len) {
    uint8_t cmd = 0xC0 | (offset & 3);
    uint8_t addrhi = offset >> 2;
    Wire.beginTransmission(0x18);
    Wire.write(cmd);  // Reads NVRAM command + low address bits
    Wire.write(addrhi);  // High address bits
    uint8_t lenlo = len & 0xff;
    uint8_t lenhi = len >> 8;
    //Serial.printf("Lenlo: %d, Lenhi %d\r\n", lenlo, lenhi);
    Wire.write(lenlo); // Send low byte of length
    Wire.write(lenhi); // Send high byte of length
    Wire.endTransmission();
    Wire.requestFrom(0x18, len);
    for(int i=0;i<len;i++) {
      uint8_t ch = Wire.read();
      *array++ = ch;
      //Serial.print(ch);Serial.print(' ');
    }
    //Serial.println();
}

/**
 * @brief Read data from the non-volatile memory (NVM) of the PMC device.
 *
 * This function reads a specified number of bytes from the NVM starting from a given offset.
 * The data is read in chunks to accommodate the I2C buffer size limitations.
 * The slave can transmit back 32 bytes max due to its internal buffer size.
 * These 32 bytes does not include the command byte, addres and length information. 
 * This means when reading data we can receive 32 bytes of actual data in one frame.
 * Anything more needs to be chunked up just like in the write case.
 *
 * @param array Pointer to the data array where the read data will be stored.
 * @param offset The starting address in NVM from where the data should be read.
 * @param len The number of bytes to read from NVM.
 */
void PMCClass::readNVram(uint8_t *array, uint16_t offset, uint16_t len) {
  // Calculate the number of chunks we need for this operation
  uint8_t whole_chunks = len / I2C_CHUNK_SIZE;
  uint8_t bytes_in_last_chunk = len - (whole_chunks * I2C_CHUNK_SIZE);

  //Serial.printf("Whole chunks to read = %d, bytes in last chunk = %d\r\n", whole_chunks, bytes_in_last_chunk);

  while (whole_chunks--) {
    readNVram32(array, offset, I2C_CHUNK_SIZE);
    array += I2C_CHUNK_SIZE;
    offset += I2C_CHUNK_SIZE;
  }

  if (bytes_in_last_chunk)
    readNVram32(array, offset, bytes_in_last_chunk);

}

// Global instance of PMCClass
PMCClass PMC;