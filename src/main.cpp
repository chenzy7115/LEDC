#include <Arduino.h>

// 定义三个LED的引脚和对应的PWM通道
const int LED_PINS[3] = {2, 0, 4};
const int LEDC_CHANNELS[3] = {0, 1, 2};
const int PWM_FREQ = 1000;
const int PWM_RESOLUTION = 10;
const int MAX_DUTY = (1 << PWM_RESOLUTION) - 1;
const int BREATH_CYCLE = 1000;  // 单个呼吸周期时间（毫秒）
const int STEPS = 30;           // 每个周期的步数
const int DELAY_TIME = BREATH_CYCLE / (STEPS * 2); // 每步延时

void setup() {
  // 初始化所有LED通道
  for (int i = 0; i < 3; i++) {
    ledcSetup(LEDC_CHANNELS[i], PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LED_PINS[i], LEDC_CHANNELS[i]);
    // 初始状态全部熄灭
    ledcWrite(LEDC_CHANNELS[i], 0);
  }
}

// 控制单个LED完成一个完整的呼吸周期
void breatheLed(int ledIndex) {
  int duty;
  
  // 呼吸上升阶段（从暗到亮）
  for (int step = 0; step <= STEPS; step++) {
    duty = (step * MAX_DUTY) / STEPS;
    ledcWrite(LEDC_CHANNELS[ledIndex], duty);
    delay(DELAY_TIME);
  }
  
  // 呼吸下降阶段（从亮到暗）
  for (int step = STEPS; step >= 0; step--) {
    duty = (step * MAX_DUTY) / STEPS;
    ledcWrite(LEDC_CHANNELS[ledIndex], duty);
    delay(DELAY_TIME);
  }
  
  // 确保LED完全熄灭
  ledcWrite(LEDC_CHANNELS[ledIndex], 0);
}

void loop() {
  // 依次控制每个LED进行呼吸
  for (int i = 0; i < 3; i++) {
    breatheLed(i);
    delay(100); // LED切换间隔
  }
}    