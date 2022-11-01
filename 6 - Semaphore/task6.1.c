#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 2;

static SemaphoreHandle_t bin_sem;

// Blink LED based on rate passed by parameter
void LED_BLINK(void *parameters)
{
  // Copy the parameter into a local variable
  int num = *(int *)parameters;
  
  // give a semaphore to signal the setup task that value has been sent successfuly
  xSemaphoreGive(bin_sem);
  
  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  // Configure the LED pin
  pinMode(led_pin, OUTPUT);

  // Blink forever and ever
  while (1)
  {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num);
  }
}

void setup()
{
  long int d;
  Serial.begin(115200);
  vTaskDelay(1000);
  
  Serial.println("enter a delay value in milliseconds");

  while (Serial.available() <= 0);
  d = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(d);

  bin_sem = xSemaphoreCreateBinary();

  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              LED_BLINK,      //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              (void*)&d,      //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos

  // wait till semaphore is given before terminating the tsk
  xSemaphoreTake(bin_sem, portMAX_DELAY);

  Serial.println("Done!");
}

void loop()
{
  
}