import router from '@ohos.router';
import greenHousesHttpRequest from '../Greenhouses/greenHousesHttp';

@Component
@Preview
export struct deviceManagement {


  senDeviceId(service_id: string, command_name: string, value: string, commandValue: number) {
    //调用 greenHousesHttpRequest.sendDeviceCommand 发送命令
    greenHousesHttpRequest.sendDeviceCommand(service_id, command_name, value, commandValue)
      .then(response => {
        console.log('命令发送成功:', JSON.stringify(response));
      })
      .catch(error => {
        console.error('命令发送失败:', error);
      });
  }


  // 定义一个CustomDialogController实例，用于控制弹窗的显示和隐藏
  fanDeviceSwitch: CustomDialogController = new CustomDialogController({
    // builder属性用于定义弹窗的内容，这里使用TextInputDialog组件
    builder: switchOpenFan({
      senDeviceId: this.senDeviceId.bind(this) // 绑定senDeviceId方法
    }),
    // alignment属性用于设置弹窗的对齐方式，这里设置居中对齐
    alignment: DialogAlignment.Center,
    // offset属性用于设置弹窗的偏移量
    offset: { dx: 0, dy: 0}
  })

  // 定义一个CustomDialogController实例，用于控制弹窗的显示和隐藏
  ledDeviceSwitch: CustomDialogController = new CustomDialogController({
    // builder属性用于定义弹窗的内容，这里使用TextInputDialog组件
    builder: switchOpenBulb({
      senDeviceId: this.senDeviceId.bind(this) // 绑定senDeviceId方法
    }),
    // alignment属性用于设置弹窗的对齐方式，这里设置居中对齐
    alignment: DialogAlignment.Center,
    // offset属性用于设置弹窗的偏移量
    offset: { dx: 0, dy: 0}
  })

  pumDeviceSwitch: CustomDialogController = new CustomDialogController({
    // builder属性用于定义弹窗的内容，这里使用TextInputDialog组件
    builder: switchOpenPum({
      senDeviceId: this.senDeviceId.bind(this) // 绑定senDeviceId方法
    }),
    // alignment属性用于设置弹窗的对齐方式，这里设置居中对齐
    alignment: DialogAlignment.Center,
    // offset属性用于设置弹窗的偏移量
    offset: { dx: 0, dy: 0}
  })

  airDeviceSwitch: CustomDialogController = new CustomDialogController({
    // builder属性用于定义弹窗的内容，这里使用TextInputDialog组件
    builder: switchOpenAir({
      senDeviceId: this.senDeviceId.bind(this) // 绑定senDeviceId方法
    }),
    // alignment属性用于设置弹窗的对齐方式，这里设置居中对齐
    alignment: DialogAlignment.Center,
    // offset属性用于设置弹窗的偏移量
    offset: { dx: 0, dy: 0}
  })

  build() {
    Column() {
      Row() {
        Text('设备管理').fontSize(40).margin({ right: 170, top: 20 });
      }
      Row() {
        Grid() {
          //灯泡
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.dengpao')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('灯泡').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text().fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){ //点亮灯泡
                Button('点击').onClick(()=> {
                  this.ledDeviceSwitch.open()
                }).margin({top:10});
              }.margin({top:10})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //风扇
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.fengsu')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('风扇').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text('在线').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('点击').onClick(()=>{
                  this.fanDeviceSwitch.open()
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //水泵
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.shuibeng')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('水泵').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('点击').onClick(()=>{
                  this.pumDeviceSwitch.open()
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //天窗
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.tianchuang')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('天窗').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text('在线').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('点击').onClick(()=>{
                  this.airDeviceSwitch.open()
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //温湿度
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.wenshiduchuanganqi')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('温湿度传感器').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text('在线').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/tempHumiDevice"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //土壤湿度
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.turangchuanganqi')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('土壤湿度传感器').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text('在线').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/soliDevice"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //光强
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.guangzhaochuanganqi')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('光强传感器').margin({ top: 20 }).fontSize(20)
              Row(){
                Column(){
                  Text('设备状态：').fontSize(20)
                }
                Column(){
                  Text('在线').fontSize(20)
                }
              }.margin({top:10})
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/lightDevice"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //CO2
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.CO2')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('二氧化碳传感器').margin({ top: 20 }).fontSize(20)
              // Row(){
              //   Column(){
              //     Text('设备状态：').fontSize(20)
              //   }
              //   Column(){
              //     Text('在线').fontSize(20)
              //   }
              // }.margin({top:10})
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/co2Device"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          //烟雾传感器
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r('app.media.yanwuchuanganqi')).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('烟雾传感器').margin({ top: 20 }).fontSize(20)
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/co2Device"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
          GridItem(){
            Column() {
              Shape() {
                Circle({ width: 80, height: 80 }).fill("#F7F7F7")
                Image($r("app.media.shexiangtou")).width(50).height(50).margin({ left: 15, top: 15 })
              }
              Text('摄像头').margin({ top: 20 }).fontSize(20)
              Row(){
                Button('查看详细').onClick(()=>{
                  router.pushUrl({
                    url:"pages/esp32cam"
                  })
                })
              }.margin({top:20})
            }
          }.width(100).height(280).borderRadius(20).backgroundColor('0xEFF0F4').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
        }.columnsTemplate('1fr 1fr').columnsGap(10).rowsGap(10).width('100%').margin({ top: 30 });
      }.height('90%');
    };
  }
}

//自定义弹窗，风扇
@CustomDialog
struct switchOpenFan {
  fanDeviceSwitch: CustomDialogController = new CustomDialogController({ builder: switchOpenFan() })
  senDeviceId: (service_id: string, command_name: string, value: string, commandValue: number) => void;
  build() {
    Column({ space: 20 }) {
      Text('选择功能')
      Row({ space: 15 }) {
        Button('一档').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenFan', 'fan', 1);
          console.log("打开成功");
          this.fanDeviceSwitch.close();
        })
        Button('二档').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenFan', 'fan', 2);
          this.fanDeviceSwitch.close();
        })
        Button('关闭').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenFan', 'fan', 0);
          this.fanDeviceSwitch.close();
        })
      }
    }.padding(20)
  }
}

//自定义弹窗，灯泡
@CustomDialog
struct switchOpenBulb {
  ledDeviceSwitch: CustomDialogController = new CustomDialogController({ builder: switchOpenBulb() })
  senDeviceId: (service_id: string, command_name: string, value: string, commandValue: number) => void;
  build() {
    Column({ space: 20 }) {
      Text('选择功能')
      Row({ space: 15 }) {
        Button('打开').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenLED', 'led', 1);
          console.log("灯泡打开成功");
          this.ledDeviceSwitch.close();
        })
        Button('关闭').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenLED', 'led', 0);
          console.log("灯泡关闭成功");
          this.ledDeviceSwitch.close();
        })
      }
    }.padding(20)
  }
}

//自定义弹窗，水泵
@CustomDialog
struct switchOpenPum {
  pumDeviceSwitch: CustomDialogController = new CustomDialogController({ builder: switchOpenPum() })
  senDeviceId: (service_id: string, command_name: string, value: string, commandValue: number) => void;
  build() {
    Column({ space: 20 }) {
      Text('选择功能')
      Row({ space: 15 }) {
        Button('打开').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenPum', 'pum', 1);
          console.log("水泵打开成功");
          this.pumDeviceSwitch.close();
        })
        Button('关闭').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenPum', 'pum', 0);
          console.log("水泵关闭成功");
          this.pumDeviceSwitch.close();
        })
      }
    }.padding(20)
  }
}

//自定义弹窗，天窗
@CustomDialog
struct switchOpenAir {
  airDeviceSwitch: CustomDialogController = new CustomDialogController({ builder: switchOpenAir() })
  senDeviceId: (service_id: string, command_name: string, value: string, commandValue: number) => void;
  build() {
    Column({ space: 20 }) {
      Text('选择功能')
      Row({ space: 15 }) {
        Button('打开').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenAir', 'air', 1);
          console.log("天窗打开成功");
          this.airDeviceSwitch.close();
        })
        Button('关闭').onClick(() => {
          this.senDeviceId('Dev_data', 'OpenAir', 'air', 0);
          console.log("天窗关闭成功");
          this.airDeviceSwitch.close();
        })
      }
    }.padding(20)
  }
}