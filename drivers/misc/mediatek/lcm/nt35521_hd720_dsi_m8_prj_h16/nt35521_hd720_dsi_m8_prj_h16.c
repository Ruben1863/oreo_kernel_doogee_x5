// ---------------------------------------------------------------------------
//  Reversed by zoggn for HCT16
// ---------------------------------------------------------------------------

#ifdef BUILD_LK
#else
#include <linux/string.h>
#endif

#include "lcm_drv.h"
#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mt-plat/mt_gpio.h>
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH                                          (720)
#define FRAME_HEIGHT                                         (1280)

#define REGFLAG_DELAY                                         0XAA
#define REGFLAG_END_OF_TABLE                                  0xAB   // END OF REGISTERS MARKER

#define LCM_ID_NT35521                                      0x5521
#define LCM_DSI_CMD_MODE                                    0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                            lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
    { 0xFF, 0x04, {0xAA, 0x55, 0x25, 0x01}},
    { 0x6F, 0x01, {0x03}},
    { 0xF4, 0x01, {0x60}},
    { 0x6F, 0x01, {0x06}},
    { 0xF4, 0x01, {0x01}},
    { 0x6F, 0x01, {0x21}},
    { 0xF7, 0x01, {0x01}},
    { 0xAA, 0x01, {0x00}},
    { 0x6F, 0x01, {0x21}},
    { 0xF7, 0x01, {0x00}},
    { 0xFC, 0x01, {0x08}},
    { 0xAA, 0x01, {0x00}},
    { 0xFC, 0x01, {0x00}},
    { 0x6F, 0x01, {0x16}},
    { 0xF7, 0x01, {0x10}},
    { 0xFF, 0x04, {0xAA, 0x55, 0x25}},
    { 0xFF, 0x04, {0xAA, 0x55, 0xA5, 0x80}},
    { 0x6F, 0x02, {0x11}},
    { 0xF7, 0x02, {0x20}},
    { 0x6F, 0x01, {0x06}},
    { 0xF7, 0x01, {0xA0}},
    { 0x6F, 0x01, {0x19}},
    { 0xF7, 0x01, {0x12}},
    { 0x6F, 0x01, {0x02}},
    { 0xF7, 0x01, {0x47}},
    { 0x6F, 0x01, {0x17}},
    { 0xF4, 0x01, {0x70}},
    { 0x6F, 0x01, {0x01}},
    { 0xF9, 0x01, {0x46}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08}},
    { 0xBD, 0x05, {0x01, 0xA0, 0x10, 0x10, 0x01}},
    { 0xB8, 0x04, {0x00}},
    { 0xBB, 0x02, {0x24, 0x24}},
    { 0xBC, 0x02, {0x00}},
    { 0xB6, 0x01, {0x04}},
    { 0xD9, 0x02, {0x01, 0x01}},
    { 0xD4, 0x01, {0xC7}},
    { 0xB1, 0x02, {0x60, 0x21}},
    { 0xFF, 0x04, {0xAA, 0x55, 0x25, 0x01}},
    { 0xFC, 0x01, {0x08}},
    { 0xAA, 0x01, {0x00}},
    { 0xFC, 0x01, {0x00}},
    { 0xAA, 0x14, {0x00}},
    { 0x6F, 0x01, {0x21}},
    { 0xF7, 0x01, {0x01}},
    { 0xAA, 0x01, {0x00}},
    { 0x6F, 0x01, {0x21}},
    { 0xF7, 0x01, {0x00}},
    { 0xFF, 0x04, {0xAA, 0x55, 0x25}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x01}},
    { 0xB0, 0x02, {0x09, 0x09}},
    { 0xB1, 0x02, {0x09, 0x09}},
    { 0xBC, 0x02, {0xA0}},
    { 0xBD, 0x02, {0xA0}},
    { 0xBE, 0x01, {0x4A}},
    { 0xCA, 0x01, {0x00}},
    { 0xC0, 0x01, {0x0C}},
    { 0xB5, 0x02, {0x03, 0x03}},
    { 0xB3, 0x02, {0x19, 0x19}},
    { 0xB4, 0x02, {0x19, 0x19}},
    { 0xB9, 0x02, {0x26, 0x26}},
    { 0xBA, 0x02, {0x25, 0x25}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x02}},
    { 0xB0, 0x10, {0x00, 0x0E, 0x00, 0x69, 0x00, 0x96, 0x00, 0xB5, 0x00, 0xD0, 0x00, 0xF4, 0x01, 0x11, 0x01, 0x3F}},
    { 0xB1, 0x10, {0x01, 0x62, 0x01, 0x9C, 0x01, 0xC9, 0x02, 0x0E, 0x02, 0x44, 0x02, 0x48, 0x02, 0x74, 0x02, 0xA9}},
    { 0xB2, 0x10, {0x02, 0xCB, 0x02, 0xF6, 0x03, 0x16, 0x03, 0x3F, 0x03, 0x5C, 0x03, 0x73, 0x03, 0x9B, 0x03, 0x9F}},
    { 0xB3, 0x04, {0x03, 0xD7, 0x03, 0xE8}},
    { 0xB4, 0x10, {0x00, 0x05, 0x00, 0x50, 0x00, 0x8D, 0x00, 0xAD, 0x00, 0xC4, 0x00, 0xEB, 0x01, 0x09, 0x01, 0x39}},
    { 0xB5, 0x10, {0x01, 0x5E, 0x01, 0x97, 0x01, 0xC4, 0x02, 0x08, 0x02, 0x3D, 0x02, 0x3E, 0x02, 0x70, 0x02, 0xA4}},
    { 0xB6, 0x10, {0x02, 0xC5, 0x02, 0xF2, 0x03, 0x11, 0x03, 0x3B, 0x03, 0x58, 0x03, 0x6C, 0x03, 0x96, 0x03, 0xCA}},
    { 0xB7, 0x04, {0x03, 0xF5, 0x03, 0xF8}},
    { 0xB8, 0x10, {0x00, 0x14, 0x00, 0x3B, 0x00, 0x6F, 0x00, 0x8E, 0x00, 0xA9, 0x00, 0xD1, 0x00, 0xF1, 0x01, 0x24}},
    { 0xB9, 0x10, {0x01, 0x4C, 0x01, 0x8A, 0x01, 0xB9, 0x02, 0x03, 0x02, 0x3A, 0x02, 0x3B, 0x02, 0x6E, 0x02, 0xA4}},
    { 0xBA, 0x10, {0x02, 0xC5, 0x02, 0xF4, 0x03, 0x16, 0x03, 0x4D, 0x03, 0x81, 0x03, 0xF9, 0x03, 0xFA, 0x03, 0xFB}},
    { 0xBB, 0x04, {0x03, 0xFD, 0x03, 0xFE}},
    { 0xBC, 0x10, {0x00, 0x0E, 0x00, 0x69, 0x00, 0x96, 0x00, 0xB5, 0x00, 0xD0, 0x00, 0xF4, 0x01, 0x11, 0x01, 0x3F}},
    { 0xBD, 0x10, {0x01, 0x62, 0x01, 0x9C, 0x01, 0xC9, 0x02, 0x0E, 0x02, 0x44, 0x02, 0x48, 0x02, 0x74, 0x02, 0xA9}},
    { 0xBE, 0x10, {0x02, 0xCB, 0x02, 0xF6, 0x03, 0x16, 0x03, 0x3F, 0x03, 0x5C, 0x03, 0x73, 0x03, 0x9B, 0x03, 0x9F}},
    { 0xBF, 0x04, {0x03, 0xD7, 0x03, 0xE8}},
    { 0xC0, 0x10, {0x00, 0x05, 0x00, 0x50, 0x00, 0x8D, 0x00, 0xAD, 0x00, 0xC4, 0x00, 0xEB, 0x01, 0x09, 0x01, 0x39}},
    { 0xC1, 0x10, {0x01, 0x5E, 0x01, 0x97, 0x01, 0xC4, 0x02, 0x08, 0x02, 0x3D, 0x02, 0x3E, 0x02, 0x70, 0x02, 0xA4}},
    { 0xC2, 0x10, {0x02, 0xC5, 0x02, 0xF2, 0x03, 0x11, 0x03, 0x3B, 0x03, 0x58, 0x03, 0x6C, 0x03, 0x96, 0x03, 0xCA}},
    { 0xC3, 0x04, {0x03, 0xF5, 0x03, 0xF8}},
    { 0xC4, 0x10, {0x00, 0x14, 0x00, 0x3B, 0x00, 0x6F, 0x00, 0x8E, 0x00, 0xA9, 0x00, 0xD1, 0x00, 0xF1, 0x01, 0x24}},
    { 0xC5, 0x10, {0x01, 0x4C, 0x01, 0x8A, 0x01, 0xB9, 0x02, 0x03, 0x02, 0x3A, 0x02, 0x3B, 0x02, 0x6E, 0x02, 0xA4}},
    { 0xC6, 0x10, {0x02, 0xC5, 0x02, 0xF4, 0x03, 0x16, 0x03, 0x4D, 0x03, 0x81, 0x03, 0xF9, 0x03, 0xFA, 0x03, 0xFB}},
    { 0xC7, 0x04, {0x03, 0xFD, 0x03, 0xFE}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x06}},
    { 0xB0, 0x02, {0x31, 0x2E}},
    { 0xB1, 0x02, {0x10, 0x12}},
    { 0xB2, 0x02, {0x16, 0x18}},
    { 0xB3, 0x02, {0x31, 0x31}},
    { 0xB4, 0x02, {0x31, 0x34}},
    { 0xB5, 0x02, {0x34, 0x34}},
    { 0xB6, 0x02, {0x34, 0x34}},
    { 0xB7, 0x02, {0x34, 0x34}},
    { 0xB8, 0x02, {0x33, 0x2D}},
    { 0xB9, 0x02, {0x00, 0x02}},
    { 0xBA, 0x02, {0x03, 0x01}},
    { 0xBB, 0x02, {0x2D, 0x33}},
    { 0xBC, 0x02, {0x34, 0x34}},
    { 0xBD, 0x02, {0x34, 0x34}},
    { 0xBE, 0x02, {0x34, 0x34}},
    { 0xBF, 0x02, {0x34, 0x31}},
    { 0xC0, 0x02, {0x31, 0x31}},
    { 0xC1, 0x02, {0x19, 0x17}},
    { 0xC2, 0x02, {0x13, 0x11}},
    { 0xC3, 0x02, {0x2E, 0x31}},
    { 0xE5, 0x02, {0x31, 0x31}},
    { 0xC4, 0x02, {0x31, 0x2D}},
    { 0xC5, 0x02, {0x19, 0x17}},
    { 0xC6, 0x02, {0x13, 0x11}},
    { 0xC7, 0x02, {0x31, 0x31}},
    { 0xC8, 0x02, {0x31, 0x34}},
    { 0xC9, 0x02, {0x34, 0x34}},
    { 0xCA, 0x02, {0x34, 0x34}},
    { 0xCB, 0x02, {0x34, 0x34}},
    { 0xCC, 0x02, {0x33, 0x2E}},
    { 0xCD, 0x02, {0x03, 0x01}},
    { 0xCE, 0x02, {0x00, 0x02}},
    { 0xCF, 0x02, {0x2E, 0x33}},
    { 0xD0, 0x02, {0x34, 0x34}},
    { 0xD1, 0x02, {0x34, 0x34}},
    { 0xD2, 0x02, {0x34, 0x34}},
    { 0xD3, 0x02, {0x34, 0x31}},
    { 0xD4, 0x02, {0x31, 0x31}},
    { 0xD5, 0x02, {0x10, 0x12}},
    { 0xD6, 0x02, {0x16, 0x18}},
    { 0xD7, 0x02, {0x2D, 0x31}},
    { 0xE6, 0x02, {0x31, 0x31}},
    { 0xD8, 0x05, {0x00}},
    { 0xD9, 0x05, {0x00}},
    { 0xE7, 0x01, {0x00}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x05}},
    { 0xED, 0x01, {0x30}},
    { 0xB0, 0x02, {0x17, 0x06}},
    { 0xB8, 0x01, {0x00}},
    { 0xC0, 0x01, {0x0D}},
    { 0xC1, 0x01, {0x0B}},
    { 0xC2, 0x01, {0x00}},
    { 0xC3, 0x01, {0x00}},
    { 0xC4, 0x01, {0x84}},
    { 0xC5, 0x01, {0x82}},
    { 0xC6, 0x01, {0x82}},
    { 0xC7, 0x01, {0x80}},
    { 0xC8, 0x02, {0x0B, 0x20}},
    { 0xC9, 0x02, {0x07, 0x20}},
    { 0xCA, 0x02, {0x01, 0x10}},
    { 0xD1, 0x05, {0x03, 0x05, 0x05, 0x07}},
    { 0xD2, 0x05, {0x03, 0x05, 0x09, 0x03}},
    { 0xD3, 0x05, {0x00, 0x00, 0x6A, 0x07, 0x10}},
    { 0xD4, 0x05, {0x30, 0x00, 0x6A, 0x07, 0x10}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x03}},
    { 0xB0, 0x02, {0x00}},
    { 0xB1, 0x02, {0x00}},
    { 0xB2, 0x05, {0x05, 0x01, 0x13}},
    { 0xB3, 0x05, {0x05, 0x01, 0x13}},
    { 0xB4, 0x05, {0x05, 0x01, 0x13}},
    { 0xB5, 0x05, {0x05, 0x01, 0x13}},
    { 0xB6, 0x05, {0x02, 0x01, 0x13}},
    { 0xB7, 0x05, {0x02, 0x01, 0x13}},
    { 0xB8, 0x05, {0x02, 0x01, 0x13}},
    { 0xB9, 0x05, {0x02, 0x01, 0x13}},
    { 0xBA, 0x05, {0x53, 0x01, 0x13}},
    { 0xBB, 0x05, {0x53, 0x01, 0x13}},
    { 0xBC, 0x05, {0x53, 0x01, 0x13}},
    { 0xBD, 0x05, {0x53, 0x01, 0x13}},
    { 0xC4, 0x01, {0x60}},
    { 0xC5, 0x01, {0x40}},
    { 0xC6, 0x01, {0x64}},
    { 0xC7, 0x01, {0x44}},
    { 0x6F, 0x01, {0x11}},
    { 0xF3, 0x01, {0x01}},
    { 0x35, 0x01, {0x00}},
    { 0xFF, 0x04, {0xAA, 0x55, 0x25, 0x01}},
    { 0x6F, 0x01, {0x1C}},
    { 0xFA, 0x01, {0x01}},
    { 0xFF, 0x04, {0xAA, 0x55, 0x25}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x04}},
    { 0xEA, 0x04, {0x00, 0x00, 0x3F, 0x7F}},
    { 0x6F, 0x01, {0x01}},
    { 0x11, 0x01, {0x00}},
    { REGFLAG_DELAY, 150, {0x00}},
    { 0x29, 0x01, {0x00}},
    { REGFLAG_DELAY, 50, {0x00}},
    { REGFLAG_END_OF_TABLE, 0x00, {0x00}},
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;
    
    for(i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
            
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
                
            case REGFLAG_END_OF_TABLE :
                break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
        }
    }
    
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    params->dsi.LANE_NUM = 3;
    params->dsi.packet_size = 256;
    params->dsi.horizontal_sync_active = 10;
    params->dsi.PLL_CLOCK = 280;
    params->type = 2;
    params->dsi.mode = 2;
    params->dsi.data_format.format = 2;
    params->dsi.PS = 2;
    params->dsi.vertical_sync_active = 2;
    params->width = 720;
    params->dsi.horizontal_active_pixel = 720;
    params->height = 1280;
    params->dsi.vertical_active_line = 1280;
    params->dbi.te_mode = 0;
    params->dbi.te_edge_polarity = 0;
    params->dsi.data_format.color_order = 0;
    params->dsi.data_format.trans_seq = 0;
    params->dsi.data_format.padding = 0;
    params->dsi.intermediat_buffer_num = 0;
    params->dsi.vertical_backporch = 20;
    params->dsi.vertical_frontporch = 20;
    params->dsi.horizontal_backporch = 80;
    params->dsi.horizontal_frontporch = 80;
    
}

static void lcm_init(void)
{   
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(50);
    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
    unsigned int data_array[16];
    data_array[0]=0x00280500;
    dsi_set_cmdq(data_array,1,1);
    MDELAY(10);
    data_array[0]=0x00100500;
    dsi_set_cmdq(data_array,1,1);
    MDELAY(100);
}


static void lcm_resume(void)
{
    return lcm_init();
}


static unsigned int lcm_compare_id(void)
{
    return 1;
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER nt35521_hd720_dsi_m8_prj_h16_lcm_drv = 
{
    .name			= "nt35521_hd720_dsi_m8_prj_h16",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id
};

