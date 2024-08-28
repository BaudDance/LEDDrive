## 使用方法

| 本库适配DRV8833等系列芯片


### 0. 工程配置
示例工程默认使用的是STM32F103C8T6的TIM2的通道1与通道2.
如有其他定时器需求, 仅需修改`drv8833.c`中的对应宏定义即可完成移植.
MAX_SPEED为占空比为100时对应的比较寄存器值（既重装载寄存器值），默认为100，若定时器重装载寄存器值为其他值 请修改MAX_SPEED

### 1. 引入
#### CubeIDE
将本库的`drv8833.c`文件放入到Src文件夹, 将`drv8833.h`文件放入Inc文件夹.

### 2. 使用
1. STM32初始化定时器完成后调用`DRV8833_Init()`初始化相应的PWM通道.
2. 调用`DRV8833_SetDecayMode()`可切换快/慢衰减模式。（不设置默认为慢衰减模式）
3. 调用`DRV8833_Forward()`、`DRV8833_Backward()`函数可分别实现电机正转与倒转（速度值从0 ~ MAX_SPEED)
4. 调用`DRV8833_Brake()`与`DRV8833_Coast()`，可实现电机滑行

```c
DRV8833_Init();
int speed = 0;
while(1){
    speed = (speed + 1) % 100;
    DRV8833_Forward(speed);
    HAL_Delay(100);
}
```