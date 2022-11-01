#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define LED  2

const char msg[] = {"welcome to freertos with esp32"};

static TaskHandle_t task1 = NULL; 
static TaskHandle_t task2 = NULL; 

//the task we run on freertos: toggles the built in led
void STR_printf (void*)
{
  int msg_len = strlen(msg);
  while(1)
  {
    Serial.println();
    for(int i=0; i<msg_len; i++)
    {
      Serial.print(msg[i]);
    }
    vTaskDelay(1000);
  }
}

//the task we run on freertos: toggles the built in led
void STAR_printf (void*)
{
  while(1)
  {
    Serial.print('*');
    vTaskDelay(100);
  }
}

void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(300);

  Serial.print("setup and loop function run on core");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority");
  Serial.print(uxTaskPriorityGet(NULL));
  
  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              STR_printf,    //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              &task1,         //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos


     // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              STAR_printf,    //function to call in task created
              "task2",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              2,              //task priority (from 0 to 24)
              &task2,         //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop() {
  for(int i=0; i<3; i++)
  {
    vTaskSuspend(task2);
    vTaskDelay(2000);
    vTaskResume(task2);
    vTaskDelay(2000);
  }

  if(task1!= NULL)
  {
    vTaskDelete(task1);
    task1 = NULL;
  }

}