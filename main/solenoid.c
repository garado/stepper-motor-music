
/* █▀ █▀█ █░░ █▀▀ █▄░█ █▀█ █ █▀▄ */
/* ▄█ █▄█ █▄▄ ██▄ █░▀█ █▄█ █ █▄▀ */

#include <inttypes.h>
#include "driver/gpio.h"
#include "solenoid.h"

uint8_t  solenoid_states[SOLENOID_COUNT] = { 0 };

uint16_t solenoid_pins[SOLENOID_COUNT] = {
  SOLENOID_PIN_0,
  SOLENOID_PIN_1,
};

void Solenoid_Init(void)
{
  // Initialize solenoid control pins
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_SOLENOID_OUT;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  ESP_ERROR_CHECK(gpio_config(&io_conf));

  for (uint8_t i = 0; i < SOLENOID_COUNT; i++) {
    solenoid_states[i] = 1;
  }
}

void Solenoid_Hit(uint8_t num)
{
  gpio_set_level(solenoid_pins[num], solenoid_states[num]);
  solenoid_states[num] = !solenoid_states[num];
}
