#include <stdio.h>

#include "ssd1306.h"
#include "oled_setup.h"
#include "i2c_setup.h" // Config I2C for OLED display

ssd1306_t disp; // global variable for the SSD1306 display


volatile int oled_needs_update = 0;
char linha1_buf[32] = {0};
char linha2_buf[32] = {0};

void setup_oled(void)
{
    // config do display SSD1306
    disp.external_vcc = false; // define if the display uses external Vcc (false means it uses internal Vcc)

    
    // parans: display, width, height, address, i2c instance
    // width: 128 pixels, height: 64 pixels, address: 0x
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);

    // clear the display buffer
    ssd1306_clear(&disp); 
}


void update_oled_text(const char* line1, const char* line2) { // function to update OLED text
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, line1);
    ssd1306_draw_string(&disp, 0, 16, 1, line2);
    ssd1306_show(&disp);
}