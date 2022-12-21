#include "DDCVCP.h"
#include <Bounce2.h>

DDCVCP ddc;

int usbSwitchPin = 3;
int personalReadPin = 5;
int workReadPin = 4;
int usb1ReadPin = 10;
int usb2ReadPin = 9;
int powerReadPin = 12;

Bounce powerBounce = Bounce();
Bounce personalPCBounce = Bounce();
Bounce workPCBounce = Bounce();

void setup() {
  Serial.begin(9600);
  while (!ddc.begin()) {
    Serial.print(F("-  Unable to find DDC/CI. Trying again in 5 seconds.\n"));
    delay(5000);
  }
  Serial.print(F("-  Found DDC/CI successfully.\n"));
  pinMode(usbSwitchPin, OUTPUT);
  personalPCBounce.attach(personalReadPin, INPUT_PULLUP);
  personalPCBounce.interval(25);
  workPCBounce.attach(workReadPin, INPUT_PULLUP);
  workPCBounce.interval(25);
  powerBounce.attach(powerReadPin, INPUT_PULLUP);
  powerBounce.interval(25);
}

void loop() {
  //Switch to personal computer
  personalPCBounce.update();
  if (personalPCBounce.rose()) {
    Serial.println("Switching inputs to personal PC\n");
    ddc.setSource(0x0f);
    if (digitalRead(usb1ReadPin) == LOW) {
      //press button to change USB switch to personal pc input
      digitalWrite(usbSwitchPin, HIGH);
      delay(200);
      digitalWrite(usbSwitchPin, LOW);
    }
  }
  //Switch to work computer
  workPCBounce.update();
  if (workPCBounce.rose()) {
    Serial.print(F("Switching inputs to work PC\n"));
    ddc.setSource(0x12);
    if (digitalRead(usb2ReadPin) == LOW) {
      //press button to change USB switch to work pc input
      digitalWrite(usbSwitchPin, HIGH);
      delay(200);
      digitalWrite(usbSwitchPin, LOW);
    }
  }

  //Toggle monitor power
  powerBounce.update();
  if (powerBounce.fell()) {
    Serial.print(F("Toggling monitor power\n"));
    //If power on, turn off. If off, turn on
    if (ddc.getPower()) {
      ddc.setPower(false);
      Serial.println("Turning off");
    } else if (!ddc.getPower()) {
      ddc.setPower(true);
      Serial.println("Turning on");
    }
  }
}
