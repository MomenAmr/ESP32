#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static int shared_value=0;

// Task: increament an integer variable badly !!
void inc(void *parameters)
{
  int local_value;
  
  // Loop forever
  while (1)
  {
    local_value = shared_value;
    local_value++;
    vTaskDelay(random(100, 500));
    shared_value = local_value;
    Serial.println(shared_value);
  }
}

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(115200);
  vTaskDelay(1000);
  
  Serial.println("two tasks edit the same variable without MUTEX");

  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              inc,            //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos

  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              inc,            //function to call in task created
              "task2",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop()
{
  
}