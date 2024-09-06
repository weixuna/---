/*
*********************************************************************************************************
*	ģ�����ƣ� esp8266+ATָ��+��Ϊ��������ƽ̨
*	�ļ����ƣ� HuaweiIOT_at_esp8266.h
*	˵����ע����
*	�޸ļ�¼����
*	�汾��     ����          ����     						˵��
*	V1.0       22-8-1      	funiot.xyz|���ںš�IOTȤ������
*	Copyright by JIA
*********************************************************************************************************
*/
#ifndef __HUAWEIIOT_H
#define __HUAWEIIOT_H

///
/*ͷ�ļ�������*/
#include "stm32f10x.h" // Device header
///
/*�궨����*/
// ʹ��ʱ�������ﲹȫ�����Ϣ
#define WIFI_SSID "LMJZ"                                                                        // WiFi��
#define WIFI_PWD "12345678"                                                                     // WiFi����
#define HUAWEI_MQTT_USERNAME "667145986bc31504f06f60ee_wulianwang"                              // Username
#define HUAWEI_MQTT_PASSWORD "63e106dff798f9ed7d313f33c5e1cffe4ad206abe2121b993d464a8f111a97ba" // Password
#define HUAWEI_MQTT_ClientID "667145986bc31504f06f60ee_wulianwang_0_0_2024063014"               // ClientId
#define HUAWEI_MQTT_ADDRESS "0ae1ed433d.st1.iotda-device.cn-north-4.myhuaweicloud.com"                 // ƽ̨��ַ
#define HUAWEI_MQTT_PORT "1883"                                                                 // �˿ں�
#define HUAWEI_MQTT_DeviceID "667145986bc31504f06f60ee_wulianwang"                              // �豸ID
#define HUAWEI_MQTT_ServiceID "Dev_data"                                                        // ����ID
///
/*�ⲿ����������*/

///
/*����������*/
void AT_write(char atstring[512]); // �����ȴ�OK
void HuaweiIot_init(void);
void HuaweiIot_DevDate_publish(char *att, uint16_t data);
void HuaweiIot_DevDate_String_publish(char *att, const char *data);
void HuaweiIot_SentData(void);
///
#endif
