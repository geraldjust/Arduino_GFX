/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"

Arduino_DataBus::Arduino_DataBus() {}

void Arduino_DataBus::writeC8D8(uint8_t c, uint8_t d)
{
  writeCommand(c);
  write(d);
}

void Arduino_DataBus::writeC8D16(uint8_t c, uint16_t d)
{
  writeCommand(c);
  write16(d);
}

void Arduino_DataBus::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  writeCommand(c);
  write16(d1);
  write16(d2);
}

void Arduino_DataBus::batchOperation(spi_operation_t batch[], uint8_t len)
{
  for (int i = 0; i < len; ++i)
  {
    switch (batch[i].type)
    {
    case BEGIN_WRITE:
      beginWrite();
      break;
    case WRITE_COMMAND_8:
      writeCommand(batch[i].value);
      break;
    case WRITE_COMMAND_16:
      writeCommand16(batch[i].value);
      break;
    case WRITE_COMMAND_32:
      break;
    case WRITE_DATA_8:
      write(batch[i].value);
      break;
    case WRITE_DATA_16:
      write16(batch[i].value);
      break;
    case WRITE_DATA_32:
      write32(batch[i].value);
      break;
    case END_WRITE:
      endWrite();
      break;
    case DELAY:
      delay(batch[i].value);
      break;
    }
  }
}

#if defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)
void Arduino_DataBus::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  while (len--)
  {
    write16(idx[*(data++)]);
  }
}

void Arduino_DataBus::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  uint32_t l = len;
  uint8_t *d = data;
  uint16_t p;
  while (l--)
  {
    p = idx[*(d++)];
    write16(p);
    write16(p);
  }
}
#endif // defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)
