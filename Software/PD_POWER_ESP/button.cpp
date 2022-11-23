#include "button.h"

#define IsRange(VALUE, MIN, MAX) ((VALUE)<(MIN)? (0): ((VALUE)>(MAX)? (0): (1)))
//#define   LIMIT_FUN(VALUE, MIN, MAX) ((VALUE)<(MIN)? (MIN): ((VALUE)>(MAX)? (MAX): (VALUE)))

void KEY_Init(void){

}
/**
  * @brief  读取指定按键对应的IO口电平值，不需要消抖，一次读取即可，该函数需要用户根据硬件自行实现
  * @param  按键的ID值，用户自行编码
  * @retval IO口电平值，本实现中1：按键按下，0：按键按下，
  */
KEY_ClickTypeDef KEY_Read_Port(uint8_t KeyId){
    if(digitalRead(KeyId)) return KEY_OFF;
    else return KEY_ON;
} 
KEY_FsmTypeDef KEY_Get_Fsm(KEY_HandleTypeDef* keyHandle){
    switch(keyHandle->KeyFsmState){
        case KEY_FSM_SHORT_PRESS:
            keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
            return KEY_FSM_SHORT_PRESS;
            break;
        case KEY_FSM_DOUBLE_PRESS:
            keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
            return KEY_FSM_DOUBLE_PRESS;
            break;
        case KEY_FSM_TRIPLE_PRESS:
            keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
            return KEY_FSM_TRIPLE_PRESS;
            break;
        case KEY_FSM_LONG_PRESS:
            keyHandle->KeyFsmState = KEY_FSM_RELEASE;
            return KEY_FSM_LONG_PRESS;
            break;  
        default: return KEY_FSM_WAIT_PRESS;                      
    }
}
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle){
    return keyHandle->KeyState;
}
void KEY_Scan(KEY_HandleTypeDef* keyHandle){
    KEY_ClickTypeDef key_state = KEY_Read_Port(keyHandle->KeyId);
    if(keyHandle->KeyClick != key_state) { //两次读取一致才进行更新操作，相当于去抖动
        keyHandle->KeyClick = key_state;
        return;
    } 

    switch(keyHandle->KeyState){
        case    KEY_RELEASED:
            if(keyHandle->KeyClick == KEY_ON){
                keyHandle->KeyState = KEY_FALLING_EDGE;
                keyHandle->KeyStateTimeStamp = millis();
                keyHandle->KeyLastState = KEY_RELEASED;
                keyHandle->KeyLastStateTimeStamp = millis();
            }
            break;
        case    KEY_FALLING_EDGE:
            if(keyHandle->KeyClick == KEY_ON){
                keyHandle->KeyLastState = KEY_FALLING_EDGE;
                keyHandle->KeyLastStateTimeStamp = keyHandle->KeyStateTimeStamp;
                keyHandle->KeyState = KEY_LOW_HOLDING;
                keyHandle->KeyStateTimeStamp = millis();
            }else{
                keyHandle->KeyState = KEY_RELEASED;
                keyHandle->KeyStateTimeStamp = millis();
            }
            break;
        case    KEY_LOW_HOLDING:
            if(keyHandle->KeyClick == KEY_OFF){
                keyHandle->KeyState = KEY_RISING_EDGE;
                keyHandle->KeyStateTimeStamp = millis();
            }       
            break;
        case    KEY_RISING_EDGE:
            if(keyHandle->KeyClick == KEY_OFF){
                keyHandle->KeyLastState = KEY_RISING_EDGE;
                keyHandle->KeyLastStateTimeStamp = keyHandle->KeyStateTimeStamp;
                keyHandle->KeyState = KEY_HIGH_HOLDING;
                keyHandle->KeyStateTimeStamp = millis();
            } else{ //上升沿结束后被按下认为是一个抖动过滤掉
                keyHandle->KeyState = KEY_LOW_HOLDING;
                keyHandle->KeyStateTimeStamp = keyHandle->KeyLastStateTimeStamp;
            } 
            break;
        case    KEY_HIGH_HOLDING:
            if(keyHandle->KeyClick == KEY_OFF){
                if(millis() - keyHandle->KeyStateTimeStamp > 1000){
                    keyHandle->KeyState = KEY_RELEASED;
                    keyHandle->KeyStateTimeStamp = millis();
                    keyHandle->KeyLastState = KEY_RELEASED;
                    keyHandle->KeyLastStateTimeStamp = millis();
                }   
            } else{
                keyHandle->KeyState = KEY_FALLING_EDGE;
                keyHandle->KeyStateTimeStamp = millis();
            }         
            break;
    }

    if(keyHandle->KeyLastState == KEY_FALLING_EDGE && keyHandle->KeyState == KEY_RISING_EDGE){
        if(IsRange(keyHandle->KeyStateTimeStamp - keyHandle->KeyLastStateTimeStamp, 70, 400)){
            if(keyHandle->KeyFsmState == KEY_FSM_WAIT_PRESS) keyHandle->KeyFsmState = KEY_FSM_SHORT_TEMP;
            else if(keyHandle->KeyFsmState == KEY_FSM_SHORT_TEMP) keyHandle->KeyFsmState = KEY_FSM_DOUBLE_TEMP;
            else if(keyHandle->KeyFsmState == KEY_FSM_DOUBLE_TEMP) keyHandle->KeyFsmState = KEY_FSM_TRIPLE_TEMP;
            else keyHandle->KeyFsmState = KEY_FSM_RELEASE;
            keyHandle->KeyFsmStateTimeStamp = millis();
        }else keyHandle->KeyFsmState = KEY_FSM_RELEASE;
    }else if(keyHandle->KeyLastState == KEY_RISING_EDGE && keyHandle->KeyState == KEY_HIGH_HOLDING){
        if(millis() - keyHandle->KeyStateTimeStamp > 500){
            if(keyHandle->KeyFsmState == KEY_FSM_SHORT_TEMP) keyHandle->KeyFsmState = KEY_FSM_SHORT_PRESS;
            else if(keyHandle->KeyFsmState == KEY_FSM_DOUBLE_TEMP) keyHandle->KeyFsmState = KEY_FSM_DOUBLE_PRESS;
            else if(keyHandle->KeyFsmState == KEY_FSM_TRIPLE_TEMP) keyHandle->KeyFsmState = KEY_FSM_TRIPLE_PRESS;
            keyHandle->KeyFsmStateTimeStamp = millis();
        }
    }else if(keyHandle->KeyLastState == KEY_FALLING_EDGE && keyHandle->KeyState == KEY_LOW_HOLDING){
        if(millis() - keyHandle->KeyStateTimeStamp > 1000){
            if(keyHandle->KeyFsmState == KEY_FSM_WAIT_PRESS) keyHandle->KeyFsmState = KEY_FSM_LONG_PRESS;
            keyHandle->KeyFsmStateTimeStamp = millis();
        }
    }

    if(keyHandle->KeyFsmState == KEY_FSM_RELEASE && keyHandle->KeyClick == KEY_OFF) 
        keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
    if(keyHandle->KeyFsmState != KEY_FSM_WAIT_PRESS && millis() - keyHandle->KeyFsmStateTimeStamp > 1000) 
        keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;

}