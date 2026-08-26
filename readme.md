# DC_motor_drive

> [!NOTE]
> - 本项目基于stm32f103cbt6开发，使用 `STM32CubeMX` 生成代码。

## 功能模块

### 应用层（APP）
- **系统管理 `app_sys`**：系统初始化，读取电机 ID（拨码开关 BIT0~BIT2），状态机管理与 LED 指示。
- **电机控制 `app_motor`**：核心控制任务，根据目标模式调度速度/位置/电流三环 PID；TIM2 中断（2kHz）中完成电流环实时控制。
- **命令解析 `app_command`**：解析 UART1 调试指令（`debug`/`step`/`work`/`disable`/`pidwrite` 等），支持在线修改 PID 参数。
- **通信协议 `app_message`**：`BB AA … CC` 帧的解析与封装，支持 RS485 与 CAN 双通道控制及电机状态回传。
- **堵转保护 `app_block`**：检测堵转（电流过大且转速接近 0），自动失能电机并延时恢复。
- **编码器 `app_encoder`**：定时读取编码器，更新电机速度与位置。

### 算法组件（Components）
- **PID 控制器 `PID`**：增量式 PID，提供电流/速度/位置三环控制。
- **电机驱动 `MotorCtrl`**：PWM 输出（TIM1）与编码器解码（TIM3），控制电机转向与转速。

### 底层驱动（BSP）
- **串口 `bsp_uart`**：UART 收发、`printf` 与回调注册。
- **RS485 `bsp_rs485`**：RS485 半双工收发。
- **CAN `bsp_can`**：CAN 收发与按 ID 回调过滤。
- **ADC `bsp_adc`**：电流采样（含 DMA 连续采样）。
- **Flash `bsp_flash`**：基于 EasyFlash 的参数持久化，存储 PID 配置。
- **LED `bsp_led`**：RGB LED 状态指示。

### 控制模式
| 模式 | 说明 |
| --- | --- |
| `disable_loop` | 失能，电机不输出 |
| `no_loop` | 开环 PWM 控制（-4499~4499） |
| `current_loop` | 电流环（-2048~2047） |
| `speed_loop` | 速度环（-60~60） |
| `position_loop` | 位置环（-32768~32767） |

## 版本更新
- v1.0.0 - 使用RS485或者CAN控制，使用UART1进行调试。
- v1.0.1 - 修复了堵转保护bug，可以使用堵转保护。
- v1.0.2 - 优化初始化时间。
- v1.1.0 - 优化了堆栈分配，修复了上位机无法更改pid的参数。
- v1.2.0 - 修改了说明书

## 快速开始
- 使用 `CLion` 打开它。
- 打开 `STM32CubeMX` 生成代码。
- 在 `Core/Src/main.c`添加头文件。
```c++
    #include "app_motor.h"
```
- 在 `Core/Src/main.c`中的定时中断函数`void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)`添加以下代码。
```c++
    if (htim->Instance == TIM2)
    {
        app_tim_current();
    }
```
- 检查无误后编译运行。
