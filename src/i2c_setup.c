#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "i2c_setup.h"

void setup_i2c(void)
{
    i2c_init(I2C_PORT, 400000);                 // initialized I2C a 400kHz
    gpio_set_function(14, GPIO_FUNC_I2C);       // SDA 14 to I2C
    gpio_set_function(15, GPIO_FUNC_I2C);       // SLC 15 to I2C
    gpio_pull_up(14);                           // Enables pull-up resistor on the pin 14
    gpio_pull_up(15);                           // Enables pull-up resistor on the pin 15
}
