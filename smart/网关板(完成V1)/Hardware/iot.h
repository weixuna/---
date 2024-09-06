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
#include "stm32f10x.h" // Device header
///
/*宏定义区*/
// 使用时请在这里补全相关信息
#define WIFI_SSID "LMJZ"                                                                        // WiFi名
#define WIFI_PWD "12345678"                                                                     // WiFi密码
#define HUAWEI_MQTT_USERNAME "667145986bc31504f06f60ee_wulianwang"                              // Username
#define HUAWEI_MQTT_PASSWORD "63e106dff798f9ed7d313f33c5e1cffe4ad206abe2121b993d464a8f111a97ba" // Password
#define HUAWEI_MQTT_ClientID "667145986bc31504f06f60ee_wulianwang_0_0_2024063014"               // ClientId
#define HUAWEI_MQTT_ADDRESS "0ae1ed433d.st1.iotda-device.cn-north-4.myhuaweicloud.com"                 // 平台地址
#define HUAWEI_MQTT_PORT "1883"                                                                 // 端口号
#define HUAWEI_MQTT_DeviceID "667145986bc31504f06f60ee_wulianwang"                              // 设备ID
#define HUAWEI_MQTT_ServiceID "Dev_data"                                                        // 服务ID
///
/*外部变量声明区*/

///
/*函数声明区*/
void AT_write(char atstring[512]); // 阻塞等待OK
void HuaweiIot_init(void);
void HuaweiIot_DevDate_publish(char *att, uint16_t data);
void HuaweiIot_DevDate_String_publish(char *att, const char *data);
void HuaweiIot_SentData(void);
///
#endif
