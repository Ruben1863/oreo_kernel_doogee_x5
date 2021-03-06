#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mt-plat/mt_gpio.h>
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH   (720)
#define FRAME_HEIGHT  (1280)


#define REGFLAG_DELAY             							0XFEE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE 
#define TRUE 1
#endif
//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
#define LCM_DSI_CMD_MODE									0

#define LCM_RM68200_ID 		(0x6820)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = { 0 };

#define SET_RESET_PIN(v)    		(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 			(lcm_util.udelay(n))
#define MDELAY(n) 			(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) lcm_util.dsi_write_regs(addr, pdata, byte_nums)
/* #define read_reg lcm_util.dsi_read_reg() */
#define read_reg_v2(cmd, buffer, buffer_size) lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static void lcm_init_power(void)
{
}


static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}
static struct LCM_setting_table lcm_initialization_setting[] = {
    { 0xFE, 0x01, {0x01}},
    { 0x00, 0x01, {0x0A}},
    { 0x24, 0x01, {0xC0}},
    { 0x25, 0x01, {0x53}},
    { 0x26, 0x01, {0x00}},
    { 0x27, 0x01, {0x0A}},
    { 0x29, 0x01, {0x0A}},
    { 0x2B, 0x01, {0xE5}},
    { 0x16, 0x01, {0x52}},
    { 0x2F, 0x01, {0x54}},
    { 0x34, 0x01, {0x55}},
    { 0x1B, 0x01, {0x00}},
    { 0x12, 0x01, {0x08}},
    { 0x1A, 0x01, {0x06}},
    { 0x46, 0x01, {0x66}},
    { 0x52, 0x01, {0x78}},
    { 0x53, 0x01, {0x00}},
    { 0x54, 0x01, {0x78}},
    { 0x55, 0x01, {0x00}},
    { 0x5F, 0x01, {0x12}},
    { 0xFE, 0x01, {0x03}},
    { 0x01, 0x01, {0x14}},
    { 0x02, 0x01, {0x01}},
    { 0x03, 0x01, {0x00}},
    { 0x04, 0x01, {0x00}},
    { 0x05, 0x01, {0x00}},
    { 0x06, 0x01, {0x00}},
    { 0x07, 0x01, {0x05}},
    { 0x08, 0x01, {0x14}},
    { 0x09, 0x01, {0x06}},
    { 0x0A, 0x01, {0x00}},
    { 0x0B, 0x01, {0x00}},
    { 0x0C, 0x01, {0x00}},
    { 0x0D, 0x01, {0x00}},
    { 0x0E, 0x01, {0x0A}},
    { 0x0F, 0x01, {0x0B}},
    { 0x10, 0x01, {0x0C}},
    { 0x11, 0x01, {0x0D}},
    { 0x12, 0x01, {0x00}},
    { 0x13, 0x01, {0x7D}},
    { 0x14, 0x01, {0x00}},
    { 0x15, 0x01, {0xC5}},
    { 0x16, 0x01, {0x08}},
    { 0x17, 0x01, {0x0E}},
    { 0x18, 0x01, {0x0F}},
    { 0x19, 0x01, {0x10}},
    { 0x1A, 0x01, {0x11}},
    { 0x1B, 0x01, {0x00}},
    { 0x1C, 0x01, {0x7D}},
    { 0x1D, 0x01, {0x00}},
    { 0x1E, 0x01, {0x85}},
    { 0x1F, 0x01, {0x08}},
    { 0x20, 0x01, {0x00}},
    { 0x21, 0x01, {0x00}},
    { 0x22, 0x01, {0x0A}},
    { 0x23, 0x01, {0x10}},
    { 0x24, 0x01, {0x12}},
    { 0x25, 0x01, {0x2D}},
    { 0x26, 0x01, {0x00}},
    { 0x27, 0x01, {0x14}},
    { 0x28, 0x01, {0x16}},
    { 0x29, 0x01, {0x2D}},
    { 0x2A, 0x01, {0x00}},
    { 0x2B, 0x01, {0x00}},
    { 0x2C, 0x01, {0x00}},
    { 0x2D, 0x01, {0x00}},
    { 0x2E, 0x01, {0x00}},
    { 0x2F, 0x01, {0x00}},
    { 0x30, 0x01, {0x00}},
    { 0x31, 0x01, {0x00}},
    { 0x32, 0x01, {0x00}},
    { 0x33, 0x01, {0x00}},
    { 0x34, 0x01, {0x00}},
    { 0x35, 0x01, {0x00}},
    { 0x36, 0x01, {0x00}},
    { 0x37, 0x01, {0x00}},
    { 0x38, 0x01, {0x00}},
    { 0x39, 0x01, {0x00}},
    { 0x3A, 0x01, {0x00}},
    { 0x3B, 0x01, {0x00}},
    { 0x3C, 0x01, {0x00}},
    { 0x3D, 0x01, {0x00}},
    { 0x3E, 0x01, {0x00}},
    { 0x3F, 0x01, {0x00}},
    { 0x40, 0x01, {0x00}},
    { 0x41, 0x01, {0x00}},
    { 0x42, 0x01, {0x00}},
    { 0x43, 0x01, {0x00}},
    { 0x44, 0x01, {0x00}},
    { 0x45, 0x01, {0x00}},
    { 0x46, 0x01, {0x00}},
    { 0x47, 0x01, {0x00}},
    { 0x48, 0x01, {0x00}},
    { 0x49, 0x01, {0x00}},
    { 0x4A, 0x01, {0x00}},
    { 0x4B, 0x01, {0x00}},
    { 0x4C, 0x01, {0x00}},
    { 0x4D, 0x01, {0x00}},
    { 0x4E, 0x01, {0x00}},
    { 0x4F, 0x01, {0x00}},
    { 0x50, 0x01, {0x00}},
    { 0x51, 0x01, {0x00}},
    { 0x52, 0x01, {0x00}},
    { 0x53, 0x01, {0x00}},
    { 0x54, 0x01, {0x00}},
    { 0x55, 0x01, {0x00}},
    { 0x56, 0x01, {0x00}},
    { 0x57, 0x01, {0x00}},
    { 0x58, 0x01, {0x00}},
    { 0x59, 0x01, {0x00}},
    { 0x5A, 0x01, {0x00}},
    { 0x5B, 0x01, {0x00}},
    { 0x5C, 0x01, {0x00}},
    { 0x5D, 0x01, {0x00}},
    { 0x5E, 0x01, {0x00}},
    { 0x5F, 0x01, {0x00}},
    { 0x60, 0x01, {0x00}},
    { 0x61, 0x01, {0x00}},
    { 0x62, 0x01, {0x00}},
    { 0x63, 0x01, {0x00}},
    { 0x64, 0x01, {0x00}},
    { 0x65, 0x01, {0x00}},
    { 0x66, 0x01, {0x00}},
    { 0x67, 0x01, {0x00}},
    { 0x68, 0x01, {0x00}},
    { 0x69, 0x01, {0x00}},
    { 0x6A, 0x01, {0x00}},
    { 0x6B, 0x01, {0x00}},
    { 0x6C, 0x01, {0x00}},
    { 0x6D, 0x01, {0x00}},
    { 0x6E, 0x01, {0x00}},
    { 0x6F, 0x01, {0x00}},
    { 0x70, 0x01, {0x00}},
    { 0x71, 0x01, {0x00}},
    { 0x72, 0x01, {0x00}},
    { 0x73, 0x01, {0x00}},
    { 0x74, 0x01, {0x00}},
    { 0x75, 0x01, {0x00}},
    { 0x76, 0x01, {0x00}},
    { 0x77, 0x01, {0x00}},
    { 0x78, 0x01, {0x00}},
    { 0x79, 0x01, {0x00}},
    { 0x7A, 0x01, {0x00}},
    { 0x7B, 0x01, {0x00}},
    { 0x7C, 0x01, {0x00}},
    { 0x7D, 0x01, {0x00}},
    { 0x7E, 0x01, {0x3F}},
    { 0x7F, 0x01, {0x3F}},
    { 0x80, 0x01, {0x1C}},
    { 0x81, 0x01, {0x1D}},
    { 0x82, 0x01, {0x0B}},
    { 0x83, 0x01, {0x09}},
    { 0x84, 0x01, {0x3F}},
    { 0x85, 0x01, {0x3F}},
    { 0x86, 0x01, {0x3F}},
    { 0x87, 0x01, {0x3F}},
    { 0x88, 0x01, {0x3F}},
    { 0x89, 0x01, {0x3F}},
    { 0x8A, 0x01, {0x0F}},
    { 0x8B, 0x01, {0x3F}},
    { 0x8C, 0x01, {0x3F}},
    { 0x8D, 0x01, {0x3F}},
    { 0x8E, 0x01, {0x3F}},
    { 0x8F, 0x01, {0x3F}},
    { 0x90, 0x01, {0x3F}},
    { 0x91, 0x01, {0x0D}},
    { 0x92, 0x01, {0x01}},
    { 0x93, 0x01, {0x03}},
    { 0x94, 0x01, {0x02}},
    { 0x95, 0x01, {0x00}},
    { 0x96, 0x01, {0x0C}},
    { 0x97, 0x01, {0x3F}},
    { 0x98, 0x01, {0x3F}},
    { 0x99, 0x01, {0x3F}},
    { 0x9A, 0x01, {0x3F}},
    { 0x9B, 0x01, {0x3F}},
    { 0x9C, 0x01, {0x3F}},
    { 0x9D, 0x01, {0x0E}},
    { 0x9E, 0x01, {0x3F}},
    { 0x9F, 0x01, {0x3F}},
    { 0xA0, 0x01, {0x3F}},
    { 0xA2, 0x01, {0x3F}},
    { 0xA3, 0x01, {0x3F}},
    { 0xA4, 0x01, {0x3F}},
    { 0xA5, 0x01, {0x08}},
    { 0xA6, 0x01, {0x0A}},
    { 0xA7, 0x01, {0x1D}},
    { 0xA9, 0x01, {0x1C}},
    { 0xAA, 0x01, {0x3F}},
    { 0xAB, 0x01, {0x3F}},
    { 0xAC, 0x01, {0x3F}},
    { 0xAD, 0x01, {0x3F}},
    { 0xAE, 0x01, {0x1D}},
    { 0xAF, 0x01, {0x1C}},
    { 0xB0, 0x01, {0x0C}},
    { 0xB1, 0x01, {0x0E}},
    { 0xB2, 0x01, {0x3F}},
    { 0xB3, 0x01, {0x3F}},
    { 0xB4, 0x01, {0x3F}},
    { 0xB5, 0x01, {0x3F}},
    { 0xB6, 0x01, {0x3F}},
    { 0xB7, 0x01, {0x3F}},
    { 0xB8, 0x01, {0x08}},
    { 0xB9, 0x01, {0x3F}},
    { 0xBA, 0x01, {0x3F}},
    { 0xBB, 0x01, {0x3F}},
    { 0xBC, 0x01, {0x3F}},
    { 0xBD, 0x01, {0x3F}},
    { 0xBE, 0x01, {0x3F}},
    { 0xBF, 0x01, {0x0A}},
    { 0xC0, 0x01, {0x02}},
    { 0xC1, 0x01, {0x00}},
    { 0xC2, 0x01, {0x01}},
    { 0xC3, 0x01, {0x03}},
    { 0xC4, 0x01, {0x0B}},
    { 0xC5, 0x01, {0x3F}},
    { 0xC6, 0x01, {0x3F}},
    { 0xC7, 0x01, {0x3F}},
    { 0xC8, 0x01, {0x3F}},
    { 0xC9, 0x01, {0x3F}},
    { 0xCA, 0x01, {0x3F}},
    { 0xCB, 0x01, {0x09}},
    { 0xCC, 0x01, {0x3F}},
    { 0xCD, 0x01, {0x3F}},
    { 0xCE, 0x01, {0x3F}},
    { 0xCF, 0x01, {0x3F}},
    { 0xD0, 0x01, {0x3F}},
    { 0xD1, 0x01, {0x3F}},
    { 0xD2, 0x01, {0x0F}},
    { 0xD3, 0x01, {0x0D}},
    { 0xD4, 0x01, {0x1C}},
    { 0xD5, 0x01, {0x1D}},
    { 0xD6, 0x01, {0x3F}},
    { 0xD7, 0x01, {0x3F}},
    { 0xDC, 0x01, {0x02}},
    { 0xDE, 0x01, {0x11}},
    { 0xFE, 0x01, {0x0E}},
    { 0x01, 0x01, {0x75}},
    { 0x54, 0x01, {0x01}},
    { 0xFE, 0x01, {0x04}},
    { 0x62, 0x01, {0x0F}},
    { 0x6D, 0x01, {0x13}},
    { 0x65, 0x01, {0x13}},
    { 0x6A, 0x01, {0x0E}},
    { 0x61, 0x01, {0x09}},
    { 0x63, 0x01, {0x0D}},
    { 0x64, 0x01, {0x05}},
    { 0x66, 0x01, {0x0E}},
    { 0x67, 0x01, {0x0A}},
    { 0x68, 0x01, {0x16}},
    { 0x69, 0x01, {0x0C}},
    { 0x6B, 0x01, {0x08}},
    { 0x6C, 0x01, {0x0F}},
    { 0x6E, 0x01, {0x0E}},
    { 0x60, 0x01, {0x00}},
    { 0x6F, 0x01, {0x04}},
    { 0x72, 0x01, {0x0F}},
    { 0x7D, 0x01, {0x13}},
    { 0x75, 0x01, {0x13}},
    { 0x7A, 0x01, {0x0E}},
    { 0x71, 0x01, {0x09}},
    { 0x73, 0x01, {0x0D}},
    { 0x74, 0x01, {0x05}},
    { 0x76, 0x01, {0x0E}},
    { 0x77, 0x01, {0x0A}},
    { 0x78, 0x01, {0x16}},
    { 0x79, 0x01, {0x0C}},
    { 0x7B, 0x01, {0x08}},
    { 0x7C, 0x01, {0x0F}},
    { 0x7E, 0x01, {0x0E}},
    { 0x70, 0x01, {0x00}},
    { 0x7F, 0x01, {0x04}},
    { 0xFE, 0x01, {0x00}},
    { 0x58, 0x01, {0x00}},
    { 0x11, 0x00, {0x00}},
    { REGFLAG_DELAY, 150, {0x00}},
    { 0x29, 0x00, {0x00}},
    { REGFLAG_DELAY, 100, {0x00}},
    { REGFLAG_END_OF_TABLE, 0x00, {0x00}}
    
};

static void push_table(struct LCM_setting_table *table, unsigned int count,
                       unsigned char force_update)
{
    unsigned int i;
    
    for (i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
            
            case REGFLAG_DELAY:
                MDELAY(table[i].count);
                break;
                
            case REGFLAG_END_OF_TABLE:
                break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count,
                                table[i].para_list, force_update);
        }
    }
    
}
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS * util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS * params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    
    params->type = LCM_TYPE_DSI;
    
    params->width = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    
    // enable tearing-free
    params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity = LCM_POLARITY_RISING;
    
    #if (LCM_DSI_CMD_MODE)
    params->dsi.mode = CMD_MODE;
    #else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;//SYNC_EVENT_VDO_MODE;//BURST_VDO_MODE;////
    #endif
    
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM = 3;
    params->dsi.packet_size = 256;
    params->dsi.vertical_backporch = 14;
    params->dsi.vertical_frontporch = 16;
    params->dsi.horizontal_sync_active = 8;
    params->dsi.PLL_CLOCK = 208;
    params->dsi.data_format.format = 2;
    params->dsi.PS = 2;
    params->dsi.vertical_sync_active = 2;
    params->dsi.horizontal_active_pixel = 720;
    params->dsi.vertical_active_line = 1280;
    params->dsi.data_format.color_order = 0;
    params->dsi.data_format.trans_seq = 0;
    params->dsi.data_format.padding = 0;
    params->dsi.intermediat_buffer_num = 0;;
    params->dsi.ssc_disable = 1;
    params->dsi.horizontal_backporch = 32;
    params->dsi.horizontal_frontporch = 32;
}

static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);
    
    push_table(lcm_initialization_setting,sizeof(lcm_initialization_setting) /sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
    unsigned int data_array[16];
    
    data_array[0] = 0x00280500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);
    data_array[0] = 0x00100500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);
    data_array[0] = 0x014f1500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);
    
}
static void lcm_resume(void)
{
    lcm_init();
}

LCM_DRIVER rm68200_tm50_xld_hd_lcm_drv =
{
    .name			= "rm68200_tm50_xld_hd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params = lcm_get_params,
    .init = lcm_init,
    .suspend = lcm_suspend,
    .resume = lcm_resume,
    .init_power        = lcm_init_power,
    .resume_power = lcm_resume_power,
    .suspend_power = lcm_suspend_power,
};
