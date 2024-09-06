/*
*********************************************************************************************************
*	模块名称： esp8266+AT指令+华为云物联网平台
*	文件名称： HuaweiIOT_at_esp8266.h
*	说明备注：无
*	修改记录：无
*	版本号     日期          作者     						说明
*	V1.0       22-8-1      	funiot.xyz|公众号“IOT趣制作”  
*	Copyright by JIA
*********************************************************************************************************
*/
#ifndef __HUAWEIIOT_H
#define __HUAWEIIOT_H

///
/*头文件包含区*/
#include "stm32f10x.h"                  // Device header
///
/*宏定义区*/
//使用时请在这里补全相关信息
#define WIFI_SSID						"LMJZ"  //WiFi名
#define WIFI_PWD						"12345678"  //WiFi密码
#define HUAWEI_MQTT_USERNAME			"66362c827bdccc0126d6c1c3_wulianwang"  //Username
#define HUAWEI_MQTT_PASSWORD			"b11cab750b1bddd45cf00c2c2e0dd454537dd2f869af1c5d3f918a5bc0617520"  //Password
#define HUAWEI_MQTT_ClientID			"66362c827bdccc0126d6c1c3_wulianwang_0_0_2024052713"  //ClientId
#define HUAWEI_MQTT_ADDRESS				"f31531a1fe.iot-mqtts.cn-north-4.myhuaweicloud.com"  //平台地址
#define HUAWEI_MQTT_PORT				"1883"  //端口号
#define HUAWEI_MQTT_DeviceID			"66362c827bdccc0126d6c1c3_wulianwang"  //设备ID
#define HUAWEI_MQTT_ServiceID			"Dev_data"  //服务ID
///
/*外部变量声明区*/

///
/*函数声明区*/
void AT_write(char atstring[512]);//阻塞等待OK
void HuaweiIot_init(void);
void HuaweiIot_DevDate_publish(char * att,uint16_t data);
void HuaweiIot_DevDate_String_publish(char * att,const char *data);
///
#endif

