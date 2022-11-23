#include <main.h>
//建议10ms扫描一次
//这些常数大小，可以根据项目实际需求进行修改
//按键按下消抖计数
#define KEY_PRESS_DB_CNT        3
//判断按键长按计数
#define KEY_LONG_PRESS_CNT      150
//判断按键连击计数
#define KEY_REPEAT_PRESS_CNT    50

//按键扫描状态机状态编码
typedef enum{
        KEY_FSM_WAIT_PRESS = 0,//按键状态机：“按键等待被按下”处理
        KEY_FSM_SHORT_TEMP,
        KEY_FSM_SHORT_WAIT,
        KEY_FSM_SHORT_PRESS,//按键状态机：“按键短按”处理
        KEY_FSM_DOUBLE_TEMP,
        KEY_FSM_DOUBLE_WAIT,
        KEY_FSM_DOUBLE_PRESS,//按键状态机：“按键连击”处理
        KEY_FSM_TRIPLE_TEMP,
        KEY_FSM_TRIPLE_WAIT,
        KEY_FSM_TRIPLE_PRESS,
        KEY_FSM_LONG_PRESS,//按键状态机：“按键长按”处理
        KEY_FSM_RELEASE//按键状态机：“按键释放”处理
}KEY_FsmTypeDef;
//按键状态编码
typedef enum{
        KEY_RELEASED = 0,//按键状态:释放状态
        KEY_FALLING_EDGE,//按键状态:下降沿
        KEY_LOW_HOLDING,//按键状态:低电平保持
        KEY_RISING_EDGE,//按键状态:上升沿
        KEY_HIGH_HOLDING//按键状态:高电平保持
}KEY_StateTypeDef;
typedef enum{
        KEY_OFF = 0,
        KEY_ON
}KEY_ClickTypeDef;
//按键句柄结构体
typedef struct{
        int KeyId; //按键编号
//        GPIO_TypeDef* GPIOx;
//        uint16_t GPIO_Pin;

        KEY_ClickTypeDef KeyClick;
        KEY_StateTypeDef KeyLastState; //按键当前状态(高4bit为上一个状态，低4bit为当前状态)
        KEY_StateTypeDef KeyState; //按键当前状态(高4bit为上一个状态，低4bit为当前状态)
        unsigned int KeyStateTimeStamp; //内部计数器(高2bit给按下/弹出计数用，低14bit给长按/连击计数用)
        unsigned int KeyLastStateTimeStamp;
        KEY_FsmTypeDef KeyFsmState; //状态机状态
        unsigned int KeyFsmStateTimeStamp;
}KEY_HandleTypeDef;

KEY_FsmTypeDef KEY_Get_Fsm(KEY_HandleTypeDef* keyHandle);
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle);
void KEY_Scan(KEY_HandleTypeDef* keyHandle);

GPIO_TypeDef* index2Gpio_x(uint32_t index);
uint16_t index2Pin_x(uint32_t index);
uint32_t Gpio_pin2Index(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void digitalWrite(int ulPin, int high_or_low);
