#include "SpinTimer.h"
#include "LcdKeypad.h"
#include "Adafruit_MCP9808.h"

Adafruit_MCP9808* tempsensor  = 0;
Adafruit_MCP9808* tempsensor2 = 0;

LcdKeypad* myLcdKeypad = 0;

class MyLcdKeypadAdapter : public LcdKeypadAdapter
{
private:
  LcdKeypad* m_lcdKeypad;
  unsigned char m_value;
public:
  MyLcdKeypadAdapter(LcdKeypad* lcdKeypad)
  : m_lcdKeypad(lcdKeypad)
  , m_value(7)
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
      if (LcdKeypad::UP_KEY == newKey)
      {
        m_value++;
      }
      else if (LcdKeypad::DOWN_KEY == newKey)
      {
        m_value--;
      }
      m_lcdKeypad->setBacklight(static_cast<LcdKeypad::LcdBacklightColor>(LcdKeypad::LCDBL_WHITE & m_value));
      Serial.print("m_value=");
      Serial.println(m_value);
    }
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello from Arduino LCD Keypad Shield test!");

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  tempsensor  = new Adafruit_MCP9808();
  tempsensor2 = new Adafruit_MCP9808();

  if (!tempsensor->begin(0x18)) {
    Serial.println("Couldn't find MCP9808 on addr 0x18!");
  }

  if (!tempsensor2->begin(0x19)) {
    Serial.println("Couldn't find MCP9808 on addr 0x19!");
  }

  myLcdKeypad = new LcdKeypad();

  myLcdKeypad->attachAdapter(new MyLcdKeypadAdapter(myLcdKeypad));
  myLcdKeypad->setBackLightOn(true);

}

void loop()
{
  // Read and print out the temperature, then convert to *F
  float c = tempsensor->readTempC();
  float f = tempsensor->readTempF();
  float c2 = tempsensor2->readTempC();
  float f2 = tempsensor2->readTempF();
  Serial.print("Temp1: "); Serial.print(c); Serial.print("*C  ");
  Serial.print(f); Serial.print("*F");
  Serial.print(" - Temp2: "); Serial.print(c2); Serial.print("*C  ");
  Serial.print(f2); Serial.println("*F \t");

  myLcdKeypad->setCursor(0, 0);
  myLcdKeypad->print("Temp1 [*C]: ");
  myLcdKeypad->print(c);

  myLcdKeypad->setCursor(0, 1);
  myLcdKeypad->print("Temp2 [*C]: ");
  myLcdKeypad->print(c2);

  delayAndSchedule(1000);
}

