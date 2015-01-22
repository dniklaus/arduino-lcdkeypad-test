// Do not remove the include below
#include "arduino_lcdkeypad_test.h"

// Timer library, https://github.com/dniklaus/arduino-utils-timer
#include <Timer.h>
//#include <TimerAdapter.h>
//#include <TimerContext.h>
//#include <UptimeInfo.h>

#include <LiquidCrystal.h>

// LcdKeypad, https://github.com/dniklaus/arduino-display-lcdkeypad
#include <LcdKeypad.h>

LcdKeypad* myLcdKeypad = 0;

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
    Serial.print("newKey: ");
    Serial.println(newKey);

    if (LcdKeypad::UP_KEY == newKey)
    {
      m_value++;
    }
    else if (LcdKeypad::DOWN_KEY == newKey)
    {
      m_value--;
    }
    if (0 != m_lcdKeypad)
    {
      m_lcdKeypad->setCursor(0, 0);
      m_lcdKeypad->print("Zahl:       ");
      if (m_value < 100)
      {
        m_lcdKeypad->print(" ");
      }
      if (m_value < 10)
      {
        m_lcdKeypad->print(" ");
      }
      m_lcdKeypad->print(m_value);

      m_lcdKeypad->setCursor(0, 1);
      m_lcdKeypad->print("Bin:  ");

      for (int i = 7; i >= 0; i--)
      {
        m_lcdKeypad->print(((m_value >> i) & 1) ? "1" : "0");
        if (i == 4)
        {
          m_lcdKeypad->print(" ");
        }
      }
    }
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello!");
  myLcdKeypad = new LcdKeypad();
  myLcdKeypad->attachAdapter(new MyLcdKeypadAdapter(myLcdKeypad));
  myLcdKeypad->setBackLightOn(true);
}

void loop()
{
  scheduleTimers();
}

