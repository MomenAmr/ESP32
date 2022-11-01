#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define LED  2

//the task we run on freertos: toggles the built in led
void LED_toggle1 (void*)
{
  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(500);
    digitalWrite(LED,LOW);
    vTaskDelay(500);
  }
}

//the task we run on freertos: toggles the built in led
void LED_toggle2 (void*)
{
  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(333);
    digitalWrite(LED,LOW);
    vTaskDelay(333);
  }
}

void setup()
{
  pinMode(LED,OUTPUT);
  
  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              LED_toggle1,    //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos


     // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              LED_toggle2,    //function to call in task created
              "task2",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop() {
  // put your main code here, to run repeatedly:

}