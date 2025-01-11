/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include "../../lvgl.h"
/* 触摸屏驱动头文件 */
#include "touch.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/* 触摸屏 */
static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

/* 鼠标 */
//static void mouse_init(void);
//static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
//static bool mouse_is_pressed(void);
//static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

/* 键盘 */
//static void keypad_init(void);
//static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
//static uint32_t keypad_get_key(void);

/* 编码器 */
//static void encoder_init(void);
//static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
//static void encoder_handler(void);

/* 按键 */
//static void button_init(void);
//static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
//static int8_t button_get_pressed_id(void);
//static bool button_is_pressed(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;    // 触摸屏
//lv_indev_t * indev_mouse;       // ���
//lv_indev_t * indev_keypad;      // ����
//lv_indev_t * indev_encoder;     // ������
//lv_indev_t * indev_button;      // ��ť

/* ��������� */
//static int32_t encoder_diff;
//static lv_indev_state_t encoder_state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * @brief       ��ʼ����ע�������豸
 * @param       ��
 * @retval      ��
 */
void lv_port_indev_init(void)
{
    /**
     * 
     * ������������ҵ� LittlevGL ֧�ֵĳ����豸��ʵ��ʾ��:
     *  - ������
     *  - ��� (֧�ֹ��)
     *  - ���� (��֧�ְ����� GUI �÷�)
     *  - ������ (֧�ֵ� GUI �÷�������: ��, ��, ����)
     *  - ��ť (������Ļ��ָ������ⲿ��ť)
     *
     *  ���� `..._read()` ֻ��ʾ��
     *  ����Ҫ���ݾ����Ӳ���������Щ����
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * ������
     * -----------------*/

    /* ��ʼ��������(�����) */
    touchpad_init();

    /* ע�ᴥ���������豸 */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    /*------------------
     * ���
     * -----------------*/

    /* ��ʼ�����(�����) */
//    mouse_init();

    /* ע����������豸 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_POINTER;
//    indev_drv.read_cb = mouse_read;
//    indev_mouse = lv_indev_drv_register(&indev_drv);

    /* ���ù�꣬Ϊ�˼��������������Ϊһ�� HOME ���� */
//    lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
//    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
//    lv_indev_set_cursor(indev_mouse, mouse_cursor);

    /*------------------
     * ����
     * -----------------*/

//    /* ��ʼ������(�����) */
//    keypad_init();

//    /* ע����������豸 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
//    indev_drv.read_cb = keypad_read;
//    indev_keypad = lv_indev_drv_register(&indev_drv);

//    /* ��������Ҫ�� `lv_group_t * group = lv_group_create()` ��������
//     * �� `lv_group_add_obj(group, obj)` ��������������
//     * ������������豸���䵽���У��Ե�������:
//     * `lv_indev_set_group(indev_keypad, group);` */

    /*------------------
     * ������
     * -----------------*/

//    /* ��ʼ��������(�����) */
//    encoder_init();

//    /* ע������������豸 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_ENCODER;
//    indev_drv.read_cb = encoder_read;
//    indev_encoder = lv_indev_drv_register(&indev_drv);

//    /* ��������Ҫ�� `lv_group_t * group = lv_group_create()` ��������
//     * �� `lv_group_add_obj(group, obj)` ��������������
//     * ������������豸���䵽���У��Ե�������:
//     * `lv_indev_set_group(indev_keypad, group);` */

    /*------------------
     * ��ť
     * -----------------*/

//    /* ��ʼ����ť(�����) */
//    button_init();

//    /* ע�ᰴť�����豸 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_BUTTON;
//    indev_drv.read_cb = button_read;
//    indev_button = lv_indev_drv_register(&indev_drv);

//    /* Ϊ��ť������Ļ�ϵĵ�
//     * �Դ����ð�ťģ������Ļ�϶�Ӧ�ĵ� */
//    static const lv_point_t btn_points[2] = {
//            {10, 10},   /*Button 0 -> x:10; y:10*/
//            {40, 100},  /*Button 1 -> x:40; y:100*/
//    };
//    lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * ������
 * -----------------*/

/**
 * @brief       ��ʼ��������
 * @param       ��
 * @retval      ��
 */
static void touchpad_init(void)
{
    /*Your code comes here*/
    tp_dev.init();
    /* ���������������ʾ��XY����������Ҫ������� */
    if (0 == (tp_dev.touchtype & 0x80)) {
        tp_adjust();
        tp_save_adjust_data();
    }
}

/**
 * @brief       ͼ�ο�Ĵ�������ȡ�ص�����
 * @param       indev_drv   : �������豸
 *   @arg       data        : �����豸���ݽṹ��
 * @retval      ��
 */
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /* ���水�µ������״̬ */
    if(touchpad_is_pressed())
    {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    } 
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /* ��������µ����� */
    data->point.x = last_x;
    data->point.y = last_y;
}

/**
 * @brief       ��ȡ�������豸��״̬
 * @param       ��
 * @retval      ���ش������豸�Ƿ񱻰���
 */
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/
    tp_dev.scan(0);

    if (tp_dev.sta & TP_PRES_DOWN)
    {
        return true;
    }

    return false;
}

/**
 * @brief       �ڴ����������µ�ʱ���ȡ x��y ����
 * @param       x   : x�����ָ��
 *   @arg       y   : y�����ָ��
 * @retval      ��
 */
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/
    (*x) = tp_dev.x[0];
    (*y) = tp_dev.y[0];
}

/*------------------
 * ���
 * -----------------*/

/**
 * @brief       ��ʼ�����
 * @param       ��
 * @retval      ��
 */
//static void mouse_init(void)
//{
//    /*Your code comes here*/
//    tp_dev.init();
//    /* ���������������ʾ��XY����������Ҫ������� */
//    if (0 == (tp_dev.touchtype & 0x80))
//    {
//        tp_adjust();
//        tp_save_adjust_data();
//    }
//}

/**
 * @brief       ͼ�ο������ȡ�ص�����
 * @param       indev_drv   : ����豸
 *   @arg       data        : �����豸���ݽṹ��
 * @retval      ��
 */
//static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
//{
//    /* ��ȡ��ǰ�� x��y ���� */
//    mouse_get_xy(&data->point.x, &data->point.y);

//    /* ��ȡ�Ƿ��»��ͷ���갴ť */
//    if(mouse_is_pressed()) {
//        data->state = LV_INDEV_STATE_PR;
//    } else {
//        data->state = LV_INDEV_STATE_REL;
//    }
//}

/**
 * @brief       ��ȡ����豸�Ƿ񱻰���
 * @param       ��
 * @retval      ��������豸�Ƿ񱻰���
 */
//static bool mouse_is_pressed(void)
//{
//    /*Your code comes here*/
//    tp_dev.scan(0);
//    
//    if (tp_dev.sta & TP_PRES_DOWN)
//    {
//        return true;
//    }
//    
//    return false;
//}

/**
 * @brief       ����걻����ʱ����ȡ���� x��y ����
 * @param       x   : x�����ָ��
 *   @arg       y   : y�����ָ��
 * @retval      ��
 */
//static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
//{
//    /*Your code comes here*/

//    (*x) = tp_dev.x[0];
//    (*y) = tp_dev.y[0];
//}

/*------------------
 * ����
 * -----------------*/

///**
// * @brief       ��ʼ������
// * @param       ��
// * @retval      ��
// */
//static void keypad_init(void)
//{
//    /*Your code comes here*/
//}

///**
// * @brief       ͼ�ο�ļ��̶�ȡ�ص�����
// * @param       indev_drv : �����豸
// *   @arg       data      : �����豸���ݽṹ��
// * @retval      ��
// */
//static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
//{
//    static uint32_t last_key = 0;

////    /* ��δ����� LVGL ���������ӣ������ȡ��������Ƕ���� */
////    /*Get the current x and y coordinates*/
////    mouse_get_xy(&data->point.x, &data->point.y);

//    /* ��ȡ�����Ƿ񱻰��£��������ֵ */
//    uint32_t act_key = keypad_get_key();
//    if(act_key != 0) {
//        data->state = LV_INDEV_STATE_PR;

//        /* ����ֵת���� LVGL �Ŀ����ַ� */
//        switch(act_key) {
//        case 1:
//            act_key = LV_KEY_NEXT;
//            break;
//        case 2:
//            act_key = LV_KEY_PREV;
//            break;
//        case 3:
//            act_key = LV_KEY_LEFT;
//            break;
//        case 4:
//            act_key = LV_KEY_RIGHT;
//            break;
//        case 5:
//            act_key = LV_KEY_ENTER;
//            break;
//        }

//        last_key = act_key;
//    } else {
//        data->state = LV_INDEV_STATE_REL;
//    }

//    data->key = last_key;
//}

///**
// * @brief       ��ȡ��ǰ���ڰ��µİ���
// * @param       ��
// * @retval      0 : ����û�б�����
// */
//static uint32_t keypad_get_key(void)
//{
//    /*Your code comes here*/

//    return 0;
//}

/*------------------
 * ������
 * -----------------*/

///**
// * @brief       ��ʼ��������
// * @param       ��
// * @retval      ��
// */
//static void encoder_init(void)
//{
//    /*Your code comes here*/
//}


///**
// * @brief       ͼ�ο�ı�������ȡ�ص�����
// * @param       indev_drv : �������豸
// *   @arg       data      : �����豸���ݽṹ��
// * @retval      ��
// */
//static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
//{

//    data->enc_diff = encoder_diff;
//    data->state = encoder_state;
//}

///**
// * @brief       ���ж��е��ô˺����Դ����������¼�(��ת������)
// * @param       ��
// * @retval      ��
// */
//static void encoder_handler(void)
//{
//    /*Your code comes here*/

//    encoder_diff += 0;
//    encoder_state = LV_INDEV_STATE_REL;
//}

/*------------------
 * ��ť
 * -----------------*/


///**
// * @brief       ��ʼ����ť
// * @param       ��
// * @retval      ��
// */
//static void button_init(void)
//{
//    /*Your code comes here*/
//}

///**
// * @brief       ͼ�ο�İ�ť��ȡ�ص�����
// * @param       indev_drv : ��ť�豸
// *   @arg       data      : �����豸���ݽṹ��
// * @retval      ��
// */
//static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
//{

//    static uint8_t last_btn = 0;

//    /* ��ȡ�����°�ť��ID */
//    int8_t btn_act = button_get_pressed_id();

//    if(btn_act >= 0) {
//        data->state = LV_INDEV_STATE_PR;
//        last_btn = btn_act;
//    } else {
//        data->state = LV_INDEV_STATE_REL;
//    }

//    /* ������󱻰��°�ť��ID */
//    data->btn_id = last_btn;
//}

///**
// * @brief       ��ȡ�����°�ť��ID
// * @param       ��
// * @retval      �����°�ť��ID
// */
//static int8_t button_get_pressed_id(void)
//{
//    uint8_t i;

//    /* ����Ǹ�����������(���������ʾ��������������ť�����) */
//    for(i = 0; i < 2; i++) {
//        /* ���ر����°�ť��ID */
//        if(button_is_pressed(i)) {
//            return i;
//        }
//    }

//    /* û�а�ť������ */
//    return -1;
//}

///**
// * @brief       ���ָ��ID�İ�ť�Ƿ񱻰���
// * @param       ��
// * @retval      ��ť�Ƿ񱻰���
// */
//static bool button_is_pressed(uint8_t id)
//{

//    /*Your code comes here*/

//    return false;
//}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
