import WeatherGetter from '../Weather/weatherHttp'
import weatherData from '../Data/weatherdata'

@Preview
@Component
export struct Weather {
  @State weatherData: weatherData | null = null;
  @State loading: boolean = true;

  @State tomorrowWeatherData: weatherData | null = null;
  @State dayAfterTomorrowWeatherData: weatherData | null = null;


  @State wenhou:string = '下午好'
  @State dateHour: string = '18:11';
  @State duoyun:string='多云'
  @State yin:string='阴'
  @State sunny:string='晴'
  @State leizhenyu:string='阵雨'
  @State zhongyu:string='中雨'
  @State xiaoyu:string='小雨'
  @State dayu:string='大雨'
  @State xiaodaozhongyu:string='中雨'

  getDate() {
    let date = new Date(); // 获取当前日期和时间
    let hours = date.getHours(); // 获取小时
    let minutes: string = date.getMinutes().toString();
    if (minutes.length < 2) {
      minutes = '0' + minutes;
    }
    let time = hours + ":" + minutes;
    this.dateHour = time;



    // 设置问候语
    let hour = date.getHours();
    if (hour >= 6 && hour < 11) {
      this.wenhou = '早上好！';
    } else if (hour >= 11 && hour < 13) {
      this.wenhou = '上午好！';
    } else if (hour >= 13 && hour < 18) {
      this.wenhou = '下午好！';
    } else if (hour >= 19 && hour < 23) {
      this.wenhou = '晚上好！';
    } else {
      this.wenhou = '凌晨好！';
    }
  }
  aboutToAppear() {
    this.loadWeatherInfo();
    setInterval(() => {
      this.getDate();
    }, 1000);
  }

  build() {
    Column() {
      Row() {
        Text(this.wenhou).fontSize(40)
      }
      Row() {
        Text('今日天气').fontSize(20);
      }.margin({ top: 20 });

      if (this.loading) {
        // 显示加载状态
        Text('加载中...').fontSize(24);
      } else if (this.weatherData) {
        // 显示天气信息
        Flex({direction: FlexDirection.Column}) {
              Text(this.weatherData.city).height(50).width(70).fontSize(20)
          Flex({wrap: FlexWrap.Wrap}){
            Row({ space: 30}){
              Column(){
                if(this.weatherData.type = this.duoyun){
                  Image($r('app.media.duoyun')).width(80).height(80)
                }else if(this.weatherData.type = this.sunny){
                  Image($r('app.media.qingtian')).width(80).height(80)
                }else if(this.weatherData.type = this.yin) {
                  Image($r('app.media.yintian')).width(80).height(80)
                }else if(this.weatherData.type = this.leizhenyu) {
                  Image($r('app.media.zhenyu')).width(80).height(80)
                }else if(this.weatherData.type = this.zhongyu) {
                  Image($r('app.media.zhongyu')).width(80).height(80)
                }else if(this.weatherData.type = this.xiaoyu) {
                  Image($r('app.media.xiaoyu')).width(80).height(80)
                }else if(this.weatherData.type = this.dayu) {
                  Image($r('app.media.dayu')).width(80).height(80)
                }else if(this.weatherData.type = this.xiaodaozhongyu) {
                  Image($r('app.media.zhongyu')).width(80).height(80)
                }
              }
              Column(){
                Row(){
                  Text(this.weatherData.type).fontSize(24)
                }
                Row(){
                  Text(this.weatherData.high).fontSize(20).margin({top:10})
                }
              }
              Column(){
                Row(){
                  Text(this.dateHour).fontSize(36)
                }
                Row(){
                  Text(this.weatherData.date).fontSize(24)
                }
              }
            }.justifyContent(FlexAlign.SpaceEvenly).width('100%')
            Row({space: 32}){
              Column(){
                Text('今天').fontSize(20).margin({right:30})
              }
              Column(){
                Text('明天').fontSize(20).margin({right:20})
              }
              Column(){
                Text('后天').fontSize(20)
              }
            }.justifyContent(FlexAlign.SpaceEvenly).width('100%').margin({top:5})
            Row({space: 30}){
              Column(){
                Text(this.weatherData.low+'/'+this.weatherData.high).fontSize(20)
              }
              Column(){
                Text(this.tomorrowWeatherData.low+'/'+this.tomorrowWeatherData.high).fontSize(20)
              }
              Column(){
                Text(this.dayAfterTomorrowWeatherData.low+'/'+this.dayAfterTomorrowWeatherData.high).fontSize(20)
              }
            }.justifyContent(FlexAlign.SpaceEvenly).width('100%').margin({top:10})
          }
        }.height(200).borderRadius(15).backgroundColor('0xEFF0F4').width('100%').shadow({ radius: 50, color: 0xEFF0F4, offsetX: 15, offsetY: 15 });
      } else {
        Text('无法获取天气信息').fontSize(24);
      }
    }.alignItems(HorizontalAlign.Start);
  }

  loadWeatherInfo() {
    WeatherGetter.getHttpData()
      .then(data => {
        if (data && data.length >= 3) {
          this.weatherData = data[0]; // 今天
          this.tomorrowWeatherData = data[1]; // 明天
          this.dayAfterTomorrowWeatherData = data[2]; // 后天
        } else {
          console.error('Failed to load weather data: Insufficient data');
        }
      })
      .catch(error => {
        console.error('Failed to load weather data:', error);
      })
      .finally(() => {
        this.loading = false;
      });
  }
}
