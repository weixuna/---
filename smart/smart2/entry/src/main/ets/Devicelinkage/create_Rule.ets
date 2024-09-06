import greenHousesHttpRequest from '../Greenhouses/greenHousesHttp';

interface DeviceRuleResponse {
  rule_id: string;
  // 其他可能的字段...
}

class Task {
  static id: number = 1;
  finish: boolean = true; // 默认激活状态
  ruleName: string;
  ruleId: string; // 添加 ruleId 属性

  constructor(public taskId: number = Task.id++, ruleName: string = '', ruleId: string = '') {
    this.ruleName = ruleName;
    this.ruleId = ruleId;
  }
}

@Preview
@Component
export struct deviceRules {

  @State ruleNumArry: Task[] = []; // 存放 ruleUI

  sendDeviceRule(rulesName: string, deviceProperties: string, dataComparison: string, NumberValue: number, value: string, commandValue: string, commandName: string) {
    greenHousesHttpRequest.deviceLinkage(rulesName, deviceProperties, dataComparison, NumberValue, value, commandValue, commandName)
      .then((response: DeviceRuleResponse) => {
        console.log('创建规则成功', JSON.stringify(response));
        const ruleId = response.rule_id; // 现在 TypeScript 知道 response 有 rule_id 属性
        let newTask = new Task(undefined, rulesName, ruleId); // 创建新 Task 实例，并将规则名和 ruleId 传递给它
        this.ruleNumArry = [...this.ruleNumArry, newTask]; // 将新任务添加到数组中
      })
      .catch(error => {
        console.error('创建规则失败', error);
      });
  }
  activationStatus(ruleId: string, status: string) {
    greenHousesHttpRequest.activationStatus(ruleId, status)
      .then(response => {
        console.log('激活成功', JSON.stringify(response));
        // 更新本地任务的状态
        const taskToUpdate = this.ruleNumArry.find(task => task.ruleId === ruleId);
        if (taskToUpdate) {
          taskToUpdate.finish = (status === 'active');
          this.ruleNumArry = [...this.ruleNumArry]; // 触发视图更新
        }
      })
      .catch(error => {
        console.error('激活失败', error);
      });
  }

  deleteRules(ruleId: string) {
    greenHousesHttpRequest.deleteDeviceRule(ruleId)
      .then(response => {
        console.log('删除规则成功', JSON.stringify(response));
        // 更新任务列表，删除对应 ruleId 的任务
        this.ruleNumArry = this.ruleNumArry.filter(task => task.ruleId !== ruleId);
      })
      .catch(error => {
        console.error('删除规则失败', error);
      });
  }

  deviceLinkage: CustomDialogController = new CustomDialogController({
    builder: devicelink({
      sendDeviceRule: this.sendDeviceRule.bind(this),
      confirm: (value) => {
        // let newTask = new Task(undefined, value); // 创建新 Task 实例，并将规则名传递给它
        // this.ruleNumArry = [...this.ruleNumArry, newTask]; // 将新任务添加到数组中
      }
    }),
    alignment: DialogAlignment.Center,
    offset: { dx: 0, dy: 0 }
  });

  build() {
    Column() {
      Column() {
        Row() {
          Column() {
            Text('设备联动').fontSize(40)
          }.width('50%')
          Column() {
            Shape() {
              Circle({ width: 70, height: 70 }).fill("#FFFFFF")
              Image($r('app.media.jiahao')).onClick(() => {
                this.deviceLinkage.open();
              })
                .width(20)
                .height(20)
                .margin({ left: 25, top: 25 })
            }
          }.width('70%')
        }.justifyContent(FlexAlign.SpaceBetween).width('100%')

        ForEach(this.ruleNumArry, (item: Task, index: number) => {
          this.ruleUI(item);
        })
      }.width('100%')
    }.width('100%').height('100%').backgroundColor('#FDFDFD').alignItems(HorizontalAlign.Center)
  }

  @Builder
  ruleUI(item: Task) {
    Column() {
      Row({ space: 32 }) {
        Column() {
          Text('规则名称').fontSize(20).margin({ right: 30 })
        }
        Column() {
          Text('运行状态').fontSize(20)
        }
      }.justifyContent(FlexAlign.SpaceEvenly).width('100%').margin({ top: 5 })
      Row({ space: 32 }) {
        Column() {
          Text(item.ruleName).fontSize(20) // 显示每个任务的独立名字
        }
        Column() {
          Text('删除').fontSize(20).margin({ left: 60 }).onClick(() => {
            // 实现删除功能
            this.deleteRules(item.ruleId); // 删除对应 ruleId 的规则
          })
        }
        Column() {
          //在这里添加规则激活状态，active:激活。inactive:未激活，点击的传递给status
          Button(item.finish ? '已激活' : '未激活')
            .onClick(() => {
              // 切换激活状态
              const newStatus = item.finish ? 'inactive' : 'active';
              this.activationStatus(item.ruleId, newStatus); // 传递 ruleId 和新的状态
            })
            .width(100)
            .margin({ left: 30 })
        }
      }.justifyContent(FlexAlign.SpaceEvenly).width('100%').margin({ top: 10 })
    }.width('100%').backgroundColor('#CCCCCC')
  }
}
@CustomDialog
struct devicelink {
  dialogController: CustomDialogController;
  confirm: (value: string) => void;
  value: string = '';
  sendDeviceRule: (rulesName: string, deviceProperties: string, dataComparison: string, NumberValue: number, value: string, commandValue: string, commandName: string) => void;

  @State commandOptions: Array<{ value: string }> = [];
  @State selectedDeviceProperty: string = '';
  @State selectedDataComparison: string = '';
  @State numberValue: string = '';
  @State selectedCommand: string = '';
  @State commandValue: string = '';
  @State commandName: string = '';

  build() {
    Column() {
      Column() {
        Row() {
          Text('创建设备联动').fontSize(25)
        }
        Row() {
          Text('规则名称：')
          TextInput({ placeholder: '请输入规则名称' }).width(170).type(InputType.Normal).onChange((value) => {
            this.value = value;
          })
        }.margin({ top: 10 })
        Row() {
          Text('设备属性：')
          Select([{ value: '温度' }, { value: '湿度' }, { value: '光照强度' }, { value: '二氧化碳' }, { value: 'TVOC' }, { value: '土壤湿度' }, { value: '烟雾' }])
            .onSelect((index: number, value?: string) => {
              if (value) {
                switch (value) {
                  case '温度':
                    value = 'temp'
                    this.selectedDeviceProperty = value;
                    break;
                  case '湿度':
                    value = 'humi'
                    this.selectedDeviceProperty = value;
                    break;
                  case '光照强度':
                    value = 'light'
                    this.selectedDeviceProperty = value;
                    break;
                  case '二氧化碳':
                    value = 'CO2'
                    this.selectedDeviceProperty = value;
                    break;
                  case 'TVOC':
                    value = 'TVOC'
                    this.selectedDeviceProperty = value;
                    break;
                  case '土壤湿度':
                    value = 'Soil'
                    this.selectedDeviceProperty = value;
                    break;
                  case '烟雾':
                    value = '烟雾'
                    this.selectedDeviceProperty = value;
                    break;
                }
              }
            }).selected(0)
            .value('请选择设备属性')
            .font({ size: 16, weight: 500 })
            .fontColor('#182431')
            .selectedOptionFont({ size: 16, weight: 400 })
            .optionFont({ size: 16, weight: 400 })
            .width(170)
        }.margin({ top: 10 })
        Row() {
          Text('数据条件：')
          TextInput({ placeholder: '>,<,>=,<=,=,in' }).width(170).type(InputType.Normal).onChange((value) => {
            this.selectedDataComparison = value;
          })
        }.margin({ top: 10 })
        Row() {
          Text('数据范围：')
          TextInput({ placeholder: '请数据范围数值' }).width(170).type(InputType.Normal).onChange((value) => {
            this.numberValue = value;
          })
        }.margin({ top: 10 })
        Row() {
          Text('命令名称：')
          Select([{ value: '灯泡' }, { value: '风扇' }, { value: '天窗' }, { value: '水泵' }])
            .onSelect((index: number, value?: string) => {
              if (value) {
                this.commandName = value;
                switch (value) {
                  case '灯泡':
                    this.commandOptions = [{ value: '打开' }, { value: '关闭' }];
                    break;
                  case '风扇':
                    this.commandOptions = [
                      { value: '风扇一档' },
                      { value: '风扇二档' },
                      { value: '关闭风扇' }
                    ];
                    break;
                  case '天窗':
                    this.commandOptions = [{ value: '打开' }, { value: '关闭' }];
                    break;
                  case '水泵':
                    this.commandOptions = [{ value: '打开' }, { value: '关闭' }];
                    break;
                }
              }
            }).selected(0)
            .value('请选择命令')
            .font({ size: 16, weight: 500 })
            .fontColor('#182431')
            .selectedOptionFont({ size: 16, weight: 400 })
            .optionFont({ size: 16, weight: 400 })
            .width(170)
        }.margin({ top: 10 })
        Row() {
          Text('下发命令：')
          Select(this.commandOptions)
            .onSelect((index: number, value?: string) => {
              if (value) {
                if (this.commandName === '灯泡') {
                  this.commandName = 'OpenLED';
                  this.selectedCommand = 'led';
                  this.commandValue = (value === '打开') ? '1' : '0';
                } else if (this.commandName === '风扇') {
                  this.commandName = 'OpenFan';
                  this.selectedCommand = 'fan';
                  if (value === '风扇一档') {
                    this.commandValue = '1';
                  } else if (value === '风扇二档') {
                    this.commandValue = '2';
                  } else if (value === '关闭风扇') {
                    this.commandValue = '0';
                  }
                } else if (this.commandName === '天窗') {
                  this.commandName = 'OpenAir';
                  this.selectedCommand = 'air';
                  this.commandValue = (value === '打开') ? '1' : '0';
                } else if (this.commandName === '水泵') {
                  this.commandName = 'OpenPum';
                  this.selectedCommand = 'pum';
                  this.commandValue = (value === '打开') ? '1' : '0';
                }
              }
            })
            .selected(0)
            .value('请选择命令')
            .font({ size: 16, weight: 500 })
            .fontColor('#182431')
            .selectedOptionFont({ size: 16, weight: 400 })
            .optionFont({ size: 16, weight: 400 })
            .width(170);
        }.margin({ top: 10 })
      }
      Row({ space: 20 }) {
        Button('确认').onClick(() => {
          this.sendDeviceRule(
            this.value,
            this.selectedDeviceProperty,
            this.selectedDataComparison,
            parseFloat(this.numberValue),
            this.selectedCommand,
            this.commandValue,
            this.commandName
          );
          this.confirm(this.value);
          this.dialogController.close(); // 关闭对话框
        }).margin({ right: 60, top: 20 })

        Button('取消').onClick(() => {
          this.dialogController.close(); // 取消时也关闭对话框
        }).margin({ top: 20 })
      }
    }.height(380)
  }
}
