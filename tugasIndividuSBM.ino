#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 2
#define TFT_CS 15
#define TFT_RST 4
#define TFT_MISO 19         
#define TFT_MOSI 23           
#define TFT_CLK 18 

#define ILI9341_RED 0xF800
#define ILI9341_BLUE 0x001F
// #define ILI9341_PURPLE 0x801F

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define LED_BUILTIN 22 // Specify the pin for your LED
#define LED_BUILTIN_2 21 // Specify the pin for your LED  `

void setup() {
  Serial.begin(115200);
  
  SPI.begin();
  tft.begin();

  uint32_t blink_delay = 4500;
  xTaskCreate(
    TaskBlink, "Task Blink",
    2048,
    (void *)&blink_delay,
    2,
    NULL
  );

  xTaskCreatePinnedToCore(
    TaskAnalogRead, "Analog Read",
    2048,
    NULL,
    1,
    NULL,
    ARDUINO_RUNNING_CORE
  );
}

void loop() {
  // Empty loop since we're using FreeRTOS tasks
}

void TaskBlink(void *pvParameters) {
  uint32_t blink_delay = *((uint32_t *)pvParameters);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);

  for(;;) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(blink_delay);

    digitalWrite(LED_BUILTIN, LOW);
    delay(blink_delay);

    digitalWrite(LED_BUILTIN_2, HIGH);
    delay(blink_delay);

    digitalWrite(LED_BUILTIN_2, LOW);
    delay(blink_delay);
  }
}

void TaskAnalogRead(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    delay(50);
    
    // If the LED is on, display red color on the ILI9341 display
    if (digitalRead(LED_BUILTIN) == HIGH) {
      tft.fillScreen(ILI9341_BLUE);
      tft.setCursor(50, 50);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.println("THIS IS BLUE");
      tft.setCursor(50, 70);
      tft.println("CODE: 0x001F");
    }

    else if (digitalRead(LED_BUILTIN_2) == HIGH) {
      tft.fillScreen(ILI9341_RED);
      tft.setCursor(50, 50);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.println("THIS IS RED");
      tft.setCursor(50, 70);
      tft.println("CODE: 0xF800");
      
    }
  }
}