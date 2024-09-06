export default class GreenHousesData {
  light: number; //光强
  temp: number; //温度
  CO2: number; //CO2
  humi: number; //湿度
  yanwu: string; //烟雾
  TVOC: number; //TVOC
  Soil: number; //土壤湿度
  feng: number; //风速
  date:number;

  constructor(data: {
    light: number,
    temp: number,
    CO2: number,
    humi: number,
    yanwu: string,
    TVOC: number,
    Soil: number,
    feng: number,
    date:number
  }) {
    this.light = data.light;
    this.temp = data.temp;
    this.CO2 = data.CO2;
    this.humi = data.humi;
    this.yanwu = data.yanwu;
    this.Soil = data.Soil;
    this.TVOC = data.TVOC;
    this.feng = data.feng;
    this.date=data.date;
  }
}