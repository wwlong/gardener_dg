## 控制板 arduino uno r3
### 简介
### version 0.0.1
*   arduino r3作为浇水系统的主控板,闭环系统
*   模块 
    *   主控--arduino uno r3
    *   土壤湿度传感器 (https://detail.tmall.com/item.htm?spm=a1z10.3-b-s.w4011-16538328900.32.6e1c3d6cOa5OaF&id=41272850226&rn=54be1b93cbb1bfe22b0fc6c5807b5592&abbucket=0)3.75元
    *   继电器(https://detail.tmall.com/item.htm?spm=a1z10.3-b-s.w4011-16538328900.64.6e1c3d6cW7rP3x&id=41231430731&rn=d9e76540889a17911c014594fe05d3b2&abbucket=0)(4元)
    *   水泵(https://detail.tmall.com/item.htm?spm=a220m.1000858.1000725.16.1ccf294ek4K2XA&id=553786802655&skuId=3687292867464&areaId=310100&user_id=673685235&cat_id=2&is_b=1&rn=486fd1f1a0996f94c01fa6e1873e1b58)(18元)
    *   7v电池 -- arduino r3电源
    *   水泵电源
    *   杜邦线若干 
*   开发环境
    *   系统 : Ubuntu1604
    *   IDE : Arduino IDE
*   系统搭建和使用
    1.  Arduino UNO R3作为主控
    >   GPIO分配
    >   A0 -- LED提示灯,土壤干燥需要浇水时点亮
    >   A1 -- 土壤湿度传感器数据采集
    >   A2 -- 控制继电器的开合,从而控制水泵的工作状态
    2.  工作
    >   正常Arduino UNO R3处于Pown Down状态,通过WTD唤醒
    >   WTD唤醒UNO后,UNO通过A1检测土壤的干湿程序,如果超过设置的干燥范围(模拟电压值>900,其中模拟电压值0~1023,0表示最湿,1023表示最干),则拉高A2,让继电器吸合,水泵浇水
    >   如果A1返回的值<900,表示土壤湿度合适,UNO继续进入睡眠,如此循环

        
### version 0.0.2
*   新增GH-xUART 2.4G无线传输模块
    *   通过GH-xUART无线传输模块代替杜邦线,通过raspberry远程控制Arduino来控制水泵,LED等的开关等
*   Arduino上新增ArduinoJSON
    *   通过JSON格式传输request和response
*   交互逻辑
    *   Arduino -- client
    *   raspberry -- server
    *   client上报数据:土壤湿度 {"SoilHumidity":"int"}
    *   server下发命令:
        *   打开LED灯 {"LightNum":"ON"}
        *   关闭LED灯 {"LightNum":"OFF"}
        *   打开或者关闭水泵 {"Pump":"ON/OFF"}