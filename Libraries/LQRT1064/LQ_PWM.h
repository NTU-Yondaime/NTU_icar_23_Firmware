#ifndef __LQPWM_H__
#define __LQPWM_H__
#include "LQ_GPIO_Cfg.h"
#include "fsl_pwm.h"
/*    龙邱母板上使用的PWM管脚
PWM1, kPWM_Module_3, kPWM_PwmA   L5     电机1接口
PWM1, kPWM_Module_3, kPWM_PwmB   M5     电机1接口
PWM2, kPWM_Module_0, kPWM_PwmA   A8     电机2接口
PWM2, kPWM_Module_0, kPWM_PwmB   A9     电机2接口
PWM2, kPWM_Module_1, kPWM_PwmA   B9     电机3接口
PWM2, kPWM_Module_1, kPWM_PwmB   C9     电机3接口
PWM2, kPWM_Module_2, kPWM_PwmB   A10    电机4接口
PWM1, kPWM_Module_1, kPWM_PwmA   J1     电机4接口
PWM2, kPWM_Module_3, kPWM_PwmA   M3     舵机接口
PWM2, kPWM_Module_3, kPWM_PwmB   M4     舵机接口*/

/* 最大占空比 可自行修改 */
#define DUTY_MAX  10000  

/**********************************  PWM(引脚复用) *******************************************************/
//     PWM输出通道                  端口       可选范围                   建议
#define PWM1_MODULE0_PWMA             J4      //  G2  J4                   G2接外部SDRAM
#define PWM1_MODULE0_PWMB             J3      //  D3  J3                   D3接外部SDRAM
#define PWM1_MODULE1_PWMA             J1      //  D2  J1                   D2接外部SDRAM
#define PWM1_MODULE1_PWMB             K1      //  B3  K1                   B3接外部SDRAM
#define PWM1_MODULE2_PWMA             H2      //  A2  H2                   A2接外部SDRAM
#define PWM1_MODULE2_PWMB             J2      //  D1  J2                   D1接外部SDRAM
#define PWM1_MODULE3_PWMA             L5      //  A11 D6  G13  H1  L5      H1 D6接外部SDRAM
#define PWM1_MODULE3_PWMB             M5      //  A6  B7  B11  G10 M5      A6 B7接外部SDRAM

#define PWM2_MODULE0_PWMA             A8      //  A8  H5                   H5 接外部SDRAM
#define PWM2_MODULE0_PWMB             A9      //  A9  H4                   H4 接外部SDRAM
#define PWM2_MODULE1_PWMA             B9      //  B9  H3                   H3 接外部SDRAM
#define PWM2_MODULE1_PWMB             C9      //  C2  C9                   C2 接外部SDRAM
#define PWM2_MODULE2_PWMA             D9      //  D9  G1                   G1 接外部SDRAM
#define PWM2_MODULE2_PWMB             A10     //  A10 G3                   G3 接外部SDRAM
#define PWM2_MODULE3_PWMA             M3      //  B4  C11  F14 M3  M14     B4 接外部SDRAM  F14 JTAG TDI接口
#define PWM2_MODULE3_PWMB             M4      //  A3  D11  H10 M4          A3 接外部SDRAM

#define PWM3_MODULE0_PWMA             E1      //  E1                       E1 接外部SDRAM
#define PWM3_MODULE0_PWMB             C6      //  C6                       C6 接外部SDRAM
#define PWM3_MODULE1_PWMA             C5      //  C5                       C5 接外部SDRAM
#define PWM3_MODULE1_PWMB             D5      //  D5                       D5 接外部SDRAM
#define PWM3_MODULE2_PWMA             C4      //  C4                       C4 接外部SDRAM
#define PWM3_MODULE2_PWMB             D4      //  D4                       D4 接外部SDRAM
#define PWM3_MODULE3_PWMA             C1      //  C1                       C1 接外部SDRAM
#define PWM3_MODULE3_PWMB             F1      //  F1                       F1 接外部SDRAM

#define PWM4_MODULE0_PWMA             H13     //  E3  H13                  E3 接外部SDRAM
#define PWM4_MODULE0_PWMB             F3      //  F3                       F3 接外部SDRAM
#define PWM4_MODULE1_PWMA             M13     //  F4  M13                  F4 接外部SDRAM
#define PWM4_MODULE1_PWMB             G4      //  G4                       G4 接外部SDRAM
#define PWM4_MODULE2_PWMA             C14     //  F2  C14                  F2 接外部SDRAM
#define PWM4_MODULE2_PWMB             G5      //  G5                       G5 接外部SDRAM
#define PWM4_MODULE3_PWMA             B14     //  A4  B14                  A4 接外部SDRAM
#define PWM4_MODULE3_PWMB             B2      //  B2                       B2 接外部SDRAM




/**
  * @brief    测试PWM
  *
  * @param    
  *
  * @return   
  *
  * @note     测试龙邱母板上的PWM接口是否有PWM输出，可用示波器测量
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void Test_PWM_nCH(void);



/**
  * @brief    电机测试
  *
  * @param    
  *
  * @return   
  *
  * @note     电机1： 使用 L5  M5   电机2：使用A8  A9  电机3：使用 B9 C9  电机4：A10 J1
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void Test_Motor(void);



/**
  * @brief    舵机测试
  *
  * @param    
  *
  * @return   
  *
  * @note     初次使用此函数时，舵机不要装舵盘，防止舵机卡死，设置中值后再装车，可以自行设置中值
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void Test_Servo(void);



/**
  * @brief    设置电机占空比
  *
  * @param    num： 1：电机1： 使用 L5  M5  
                    2：电机2： 使用 A8  A9  
                    3：电机3： 使用 B9  C9  
                    4：电机4： 使用 A10 J1
  *
  * @return   
  *
  * @note     电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_SetMotorDty(uint8_t num, short duty);



/**
  * @brief    设置舵机占空比
  *
  * @param    duty          ：占空比*DUTY_MAX   
  *
  * @return   
  *
  * @note      舵机中值 1.5ms高电平的PWM波    限幅在 1.3 - 1.7ms之间
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_SetServoDty(uint16_t duty);


/**
  * @brief    同时设置 一个子模块的A 和 B相  用这个函数
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  * @param    duty          ：占空比*DUTY_MAX  
  *
  * @return   
  *
  * @note     同时设置 一个子模块的A 和 B相  可以用这个函数
  * @note     默认对齐方式kPWM_SignedCenterAligned， 可以自行修改  
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_PWMA_B_SetDuty(PWM_Type *base, pwm_submodule_t subModule, uint16_t dutyA, uint16_t dutyB);


/**
  * @brief    设置占空比
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  * @param    duty          ：占空比*DUTY_MAX  
  *
  * @return   
  *
  * @note     默认对齐方式kPWM_SignedCenterAligned， 可以自行修改  
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_PWM_SetDuty(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t duty);



/**
  * @brief    PWM功能初始化
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  * @param    Frequency     ：PWM模块下的子模块的频率
  *
  * @return   
  *
  * @note     PWM 使用 IPG时钟  150M
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_PWM_Init(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t Frequency);


      

/**
  * @brief    PWM引脚初始化
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  *
  * @return   
  *
  * @note     内部调用的，仅用来设置串口管脚复用的  
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void LQ_PWM_InitPins(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels);


/**
  * @brief    改变占空比
  *
  * @param    base       ：PWM模块 PWM1~PWM4
  * @param    subModule  ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwmSignal  ：kPWM_PwmA/kPWM_PwmB
  * @param    currPwmMode：对齐方式
  * @param    duty       ：占空比*DUTY_MAX
  *
  * @return   无
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void PWM_UpdateDuty(PWM_Type *base,
                            pwm_submodule_t subModule,
                            pwm_channels_t pwmSignal,
                            pwm_mode_t currPwmMode,
                            uint16_t duty);

#endif 



