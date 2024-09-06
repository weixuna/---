import http from '@ohos.net.http';
import GreenHousesData from '../Data/greenHousesData'

class greenHousesHttpRequest{

  private token: string | null;

  constructor() {
    this.token = null;
  }
  //获取token
  async getAuthToken() {
    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      httpRequest.request("https://iam.cn-north-4.myhuaweicloud.com/v3/auth/tokens", {
        method: http.RequestMethod.POST,
        extraData: {
          "auth": {
            "identity": {
              "methods": ["password"],
              "password": {
                "user": {
                  "domain": { "name": "limingjiangzhi" },
                  "name": "limingjiangzhi",
                  "password": "lmjzwlwjs0318"
                }
              }
            },
            "scope": {
              "project": { "name": "cn-north-1" }
            }
          }
        }
      }).then(resp => {
        if (resp.responseCode == 201) {
          let token = resp.header['x-subject-token'];
          console.log('Token received:', token);
          this.token = token;
          resolve(token);
        } else {
          reject('Failed to retrieve token, response code: ' + resp.responseCode);
        }
      }).catch(error => {
        console.error('Failed to fetch token:', JSON.stringify(error));
        reject(error);
      });
    });
  }
  // 获取设备影子
  async getGreenData(): Promise<GreenHousesData[]> {
    if (!this.token) {
      await this.getAuthToken();
    }
    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      httpRequest.request("https://0ae1ed433d.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/devices/667145986bc31504f06f60ee_wulianwang/shadow", {
        method: http.RequestMethod.GET,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token
        }
      }).then(resp => {
        if (resp.responseCode === 200) {
          const result = JSON.parse(resp.result.toString());
          console.log("打印出的数据", JSON.stringify(result, null, 2));
          const devDataService = result.shadow.find(service => service.service_id === 'Dev_data');
          if (devDataService && devDataService.reported && devDataService.reported.properties) {
            const properties = devDataService.reported.properties;

            const eventTime = devDataService.reported.event_time;
            console.log('提取到的 event_time:', eventTime); // 打印提取到的 event_time

            // 根据 event_time 直接构造日期时间字符串
            const formattedDateTime = `${eventTime.slice(0, 4)}-${eventTime.slice(4, 6)}-${eventTime.slice(6, 8)} ${eventTime.slice(9, 11)}:${eventTime.slice(11, 13)}:${eventTime.slice(13, 15)}`;
            console.log('转换后的日期时间:', formattedDateTime); // 打印转换后的日期时间字符串

            // 将格式化的日期时间字符串添加到 properties 对象中
            properties.date = formattedDateTime;

            const data = new GreenHousesData(properties); // 使用 properties 创建 GreenHousesData 实例
            console.log('传递给 GreenHousesData 的 properties:', JSON.stringify(properties, null, 2)); // 打印传递给 GreenHousesData 的 properties
            resolve([data]);
          } else {
            console.warn('未找到 Dev_data 服务或 reported properties');
            reject(new Error('没有获取到Devdata'));
          }
        } else {
          console.error('响应码不是 200:', resp.responseCode);
          reject(new Error('获取失败: ' + resp.responseCode));
        }
      }).catch(error => {
        console.error('获取温室数据失败:', error.message || error);
        reject(error);
      });
    });
  }

  //设备状态
  async getDeviceStatus() {
    if (!this.token) {
      await this.getAuthToken();
    }

    let httpRequest = http.createHttp();
    const url = `https://f31531a1fe.iotda.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/devices`;

    try {
      const response = await httpRequest.request(url, {
        method: http.RequestMethod.GET,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token
        }
      });

      if (response.responseCode == 200) {
        const result = JSON.parse(response.result.toString());
        return result.devices.map(device => device.status);
      } else {
        console.error('获取错误:', response.responseCode);
        return [];
      }
    } catch (err) {
      console.error('错误:', err);
      return [];
    }
  }
  // 设备下发命令

  async sendDeviceCommand(serviceId, commandName, value, commandValue) {
    if (!this.token) {
      await this.getAuthToken();
    }

    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      const url = "https://0ae1ed433d.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/devices/667145986bc31504f06f60ee_wulianwang/commands";

      const requestBody = {
        "service_id": serviceId,
        "command_name": commandName,
        "paras": {
          [value]: String(commandValue)
        }
      };

      console.log('POST请求体:', JSON.stringify(requestBody));

      httpRequest.request(url, {
        method: http.RequestMethod.POST,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token
        },
        extraData: requestBody
      }).then(resp => {
        if (resp.responseCode == 200) { // 处理200响应码
          resolve(JSON.parse(resp.result.toString()));
        } else {
          reject('获取错误: ' + resp.responseCode);
        }
      }).catch(error => {
        console.error('错误:', JSON.stringify(error));
        reject(error);
      });
    });
  }

  //设备联动
  async deviceLinkage(rulesName,deviceProperties,dataComparison,NumberValue,value,commandValue,commandName){
    if (!this.token) {
      await this.getAuthToken();
    }
    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      const url = "https://0ae1ed433d.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/rules";

      const requestBody = {
        "name": rulesName,  //规则名称
        "condition_group": {
          "conditions": [
            {
              "type": "DEVICE_DATA",   //规则条件；类型
              "device_property_condition": {
                "device_id": "667145986bc31504f06f60ee_wulianwang", //设备ID
                "filters": [
                  {
                    "path": `Dev_data/${deviceProperties}`,  //设备属性的路径信息
                    "operator": dataComparison,  //数据比较操作符
                    "value": NumberValue,    //数据范围
                    "in_values": [],
                    "strategy": {
                      "trigger": "pulse",
                      "event_valid_time": 300
                    }
                  }
                ]
              }
            }
          ],
          "logic": "and"
        },
        "actions": [
          {
            "type": "DEVICE_CMD",//规则动作的类型,端侧执行只支持下发设备命令消息类型。
            "addition": [],
            "device_command": {
              "device_id": "667145986bc31504f06f60ee_wulianwang", //设备Id
              "cmd": {
                "command_name": commandName, //设备命令名称
                "command_body": {
                  [value]: String(commandValue)    //下发命令
                },
                "service_id": "Dev_data", //服务ID
                "buffer_timeout": 172800,
                "response_timeout": 1800
              }
            }
          }
        ],
        "rule_type": "DEVICE_LINKAGE",
        "status": "active",
        "app_id": "d47eeb52098345898ba95b1be5cbd7c4",
        "edge_node_ids": [],
        "last_update_time": "20240717T083929Z"
      };
      console.log('POST请求体:', JSON.stringify(requestBody));

      httpRequest.request(url, {
        method: http.RequestMethod.POST,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token // 确保已经获取了token
        },
        extraData: requestBody
      }).then(resp => {
        if (resp.responseCode == 201) { // 处理200响应码
          resolve(JSON.parse(resp.result.toString()));
        } else {
          reject('获取错误: ' + resp.responseCode);
        }
      }).catch(error => {
        console.error('错误:', JSON.stringify(error));
        reject(error);
      });
    });
  }
  //删除规则
  async deleteDeviceRule(ruleId: string) {
    if (!this.token) {
      await this.getAuthToken();
    }

    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      const url = `https://0ae1ed433d.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/rules/${ruleId}`;

      console.log('DELETE请求的URL:', url);

      httpRequest.request(url, {
        method: http.RequestMethod.DELETE,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token // 确保已经获取了token
        }
      }).then(resp => {
        if (resp.responseCode == 204) { // 204表示成功删除
          resolve('删除成功');
        } else {
          reject('获取错误: ' + resp.responseCode);
        }
      }).catch(error => {
        console.error('错误:', JSON.stringify(error));
        reject(error);
      });
    });
  }

  //activationStatus
  async activationStatus(ruleId: string, status: string) {
    if (!this.token) {
      await this.getAuthToken();
    }

    return new Promise((resolve, reject) => {
      let httpRequest = http.createHttp();
      // 使用 ruleId 替换 URL 中的硬编码部分
      const url = `https://0ae1ed433d.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/34dd0bcb3a0b46fab400809bca1b6e3e/rules/${ruleId}/status`;

      console.log('PUT请求的URL:', url);

      httpRequest.request(url, {
        method: http.RequestMethod.PUT,
        header: {
          'Content-Type': 'application/json',
          'X-Auth-Token': this.token // 确保已经获取了token
        },
        extraData: {
          status: status
        }
      }).then(resp => {
        if (resp.responseCode === 200) { // 200表示成功
          resolve('状态更新成功');
        } else {
          reject('获取错误: ' + resp.responseCode);
        }
      }).catch(error => {
        console.error('错误:', JSON.stringify(error));
        reject(error);
      });
    });
  }





}
const GreenHousesHttpRequest = new greenHousesHttpRequest();
export default GreenHousesHttpRequest as greenHousesHttpRequest;