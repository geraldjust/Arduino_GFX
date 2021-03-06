/*
 * start rewrite from:
 * https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test
 */
#ifdef ESP32
#ifndef _ARDUINO_ESP32PAR8_H_
#define _ARDUINO_ESP32PAR8_H_

typedef volatile uint32_t *PORTreg_t; ///< PORT register type

#include "Arduino_DataBus.h"

class Arduino_ESP32PAR8 : public Arduino_DataBus
{
public:
  Arduino_ESP32PAR8(
      int8_t dc, int8_t cs, int8_t wr, int8_t rd,
      int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7); // Constructor

  virtual void begin(int speed = 0, int8_t dataMode = 0);
  virtual void beginWrite();
  virtual void writeCommand(uint8_t);
  virtual void writeCommand16(uint16_t);
  virtual void writeCommand32(uint32_t);
  virtual void write(uint8_t);
  virtual void write16(uint16_t);
  virtual void write32(uint32_t);
  virtual void writeC8D8(uint8_t c, uint8_t d);
  virtual void writeC8D16(uint8_t c, uint16_t d);
  virtual void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2);
  virtual void writeRepeat(uint16_t p, uint32_t len);
  virtual void writeBytes(uint8_t *data, uint32_t len);
  virtual void writePixels(uint16_t *data, uint32_t len);
  virtual void writePattern(uint8_t *data, uint8_t len, uint32_t repeat);
  virtual void endWrite();

  virtual void sendCommand(uint8_t);
  virtual void sendCommand16(uint16_t);
  virtual void sendCommand32(uint32_t);
  virtual void sendData(uint8_t);
  virtual void sendData16(uint16_t);
  virtual void sendData32(uint32_t);

protected:
private:
  INLINE void WRITE(uint8_t d);
  INLINE void DC_HIGH(void);
  INLINE void DC_LOW(void);
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);

  int8_t _dc, _cs, _wr, _rd;
  int8_t _d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7;
  uint32_t _div = 0;

  PORTreg_t dcPortSet; ///< PORT register SET
  PORTreg_t dcPortClr; ///< PORT register CLEAR
  uint32_t dcPinMask;  ///< Bitmask

  PORTreg_t csPortSet; ///< PORT register SET
  PORTreg_t csPortClr; ///< PORT register CLEAR
  uint32_t csPinMask;  ///< Bitmask

  PORTreg_t wrPortSet; ///< PORT register SET
  PORTreg_t wrPortClr; ///< PORT register CLEAR
  uint32_t wrPinMask;  ///< Bitmask

  PORTreg_t rdPortSet; ///< PORT register SET
  PORTreg_t rdPortClr; ///< PORT register CLEAR
  uint32_t rdPinMask;  ///< Bitmask

  PORTreg_t dataPortSet; ///< PORT register SET
  PORTreg_t dataPortClr; ///< PORT register CLEAR
  uint32_t dataClrMask;
  // Lookup table for ESP32 parallel bus interface uses 1kbyte RAM,
  uint32_t xset_mask[256]; // Makes Sprite rendering test 33% faster, for slower macro equivalent
                           // see commented out #define set_mask(C) within TFT_eSPI_ESP32.h
};

#endif // _ARDUINO_ESP32PAR8_H_
#endif // #ifdef ESP32
