1. CUBEMX配置好基础工程后，关闭pendsv中断服务函数自动代码生成

![image-20251208122114544](.\pic\image-20251208122114544.png)

2. 新建文件夹`ulOS`

   <img src=".\pic\image-20251208122653544.png" alt="image-20251208122653544" style="zoom: 33%;" />

   

3. 将源码中的`kernel`，`platform`文件夹与`ul_config.h`移至刚刚创建的文件夹内

   <img src=".\pic\image-20251208123021936.png" alt="image-20251208123208810" style="zoom: 33%;" />

4. 添加头文件路径

   <img src=".\pic\image-20251208123400790.png" alt="image-20251208123400790" style="zoom:50%;" />

5. 添加文件夹

<img src=".\pic\image-20251208123502116.png" alt="image-20251208123502116" style="zoom:50%;" />

6. 分别添加`ulOS/kernel/src` 与 `ulOS/platform/cortex-m3`内全部文件

<img src=".\pic\image-20251208123722056.png" alt="image-20251208123722056" style="zoom: 50%;" />

7. 编译无错误，main函数内添加操作系统启动代码，创建启动线程与两个测试线程，具体代码见template文件

   ```c
   #include "ulOS_thread.h"
   
   void thread1_entry(void *p)
   {
       while(1)
       {
           HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
           ul_thread_delay(100);
       }
   }
   
   void thread2_entry(void *p)
   {
       while(1)
       {
           HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
           ul_thread_delay(300);
       }
   }
   
   void start_thread_entry(void *p)
   {
       ul_thread_t *tid1 = ul_thread_create("t1", thread1_entry, UL_NULL, 512, 0, 1);
       ul_thread_t *tid2 = ul_thread_create("t2", thread2_entry, UL_NULL, 512, 0, 1);
       
       ul_thread_startup(tid1);
       ul_thread_startup(tid2);
       
       while(1)
       {
           ul_thread_delay(10);
       }
   }
   
   void main()
   {
       ul_kernel_init();
       
       ul_thread_startup(ul_thread_create("start", start_thread_entry, UL_NULL, 512, 1, 1));
       
       ul_scheduler_start();
       while(1);...
   }
   ```

   

   <img src=".\pic\image-20251208124017710.png" alt="image-20251208124017710" style="zoom: 33%;" />

8. `stm32f1xx_it.c`文件内，systick中断添加代码如下，编译无错误

   ```c
   #include "ulOS_thread.h"
   
   void SysTick_Handler(void)
   {
   	ul_tick_increase();
       //.......其他代码
   }
   ```

   <img src=".\pic\image-20251208124551971.png" alt="image-20251208124551971" style="zoom: 33%;" />

   9. 编译无错误，下载发现两个LED交替闪烁