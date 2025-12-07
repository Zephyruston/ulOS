/*
 * Change Logs:
 * Date           Author       Notes
 * 2025-7-21     zhuqinsheng   the first version
 * 2025-8-13     zhuqinsheng   修改了ul_button_t的一些属性, 主要增加了type成员
 */

#include "ul_button.h"

#define BUTTON_STAGE_NORMAL         0
#define BUTTON_STAGE_SHAKE          1
#define BUTTON_STAGE_LONGPRESS      2
#define BUTTON_STAGE_DOUBLEPRESS    3
#define BUTTON_STAGE_IDLE           4

static ul_list_t *button_list_head_node;
static ul_uint8_t ul_list_head_node_created_flag = 0;

static void ul_button_task_handler(ul_button_t * self, ul_uint32_t period)
{
    if (self == NULL)
    {
        return;
    }
    ul_uint32_t pin = self->read_pin(self);

    switch (self->stage)
    {
        case BUTTON_STAGE_NORMAL:
        {
            if (pin == 1)
            {
                self->stage = BUTTON_STAGE_SHAKE;
            }
        }
        break;

        case BUTTON_STAGE_SHAKE:
        {
            if (pin == 1)
            {
                self->stage = BUTTON_STAGE_LONGPRESS;
            }
            else
            {
                self->stage = BUTTON_STAGE_NORMAL;
            }
        }
        break;

        case BUTTON_STAGE_LONGPRESS:
        {
            if (pin == 0)
            {
                self->stage = BUTTON_STAGE_DOUBLEPRESS;
            }
            else
            {
                self->long_ticks_count += period;

                if (self->long_ticks_count >= self->config.lp_th)
                {
                    self->event_callback(self, BUTTON_EVENT_LONGPRESS);
                    self->stage = BUTTON_STAGE_IDLE;
                }
            }
        }
        break;

        case BUTTON_STAGE_DOUBLEPRESS:
        {
            self->double_ticks_count += period;

            if (pin == 1)
            {
                self->event_callback(self, BUTTON_EVENT_DOUBLE_CLICK);
                self->stage = BUTTON_STAGE_IDLE;
            }
            else if (self->double_ticks_count > self->config.dp_th)
            {
                self->event_callback(self, BUTTON_EVENT_PRESSED);
                self->stage = BUTTON_STAGE_IDLE;
            }
        }
        break;

        case BUTTON_STAGE_IDLE:
        {
            if (pin == 0)
            {
                self->double_ticks_count = 0;
                self->long_ticks_count = 0;
                self->stage = BUTTON_STAGE_NORMAL;
            }
        }
        break;
    }
}

static void ul_list_head_node_create(ul_list_t **head)
{
    /* 如果没有pin设备的头节点，创建一个头节点 */
    if (ul_list_head_node_created_flag == 0)
    {
        *head = (ul_list_t *)ul_malloc(sizeof(ul_list_t));

        if (*head != NULL)
        {
            ul_list_init(*head);

            if (ul_list_isempty(*head))
            {
                ul_list_head_node_created_flag = 1;
            }
        }
    }
}


/**
 * 注册button事件回调函数
 *
 * @param self button对象的地址
 * @param button_event_callback_t 要注册的事件回调函数地址
 */
void ul_button_register_callback(ul_button_t * self, button_event_callback_t func)
{
    if (func == NULL)
    {
        return;
    }
    self->event_callback = func;
}

/**
 * button对象默认的事件回调函数
 *
 * @param self button对象的地址
 * @param event 事件代码
 */
ul_weak void ul_button_defalut_event_callback(ul_button_t * self, button_event_e event)
{
    
}

/**
 * 初始化一个静态的button对象，并将其节点插入到button链表的最后
 *
 * @param self button对象的地址
 *
 * @return 错误代码
 */
ul_ecode ul_button_object_init(ul_button_t * self)
{
    if (self == NULL)
    {
        return UL_ERROR;
    }
    /* 第一次创建头节点 */
    ul_list_head_node_create(&button_list_head_node);
    
    self->long_ticks_count      = 0;
    self->double_ticks_count    = 0;
    self->event_callback        = ul_button_defalut_event_callback;
    /* 对象节点插入链表最后 */
    ul_list_insert_before(button_list_head_node, &self->node);   // 节点插入到链表最后一个
    
    self->type                  |= UL_BUTTON_TYPE_STATIC;
    self->stage = BUTTON_STAGE_NORMAL;
    return UL_EOK;
}

/**
 * 动态创建一个button对象，并将其节点插入到button链表的最后
 *
 * @return button对象的地址
 */
ul_button_t* ul_button_object_create(void)
{
    ul_button_t * button_object;
    /* 第一次创建头节点 */
    ul_list_head_node_create(&button_list_head_node);
        
    button_object = (ul_button_t *)ul_malloc(sizeof(struct ul_button));

    if (button_object == NULL)
    {
        return UL_NULL;
    }
    
    button_object->long_ticks_count     = 0;
    button_object->double_ticks_count   = 0;
    button_object->stage                = BUTTON_STAGE_NORMAL;
    button_object->event_callback       = ul_button_defalut_event_callback;
    button_object->type                |= UL_BUTTON_TYPE_DYNAMIC;
    /* 对象节点插入链表最后 */
    ul_list_insert_before(button_list_head_node, &button_object->node);

    
    return button_object;
}

/**
 * 删除一个动态的button对象，并将其节点从button链表移除
 *
 * @return 错误代码
 */
ul_ecode ul_button_object_delete(ul_button_t * self)
{
    if (self == NULL)
    {
        return UL_ERROR;
    }
    /* 从button链表移除 */
    ul_list_remove(&self->node);
    
    /* 动态的对象需要释放内存 */
    if (self->type & UL_BUTTON_TYPE_DYNAMIC)
    {
        ul_free(self);
    }
    
    return UL_EOK;
}

/**
 * 设置button长按和双击的时间阈值
 *
 * @param self button对象的地址
 * @param longpress_time 长按时间阈值
 * @param doublepress_time 双击时间阈值
 */
void ul_button_presstime_config(ul_button_t * self, ul_uint32_t longpress_time, ul_uint32_t doublepress_time)
{
    self->config.lp_th = longpress_time;
    self->config.dp_th = doublepress_time;
}

/**
 * button定时器回调函数，需以一个固定周期调用此函数
 *
 * @param period 周期ms
 */
void ul_button_timer_callback(ul_uint32_t period)
{
    ul_list_t * now;

    /* 头节点未创建 */
    if (button_list_head_node == NULL)
    {
        return;
    }
    /* 遍历button链表 */
    ul_list_for_each(now, button_list_head_node)
    {
        ul_button_task_handler(ul_list_entry(now, struct ul_button, node), period);
    }
}
