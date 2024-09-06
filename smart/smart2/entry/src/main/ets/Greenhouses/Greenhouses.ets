import GreenHousesHttpRequest from '../Greenhouses/greenHousesHttp'
import GreenHousesData from '../Data/greenHousesData'
class climaticModel{
  public climaticImage:ResourceStr
  public climaticNum:string
  public climaticName:string

  constructor(climaticImage:ResourceStr,climaticNum:string,climaticName:string) {
    this.climaticImage = climaticImage
    this.climaticNum = climaticNum
    this.climaticName = climaticName
  }
}

@Component
export struct greenHouse {
  scrpller: Scroller = new Scroller();
  @State climaticList: climaticModel[] = []

  @State getIot:GreenHousesData|null=null


  build() {
      Row(){
        Grid(this.scrpller){
          ForEach(this.climaticList,(climatic:climaticModel)=>{
            GridItem(){
              this.dogItem(climatic)
            }
            .width(140).height(100).borderRadius(20).backgroundColor('0xEFF0F4').shadow({radius:50,color: 0xEFF0F4,offsetX:15,offsetY:15})
          })
        }.columnsTemplate('1fr 1fr').columnsGap(10).rowsGap(10).width('90%')
      }
  }


  @Builder
  dogItem(climatic:climaticModel){
    Image(climatic.climaticImage).width(50).height(50).margin({top:30,right:70})
    Text(climatic.climaticNum).padding({left:95,top:50}).fontSize(20)
    Text(climatic.climaticName).margin({bottom:85,right:70}).fontSize(20).height(50)
  }
  aboutToAppear(){
    this.loadIotDataInfo()
  }

  loadIotDataInfo() {
    GreenHousesHttpRequest.getGreenData().then(data => {
      this.getIot = data[0];
      console.log('getIot的数据为', JSON.stringify(this.getIot, null, 2)); // 添加这行来打印更新后的getIot数据
      this.updateClimaticList();
    }).catch(error => {
      console.error('错误数据:', error);
    });
  }

  updateClimaticList() {
    if (this.getIot) {
      const yanwuDisplay = this.getIot.yanwu === 'youyan' ? '有烟' : this.getIot.yanwu === 'wuyan' ? '无烟' : this.getIot.yanwu;
      this.climaticList = [
        new climaticModel($r('app.media.wendu'), `${this.getIot.temp}°C`, '温度'),
        new climaticModel($r('app.media.shidu'), `${this.getIot.humi}%`, '湿度'),
        new climaticModel($r('app.media.CO2'), `${this.getIot.CO2}`, 'CO2'),
        new climaticModel($r('app.media.guangzhao'), `${this.getIot.light}lx`, '光照'),
        new climaticModel($r('app.media.soil'), `${this.getIot.Soil}%`, '土壤湿度'),
        new climaticModel($r('app.media.yanwu'), yanwuDisplay, '烟雾'),
        new climaticModel($r('app.media.fengsu'), '3级', '风速'),
        new climaticModel($r('app.media.TVOC'), `${this.getIot.TVOC}`,'TVOC')
      ];
    }
  }

}

