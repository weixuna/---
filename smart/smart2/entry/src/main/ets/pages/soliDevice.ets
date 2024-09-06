import router from '@ohos.router';
import GreenHousesHttpRequest from '../Greenhouses/greenHousesHttp'
import GreenHousesData from '../Data/greenHousesData'
import web_webview from '@ohos.web.webview';
@Entry
@Component
export struct soliDevice {

  webviewController: web_webview.WebviewController = new web_webview.WebviewController()

  build() {
    // 线性布局
    Row() {
      Column() {
        Row({ space: 10 }) {
          Column({ space: 10 }) {
            Row() {
              Shape() {
                Circle({ width: 50, height: 50 }).fill("#FFFFFF")
                Image($r('app.media.black')).onClick(() => {
                  router.back();
                }).width(20).height(20).margin({ left: 15, top: 15 });
              }
              Text('土壤湿度传感器').fontSize(25).margin({ left: 70 });
            }
          }.alignItems(HorizontalAlign.Start);
        }.width('98%').height(80).borderRadius(15);
        Row() {
          Web({ src: $rawfile("web/soliDevice.html"), controller: this.webviewController })
        }.width('100%').padding({left:5,right:5})
      }
    }
  }
}
