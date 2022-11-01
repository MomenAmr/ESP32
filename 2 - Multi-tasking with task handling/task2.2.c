#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define LED  2

#include <stdlib.h>

static int led_delay = 500;

static TaskHandle_t task1 = NULL; 
static TaskHandle_t task2 = NULL; 

//the task we run on freertos: toggles the built in led
void LED_toggle (void*)
{
  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(led_delay);
    digitalWrite(LED,LOW);
    vTaskDelay(led_delay);
  }
}

//the task we run on freertos: takes delay as input from user
void readSerial(void *parameters) {

  char c;
  char buf[10];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, 10);

  // Loop forever
  while (1) {

    // Read characters from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // Update delay variable and reset buffer if we get a newline character
      if (c == '\n') {
        led_delay = atoi(buf);
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        memset(buf, 0, 10);
        idx = 0;
      } else {
        
        // Only append if index is not over message limit
        if (idx < 9) {
          buf[idx] = c;
          idx++  ;
        }
      }
    }
  }
}

void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  vTaskDelay(1000);

  Serial.println("multitask with freertos to control led duration");
  Serial.println("enter a delay in millisecond");
  
  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              LED_toggle,    //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              &task1,         //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos


     // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              readSerial,    //function to call in task created
              "task2",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              &task2,         //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop()
{

}