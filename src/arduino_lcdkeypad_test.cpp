// Do not remove the include below
#include "arduino_lcdkeypad_test.h"

// Timer library, https://github.com/dniklaus/arduino-utils-timer
#include <Timer.h>

// LcdKeypad, https://github.com/dniklaus/arduino-display-lcdkeypad
#include <LcdKeypad.h>

#include "Adafruit_MCP9808.h"
// Create the MCP9808 temperature sensor object
Adafruit_MCP9808* tempsensor = 0;
LcdKeypad* myLcdKeypad = 0;
bool isSelectPressed = false;

class MyLcdKeypadAdapter : public LcdKeypadAdapter
{
private:
  LcdKeypad* m_lcdKeypad;
  unsigned char m_value;
public:
  MyLcdKeypadAdapter(LcdKeypad* lcdKeypad)
  : m_lcdKeypad(lcdKeypad)
  , m_value(0)
  { }

  void handleKeyChanged(LcdKeypad::Key newKey)
  {
    Serial.print("MyLcdKeypadAdapter::handleKeyChanged(), newKey: ");
    Serial.println((LcdKeypad::NO_KEY == newKey)     ? "NO_KEY"     :
                   (LcdKeypad::SELECT_KEY == newKey) ? "SELECT_KEY" :
                   (LcdKeypad::LEFT_KEY == newKey)   ? "LEFT_KEY"   :
                   (LcdKeypad::UP_KEY == newKey)     ? "UP_KEY"     :
                   (LcdKeypad::DOWN_KEY == newKey)   ? "DOWN_KEY"   :
                   (LcdKeypad::RIGHT_KEY == newKey)  ? "RIGHT_KEY"  : "OOPS!! Invalid value!!");

    if (0 != m_lcdKeypad)
    {
      if (!isSelectPressed)
      {
        if (LcdKeypad::SELECT_KEY == newKey)
        {
          isSelectPressed = true;
        }
      }
      else
      {
        if (LcdKeypad::UP_KEY == newKey)
        {
          m_value++;
        }
        else if (LcdKeypad::DOWN_KEY == newKey)
        {
          m_value--;
        }

//        m_lcdKeypad->setCursor(0, 0);
//        m_lcdKeypad->print("Temp [°C]:  ");
//        if (m_value < 100)
//        {
//          m_lcdKeypad->print(" ");
//        }
//        if (m_value < 10)
//        {
//          m_lcdKeypad->print(" ");
//        }
//        m_lcdKeypad->print(m_value);
//
//        m_lcdKeypad->setCursor(0, 1);
//        m_lcdKeypad->print("Bin:  ");
//
//        for (int i = 7; i >= 0; i--)
//        {
//          m_lcdKeypad->print(((m_value >> i) & 1) ? "1" : "0");
//          if (i == 4)
//          {
//            m_lcdKeypad->print(" ");
//          }
//        }
      }
    }
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello from Arduino LCD Keypad Shield test!");

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  tempsensor = new Adafruit_MCP9808();
  if (!tempsensor->begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }

  myLcdKeypad = new LcdKeypad();
  myLcdKeypad->attachAdapter(new MyLcdKeypadAdapter(myLcdKeypad));
  myLcdKeypad->setBackLightOn(true);

  myLcdKeypad->setCursor(0, 0);
//  myLcdKeypad->print("LCD Keypad Test");
//
//  myLcdKeypad->setCursor(0, 1);
//  myLcdKeypad->print("Press SELECT!");
}

void loop()
{
  // Read and print out the temperature, then convert to *F
  float c = tempsensor->readTempC();
  float fc = c * 9.0 / 5.0 + 32;
  float f = tempsensor->readTempF();
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t");
  Serial.print(f); Serial.print("*F \t");
  Serial.print(fc); Serial.println("*F (calc)");

  myLcdKeypad->setCursor(0, 0);
  myLcdKeypad->print("Temp. [*C]: ");
  myLcdKeypad->print(c);

  delay(1000);
}

