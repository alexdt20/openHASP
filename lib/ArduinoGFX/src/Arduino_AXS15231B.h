#pragma once

#include "Arduino_TFT.h"

class Arduino_AXS15231B : public Arduino_TFT
{
public:
    Arduino_AXS15231B(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0, bool ips = false);
    ~Arduino_AXS15231B();

    bool begin(int32_t speed = GFX_NOT_DEFINED) override;
    void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;
    void setRotation(uint8_t r) override;
    void invertDisplay(bool) override;
    void displayOn() override;
    void displayOff() override;

protected:
    void tftInit() override;

private:
    uint8_t _x_start = 0;
    uint8_t _y_start = 0;
};
