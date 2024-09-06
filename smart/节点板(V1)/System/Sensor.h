#ifndef SENSOR_H
#define SENSOR_H

extern uint32_t CO2Data;
extern uint32_t TVOCData;



void InitSystem(void);
void WaitForSGP30Stable(void);
void ShowSensorData(void);
void ReadSGP30Data(void);
void HandleKeyInput(void);


#endif // SENSOR_H
