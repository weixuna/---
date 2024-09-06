#ifndef __SYS_H
#define __SYS_H

//头文件
#include "stm32f10x.h"
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


//内部外设
#include "time1.h"
#include "time2.h"
#include "time3.h"
#include "Delay.h"
#include "usart1.h"
#include "usart3.h"


//外部外设
#include "LED.h"
#include "Key.h"
#include "DHT11.h"
#include "OLED.h"
#include "JDQ.h"
#include "Motor.h"
#include "PWM.h"
#include "soil_sensor.h"
#include "buzzer.h"

//传感器
#include "Sensor.h"
#include "BH1750.h"
#include "SGP30.h"
#include "lora.h"
#include "MQ-2.h"


#endif
