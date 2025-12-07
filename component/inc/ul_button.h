/*
 * Change Logs:
 * Date           Author       Notes
 * 2025-7-21     zhuqinsheng   the first version
 * 2025-8-13     zhuqinsheng   修改了ul_button_t的一些属性, 主要增加了type成员
 */
#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "ul_config.h"
#include "ul_list.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
 * button type definition
 */
#define UL_BUTTON_TYPE_STATIC      0x01
#define UL_BUTTON_TYPE_DYNAMIC     0x02

/*
 * button event enumeration
 */
typedef enum
{
    BUTTON_EVENT_PRESSED        = 0x01,      /* button单击 */
    BUTTON_EVENT_LONGPRESS      = 0x02,      /* button长按 */
    BUTTON_EVENT_DOUBLE_CLICK   = 0x03,      /* button双击 */
} button_event_e;

/*
 * button structure
 */
typedef struct ul_button ul_button_t;
typedef void (*button_event_callback_t)(ul_button_t *self,  button_event_e event);
struct ul_button 
{
	ul_uint32_t             id;
    ul_uint8_t              stage;
    ul_uint32_t             long_ticks_count;
    ul_uint32_t             double_ticks_count;
    
    struct 
    {
        ul_uint32_t         dp_th; /* 双击时间阈值ms */
        ul_uint32_t         lp_th; /* 长按时间阈值ms */
    }config;
    
    button_event_callback_t event_callback;
    button_event_e          event;
    ul_uint8_t              type;
    ul_list_t               node;
    
    /* 移植接口 */
    /**
      * @brief 读取button状态
      * @attention 用户实现此接口时其返回值必须为按下时为1,松开时为0
      * @return 0 按钮未被按下
      * @return 1 按钮已被按下
    */
    ul_uint32_t (*read_pin)(ul_button_t *self);

};

/*
 * button interface
 */
ul_ecode ul_button_object_init(ul_button_t *self);
ul_ecode ul_button_object_detach(ul_button_t *self);
ul_button_t* ul_button_object_create(void);
ul_ecode ul_button_object_delete(ul_button_t *self);

void ul_button_presstime_config(ul_button_t *self, ul_uint32_t longpress_time, ul_uint32_t doublepress_time);
void ul_button_register_callback(ul_button_t *self, button_event_callback_t func);

void ul_button_defalut_event_callback(ul_button_t *self, button_event_e event);

void ul_button_timer_callback(ul_uint32_t period);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

