/* This application can test if the sound sensor connected to imix board
The information of this kind of sensor is listed:
https://www.newegg.com/p/2S7-00VK-00PP4?item=9SIADG45WJ3684&source=region&nm_mc=knc-googlemkp-pc&cm_mmc=knc-googlemkp-pc-_-pla-keyboardmall-_-gadgets-_-9SIADG45WJ3684&gclid=Cj0KCQjwrrXtBRCKARIsAMbU6bEjGRZTqpu61IrQ-JDXEfAe2vEzj03SCVEmH8q5ldqsFxzW-L2YweQaAq62EALw_wcB&gclsrc=aw.ds
 *   This app uses GPIO pin 0 (the 0th pin made available
 *   to show if the sound sensor is connected
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <gpio.h>
#include <led.h>
#include <timer.h>
#include <tock.h>

int callback_count = 0;
// callback for timers
static void timer_cb (__attribute__ ((unused)) int arg0,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      __attribute__ ((unused)) void* userdata) {
  callback_count     = callback_count + 1;
  *((bool*)userdata) = 1;
}

// **************************************************

static void sound_sensor_input(void) {
  printf("Periodically reading value of the GPIO 0 pin\n");
  printf("Jump pin high to test (defaults to low)\n");

  // set userspace pin 0 as input and start repeating timer
  // pin is configured with a pull-down resistor, so it should read 0 as default
  gpio_enable_input(0, PullDown);
  tock_timer_t timer;
  static bool resume = 0;
  timer_every(500, timer_cb, &resume, &timer);

  while (1) {
    // print pin value
    int pin_val = gpio_read(0);
    printf("\tValue(%d)\n", pin_val);
    yield_for(&resume);
    resume = 0;
  }
}


int main(void) {
  printf("*********************\n");
  printf("Test sound sensor\n");

  sound_sensor_input();

  return 0;
}
