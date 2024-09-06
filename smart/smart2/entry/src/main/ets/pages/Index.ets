import { Weather } from '../Weather/Weather'
import {greenHouse} from '../Greenhouses/Greenhouses'
import {deviceStatus} from '../IoTdevices/deviceStatus'
import {deviceManagement} from '../deviceManagementPages/deviceManagment'
import {dataAnalysis} from '../dataAnalysis/dataAnalysis'
import {deviceRules} from  '../Devicelinkage/create_Rule'
import web_webview from '@ohos.web.webview'
@Entry
@Component
struct Index {
  webviewController: web_webview.WebviewController = new web_webview.WebviewController()
  build() {
    Tabs({ barPosition: BarPosition.End }) {
      //首页
      TabContent() {
        Column({ space: 5 }) {
          //顶部
          Row() {
            Weather();
          }.width('90%').height(160).backgroundColor(0xFFFFFF)
          Text('大棚信息').width('90%').height(30).fontSize(20).padding({ right: 200}).margin({top:150})
          //气象信息
          Row() {
              greenHouse()
          }.height('50%')
        }.justifyContent(FlexAlign.SpaceBetween)
      }
      .tabBar('首页')

      TabContent() {
        deviceManagement()
      }
      .tabBar('设备管理')

      TabContent() {
        dataAnalysis()
      }
      .tabBar('数据采集')

      TabContent() {
        deviceRules()
      }
      .tabBar('设备联动')

    }
    .height('100%').width('100%')
  }
}