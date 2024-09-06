import web_webview from '@ohos.web.webview';

import router from '@ohos.router';
import GreenHousesHttpRequest from '../Greenhouses/greenHousesHttp'
import GreenHousesData from '../Data/greenHousesData'

@Entry
@Preview
@Component
export struct dataDisplayPage {
  webviewController: web_webview.WebviewController = new web_webview.WebviewController()

  @State deviceName: string = router.getParams()['deviceName'] || '未提供设备名称'

  @State getIot:GreenHousesData|null=null

  @State deviceDataArray: { temp: number; yanwu: string; CO2: number; humi: number;light:number;TVOC:number; }[] = [];

  getDevice() {
    if (this.getIot) {
      const dataObject = {
        temp: this.getIot.temp, // 修改为'temp'
        yanwu: this.getIot.yanwu.toString(), // 确保是字符串类型
        CO2: this.getIot.CO2,
        humi: this.getIot.humi, // 修改为'humi'
        light: this.getIot.light, // 添加'light'属性
        TVOC: this.getIot.TVOC
      };
      // 清空数组后再添加新的数据对象
      this.deviceDataArray = [];  // 清空数组
      this.deviceDataArray.push(dataObject);  // 添加新的数据对象
      console.log('结果：', JSON.stringify(this.deviceDataArray, null, 2));
    }
  }



  build() {
    Column(){
      Row(){
        //返回
        Column(){
          Shape() {
            Circle({ width: 70, height: 70 }).fill("#FFFFFF")
            Image($r('app.media.black')).onClick(()=>{
              router.back()
            })
              .width(20)
              .height(20)
              .margin({ left: 25, top: 25 })
          }
        }
        Blank()
        //设备名称
        Column(){
          Text(this.deviceName)
            .fontSize(25)
        }.margin({left:30})
        Blank()
        //刷新数据
      }.width('100%').height(100).justifyContent(FlexAlign.Start)
      //加载柱状图
      Row(){
        if(this.deviceName==='大棚温度'){
          Web({ src: $rawfile("web/data_analysis.html"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }else if(this.deviceName==='大棚湿度'){
          Web({ src: $rawfile("web/showHumi.html"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }else if(this.deviceName==='土壤湿度'){
          Web({ src: $rawfile("web/showSoil.html"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }else if(this.deviceName==='大棚光照强度'){
          Web({ src: $rawfile("web/showLight.html"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }else if(this.deviceName==='大棚二氧化碳'){
          Web({ src: $rawfile("web/showCo2.html"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }else{
          Web({ src: $rawfile("web/"), controller: this.webviewController}).backgroundColor("#EDF8FF").width("100%").height("100%")
        }
      }.width("100%").height("100%")
    }.width('100%').height('100%').backgroundColor("#EDF8FF").justifyContent(FlexAlign.SpaceBetween)
  }

  aboutToAppear(){
    this.loadIotDataInfo()
  }
  loadIotDataInfo() {
    GreenHousesHttpRequest.getGreenData().then(data => {
      if (data && data.length > 0) {
        this.getIot = data[0];
      }
    }).catch(error => {
      console.error('错误数据:', error);

    });
  }
}
