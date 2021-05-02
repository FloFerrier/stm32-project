#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Necessary for FreeRTOS */
uint32_t SystemCoreClock;

extern void vTaskBlinky(void *pvParameters);

extern void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);

void led_on_board_setup(void);

int main(void)
{
  rcc_clock_setup_hse_3v3(rcc_hse_8mhz_3v3);

  SystemCoreClock = rcc_ahb_frequency;
  systick_set_frequency(configTICK_RATE_HZ, SystemCoreClock);

  /* HAL initialization */
  led_on_board_setup();

  /* Tasks creation */
  xTaskCreate(vTaskBlinky, "blinky", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

  vTaskStartScheduler();
  taskENABLE_INTERRUPTS();

  while(1);

  return 0;
}

void vTaskBlinky(void *pvParameters)
{
  (void) pvParameters;

  gpio_clear(GPIOA, GPIO5);

  while(1)
  {
    gpio_toggle(GPIOA, GPIO5);
    vTaskDelay(pdMS_TO_TICKS(500)); // in milliseconds
  }
}

void led_on_board_setup(void)
{
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
  gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO5);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
  (void) xTask;
  (void) pcTaskName;
  // Nothing
}
