import greenHousesHttpRequest from '../Greenhouses/greenHousesHttp'


@Component
export struct deviceStatus {

  @State deviceONLINE:string='…'  //在线设备个数
  @State deviceOFFLINE:string='…'  //离线设备个数

  build() {
    Row() {
      Row(){
        Image($r('app.media.chuanganqi')).width(60)
        Row(){
          Column(){
            Text('在线：'+this.deviceONLINE).fontSize(26).margin({left:20}) //显示传感器在线个数
          }
          Column(){
            Text('离线：'+this.deviceOFFLINE).fontSize(26).margin({left:20}) //显示传感器离线个数
          }
        }.height(40).justifyContent(FlexAlign.SpaceEvenly)
      }.width('100%').height('90%').borderRadius(15).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 })
    }.width('90%').backgroundColor(0xFFFFFF).height(100)
  }


  aboutToAppear(){
    this.localDeviceStatus()
  }

  async localDeviceStatus(){
    try {
      const statuses = await greenHousesHttpRequest.getDeviceStatus();
      const onlineCount = statuses.filter(status => status === 'ONLINE').length;
      const offlineCount = statuses.filter(status => status !== 'ONLINE').length;
      this.deviceONLINE = onlineCount.toString();
      this.deviceOFFLINE = offlineCount.toString();
    } catch (error) {
      console.error('Error fetching device statuses:', error);
    }
  }

}