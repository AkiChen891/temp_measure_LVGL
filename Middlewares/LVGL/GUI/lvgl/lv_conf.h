/**
 * @file lv_conf.h
 * Configuration file for v8.2.0
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to the `lvgl` folder
 * 2. or any other places and
 *    - define `LV_CONF_INCLUDE_SIMPLE`
 *    - add the path as include path
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>
#include "stm32f429xx.h"
/*********************************************************************************

                                        颜色设置
                                        
 ***********************************************************************************/

/* color depth: 1(1 byte per pixel), 8(RGB332), 16(RGB565), 32(ARGB8888) */
#define LV_COLOR_DEPTH                      16

/* exchange 2 byte RGB565 color (eg SPI) */
#define LV_COLOR_16_SWAP                    0

#define LV_COLOR_SCREEN_TRANSP              0


#define LV_COLOR_MIX_ROUND_OFS              (LV_COLOR_DEPTH == 32 ? 0: 128)


#define LV_COLOR_CHROMA_KEY                 lv_color_hex(0x00ff00)         



/*********************************************************************************

                                        内存管制
                                        
 ***********************************************************************************/

#define LV_MEM_CUSTOM                       0
#if LV_MEM_CUSTOM == 0
    #define LV_MEM_SIZE                     (45U * 1024U)     /* 缓冲区大小，默认为45KB */    

    
    #define LV_MEM_ADR                      0     
    
    #if LV_MEM_ADR == 0
        //#define LV_MEM_POOL_INCLUDE your_alloc_library  /* 
        //#define LV_MEM_POOL_ALLOC   your_alloc          /* 
    #endif

#else       /*LV_MEM_CUSTOM*/
    #define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   
    #define LV_MEM_CUSTOM_ALLOC   malloc
    #define LV_MEM_CUSTOM_FREE    free
    #define LV_MEM_CUSTOM_REALLOC realloc
#endif     /*LV_MEM_CUSTOM*/


#define LV_MEM_BUF_MAX_NUM                  16


#define LV_MEMCPY_MEMSET_STD                0



/*********************************************************************************

                                        HAL 设置
                                        
 ***********************************************************************************/
 
/* 默认的显示刷新周期 */
#define LV_DISP_DEF_REFR_PERIOD             4      /*[ms]*/

/* 设备读取周期(ms) */
#define LV_INDEV_DEF_READ_PERIOD            4     /*[ms]*/

/* 自定义tick源 */
#define LV_TICK_CUSTOM                      0
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE          "FreeRTOS.h"                
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR    (xTaskGetTickCount())       
#endif   /*LV_TICK_CUSTOM*/

#define LV_DPI_DEF                          215    /*[px/inch]*/    /* DPI，影响动画效果，最佳值为(横向像素 * 纵向像素)^0.5 / 屏幕英寸 */



/*********************************************************************************

                                        特征设置
                                        
 ***********************************************************************************/
/*-------------
 * 1. 绘制
 *-----------*/


/* 启用复杂的绘制引擎
 * 需要绘制阴影、梯度、圆角、圆、弧线、斜线、图像转换或任何遮罩 */
#define LV_DRAW_COMPLEX                     1
#if LV_DRAW_COMPLEX != 0

    /* 允许缓冲一些阴影计算
     * LV_SHADOW_CACHE_SIZE为最大的缓冲大小，缓冲大小为阴影宽度 + 半径
     * 该操作会产生 LV_SHADOW_CACHE_SIZE^2 的内存开销 */
    #define LV_SHADOW_CACHE_SIZE            0

    /* 设置最大缓存循环数据的数量
     * 保存1/4圆的周长用于抗锯齿
     * 0:禁用缓存 */
    #define LV_CIRCLE_CACHE_SIZE            4
    
#endif /*LV_DRAW_COMPLEX*/

/* 默认图像缓存大小。图像缓存将保持图像打开
 * 0:禁用缓存 */
#define LV_IMG_CACHE_DEF_SIZE               0

#define LV_GRADIENT_MAX_STOPS               2

/* 默认梯度缓冲区大小
 * 0：无缓存*/
#define LV_GRAD_CACHE_DEF_SIZE              0

/* 允许抖动渐变，在有限的颜色深度显示上实现视觉平滑的颜色渐变 */
#define LV_DITHER_GRADIENT                  0
#if LV_DITHER_GRADIENT
    #define LV_DITHER_ERROR_DIFFUSION       0
#endif

/* 为旋转分配的最大缓冲区大小，仅在驱动程序中启用软件旋转时使用 */
#define LV_DISP_ROT_MAX_BUF                 (10*1024)

/*-------------
 * 2. GPU
 *-----------*/

/* 启用STM32的DMA2D(Chrom Art) GPU */
#define LV_USE_GPU_STM32_DMA2D              1
#if LV_USE_GPU_STM32_DMA2D
    /* 必须定义包括目标处理器的CMSIS头路径，如stm32f429xx.h */
    #define LV_GPU_DMA2D_CMSIS_INCLUDE      "stm32f429xx.h"
#endif


#define LV_USE_GPU_NXP_PXP                  0
#if LV_USE_GPU_NXP_PXP
    #define LV_USE_GPU_NXP_PXP_AUTO_INIT    0
#endif

#define LV_USE_GPU_NXP_VG_LITE              0

#define LV_USE_GPU_SDL                      0
#if LV_USE_GPU_SDL
    #define LV_GPU_SDL_INCLUDE_PATH <SDL2/SDL.h>
    
    #define LV_GPU_SDL_LRU_SIZE (1024 * 1024 * 8)
    
    #define LV_GPU_SDL_CUSTOM_BLEND_MODE (SDL_VERSION_ATLEAST(2, 0, 6))
#endif

/*-------------
 * 3. 日志
 *-----------*/

/* 启用日志 */
#define LV_USE_LOG                          0
#if LV_USE_LOG

    /*日志类型:
    *LV_LOG_LEVEL_TRACE       详细信息
    *LV_LOG_LEVEL_INFO        记录重要事件
    *LV_LOG_LEVEL_WARN        记录警告
    *LV_LOG_LEVEL_ERROR       记录错误
    *LV_LOG_LEVEL_USER        仅用户日志
    *LV_LOG_LEVEL_NONE        不记录任何内容*/
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

    /*1: 使用printf打印日志
     *0: 用户需要用 lv_log_register_print_cb() 注册回调函数 */
    #define LV_LOG_PRINTF                   0

    /* 在产生大量日志的模块中启用/禁用LV_LOG_TRACE */
    #define LV_LOG_TRACE_MEM                1
    #define LV_LOG_TRACE_TIMER              1
    #define LV_LOG_TRACE_INDEV              1
    #define LV_LOG_TRACE_DISP_REFR          1
    #define LV_LOG_TRACE_EVENT              1
    #define LV_LOG_TRACE_OBJ_CREATE         1
    #define LV_LOG_TRACE_LAYOUT             1
    #define LV_LOG_TRACE_ANIM               1

#endif  /*LV_USE_LOG*/

/*-------------
 * 4. assert
 *-----------*/

#define LV_USE_ASSERT_NULL                  1   
#define LV_USE_ASSERT_MALLOC                1   
#define LV_USE_ASSERT_STYLE                 0   
#define LV_USE_ASSERT_MEM_INTEGRITY         0  
#define LV_USE_ASSERT_OBJ                   0   

#define LV_ASSERT_HANDLER_INCLUDE           <stdint.h>
#define LV_ASSERT_HANDLER while(1);        

/*-------------
 * 5. 其他
 *-----------*/

/* 1:显示CPU使用率和FPS */
#define LV_USE_PERF_MONITOR                 1
#if LV_USE_PERF_MONITOR
    #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT       /* 位置在右下角 */
#endif

/* 1：显示使用的内存和内存碎片
 * 要求LV_MEM_CUSTOM = 0*/
#define LV_USE_MEM_MONITOR                  1
#if LV_USE_MEM_MONITOR
    #define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT         /* 位置在左下角 */
#endif

/* 1:在重新绘制的区域上绘制随机的彩色矩形 */
#define LV_USE_REFR_DEBUG                   0

/* 改变内置的(v)snprintf函数 */
#define LV_SPRINTF_CUSTOM                   1
#if LV_SPRINTF_CUSTOM
    #define LV_SPRINTF_INCLUDE  <stdio.h>
    #define lv_snprintf         snprintf
    #define lv_vsnprintf        vsnprintf
#else   /*LV_SPRINTF_CUSTOM*/
    #define LV_SPRINTF_USE_FLOAT            0
#endif  /*LV_SPRINTF_CUSTOM*/

#define LV_USE_USER_DATA                    1

/* 垃圾收集器设置
 * 如果LVGL绑定到高级语言，并且内存由该语言管理时启用 */
#define LV_ENABLE_GC                        0
#if LV_ENABLE_GC != 0
    #define LV_GC_INCLUDE "gc.h"                           
#endif /*LV_ENABLE_GC*/

 
 
/*********************************************************************************

                                        编译器设置
                                        
 ***********************************************************************************/

#define LV_BIG_ENDIAN_SYSTEM                0

#define LV_ATTRIBUTE_TICK_INC

#define LV_ATTRIBUTE_TIMER_HANDLER

#define LV_ATTRIBUTE_FLUSH_READY

/* 缓冲区所需的对齐大小 */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE         1

#define LV_ATTRIBUTE_MEM_ALIGN

#define LV_ATTRIBUTE_LARGE_CONST

#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

#define LV_ATTRIBUTE_FAST_MEM

#define LV_ATTRIBUTE_DMA

#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning 

#define LV_USE_LARGE_COORD                  0


/*********************************************************************************

                                        字库设置
                                        
 ***********************************************************************************/
/* Montserrat字体的ASCII范围和一些符号，使用bpp = 4
 * https://fonts.google.com/specimen/Montserrat */
#define LV_FONT_MONTSERRAT_8                0
#define LV_FONT_MONTSERRAT_10               0
#define LV_FONT_MONTSERRAT_12               1
#define LV_FONT_MONTSERRAT_14               1
#define LV_FONT_MONTSERRAT_16               1
#define LV_FONT_MONTSERRAT_18               1
#define LV_FONT_MONTSERRAT_20               1
#define LV_FONT_MONTSERRAT_22               1
#define LV_FONT_MONTSERRAT_24               0
#define LV_FONT_MONTSERRAT_26               0
#define LV_FONT_MONTSERRAT_28               0
#define LV_FONT_MONTSERRAT_30               0
#define LV_FONT_MONTSERRAT_32               1
#define LV_FONT_MONTSERRAT_34               0
#define LV_FONT_MONTSERRAT_36               0
#define LV_FONT_MONTSERRAT_38               0
#define LV_FONT_MONTSERRAT_40               1
#define LV_FONT_MONTSERRAT_42               0
#define LV_FONT_MONTSERRAT_44               0
#define LV_FONT_MONTSERRAT_46               0
#define LV_FONT_MONTSERRAT_48               1

/* 展示特色 */
#define LV_FONT_MONTSERRAT_12_SUBPX         0
#define LV_FONT_MONTSERRAT_28_COMPRESSED    0  /* bpp = 3 */
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW    0  
#define LV_FONT_SIMSUN_16_CJK               1  /* 1000个最常见的CJK自由基 */

/* 像素完美的单空间字体 */
#define LV_FONT_UNSCII_8                    0
#define LV_FONT_UNSCII_16                   0

/* 可选声明自定义字体 */
#define LV_FONT_CUSTOM_DECLARE

/* 默认字体 */
#define LV_FONT_DEFAULT                     &lv_font_montserrat_14

/* 启用处理大字体和/或带有大量字符的字体
 * 限制取决于字体大小、字体面和bpp */
#define LV_FONT_FMT_TXT_LARGE               0

/* 启用/禁用压缩字体支持 */
#define LV_USE_FONT_COMPRESSED              0

/* 亚像素渲染 */
#define LV_USE_FONT_SUBPX                   0
#if LV_USE_FONT_SUBPX
    #define LV_FONT_SUBPX_BGR               0  
#endif

/*********************************************************************************

                                        文本设置
                                        
 ***********************************************************************************/
/**
 * 为字符串选择字符编码
 * IDE或编辑器应具有相同的字符编码
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/* 允许在这些字符上中断（换行）文本 */
#define LV_TXT_BREAK_CHARS                  " ,.;:-_"

/* 设置perfect break long */
#define LV_TXT_LINE_BREAK_LONG_LEN          0

/* ��һ���������У���ͣ��֮ǰ����һ�е���С�ַ�����
 * ȡ����LV_TXT_LINE_BREAK_LONG_LEN�� */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3

/* ��һ���������У���ͣ�ٺ����һ�е���С�ַ�����
 * ȡ����LV_TXT_LINE_BREAK_LONG_LEN��*/
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* ���������ı�������ɫ�Ŀ����ַ��� */
#define LV_TXT_COLOR_CMD                    "#"

/* ֧��˫���ı���������ϴ����Һʹ��ҵ�����ı���
 * ��������Unicode˫���㷨���д���:
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI                         0
#if LV_USE_BIDI
    /* ����Ĭ�Ϸ���֧�ֵ�ֵ:
    *`LV_BASE_DIR_LTR` ������
    *`LV_BASE_DIR_RTL` ���ҵ���
    *`LV_BASE_DIR_AUTO` ����ı��������� */
    #define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/* ֧�ְ�������/��˹����
 * ����Щ�����У��ַ�Ӧ�ø��������ı��е�λ�ñ��滻Ϊ������ʽ */
#define LV_USE_ARABIC_PERSIAN_CHARS         0



/*********************************************************************************

                                        �ؼ�����
                                        
 ***********************************************************************************/
/* С�������ĵ�:https://docs.lvgl.io/latest/en/html/widgets/index.html */

#define LV_USE_ARC                          1

#define LV_USE_ANIMIMG                      1

#define LV_USE_BAR                          1

#define LV_USE_BTN                          1

#define LV_USE_BTNMATRIX                    1

#define LV_USE_CANVAS                       1

#define LV_USE_CHECKBOX                     1

#define LV_USE_DROPDOWN                     1   /* ����: lv_label */

#define LV_USE_IMG                          1   /* ����: lv_label */

#define LV_USE_LABEL                        1
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION         1  /* ���ñ�ǩ��ѡ���ı�*/
    #define LV_LABEL_LONG_TXT_HINT          1  /* �ڱ�ǩ�д洢һЩ�������Ϣ���Լӿ���Ʒǳ������ı� */
#endif

#define LV_USE_LINE                         1

#define LV_USE_ROLLER                       1  /* ����: lv_label */
#if LV_USE_ROLLER
    #define LV_ROLLER_INF_PAGES             7  /* ����Ͳ����ʱ������ġ�ҳ���� */
#endif

#define LV_USE_SLIDER                       1  /* ����: lv_bar*/

#define LV_USE_SWITCH                       1

#define LV_USE_TEXTAREA                     1  /* ����: lv_label*/
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME   1500    /*ms*/
#endif

#define LV_USE_TABLE                        1


/*********************************************************************************

                                        �ر���
                                        
 ***********************************************************************************/
/*-----------
 * 1. �ؼ�
 *----------*/
#define LV_USE_CALENDAR                     1
#if LV_USE_CALENDAR
    #define LV_CALENDAR_WEEK_STARTS_MONDAY  0
    #if LV_CALENDAR_WEEK_STARTS_MONDAY
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
    #else
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
    #endif

    #define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
    #define LV_USE_CALENDAR_HEADER_ARROW    1
    #define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif  /*LV_USE_CALENDAR*/

#define LV_USE_CHART                        1

#define LV_USE_COLORWHEEL                   1

#define LV_USE_IMGBTN                       1

#define LV_USE_KEYBOARD                     1

#define LV_USE_LED                          1

#define LV_USE_LIST                         1

#define LV_USE_MENU                         1

#define LV_USE_METER                        1

#define LV_USE_MSGBOX                       1

#define LV_USE_SPINBOX                      1

#define LV_USE_SPINNER                      1

#define LV_USE_TABVIEW                      1

#define LV_USE_TILEVIEW                     1

#define LV_USE_WIN                          1

#define LV_USE_SPAN                         1
#if LV_USE_SPAN
    /* һ�����ı����԰������������span������ */
    #define LV_SPAN_SNIPPET_STACK_SIZE      64
#endif

/*-----------
 * 2. ����
 *----------*/

/* һ���򵥣�����ӡ����̺ͷǳ����������� e*/
#define LV_USE_THEME_DEFAULT                1
#if LV_USE_THEME_DEFAULT

    /* 0:��ģʽ;1:�ڰ���ģʽ */
    #define LV_THEME_DEFAULT_DARK           0

    /* 1:����ʹ������ */
    #define LV_THEME_DEFAULT_GROW           1

    /* Ĭ��ת��ʱ��[ms] */
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/* һ���ǳ��򵥵����⣬����һ���Զ��������������� */
#define LV_USE_THEME_BASIC                  1

/* רΪ��ɫ��ʾ����Ƶ����� */
#define LV_USE_THEME_MONO                   1

/*-----------
 * 3. ����
 *----------*/

/* ������CSS�е�Flexbox���� */
#define LV_USE_FLEX                         1

/* ������CSS�е����񲼾� */
#define LV_USE_GRID                         1

/*---------------------
 * 4. ��������
 *--------------------*/

/* ����ͨ��api���ļ�ϵͳ�ӿ�
 * Ϊ��API����������������� */
 
/* STDIO */
#define LV_USE_FS_STDIO            0
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER      '\0'        /* ����һ���ɷ����������Ĵ�д��ĸ(���硣��A��) */
    #define LV_FS_STDIO_PATH        ""          /* ���ù���Ŀ¼���ļ�/Ŀ¼·������׷�ӵ��� */
    #define LV_FS_STDIO_CACHE_SIZE  0           /* >0��lv_fs_read()�л�������ֽ��� */
#endif

/* POSIX */
#define LV_USE_FS_POSIX             0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER      '\0'        /* ����һ���ɷ����������Ĵ�д��ĸ(���硣��A��) */
    #define LV_FS_POSIX_PATH        ""          /* ���ù���Ŀ¼���ļ�/Ŀ¼·������׷�ӵ��� */
    #define LV_FS_POSIX_CACHE_SIZE  0           /* >0��lv_fs_read()�л�������ֽ��� */
#endif

/* WIN32 */
#define LV_USE_FS_WIN32             0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER      '\0'        /* ����һ���ɷ����������Ĵ�д��ĸ(���硣��A��) */
    #define LV_FS_WIN32_PATH        ""          /* ���ù���Ŀ¼���ļ�/Ŀ¼·������׷�ӵ��� */
    #define LV_FS_WIN32_CACHE_SIZE  0           /* >0��lv_fs_read()�л�������ֽ��� */
#endif

/* FATFS */
#define LV_USE_FS_FATFS             0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER      '\0'        /* ����һ���ɷ����������Ĵ�д��ĸ(���硣��A��) */
    #define LV_FS_FATFS_CACHE_SIZE  0           /* >0��lv_fs_read()�л�������ֽ��� */
#endif

/* PNG�������� */
#define LV_USE_PNG                          0

/* BMP �������� */
#define LV_USE_BMP                          0

/* JPG +�ָ�JPG�������⡣
 * Split JPG��ΪǶ��ʽϵͳ�Ż����Զ����ʽ */
#define LV_USE_SJPG                         0

/* GIF�������� */
#define LV_USE_GIF                          0

/* QR�������� */
#define LV_USE_QRCODE                       0

/* FreeType�� */
#define LV_USE_FREETYPE                     0
#if LV_USE_FREETYPE
    /* FreeType���ڻ����ַ�[bytes]���ڴ�(-1:û�л���) */
    #define LV_FREETYPE_CACHE_SIZE          (16 * 1024)
    #if LV_FREETYPE_CACHE_SIZE >= 0
        /* 1:λͼcacheʹ��sbit cache, 0:λͼcacheʹ��ͼ��cache
         * sbit����:����С��λͼ(�����С< 256)�������ڴ�Ч�ʸ���
         *��������С>= 256����������Ϊͼ�񻺴� */
        #define LV_FREETYPE_SBIT_CACHE      0
        /* ���������ʵ�������Ĵ򿪵�FT_Face/FT_Size��������������*��
           (0:ʹ��ϵͳĬ��ֵ) */
        #define LV_FREETYPE_CACHE_FT_FACES  0
        #define LV_FREETYPE_CACHE_FT_SIZES  0
    #endif
#endif

/* Rlottie �� */
#define LV_USE_RLOTTIE                      0

/* FFmpeg���ͼ�����Ͳ�����Ƶ
 * ֧��������Ҫ��ͼ���ʽ�����Բ���������ͼ�����������t*/
#define LV_USE_FFMPEG                       0
#if LV_USE_FFMPEG
    /* ��������Ϣת����stderr */
    #define LV_FFMPEG_AV_DUMP_FORMAT        0
#endif

/*-----------
 * 5. ����
 *----------*/

/* 1:����API�Զ�����п��� */
#define LV_USE_SNAPSHOT                     1

/* 1:ʹ��Monkey���� */
#define LV_USE_MONKEY                       0

/* 1:�������񵼺� */
#define LV_USE_GRIDNAV                      0

/*********************************************************************************

                                        ʵ��
                                        
 ***********************************************************************************/
/* �����ÿ⹹��ʾ�� */
#define LV_BUILD_EXAMPLES                   1

/*===================
 * ��ʾʹ��
 ====================*/

/* ��ʾ��һЩ������������Ҫ���ӡ�LV_MEM_SIZE�� */
#define LV_USE_DEMO_WIDGETS                 0
#if LV_USE_DEMO_WIDGETS
#define LV_DEMO_WIDGETS_SLIDESHOW           0
#endif

/* ��ʾ�������ͼ��̵��÷� */
#define LV_USE_DEMO_KEYPAD_AND_ENCODER      0

/* ��׼ϵͳ */
#define LV_USE_DEMO_BENCHMARK               0

/* LVGLѹ������ */
#define LV_USE_DEMO_STRESS                  0

/* ���ֲ���������ʾ */
#define LV_USE_DEMO_MUSIC                   1
#if LV_USE_DEMO_MUSIC
# define LV_DEMO_MUSIC_SQUARE               0
# define LV_DEMO_MUSIC_LANDSCAPE            0
# define LV_DEMO_MUSIC_ROUND                0
# define LV_DEMO_MUSIC_LARGE                0
# define LV_DEMO_MUSIC_AUTO_PLAY            0
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
