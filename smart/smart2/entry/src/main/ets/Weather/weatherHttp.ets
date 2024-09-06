import http from '@ohos.net.http';
import weatherData from '../Data/weatherdata';

class WeatherGetter {
  getHttpData(): Promise<weatherData[]> {
    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      httpRequest.request(
        "https://api.vvhan.com/api/weather?city=%E9%95%BF%E6%B2%99%E5%B8%82&type=week",
        {
          method: http.RequestMethod.GET
        }
      )
        .then(resp => {
          if (resp.responseCode == 200) {
            console.info('Result:' + JSON.stringify(resp.result));
            let data = JSON.parse(resp.result.toString());
            if (data.success && Array.isArray(data.data)) {
              const weatherDataArray: weatherData[] = data.data.map((item: any) => {
                return {
                  city: data.city,
                  date: item.date,
                  week: item.week,
                  type: item.type,
                  low: item.low,
                  high: item.high
                };
              });
              resolve(weatherDataArray);
            } else {
            }
          } else {
          }
        })
        .catch(error => {
          console.error('错误:', JSON.stringify(error));
          reject('错误');
        })
    });
  }
}
const weatherGetter = new WeatherGetter();

export default weatherGetter as WeatherGetter;
