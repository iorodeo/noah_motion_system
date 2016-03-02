#include <AccelStepper.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "motor_pin.h"
#include "msg_types.h"
#include "constants.h"
#include "motion_controller.h"

MotionController motionController;
Adafruit_SSD1306 display(constants::DisplayDC, constants::DisplayReset, constants::DisplayCS);
elapsedMillis last_display;

void setup()
{
    motionController.initialize();

    Serial.begin(115200);
    display.begin(SSD1306_SWITCHCAPVCC);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Controller Demo");
    display.display();
    SPI.endTransaction();

}

void loop()
{
    motionController.update();
}


    




