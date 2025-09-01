#include "axs15231b.h"
#include "disp_spi.h"
#include "driver/gpio.h"

void axs15231b_init(void)
{
    // Reset display
    gpio_set_level(CONFIG_LVGL_DISP_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(CONFIG_LVGL_DISP_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));
    
    // Initialization commands from MicroPython
    uint8_t init_cmds[] = {
        0x11, 0x00, // Sleep out
        0x36, 0x01, 0x00, // MADCTL - portrait
        0x3A, 0x01, 0x55, // COLMOD - 16bit
        0x29, 0x00, // Display on
    };
    
    disp_spi_send_data(init_cmds, sizeof(init_cmds));
}