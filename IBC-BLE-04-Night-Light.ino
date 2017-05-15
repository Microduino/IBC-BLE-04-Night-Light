#include <Microduino_Key.h>
#include "colorLed.h"
#include "userDef.h"
#include "eeprom.h"
#include "NightlightJson.h"

Key keyButton(PIN_KEY, INPUT_PULLUP);
Key keyMic(PIN_MIC, INPUT_PULLUP);
Key keyLight(PIN_LIGHT, INPUT_PULLUP);

void ONlight()
{
  uint32_t ledonTime = millis();
  while (millis() - ledonTime < TIME_ON)
  {
    Nightlightjson_data();
    setAllLed(colorindex);
    onlight = false;
  }
}

void setup() {
  Serial.begin(57600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  read_eeprom();
  if (voice_switch < 0 || voice_switch > 1||light_switch < 0 || light_switch > 1||voice_min < 100 || voice_min > 800 || light_max<100>800 || colorindex < 1 || colorindex > 9)
  {
    voice_switch = false;
    light_switch = true;
    voice_min = 200;
    light_max = 400;
    colorindex = 5;
  }

#if DEBUG
  Serial.println("**************START************");
  Serial.print("ON DATA:");
  Serial.println(onlight);
  Serial.print("soundControl DATA:");
  Serial.println(voice_switch);
  Serial.print("lightControl DATA:");
  Serial.println(light_switch);
  Serial.print("micThreshold DATA:");
  Serial.println(voice_min);
  Serial.print("luxThreshold DATA:");
  Serial.println(light_max);
  Serial.print("colorindex DATA:");
  Serial.println(colorindex);
  Serial.println("--------------");
#endif

  for (int i = 0; i < 10; i++)
  {
    setAllLed(i);
    delay(150);   //每个颜色持续时间
  }
}

void loop() {
  //    Serial.print("LIGHT Val:");
  //    Serial.print(analogRead(PIN_LIGHT));
  //    Serial.print("\tMIC Val:");
  //    Serial.println(analogRead(PIN_MIC));
  Nightlightjson_data();
  if (light_switch && voice_switch)
  {
    if ( keyMic.read(voice_min, VOICE_MAX) == SHORT_PRESS && keyLight.read(LIGHT_MIN, light_max) == LONG_PRESS || keyButton.read() == SHORT_PRESS || onlight)
      ONlight();
    else
      setAllLed(COLOR_NONE);
  }
  else if (light_switch && !voice_switch)
  {
    if ( keyLight.read(LIGHT_MIN, light_max) == LONG_PRESS || keyButton.read() == SHORT_PRESS || onlight)
      ONlight();
    else
      setAllLed(COLOR_NONE);
  }
  else if (!light_switch && voice_switch)
  {
    if ( keyMic.read(voice_min, VOICE_MAX) == SHORT_PRESS || keyButton.read() == SHORT_PRESS || onlight)
      ONlight();
    else
      setAllLed(COLOR_NONE);
  }
  else if (keyButton.read() == SHORT_PRESS || onlight)
    ONlight();
  else
    setAllLed(COLOR_NONE);
  delay(15);
}

