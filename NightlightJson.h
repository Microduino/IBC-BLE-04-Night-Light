#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String onS = "";
String voicesS = "";
String lightsS = "";
String voicemS = "";
String lightmS = "";
String colorindexS = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2da0\"}";

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}

//{"on":"true"}
//{"soundControl":"0"}
//{"lightControl":"1"}
//{"micThreshold":"200"}
//{"luxThreshold":"400"}
//{"colorIndex":"5"}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("soundControl", voice_switch);
      Ctrldata("lightControl", light_switch);
      Ctrldata("micThreshold", voice_min);
      Ctrldata("luxThreshold", light_max);
      Ctrldata("colorIndex", colorindex);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *lighton = aJson.getObjectItem(msg, "on");
  if (lighton) {
    onS = lighton->valuestring;
    if (onS == "true")
      onlight = true;
    else
      onlight = false;
  }
  else {
#if DEBUG
    Serial.println("no voice_switch data");
#endif
  }
  aJsonObject *voices = aJson.getObjectItem(msg, "soundControl");
  if (voices) {
    voicesS  = voices->valuestring;
    voice_switch=voicesS.toInt();
    allLedBlinkNum(1, COLOR_GREEN, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no voice_switch data");
#endif
  }
  aJsonObject *lights = aJson.getObjectItem(msg, "lightControl");
  if (lights) {
    lightsS = lights->valuestring;
    light_switch=lightsS.toInt();
    allLedBlinkNum(1, COLOR_GREEN, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no light_switch data");
#endif
  }
  aJsonObject *voicem = aJson.getObjectItem(msg, "micThreshold");
  if (voicem) {
    voicemS = voicem->valuestring;
    voice_min = voicemS.toInt();
    allLedBlinkNum(1, colorindex, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no voice_min data");
#endif
  }
  aJsonObject *lightm = aJson.getObjectItem(msg, "luxThreshold");
  if (lightm) {
    lightmS = lightm->valuestring;
    light_max = lightmS.toInt();
    allLedBlinkNum(1, colorindex, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no light_max data");
#endif
  }
  aJsonObject *index = aJson.getObjectItem(msg, "colorIndex");
  if (index) {
    colorindexS = index->valuestring;
    colorindex = colorindexS.toInt();
    write_eeprom();
    allLedBlinkNum(1, colorindex, 100);
  }
  else {
#if DEBUG
    Serial.println("no time_on data");
#endif
  }
#if DEBUG
  Serial.println("--------------");
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
}

void Nightlightjson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

