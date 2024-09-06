#include "sys.h"

/**
 * author: zhenpeng on Wed, 21 May 2024
 * brief: 定义所有函数
 **/
static void BH1750_I2C_GPIOInit(void);
void BH1750_I2C_Start(void);
void BH1750_I2C_Stop(void);
void BH1750_I2C_SendByte(uint8_t Byte);
uint8_t BH1750_I2C_ReadByte(void);
uint8_t BH1750_I2C_WaitAck(void);
uint16_t BH1750_FluxValue(void);
void BH1750_Init(void);

/**
 * author: zhenpeng on Wed, 21 May 2024
 * brief: GPIO初始化函数
 * param: void
 * return: void
 **/
static void BH1750_I2C_GPIOInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = BH1750_I2C_SCL_GPIOPin | BH1750_I2C_SDA_GPIOPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BH1750_I2C_GPIO_Port, &GPIO_InitStructure);

    BH1750_I2C_SDA(HIGH);
    BH1750_I2C_SCL(HIGH);
}

void BH1750_I2C_Start(void)
{
    BH1750_I2C_SDA(HIGH);
    BH1750_I2C_SCL(HIGH);
    delay_us(10);
    BH1750_I2C_SDA(LOW);
    delay_us(10);
    BH1750_I2C_SCL(LOW);
    delay_us(10);
}

void BH1750_I2C_Stop(void)
{
    BH1750_I2C_SCL(LOW);
    BH1750_I2C_SDA(LOW);
    delay_us(10);
    BH1750_I2C_SCL(HIGH);
    delay_us(10);
    BH1750_I2C_SDA(HIGH);
    delay_us(10);
}

void BH1750_I2C_SendByte(uint8_t Byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        BH1750_I2C_SDA(Byte & (0x80 >> i) ? HIGH : LOW);
        BH1750_I2C_SCL(HIGH);
        delay_us(10);
        BH1750_I2C_SCL(LOW);
        delay_us(10);
    }
}

uint8_t BH1750_I2C_ReadByte(void)
{
    uint8_t byte = 0x00;
    BH1750_I2C_SDA(HIGH);
    delay_us(10);
    for (uint8_t i = 0; i < 8; i++)
    {
        BH1750_I2C_SCL(HIGH);
        delay_us(10);
        if (BH1750_I2C_SDARead())
        {
            byte |= (0x80 >> i);
        }
        BH1750_I2C_SCL(LOW);
        delay_us(10);
    }
    return byte;
}

uint8_t BH1750_I2C_WaitAck(void)
{
    uint8_t AckTag = 0;
    BH1750_I2C_SDA(HIGH);
    BH1750_I2C_SCL(HIGH);
    delay_us(10);
    if (BH1750_I2C_SDARead())
    {
        AckTag = 1;
    }
    BH1750_I2C_SCL(LOW);
    delay_us(10);
    return AckTag;
}

void BH1750_I2C_SendAck(void)
{
    BH1750_I2C_SDA(LOW);
    delay_us(10);
    BH1750_I2C_SCL(HIGH);
    delay_us(10);
    BH1750_I2C_SCL(LOW);
    delay_us(10);
}

void BH1750_I2C_SendNAck(void)
{
    BH1750_I2C_SDA(HIGH);
    delay_us(10);
    BH1750_I2C_SCL(HIGH);
    delay_us(10);
    BH1750_I2C_SCL(LOW);
    delay_us(10);
}

uint8_t BH1750_SendData(uint8_t data)
{
    BH1750_I2C_Start();
    BH1750_I2C_SendByte(BH1750_CMD_AddWrite);
    if (BH1750_I2C_WaitAck())
    {
        BH1750_I2C_Stop();
        return 1;
    }
    BH1750_I2C_SendByte(data);
    if (BH1750_I2C_WaitAck())
    {
        BH1750_I2C_Stop();
        return 2;
    }
    BH1750_I2C_Stop();
    return 0;
}

uint16_t BH1750_ReadData(void)
{
    uint8_t MSB, LSB;
    BH1750_I2C_Start();
    BH1750_I2C_SendByte(BH1750_CMD_AddRead);
    if (BH1750_I2C_WaitAck())
    {
        BH1750_I2C_Stop();
        return 0;
    }
    MSB = BH1750_I2C_ReadByte();
    BH1750_I2C_SendAck();
    LSB = BH1750_I2C_ReadByte();
    BH1750_I2C_SendNAck();
    BH1750_I2C_Stop();
    return (MSB << 8) | LSB;
}

/**
 * author: zhenpeng on Wed, 21 May 2024
 * brief: BH1750读取并转换光照值函数
 * param: void
 * return: float temp 返回Flux单位的光照值
 * note: 需要先调用BH1750_Init()函数初始化整个BH1750后，才能调用.
 **/
uint16_t BH1750_FluxValue(void)
{
    uint16_t temp = 0;
    uint16_t raw_data = BH1750_ReadData();
    temp = raw_data / 1.2;
    return temp;
}

void BH1750_Init(void)
{
    BH1750_I2C_GPIOInit();
    BH1750_SendData(BH1750_CMD_PowOn);
    BH1750_SendData(BH1750_CMD_ModeH1);
}
