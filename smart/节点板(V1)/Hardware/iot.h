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
#include "stm32f10x.h"                  // Device header
///
/*�궨����*/
//ʹ��ʱ�������ﲹȫ�����Ϣ
#define WIFI_SSID						"LMJZ"  //WiFi��
#define WIFI_PWD						"12345678"  //WiFi����
#define HUAWEI_MQTT_USERNAME			"66362c827bdccc0126d6c1c3_wulianwang"  //Username
#define HUAWEI_MQTT_PASSWORD			"b11cab750b1bddd45cf00c2c2e0dd454537dd2f869af1c5d3f918a5bc0617520"  //Password
#define HUAWEI_MQTT_ClientID			"66362c827bdccc0126d6c1c3_wulianwang_0_0_2024052713"  //ClientId
#define HUAWEI_MQTT_ADDRESS				"f31531a1fe.iot-mqtts.cn-north-4.myhuaweicloud.com"  //ƽ̨��ַ
#define HUAWEI_MQTT_PORT				"1883"  //�˿ں�
#define HUAWEI_MQTT_DeviceID			"66362c827bdccc0126d6c1c3_wulianwang"  //�豸ID
#define HUAWEI_MQTT_ServiceID			"Dev_data"  //����ID
///
/*�ⲿ����������*/

///
/*����������*/
void AT_write(char atstring[512]);//�����ȴ�OK
void HuaweiIot_init(void);
void HuaweiIot_DevDate_publish(char * att,uint16_t data);
void HuaweiIot_DevDate_String_publish(char * att,const char *data);
///
#endif

