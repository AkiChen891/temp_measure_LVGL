/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "lvgl.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "heu_logo.h"
#include "touch.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
lv_obj_t *title;
lv_obj_t *temp_label;   
lv_obj_t *scroll_test_label;
lv_timer_t *temp_timer;
lv_obj_t *temp_chart;
lv_obj_t *sampling_time;
lv_obj_t *screen1;
lv_obj_t *screen2;
lv_obj_t *btn_scr1_next_page;
lv_obj_t *btn_scr2_prev_page;
float temp = 0;   /* 检测到的温度值 */

#define TEMP_SAMPLING_TIME 500
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void create_screen_1(void);
void create_screen_2(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/** 
    @brief LVGL定时器更新回调
    @param timer:定时器事件
    @note 用于循环采集温度值并更新label。注意：LVGL默认不支持显示float和double类型，要显示则必须在lv_conf.h中打开LV_SPRINTF_CUSTOM
    @return NULL
*/
void temp_timer_update_callback(lv_timer_t *timer)
{
  static lv_coord_t last_point = 0;
  static lv_chart_series_t *ser1;
  temp = (float)read_temperature();   /* 获取温度 */
  lv_label_set_recolor(temp_label, true);

  if (temp < 40)    /* 温度小于40度为绿色 */
  {
    lv_label_set_text_fmt(temp_label, "Temperature is: #00FF00 %.2f# C", temp);    /* 更新label，温度数值为绿色 */
  }
  else              /* 温度大于40度为红色 */
  {
    lv_label_set_text_fmt(temp_label, "Temperature is: #FF0000 %.2f# C", temp);    /* 更新label，温度数值为红色 */
  }
  
  /* 为chart创建新序列，Y轴变量为temp，颜色为灰色 */
  if (ser1 == NULL)
  {
    ser1 = lv_chart_add_series(temp_chart, lv_palette_main(LV_PALETTE_GREY), LV_CHART_AXIS_PRIMARY_Y);
  }
  lv_chart_set_next_value(temp_chart, ser1, temp);    /* 将temp更新为序列的下一个值 */
  lv_chart_refresh(temp_chart);                       /* 更新chart */
}

/** 
    @brief LVGL button 更新回调
    @param e:指定输入的event
    @note 此处切换屏幕不能直接lv_obj_delete，必须通过obj_flag操作，否则切屏后卡死，原因未知
    @return NULL
*/
static void btn_switch_scr_cb(lv_event_t *e)
{
  lv_obj_t *target = lv_event_get_target(e);

  if (target == btn_scr1_next_page)   /* 如果事件触发源来自btn_scr1_next_page */
  {
    lv_obj_add_flag(screen1, LV_OBJ_FLAG_HIDDEN);   /* 隐藏screen1 */
    lv_obj_clear_flag(screen2, LV_OBJ_FLAG_HIDDEN); /* 显示screen2 */
    lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0, false);  /* 动态加载screen2 */
  }
  else if (target == btn_scr2_prev_page)    /* 如果事件触发源来自btn_scr2_prev_page */
  {
    lv_obj_add_flag(screen2, LV_OBJ_FLAG_HIDDEN);     /* 隐藏screen2 */
    lv_obj_clear_flag(screen1, LV_OBJ_FLAG_HIDDEN);   /* 显示screen1 */
    lv_scr_load_anim(screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0, false);  /* 动态加载screen1 */
  }
}

/** 
    @brief LVGL 创建屏幕1作为父类
    @param 无
    @return NULL
*/
void create_screen_1(void)
{
  /* 创建屏幕1 */
  screen1 = lv_obj_create(NULL);
  lv_obj_set_size(screen1, 1280, 800);    /* 界面大小等于屏幕大小 */

  /* 屏幕1控件建立 */
  /* 创建标题label */
  title = lv_label_create(screen1);
  lv_label_set_text_fmt(title, "MVDC IPS Fault detection system");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, lv_pct(3));
  lv_obj_set_style_text_font(title, &lv_font_montserrat_40, NULL);

  /* 创建未收到数据前的温度label */
  temp_label = lv_label_create(screen1);
  lv_label_set_text_fmt(temp_label, "Temperature is: -- C");
  lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, lv_pct(10), lv_pct(15));
  lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_32, NULL);

  /* 创建采样时间提示label */
  sampling_time = lv_label_create(screen1);
  lv_label_set_text_fmt(sampling_time, "Current sampling interval: %d ms", TEMP_SAMPLING_TIME);
  lv_obj_align(sampling_time, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_text_font(sampling_time, &lv_font_montserrat_32, NULL);

  /* 滚动条，会占用约15%的CPU资源，尽量不要启用 */
  // scroll_test_label = lv_label_create(lv_scr_act());
  // lv_label_set_long_mode(scroll_test_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
  // lv_obj_set_width(scroll_test_label, 300);
  // lv_label_set_text_fmt(scroll_test_label, "This is a scrolling label test...If you see this message, testing is in progress");
  // lv_obj_align(scroll_test_label, LV_ALIGN_BOTTOM_MID, 0, 0);
  // lv_obj_set_style_text_font(scroll_test_label, &lv_font_montserrat_32, NULL);

  /* 创建chart */
  temp_chart = lv_chart_create(screen1);
  lv_obj_set_size(temp_chart, 900, 300);    /* chart大小 */
  lv_obj_center(temp_chart);
  lv_chart_set_type(temp_chart, LV_CHART_TYPE_LINE);    /* 折线图 */
  lv_chart_set_range(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 25, 45);    /* Y轴刻度范围: 25~45 */
  lv_chart_set_axis_tick(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 5, 4, 10, LV_PALETTE_GREY, true, 70); /* Y轴刻度条设置 */
  lv_chart_set_point_count(temp_chart, 100);    /* chart内最多显示100个采样点 */

  /* 创建“下一页”button */
  btn_scr1_next_page = lv_obj_create(screen1);
  lv_obj_set_size(btn_scr1_next_page, 100, 50);
  lv_obj_align(btn_scr1_next_page, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_t *btn_scr1_next_page_label = lv_label_create(btn_scr1_next_page);          /* button上的label，作为button的子类 */
  lv_label_set_text(btn_scr1_next_page_label, "Next");
  lv_obj_set_style_text_font(btn_scr1_next_page_label, &lv_font_montserrat_22, 0);
  lv_obj_clear_flag(btn_scr1_next_page, LV_OBJ_FLAG_SCROLLABLE);        /* 禁用滚动条 */
  lv_obj_center(btn_scr1_next_page_label);
  lv_obj_set_style_bg_color(btn_scr1_next_page, lv_palette_main(LV_PALETTE_GREY), NULL);    /* button填充为灰色 */
  lv_obj_add_event_cb(btn_scr1_next_page, btn_switch_scr_cb, LV_EVENT_CLICKED, NULL);       /* 添加点击事件后的callback */
}

void create_screen_2(void)
{
  /* 创建屏幕2 */
  screen2 = lv_obj_create(NULL);
  lv_obj_set_size(screen2, 1280, 800);

  /* 创建图像，来源为heu_logo.c */
  lv_obj_t *bg_img = lv_img_create(screen2);
  lv_img_set_src(bg_img, &heu_logo);
  lv_obj_align(bg_img, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_opa(bg_img, LV_OPA_50, 0);   /* 不透明度为50% */

  /* 创建"上一页" button */
  btn_scr2_prev_page = lv_obj_create(screen2);
  lv_obj_set_size(btn_scr2_prev_page, 100, 50);
  lv_obj_align(btn_scr2_prev_page, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_t *btn_scr2_prev_page_label = lv_label_create(btn_scr2_prev_page);
  lv_label_set_text(btn_scr2_prev_page_label, "Prev");
  lv_obj_set_style_text_font(btn_scr2_prev_page_label, &lv_font_montserrat_22, 0);
  lv_obj_clear_flag(btn_scr2_prev_page, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_center(btn_scr2_prev_page_label);
  lv_obj_set_style_bg_color(btn_scr2_prev_page, lv_palette_main(LV_PALETTE_GREY), NULL);
  lv_obj_add_event_cb(btn_scr2_prev_page, btn_switch_scr_cb, LV_EVENT_CLICKED, NULL);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  delay_init(180);
  lv_init();    /* 初始化LVGL图形库 */
  lv_port_disp_init();    /* 注册显示设备，必须在lv_init之后调用 */
  lv_port_indev_init();   /* 注册输入设备 */
  led_init();
  key_init();
  sdram_init();
  lcd_init();
  tp_dev.init();          /* 触摸屏初始化 注意：IAR没有microlib，必须注释掉ft5206和gt9xxx中所有的printf函数，否则MCU上电自主运行时会卡死在printf处 */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  create_screen_1();
  create_screen_2();
  lv_scr_load(screen1);    /* 首次开机加载屏幕1 */

  /* 创建LVGL定时器事件 */
  /* 注意：创建前确认TIM6已经启用并且HAL_TIM_Base_Start_IT()已经调用！*/
  temp_timer = lv_timer_create(temp_timer_update_callback, TEMP_SAMPLING_TIME, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    lv_timer_handler();
    delay_ms(5);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
