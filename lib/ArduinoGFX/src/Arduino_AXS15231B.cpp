#include "Arduino_AXS15231B.h"
#include "Arduino_DataBus.h"

// Command definitions from _axs15231b_init.py
#define AXS_LCD_NORON 0x13
#define AXS_LCD_SLPOUT 0x11
#define AXS_LCD_DISPON 0x29
#define AXS_LCD_MADCTL 0x36
#define AXS_LCD_IPF 0x3A
#define AXS_LCD_WRDISBV 0x51

Arduino_AXS15231B::Arduino_AXS15231B(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips)
    : Arduino_TFT(bus, rst, r, ips, 320, 480, 0, 0, 0, 0, false)
{
}

Arduino_AXS15231B::~Arduino_AXS15231B()
{
    if (_bus)
    {
        delete _bus;
    }
}

bool Arduino_AXS15231B::begin(int32_t speed)
{
    if (!Arduino_TFT::begin(speed))
    {
        return false;
    }

    return true;
}

void Arduino_AXS15231B::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
    {
        _bus->writeC8D16D16(0x2A, x + _xStart, x + w - 1 + _xStart);
        _bus->writeC8D16D16(0x2B, y + _yStart, y + h - 1 + _yStart);

        _currentX = x;
        _currentY = y;
        _currentW = w;
        _currentH = h;
    }

    _bus->writeCommand(0x2C);
}

void Arduino_AXS15231B::setRotation(uint8_t r)
{
    Arduino_TFT::setRotation(r);
    
    uint8_t madctl = 0;
    switch (_rotation)
    {
    case 0:
        madctl = 0xC0; // Rotation 0
        break;
    case 1:
        madctl = 0xA0; // Rotation 90
        break;
    case 2:
        madctl = 0x00; // Rotation 180
        break;
    case 3:
        madctl = 0x60; // Rotation 270
        break;
    }

    _bus->beginWrite();
    _bus->writeCommand(AXS_LCD_MADCTL);
    _bus->write(madctl);
    _bus->endWrite();
}

void Arduino_AXS15231B::invertDisplay(bool invert)
{
    _bus->beginWrite();
    _bus->writeCommand(invert ? 0x21 : 0x20); // INVON/INVOFF
    _bus->endWrite();
}

void Arduino_AXS15231B::displayOn()
{
    _bus->beginWrite();
    _bus->writeCommand(AXS_LCD_DISPON);
    _bus->endWrite();
}

void Arduino_AXS15231B::displayOff()
{
    _bus->beginWrite();
    _bus->writeCommand(0x28); // DISPOFF
    _bus->endWrite();
}

void Arduino_AXS15231B::tftInit()
{
    if (_rst != GFX_NOT_DEFINED)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(100);
        digitalWrite(_rst, LOW);
        delay(100);
        digitalWrite(_rst, HIGH);
        delay(200);
    }

    _bus->begin();

    // Initialization sequence for AXS15231B
    _bus->batchOperation(true);
    
    // Color and Orientation
    _bus->writeCommand(AXS_LCD_MADCTL);
    _bus->write(0xC0); // Default rotation

    // Pixel format - RGB565
    _bus->writeCommand(AXS_LCD_IPF);
    _bus->write(0x55);

    // Disable Partial Display Mode
    _bus->writeCommand(AXS_LCD_NORON);
    delay(10);

    // Sleep out
    _bus->writeCommand(AXS_LCD_SLPOUT);
    delay(150);

    // Display on
    _bus->writeCommand(AXS_LCD_DISPON);
    delay(150);

    // Set default brightness
    _bus->writeCommand(AXS_LCD_WRDISBV);
    _bus->write(0xD0);

    _bus->batchOperation(false);

    // Set default rotation
    setRotation(_rotation);
}
