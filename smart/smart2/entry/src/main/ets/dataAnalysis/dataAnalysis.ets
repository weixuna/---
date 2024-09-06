import router from '@ohos.router';
@Preview
@Component
export struct dataAnalysis {

  @State canClick: boolean = false; // 增加一个状态用于控制点击
  handleItemClick(deviceIndex: number) {
    if (deviceIndex >= 0 && deviceIndex < this.dataList.length) {
      let deviceName = this.dataList[deviceIndex].dataDeviceName;
      console.log('设备名称',deviceName)
      router.pushUrl({
        url: 'pages/dataDisplayPage',
        params: {
          deviceName: deviceName
        }
      }, router.RouterMode.Single);
    }
  }

  data:Scroller=new Scroller()

  @State dataList : dataAnalysisItem[] = []
  build() {
    Column(){
      Row() {
        Text('数据采集').fontSize(40).margin({ right: 170, top: 20 });
      }
      List({space:8}){
          ForEach(this.dataList,(dataAnalysisListItem:dataAnalysisItem)=>{
            ListItem(){
              this.dataItem(dataAnalysisListItem)
            }.height(80).borderRadius(10).backgroundColor('#FFFFFF').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 }).margin({left:15,top:20}).width('90%')
          })
        }.width('100%').layoutWeight(1)
    }.width('100%').backgroundColor('#FDFDFD')
  }

  aboutToAppear() {
    this.dataList = [
      new dataAnalysisItem($r('app.media.wendu'), '点击获取数据', '大棚温度', () => this.handleItemClick(0)),
      new dataAnalysisItem($r('app.media.shidu'), '点击获取数据', '大棚湿度', () => this.handleItemClick(1)),
      new dataAnalysisItem($r('app.media.soil'), '点击获取数据', '土壤湿度', () => this.handleItemClick(2)),
      new dataAnalysisItem($r('app.media.guangzhao'), '点击获取数据', '大棚光照强度', () => this.handleItemClick(3)),
      new dataAnalysisItem($r('app.media.eryht'), '点击获取数据', '大棚二氧化碳', () => this.handleItemClick(4)),
      new dataAnalysisItem($r('app.media.TVOC'), '点击获取数据', '大棚TVOC', () => this.handleItemClick(5)),
    ];
    this.canClick = true;
  }

  @Builder
  dataItem(dataAnalysisListItem:dataAnalysisItem){
    Flex({direction: FlexDirection.Column}) {
      Row() {
        Column() {
          Shape() {
            Circle({ width: 50, height: 50 }).fill("#F7F7F7")
            Image(dataAnalysisListItem.dataImage)
              .width(30)
              .height(30)
              .margin({ left: 10, top: 10 })
          }
      }.margin({left:20})
      }.width('100%')
      Row(){
        Column(){
          Text(dataAnalysisListItem.dataDeviceName).fontSize(20)
        }.margin({left:10})
      }.width('100%').margin({top:5})
    }
    Column(){
      Text(dataAnalysisListItem.dataText).onClick(() => dataAnalysisListItem.onClick())
    }.margin({left:200})
  }


}
class dataAnalysisItem {
  dataImage: ResourceStr;
  dataText: string;
  dataDeviceName: string;
  onClick: () => void;

  constructor(dataImage: ResourceStr, dataText: string, dataDeviceName: string, onClick: () => void) {
    this.dataImage = dataImage;
    this.dataText = dataText;
    this.dataDeviceName = dataDeviceName; // 设置设备名称属性
    this.onClick = onClick;
  }
}
