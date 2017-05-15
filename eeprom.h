#include <EEPROM.h>

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

boolean onlight;
int voice_switch;
int light_switch;
int voice_min = 200;
int light_max = 400;
int colorindex = 5;

struct config_type
{
  int EEPROMvoice_s;
  int EEPROMlight_s;
  int EEPROMvoice_m;
  int EEPROMlight_m;
  int EEPROMcolorindex_r;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(4, config_readback);
  voice_switch = config_readback.EEPROMvoice_s;
  light_switch = config_readback.EEPROMlight_s;
  voice_min = config_readback.EEPROMvoice_m;
  light_max = config_readback.EEPROMlight_m;
  colorindex = config_readback.EEPROMcolorindex_r;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMvoice_s = voice_switch;
  config.EEPROMlight_s = light_switch;
  config.EEPROMvoice_m = voice_min;
  config.EEPROMlight_m = light_max;
  config.EEPROMcolorindex_r = colorindex;
  EEPROM_write(4, config);         // 变量config存储到EEPROM，地址0写入
}
