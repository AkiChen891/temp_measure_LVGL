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
lv_obj_t *dc_bus_voltage_label;
float temp = 0;   /* MCU internal temperature */
float u_dc = 0;   /* DC bus voltage*/
#define TEMP_SAMPLING_TIME 500    /* LVGL timer trigger interval, in ms */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/** 
    @brief LVGL timer trigger callback
    @param timer:LVGL timer event
    @note LVGL does not support the display of float and double types by default. To display them, must enable LV_SPRINTF_CUSTOM in lv_conf.h
    @return NULL
*/
void temp_timer_update_callback(lv_timer_t *timer)
{
  static lv_coord_t last_point = 0;
  static lv_chart_series_t *ser1;   /* new chart series */
  temp = read_values(10)[2];   /* read averaged temperature from ADC */
  u_dc = read_values(10)[0];   /* read averaged dc bus voltage from ADC */
  lv_label_set_recolor(temp_label, true);   /* enable temp_label recolor */
  lv_label_set_recolor(dc_bus_voltage_label, true);   /* enable dc bus voltage label recolor */

  if (temp < 40)    /* label set to GREEN if temp < 40 deg */
  {
    lv_label_set_text_fmt(temp_label, "Temperature is: #00FF00 %.2f# C", temp);    /* update label */
  }
  else              /* label set to RED if temp > 40 deg */
  {
    lv_label_set_text_fmt(temp_label, "Temperature is: #FF0000 %.2f# C", temp);    /* update label */
  }

  lv_label_set_text_fmt(dc_bus_voltage_label, "DC bus voltage is: %.2f V", u_dc);
  
  // /* create new series for chart1 as Y axis, color GREY */
  // if (ser1 == NULL)
  // {
  //   ser1 = lv_chart_add_series(temp_chart, lv_palette_main(LV_PALETTE_GREY), LV_CHART_AXIS_PRIMARY_Y);
  // }
  // lv_chart_set_next_value(temp_chart, ser1, temp);    /* Update temp to the next value in ser1 */
  // lv_chart_refresh(temp_chart);                       /* update chart */
}

/** 
    @brief LVGL button event callback
    @param e: event
    @note DO NOT use other methods to switch the page except add/clear flag, otherwise program may freeze
    @return NULL
*/
static void btn_switch_scr_cb(lv_event_t *e)
{
  lv_obj_t *target = lv_event_get_target(e);

  if (target == btn_scr1_next_page)   /* if event is triggered by btn_scr1_next_page */
  {
    lv_obj_add_flag(screen1, LV_OBJ_FLAG_HIDDEN);   /* hid screen1 */
    lv_obj_clear_flag(screen2, LV_OBJ_FLAG_HIDDEN); /* display screen2 */
    lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0, false);  /* dynamic load screen2 */
  }
  else if (target == btn_scr2_prev_page)    /* if event is triggered by btn_scr2_prev_page */
  {
    lv_obj_add_flag(screen2, LV_OBJ_FLAG_HIDDEN);     /* hid screen2 */
    lv_obj_clear_flag(screen1, LV_OBJ_FLAG_HIDDEN);   /* display screen1 */
    lv_scr_load_anim(screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0, false);  /* dynamic load screen1 */
  }
}

/** 
    @brief LVGL create screen 1 as parent
    @param NULL
    @return NULL
*/
void create_screen_1(void)
{
  /* create screen 1 */
  screen1 = lv_obj_create(NULL);
  lv_obj_set_size(screen1, 1280, 800);    /* object size = screen size */

  /* screen 1 widgets */
  /* create title label */
  title = lv_label_create(screen1);
  lv_label_set_text_fmt(title, "MVDC IPS Fault detection system");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, lv_pct(3));
  lv_obj_set_style_text_font(title, &lv_font_montserrat_40, NULL);

  /* create temperature label */
  temp_label = lv_label_create(screen1);
  lv_label_set_text_fmt(temp_label, "Temperature is: -- C");
  lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, lv_pct(10), lv_pct(15));
  lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_32, NULL);

  /* create dc bus voltage label */
  dc_bus_voltage_label = lv_label_create(screen1);
  lv_label_set_text_fmt(dc_bus_voltage_label, "DC bus voltage is: -- V");
  lv_obj_align(dc_bus_voltage_label, LV_ALIGN_TOP_LEFT, lv_pct(10), lv_pct(25));
  lv_obj_set_style_text_font(dc_bus_voltage_label, &lv_font_montserrat_32, NULL);

  /* create sampling time label */
  sampling_time = lv_label_create(screen1);
  lv_label_set_text_fmt(sampling_time, "Current sampling interval: %d ms", TEMP_SAMPLING_TIME);
  lv_obj_align(sampling_time, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_text_font(sampling_time, &lv_font_montserrat_32, NULL);

  /* scroll bar, may occupt 15% CPU resource, DO NOT enable unless necessary */
  // scroll_test_label = lv_label_create(lv_scr_act());
  // lv_label_set_long_mode(scroll_test_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
  // lv_obj_set_width(scroll_test_label, 300);
  // lv_label_set_text_fmt(scroll_test_label, "This is a scrolling label test...If you see this message, testing is in progress");
  // lv_obj_align(scroll_test_label, LV_ALIGN_BOTTOM_MID, 0, 0);
  // lv_obj_set_style_text_font(scroll_test_label, &lv_font_montserrat_32, NULL);

  /* create temperature chart */
  // temp_chart = lv_chart_create(screen1);
  // lv_obj_set_size(temp_chart, 900, 300);    /* chart size */
  // lv_obj_center(temp_chart);
  // lv_chart_set_type(temp_chart, LV_CHART_TYPE_LINE);    /* chart type */
  // lv_chart_set_range(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 25, 45);    /* Y axis range: 25 - 45 */
  // lv_chart_set_axis_tick(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 5, 4, 10, LV_PALETTE_GREY, true, 70); /* Y axis style setting */
  // lv_chart_set_point_count(temp_chart, 100);    /* max 100 samping points in chart */

  /* chreate "next page" button for screen1 */
  btn_scr1_next_page = lv_obj_create(screen1);
  lv_obj_set_size(btn_scr1_next_page, 100, 50);
  lv_obj_align(btn_scr1_next_page, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_t *btn_scr1_next_page_label = lv_label_create(btn_scr1_next_page);          /* label of this button, as child */
  lv_label_set_text(btn_scr1_next_page_label, "Next");
  lv_obj_set_style_text_font(btn_scr1_next_page_label, &lv_font_montserrat_22, 0);
  lv_obj_clear_flag(btn_scr1_next_page, LV_OBJ_FLAG_SCROLLABLE);        /* disable scroll bar */
  lv_obj_center(btn_scr1_next_page_label);
  lv_obj_set_style_bg_color(btn_scr1_next_page, lv_palette_main(LV_PALETTE_GREY), NULL);    /* filled button with GREY */
  lv_obj_add_event_cb(btn_scr1_next_page, btn_switch_scr_cb, LV_EVENT_CLICKED, NULL);       /* link button event callback after clicked */
}

/** 
    @brief LVGL create screen 2 as parent
    @param NULL
    @return NULL
*/
void create_screen_2(void)
{
  /* create screen 2 */
  screen2 = lv_obj_create(NULL);
  lv_obj_set_size(screen2, 1280, 800);

  /* create image, from heu_logo.c */
  lv_obj_t *bg_img = lv_img_create(screen2);
  lv_img_set_src(bg_img, &heu_logo);
  lv_obj_align(bg_img, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_opa(bg_img, LV_OPA_50, 0);   /* Opacity is 50% */

  /* create "prev page" button for screen2 */
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
  lv_init();    /* initialize LVGL */
  lv_port_disp_init();    /* reg disp equipment */
  lv_port_indev_init();   /* reg indev equipment */
  led_init();
  key_init();
  sdram_init();
  lcd_init();
  tp_dev.init();          /* initialize indev screen, NOTE：IAR does not have microlib, DO delete printf() in ft5206.c and gt9xxx.c otherwise MCU may freeze */
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
  lv_scr_load(screen1);    /* load screen1 when first startup */

  /* create LVGL timer */
  /* NOTE: confirm TIM6 is enabled and STARTED! */
  temp_timer = lv_timer_create(temp_timer_update_callback, TEMP_SAMPLING_TIME, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    lv_timer_handler();   /* DO NOT delete this! */
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
