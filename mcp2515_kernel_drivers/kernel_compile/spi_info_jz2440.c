#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <linux/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

#include <linux/can/platform/mcp251x.h>


static struct mcp251x_platform_data mcp251x_info = {
                .oscillator_frequency = 8 * 1000 * 1000, //设置MCP2515外部晶振频率
        };


static struct spi_board_info spi_info_jz2440[] = {
	{
    	 .modalias = "mcp2515",  /* ��Ӧ��spi_driver����Ҳ��"oled" */
    	 .max_speed_hz = 2*1000*1000,	/* max spi clock (SCK) speed in HZ */
    	 .bus_num = 1,     /* jz2440��OLED����SPI CONTROLLER 1 */
    	 .mode    = SPI_MODE_0,
    	 .irq=IRQ_EINT(12),
    	 .chip_select   = S3C2410_GPF(1), /* oled_cs, ���ĺ�����spi_masterȷ�� */
    	 .platform_data = &mcp251x_info, /* oled_dc, ����spi_driver��ʹ�� */    	
	 }
	
};

static int spi_info_jz2440_init(void)
{
    return spi_register_board_info(spi_info_jz2440, ARRAY_SIZE(spi_info_jz2440));
}

module_init(spi_info_jz2440_init);
MODULE_DESCRIPTION("OLED SPI Driver");
MODULE_AUTHOR("weidongshan@qq.com,www.100ask.net");
MODULE_LICENSE("GPL");


