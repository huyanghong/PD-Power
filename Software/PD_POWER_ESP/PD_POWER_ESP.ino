#include <Arduino.h>
#include <SPI.h>
#include "src/U8g2/src/U8g2lib.h"
#include "button.h"

const char *ssid = "BE-WIFI";
const char *password = "12345678";

int PIN_BTN = 27;
int PIN_CH225K_CFG1 = 12, PIN_CH225K_CFG2 = 13, PIN_CH225K_CFG3 = 14;
int PIN_VOLTAGE = 32, PIN_CURRENT = 33;
int PIN_POWER_SWITCH = 15;
void setCh224kVoltage(int v){
  switch(v){
    case 5:  digitalWrite(PIN_CH225K_CFG1, HIGH); digitalWrite(PIN_CH225K_CFG2, LOW);  digitalWrite(PIN_CH225K_CFG3, LOW);  break;
    case 9:  digitalWrite(PIN_CH225K_CFG1, LOW);  digitalWrite(PIN_CH225K_CFG2, LOW);  digitalWrite(PIN_CH225K_CFG3, LOW);  break;
    case 12: digitalWrite(PIN_CH225K_CFG1, LOW);  digitalWrite(PIN_CH225K_CFG2, LOW);  digitalWrite(PIN_CH225K_CFG3, HIGH); break;
    case 15: digitalWrite(PIN_CH225K_CFG1, LOW);  digitalWrite(PIN_CH225K_CFG2, HIGH); digitalWrite(PIN_CH225K_CFG3, HIGH); break;
    case 20: digitalWrite(PIN_CH225K_CFG1, LOW);  digitalWrite(PIN_CH225K_CFG2, HIGH); digitalWrite(PIN_CH225K_CFG3, LOW);  break;
  }
}
void powerSwitch(int sta){
  if(sta == 1) digitalWrite(PIN_POWER_SWITCH, HIGH);
  else if(sta == 0)digitalWrite(PIN_POWER_SWITCH, LOW);
  else if(sta == -1){
    if(digitalRead(PIN_POWER_SWITCH))
      digitalWrite(PIN_POWER_SWITCH, LOW);
    else digitalWrite(PIN_POWER_SWITCH, HIGH);
  }
}
      
void taskOled(void* parameter){
  //U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ 19, /* data=*/ 25, /* cs=*/ 22, /* dc=*/ 21, /* reset=*/ 18);
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0 , /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 13, /* data=*/ 15);
  u8g2.begin();
  u8g2.enableUTF8Print();	
  int adc_v ,adc_c;
  float volage, current, power;
  String str_v, str_c, str_p;
  while(1){
    vTaskDelay(40);
    adc_v = 0.5f * adc_v + 0.5f * analogRead(PIN_VOLTAGE);
    adc_c = 0.8f * adc_c + 0.2f * analogRead(PIN_CURRENT);
    volage = ((3.3f * adc_v / 4096.0) + 0.175f) * 11.0f;
    current = (3.3f * adc_c / 4096.0) * 2.0f;
    power = volage * current;
    if(volage > 9.999999f) str_v = String(volage, 2);
    else str_v = String(volage, 3);
    if(current > 9.999999f) str_c = String(current, 2);
    else str_c = String(current, 3);
    if(power > 9.999999f) str_p = String(power, 2);
    else str_p = String(power, 3);

    u8g2.clearBuffer();    
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setCursor(0, 14);u8g2.print("单击"); 
    u8g2.setCursor(64, 14);u8g2.print("长按");
    u8g2.setCursor(0, 22);u8g2.print("----------------------------"); 
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.setCursor(32, 14);u8g2.print(":切换");
    u8g2.setCursor(96, 14);u8g2.print(":输出"); 
    u8g2.setFont(u8g2_font_wqy14_t_gb2312);
    u8g2.setCursor(0, 32);u8g2.print("电压: "); 
    u8g2.setCursor(35, 32);u8g2.print(str_v.c_str()); 
    u8g2.setCursor(66, 32);u8g2.print(" V"); 
    u8g2.setCursor(0, 48);u8g2.print("电流: ");
    u8g2.setCursor(35, 48);u8g2.print(str_c.c_str()); 
    u8g2.setCursor(65, 48);u8g2.print(" A"); 

    u8g2.setCursor(0, 64);u8g2.print("功率: "); 
    u8g2.setCursor(35, 64);u8g2.print(str_p.c_str()); 
    u8g2.setCursor(64, 64);u8g2.print(" W"); 

    u8g2.setFont(u8g2_font_logisoso24_tr);
    if(digitalRead(PIN_POWER_SWITCH))
      u8g2.drawStr(88, 52, "ON"); 
    else u8g2.drawStr(82, 52, "OFF"); 
    u8g2.sendBuffer();        
  }
}
KEY_HandleTypeDef key1{
  .KeyId = PIN_BTN
};

void taskButton(void* parameter){
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_CH225K_CFG1, OUTPUT);
  pinMode(PIN_CH225K_CFG2, OUTPUT);
  pinMode(PIN_CH225K_CFG3, OUTPUT);
  pinMode(PIN_VOLTAGE, INPUT);
  pinMode(PIN_CURRENT, INPUT);
  pinMode(PIN_POWER_SWITCH, OUTPUT);
  setCh224kVoltage(5);
  powerSwitch(0);
  const int CFG_VOLTAGE[5] = {5, 9, 12, 15, 20};
  int cfg_index = 0;
  while(1){
    vTaskDelay(10); 

    KEY_Scan(&key1);
    KEY_FsmTypeDef fsm = KEY_Get_Fsm(&key1);
    if(fsm == KEY_FSM_SHORT_PRESS){
      powerSwitch(0);
      if(cfg_index >= 4) cfg_index = 0;
      else cfg_index++;
      setCh224kVoltage(CFG_VOLTAGE[cfg_index]);
      //Serial.println(CFG_VOLTAGE[cfg_index]);
    }else if(fsm == KEY_FSM_LONG_PRESS){
      powerSwitch(-1);
      //Serial.println("open");
    }

  }
}
void setup() { 
    Serial.begin(115200); 

    delay(100);

    xTaskCreatePinnedToCore(taskOled,          /* Task function. */
            "taskOled",        /* String with name of task. */
            10000,            /* Stack size in bytes. */
            NULL,             /* Parameter passed as input of the task */
            1,                /* Priority of the task. */
            NULL, 1);         /* Task handle. */  
    xTaskCreatePinnedToCore(taskButton,          /* Task function. */
            "taskButton",        /* String with name of task. */
            10000,            /* Stack size in bytes. */
            NULL,             /* Parameter passed as input of the task */
            2,                /* Priority of the task. */
            NULL, 1);         /* Task handle. */  
              
} 
void loop() { // run over and over again 

} 