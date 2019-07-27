/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_ST7735.h"

Arduino_ST7735::Arduino_ST7735(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2,
    bool bgr)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
  _bgr = bgr;
}

void Arduino_ST7735::begin(uint32_t speed)
{
#if defined(ESP8266) or defined(ESP32)
  if (speed == 0) {
    speed = 27000000; // ST7735 Maximum supported speed
  }
#endif
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST7735::tftInit()
{
  // if (_rst < 0)
  // {
  _bus->writeCommand(ST7735_SWRESET); // 1: Software reset
  delay(ST7735_RST_DELAY);
  // }

  _bus->writeCommand(ST7735_SLPOUT); //  2: Out of sleep mode, no args, w/delay
  delay(ST7735_SLPOUT_DELAY);
  _bus->writeCommand(ST7735_COLMOD); // 3: Set color mode, 1 arg + delay:
  _bus->writeData(0x05);             // 16-bit color
  if (_ips)
  {
    _bus->writeCommand(ST7735_INVON);
  }
  _bus->writeCommand(ST7735_NORON); // 4: Normal display on, no args, w/delay
  delay(10);
  _bus->writeCommand(ST7735_DISPON); // 5: Main screen turn on, no args, w/delay
}


void Arduino_ST7735::writeAddrColumn(uint16_t x, uint16_t w)
{
#if defined(ESP8266) || defined(ESP32)
  uint32_t x_range = ((uint32_t)(x + _xStart) << 16) | (x + w - 1 + _xStart);

  _bus->writeCommandCore(ST7735_CASET); // Column addr set
  _bus->write32(x_range);
#else
  uint16_t x_start = x + _xStart, x_end = x + w - 1 + _xStart;

  _bus->writeCommandCore(ST7735_CASET); // Column addr set
  _bus->write(x_start >> 8);
  _bus->write(x_start & 0xFF); // XSTART
  _bus->write(x_end >> 8);
  _bus->write(x_end & 0xFF); // XEND
#endif
}

void Arduino_ST7735::writeAddrRow(uint16_t y, uint16_t h)
{
#if defined(ESP8266) || defined(ESP32)
  uint32_t y_range = ((uint32_t)(y + _yStart) << 16) | (y + h - 1 + _yStart);

  _bus->writeCommandCore(ST7735_RASET); // Row addr set
  _bus->write32(y_range);
#else
  uint16_t y_start = y + _yStart, y_end = y + h - 1 + _yStart;

  _bus->writeCommandCore(ST7735_RASET); // Row addr set
  _bus->write(y_start >> 8);
  _bus->write(y_start & 0xFF); // YSTART
  _bus->write(y_end >> 8);
  _bus->write(y_end & 0xFF); // YEND
#endif
}

void Arduino_ST7735::writeAddrMemWrite()
{
  _bus->writeCommandCore(ST7735_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST7735::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = ST7735_MADCTL_MX | ST7735_MADCTL_MY | (_bgr ? ST7735_MADCTL_BGR : ST7735_MADCTL_RGB);
    break;

  case 1:
    r = ST7735_MADCTL_MY | ST7735_MADCTL_MV | (_bgr ? ST7735_MADCTL_BGR : ST7735_MADCTL_RGB);
    break;

  case 2:
    r = (_bgr ? ST7735_MADCTL_BGR : ST7735_MADCTL_RGB);
    break;

  case 3:
    r = ST7735_MADCTL_MX | ST7735_MADCTL_MV | (_bgr ? ST7735_MADCTL_BGR : ST7735_MADCTL_RGB);
    break;
  }

  _bus->beginWrite();
  _bus->writeCommandCore(ST7735_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_ST7735::invertDisplay(bool i)
{
  _bus->writeCommand(_ips ? (i ? ST7735_INVOFF : ST7735_INVON) : (i ? ST7735_INVON : ST7735_INVOFF));
}

void Arduino_ST7735::displayOn(void)
{
  _bus->writeCommand(ST7735_SLPOUT);
  delay(ST7735_SLPOUT_DELAY);
}

void Arduino_ST7735::displayOff(void)
{
  _bus->writeCommand(ST7735_SLPIN);
  delay(ST7735_SLPIN_DELAY);
}