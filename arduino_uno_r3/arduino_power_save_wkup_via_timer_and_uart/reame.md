# arduino wkup by timer and uart
## 简介
*   此处Arduino作为采集土壤湿度的数据和控制水泵的主控,其主要工作有
    *   采集土壤湿度的数据
    *   控制水泵的继电器的开合
*   Arduino的状态如下:
    *   一般情况下处于SLEEP_MODE_PWR_DOWN
    *   唤醒方式有二,正常通过WTD中断,唤醒Arduino UNO,唤醒后采集土壤湿度信息,将数据通过GH-xUART发送到Server
    *   另一种唤醒的方式是通过UART产生中断唤醒,Server通过GH-xUART远程唤醒设备

