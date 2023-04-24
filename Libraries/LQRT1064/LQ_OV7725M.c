/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "LQ_OV7725M.h"
#include "fsl_sccb.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_csi_camera_adapter.h"    
     
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV7725_SCCB_ADDR 0x21U
#define OV7725_REVISION 0x7721U

#define OV7725_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, OV7725_SCCB_ADDR, kSCCB_RegAddr8Bit, (reg), (val))

#define OV7725_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, OV7725_SCCB_ADDR, kSCCB_RegAddr8Bit, (reg), (val))

#define OV7725_ModifyReg(i2c, reg, clrMask, val) \
    SCCB_ModifyReg(i2c, OV7725_SCCB_ADDR, kSCCB_RegAddr8Bit, (reg), (clrMask), (val))

typedef struct _ov7725_clock_config
{
    uint32_t frameRate_Hz;
    uint32_t inputClk_Hz;
    uint8_t clkrc;  /*!< Register CLKRC. */
    uint8_t com4;   /*!< Register COM4. */
    uint8_t dm_lnl; /*!< Register DM_LNL. */
} ov7725_clock_config_t;

typedef struct _ov7725_light_mode
{
    uint8_t lightMode;
    uint8_t com8;
    uint8_t blue;
    uint8_t red;
    uint8_t com5;
} ov7725_light_mode_config_t;

typedef struct _ov7725_special_effect_config
{
    uint8_t effect;
    uint8_t sde;
    uint8_t ufix;
    uint8_t vfix;
} ov7725_special_effect_config_t;

typedef struct _ov7725_night_mode
{
    uint8_t nightMode;
    uint8_t com5;
} ov7725_night_mode_t;

typedef struct _ov7725_pixel_format_config
{
    video_pixel_format_t fmt;
    uint8_t com7;
} ov7725_pixel_format_config_t;

typedef status_t (*ov7725_cmd_func_t)(camera_device_handle_t *handle, int32_t arg);

typedef struct _ov7725_cmd_func_map
{
    camera_device_cmd_t cmd;
    ov7725_cmd_func_t func;
} ov7725_cmd_func_map_t;

typedef struct _ov7725_reg
{
    uint8_t reg;
    uint8_t val;
} ov7725_reg_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t OV7725_Init(camera_device_handle_t *handle, const camera_config_t *config);

status_t OV7725_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

status_t OV7725_Deinit(camera_device_handle_t *handle);

status_t OV7725_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);

status_t OV7725_Start(camera_device_handle_t *handle);

status_t OV7725_Stop(camera_device_handle_t *handle);

status_t OV7725_SetSpecialEffect(camera_device_handle_t *handle, int32_t effect);

status_t OV7725_SetLightMode(camera_device_handle_t *handle, int32_t lightMode);

status_t OV7725_SetNightMode(camera_device_handle_t *handle, int32_t nightMode);

status_t OV7725_SetSaturation(camera_device_handle_t *handle, int32_t saturation);

status_t OV7725_SetContrast(camera_device_handle_t *handle, int32_t contrast);

status_t OV7725_SetBrightness(camera_device_handle_t *handle, int32_t brightness);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t ov7725_ops = {
    .init = OV7725_Init,
    .deinit = OV7725_Deinit,
    .start = OV7725_Start,
    .stop = OV7725_Stop,
    .control = OV7725_Control,
    .init_ext = OV7725_InitExt,
};
/*7725 帧率 和 输出频率配置,帧率和图像大小也有关系*/
//static const ov7725_clock_config_t ov7725ClockConfigs[] = { 
//    /*QVGA 最大320 * 240 时，最大帧率150fps 可以使用更小的任意分辨率（只输出320 * 240 图像的一部分）会丢失视野    dm_lnl 添加虚拟行，可以降低帧率，从而实现输出自己想要的帧率，这里只是列举的几种分辨率，可以自行添加*/
//    {.frameRate_Hz = 150, .inputClk_Hz = 48000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x00},
//    {.frameRate_Hz = 100, .inputClk_Hz = 48000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x8a},//通过添加虚拟行 降低帧率 分辨率 300 * 200下的100帧
//    {.frameRate_Hz = 75, .inputClk_Hz = 24000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x00}, //通过降低plck 降低输出帧率
//    {.frameRate_Hz = 50, .inputClk_Hz = 24000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x45},
//    {.frameRate_Hz = 37, .inputClk_Hz = 24000000, .clkrc = 0x03, .com4 = 0x41, .dm_lnl = 0x00},
//
////    /*分辨率 640 * 480时，最大帧率60fps 可以使用更小窗口分辨率（只输出640 * 480 图像的一部分）会丢失视野*/
//    {.frameRate_Hz = 60, .inputClk_Hz = 48000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x00},
//    {.frameRate_Hz = 50, .inputClk_Hz = 48000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x20},
//    {.frameRate_Hz = 30, .inputClk_Hz = 24000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x00},
//    {.frameRate_Hz = 15, .inputClk_Hz = 24000000, .clkrc = 0x03, .com4 = 0x41, .dm_lnl = 0x00},
//    {.frameRate_Hz = 25, .inputClk_Hz = 24000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x66},
//    {.frameRate_Hz = 14, .inputClk_Hz = 24000000, .clkrc = 0x03, .com4 = 0x41, .dm_lnl = 0x1a},
//    {.frameRate_Hz = 30, .inputClk_Hz = 26000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x2b},
//    {.frameRate_Hz = 15, .inputClk_Hz = 26000000, .clkrc = 0x03, .com4 = 0x41, .dm_lnl = 0x2b},
//    {.frameRate_Hz = 25, .inputClk_Hz = 26000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x99},
//    {.frameRate_Hz = 14, .inputClk_Hz = 26000000, .clkrc = 0x03, .com4 = 0x41, .dm_lnl = 0x46},
//    {.frameRate_Hz = 30, .inputClk_Hz = 13000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x2b},
//    {.frameRate_Hz = 15, .inputClk_Hz = 13000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x2b},
//    {.frameRate_Hz = 25, .inputClk_Hz = 13000000, .clkrc = 0x00, .com4 = 0x41, .dm_lnl = 0x99},
//    {.frameRate_Hz = 14, .inputClk_Hz = 13000000, .clkrc = 0x01, .com4 = 0x41, .dm_lnl = 0x46},
//};

/*7725支持一些简单的特效0:普通模式 1.黑白 2.复古  3,偏蓝4,偏红5,偏绿 6,负片*/
static const ov7725_special_effect_config_t ov7725SpecialEffectConfigs[] = {
    /* Normal. */
    {.effect = CAMERA_SPECIAL_EFFECT_NORMAL, .sde = 0x06, .ufix = 0x80, .vfix = 0x80},
    /* B & W */
    {.effect = CAMERA_SPECIAL_EFFECT_BW, .sde = 0x26, .ufix = 0x80, .vfix = 0x80},
    /* Sepia. */
    {.effect = CAMERA_SPECIAL_EFFECT_SEPIA, .sde = 0x1e, .ufix = 0x40, .vfix = 0xa0},
    /* Bluish. */
    {.effect = CAMERA_SPECIAL_EFFECT_BLUISH, .sde = 0x1e, .ufix = 0xa0, .vfix = 0x40},
    /* Redish. */
    {.effect = CAMERA_SPECIAL_EFFECT_REDISH, .sde = 0x1e, .ufix = 0x80, .vfix = 0x40},
    /* Greenish. */
    {.effect = CAMERA_SPECIAL_EFFECT_GREENISH, .sde = 0x1e, .ufix = 0x60, .vfix = 0x60},
    /* Negtive. */
    {.effect = CAMERA_SPECIAL_EFFECT_NEGTIVE, .sde = 0x46, .ufix = 0x00, .vfix = 0x00},
};

/*白平衡设置 0:自动模式1:晴天2,多云3,办公室4,家里5,夜晚*/
static const ov7725_light_mode_config_t ov7725LightModeConfigs[] = {
    /* Auto. */
    {.lightMode = CAMERA_LIGHT_MODE_AUTO, .com8 = 0xff, .blue = 0x80, .red = 0x80, .com5 = 0x65},
    /* Sunny. */
    {.lightMode = CAMERA_LIGHT_MODE_SUNNY, .com8 = 0xfd, .blue = 0x5a, .red = 0x5c, .com5 = 0x65},
    /* Cloudy. */
    {.lightMode = CAMERA_LIGHT_MODE_CLOUDY, .com8 = 0xfd, .blue = 0x58, .red = 0x60, .com5 = 0x65},
    /* Office. */
    {.lightMode = CAMERA_LIGHT_MODE_OFFICE, .com8 = 0xfd, .blue = 0x84, .red = 0x4c, .com5 = 0x65},
    /* Home. */
    {.lightMode = CAMERA_LIGHT_MODE_HOME, .com8 = 0xfd, .blue = 0x96, .red = 0x40, .com5 = 0x65},
    /* Night. */
    {.lightMode = CAMERA_LIGHT_MODE_NIGHT, .com8 = 0xff, .blue = 0x80, .red = 0x80, .com5 = 0xe5}};

/**配置7725的输出像素格式*/
static const ov7725_pixel_format_config_t ov7725PixelFormatConfigs[] = {
    {.fmt = kVIDEO_PixelFormatYUYV, .com7 = (0)},
    {.fmt = kVIDEO_PixelFormatRGB565, .com7 = (1 << 2) | (2)},
    {.fmt = kVIDEO_PixelFormatXRGB1555, .com7 = (2 << 2) | (2)},
    {.fmt = kVIDEO_PixelFormatXRGB4444, .com7 = (3 << 2) | (2)}};

/*晚上光线不好时，帧率控制*/
static const ov7725_night_mode_t ov7725NightModeConfigs[] = {
    {.nightMode = CAMERA_NIGHT_MODE_DISABLED, .com5 = 0},   // 不使用夜晚自动帧率
    {.nightMode = CAMERA_NIGHT_MODE_AUTO_FR_DIVBY2, .com5 = 8 | (1 << 4)},  //使用夜晚自动帧率，最大帧率为原来1/2
    {.nightMode = CAMERA_NIGHT_MODE_AUTO_FR_DIVBY4, .com5 = 8 | (2 << 4)},  //使用夜晚自动帧率，最大帧率为原来1/4
    {.nightMode = CAMERA_NIGHT_MODE_AUTO_FR_DIVBY8, .com5 = 8 | (3 << 4)}}; //使用夜晚自动帧率，最大帧率为原来1/8

/*7725 CMD命令*/
static const ov7725_cmd_func_map_t ov7725CmdFuncMap[] = {
    {
        kCAMERA_DeviceLightMode, OV7725_SetLightMode,
    },
    {
        kCAMERA_DeviceSaturation, OV7725_SetSaturation,
    },
    {
        kCAMERA_DeviceBrightness, OV7725_SetBrightness,
    },
    {
        kCAMERA_DeviceContrast, OV7725_SetContrast,
    },
    {
        kCAMERA_DeviceSpecialEffect, OV7725_SetSpecialEffect,
    },
    {
        kCAMERA_DeviceNightMode, OV7725_SetNightMode,
    },
};
/*7725初始化寄存器*/
static const ov7725_reg_t ov7725InitRegs[] = {
    {0x3d, 0x03},
    {0x42, 0x7f},
    {0x4d, 0x09},

    /* DSP */
    {0x64, 0xff},
    {0x65, 0x20},
    {0x66, 0x00},
    {0x67, 0x48},
    {0x0f, 0xc5},
    {0x13, 0xff},

    /* AEC/AGC/AWB */
    {0x63, 0xe0},
    {0x14, 0x11},
    {0x22, 0x3f},
    {0x23, 0x07},
    {0x24, 0x40},
    {0x25, 0x30},
    {0x26, 0xa1},
    {0x2b, 0x00},
    {0x6b, 0xaa},
    {0x0d, 0x41},

    /* Sharpness. */
    {0x90, 0x05},
    {0x91, 0x01},
    {0x92, 0x03},
    {0x93, 0x00},

    /* Matrix. */
    {0x94, 0x90},
    {0x95, 0x8a},
    {0x96, 0x06},
    {0x97, 0x0b},
    {0x98, 0x95},
    {0x99, 0xa0},
    {0x9a, 0x1e},

    /* Brightness. */
    {0x9b, 0x08},
    /* Contrast. */
    {0x9c, 0x20},
    /* UV */
    {0x9e, 0x81},
    /* DSE */
    {0xa6, 0x04},

    /* Gamma. */
    {0x7e, 0x0c},
    {0x7f, 0x16},
    {0x80, 0x2a},
    {0x81, 0x4e},
    {0x82, 0x61},
    {0x83, 0x6f},
    {0x84, 0x7b},
    {0x85, 0x86},
    {0x86, 0x8e},
    {0x87, 0x97},
    {0x88, 0xa4},
    {0x89, 0xaf},
    {0x8a, 0xc5},
    {0x8b, 0xd7},
    {0x8c, 0xe8},
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void OV7725_DelayMs(uint32_t ms)
{
    volatile uint32_t i;
    uint32_t loopPerMs = SystemCoreClock / 3000;

    while (ms--)
    {
        i = loopPerMs;
        while (i--)
        {
        }
    }
}

static status_t OV7725_WriteRegs(sccb_i2c_t i2c, const ov7725_reg_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {
        status = OV7725_WriteReg(i2c, regs[i].reg, regs[i].val);

        if (kStatus_Success != status)
        {
            break;
        }
    }

    return status;
}
/**7725软件复位函数*/
static status_t OV7725_SoftwareReset(sccb_i2c_t i2c)
{
    return OV7725_WriteReg(i2c, OV7725_COM7_REG, 0x80);
}
/*7725 设置帧率， 时钟频率*/
static status_t OV7725_SetClockConfig(sccb_i2c_t i2c, uint32_t fps)
{
   
    if(fps > 75)
    {
        /* 锁相环8倍频 满窗口
        Bit[7:6]: 锁相环倍频器控制
        00: 不用倍频
        01: 4倍频
        10: 6倍频
        11: 8倍频
        Bit[5:4]: 自动曝光窗口大小
        00: Full window
        01: 1/2 window
        10: 1/4 window
        11: Low 2/3 window*/
        OV7725_WriteReg(i2c,OV7725_COM4_REG, 0xC1);  //PCLK 48M   
    }
    else
    {
        OV7725_WriteReg(i2c,OV7725_COM4_REG, 0x41);  //PCLK 24M   
    }
    uint16_t dm_lnl = 0;                                               //虚拟行
    uint8_t  div    = 1;                                               //时钟分频系数
    if(fps > 150)
    {
        fps = 150;
        div = 1;
        dm_lnl = 0;     
    }
    else if(fps > 75)
    {
        div = 1;
        dm_lnl = (uint16_t)((150 - fps) / 150.0 * (340 + 36*(div + 1)));
    }
    else if(fps > 50)
    {
        div = 1;
        dm_lnl = (uint16_t)((75 - fps) / 75.0 * (240 + 36*(div + 1)));
    }
    else if(fps > 37)
    {
        div = 2;
        dm_lnl = (uint16_t)((50 - fps) / 50.0 * (240 + 36*(div + 1)));
    }
    else if(fps > 18)
    {
        div = 3;
        dm_lnl = (uint16_t)((37.5 - fps) / 37.5 * (240 + 36*(div + 1)));
    }
    else 
    {
        div = 14;
        dm_lnl = 0;
    }
    /*时钟配置 时钟 = PCLK * 4 / （（1+1）*4） 时钟越高，帧率越快 不过DMA可能接受不了会有噪点 
    Bit[6]: 直接使用外部时钟(没有时钟预刻度可用)
    Bit[5:0]: 时钟 = PCLK × 锁相环倍频器 /[(CLKRC[5:0] + 1) × 4]*/
    OV7725_WriteReg(i2c, OV7725_CLKRC_REG, div);
    
    /*虚拟行低八位， 增加虚拟行可以降低帧率，适当添加把帧率配置到自己想要的帧率*/
    OV7725_WriteReg(i2c, OV7725_DM_LNL_REG, (uint8_t)dm_lnl);  
    
    /*虚拟行高八位*/
    OV7725_WriteReg(i2c, OV7725_DM_LNH_REG, (uint8_t)(dm_lnl>>8)); 
    
    OV7725_WriteReg(i2c, OV7725_EXHCL_REG, 0x00); //虚拟像素插入LSB用于在水平方向插入虚拟像素的LSB
    OV7725_WriteReg(i2c, OV7725_ADVFL_REG, 0x00); //垂直同步插入虚拟行(1位等于1行)的LSB
    return OV7725_WriteReg(i2c, OV7725_ADVFH_REG, 0x00); //垂直同步插入虚拟行的MSB 
    
}

/*设置像素输出格式*/
static status_t OV7725_SetPixelFormat(sccb_i2c_t i2c, video_pixel_format_t fmt)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(ov7725PixelFormatConfigs); i++)
    {
        if (ov7725PixelFormatConfigs[i].fmt == fmt)
        {
            return OV7725_ModifyReg(i2c, OV7725_COM7_REG, 0x1FU, ov7725PixelFormatConfigs[i].com7);
        }
    }

    return kStatus_InvalidArgument;
}

/*初始化*/
status_t OV7725_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    
    status_t status;
    uint8_t pid, ver;
    uint8_t com10 = 0, com7 = 0;
    uint16_t width, height;
    uint16_t hstart, vstart, hsize;
//    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = LPI2C1;
    /*校验摄像头接口*/
    if ((kCAMERA_InterfaceNonGatedClock != config->interface) && (kCAMERA_InterfaceGatedClock != config->interface) &&
        (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }
    /*摄像头输出图片高度和宽度*/
    width = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);

    /*7725最大分辨率 640 * 480*/
    if ((width > 640) || (height > 480))
    {
        return kStatus_InvalidArgument;
    }

//    resource->pullPowerDownPin(true);

    /* Delay 1ms. */
    OV7725_DelayMs(1);

//    resource->pullPowerDownPin(false);

    /* Delay 1ms. */
    OV7725_DelayMs(1);

//    resource->pullResetPin(false);

    /* Delay 1ms. */
    OV7725_DelayMs(1);

//    resource->pullResetPin(true);

    /* Delay 1ms. */
    OV7725_DelayMs(1);

    /* 读取7725 id. */
    status = OV7725_ReadReg(i2c, OV7725_PID_REG, &pid);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = OV7725_ReadReg(i2c, OV7725_VER_REG, &ver);
    if (kStatus_Success != status)
    {
        return status;
    }
    /*判断是否是7725*/
    if (OV7725_REVISION != (((uint32_t)pid << 8U) | (uint32_t)ver))
    {
        while(1); /* 初始化失败 检查摄像头接线 */
    }

    /* Device identify OK, perform software reset. */
    OV7725_SoftwareReset(i2c);

    /* Delay 2ms. */
    OV7725_DelayMs(2);

    /* 初始化7725寄存器  自动曝光 普通模式 */
    status = OV7725_WriteRegs(i2c, ov7725InitRegs, ARRAY_SIZE(ov7725InitRegs));
    if (kStatus_Success != status)
    {
        return status;
    }
    /* 配置帧率 和 时钟. */
    status = OV7725_SetClockConfig(i2c, config->framePerSec);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* 配置输出图像格式. */
    status = OV7725_SetPixelFormat(i2c, config->pixelFormat);
    if (kStatus_Success != status)
    {
        return status;
    }
    /* 配置接口类型 */
    if (kCAMERA_InterfaceCCIR656 == config->interface)
    {
        OV7725_ModifyReg(i2c, OV7725_COM7_REG, (1 << 5), (1 << 5));
        width += 2;
    }
    else
    {
        OV7725_ModifyReg(i2c, OV7725_COM7_REG, (1 << 5), (0 << 5));
    }

    /*行中断 上升沿/下降沿*/
    if (kCAMERA_HrefActiveHigh != (config->controlFlags & kCAMERA_HrefActiveHigh))
    {
        com10 |= OV7725_COM10_HREF_REVERSE_MASK;
    }
    /*场中断 上升沿/下降沿*/
    if (kCAMERA_VsyncActiveHigh != (config->controlFlags & kCAMERA_VsyncActiveHigh))
    {
        com10 |= OV7725_COM10_VSYNC_NEG_MASK;
    }
    /*plck 上升沿/下降沿*/
    if (kCAMERA_DataLatchOnRisingEdge != (config->controlFlags & kCAMERA_DataLatchOnRisingEdge))
    {
        com10 |= OV7725_COM10_PCLK_REVERSE_MASK;
    }
    /*VSYNC, and PIXCLK signals are used */
    if (kCAMERA_InterfaceNonGatedClock == config->interface)
    {
        com10 |= OV7725_COM10_PCLK_OUT_MASK;
    }

    OV7725_WriteReg(i2c, OV7725_COM10_REG, com10);

    /* Don't swap output MSB/LSB. */
    OV7725_WriteReg(i2c, OV7725_COM3_REG, 0x00);  //Bit[6]: 0水平镜像关 1水平镜像开  注意开启水平镜像时最好开启0x32寄存器的Bit[7]:镜像图像边缘对齐-在镜像模式下应该设置为1
                                                  //Bit[4]: 0 UYVY模式  1 YUYV模式  
                                                  //Bit[3]: 0 小端      1 大端
    /*
     * Output drive capability
     * 0: 1X
     * 1: 2X
     * 2: 3X
     * 3: 4X
     */
    OV7725_ModifyReg(i2c, OV7725_COM2_REG, 0x03, 0x03);

    
    OV7725_ReadReg(i2c, OV7725_COM7_REG, &com7);
    if(height > 240)    //  使用 VGA 分辨率  640 * 480
    {
//        OV7725_WriteReg(i2c, OV7725_COM7_REG, com7 & ~(1 << 6)); //初始化时默认VGA，这里不用修改
        hstart = 0x22U << 2U;   //图像水平开始位置
        vstart = 0x07U << 1U;   //图像垂直开始位置
    }
    else               //  使用 QVGA 分辨率  320 * 240
    {
        OV7725_WriteReg(i2c, OV7725_COM7_REG, com7 | (1 << 6));
        hstart = 0x3fU << 2U;   //图像水平开始位置
        vstart = 0x03U << 1U;   //图像垂直开始位置
    }
    
    /* Resolution and timing. */
    hsize = width + 16U;

    /* 设置输出图片大小. */
    OV7725_WriteReg(i2c, OV7725_HSTART_REG, hstart >> 2U);  //水平起始位置高位  因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了10位数据，这里是高8位，剩下的两位放在OV7725_HREF_REG中
    OV7725_WriteReg(i2c, OV7725_HSIZE_REG, hsize >> 2U);    //水平宽度高位      因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了10位数据，这里是高8位，剩下的两位放在OV7725_HREF_REG中
    OV7725_WriteReg(i2c, OV7725_VSTART_REG, vstart >> 1U);  //垂直起始位置高位  因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了9位数据，这里是高8位，剩下的两位放在OV7725_HREF_REG中
    OV7725_WriteReg(i2c, OV7725_VSIZE_REG, height >> 1U);   //垂直高度高位      因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了9位数据，这里是高8位，剩下的两位放在OV7725_HREF_REG中
    OV7725_WriteReg(i2c, OV7725_HOUTSIZE_REG, width >> 2U); //水平输出宽度高位  因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了10位数据，这里是高8位，剩下的两位放在OV7725_EXHCH_REG中
    OV7725_WriteReg(i2c, OV7725_VOUTSIZE_REG, height >> 1U);//垂直输出高度高位  因为寄存器是8位的 最大255，像素最大640 * 480 放不下，用了9位数据，这里是高8位，剩下的两位放在OV7725_EXHCH_REG中
    OV7725_WriteReg(i2c, OV7725_HREF_REG,
                    ((vstart & 1U) << 6U) | ((hstart & 3U) << 4U) | ((height & 1U) << 2U) | ((hsize & 3U) << 0U)); //水平宽度和垂直高度的低位
    return OV7725_WriteReg(i2c, OV7725_EXHCH_REG, ((height & 1U) << 2U) | ((width & 3U) << 0U));  //水平输出和垂直输出的低2位和低1位
    
}

status_t OV7725_SetSpecialEffect(camera_device_handle_t *handle, int32_t effect)
{
    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(ov7725SpecialEffectConfigs); i++)
    {
        if (effect == ov7725SpecialEffectConfigs[i].effect)
        {
            OV7725_WriteReg(i2c, OV7725_SDE_REG, ov7725SpecialEffectConfigs[i].sde);
            OV7725_WriteReg(i2c, OV7725_UFIX_REG, ov7725SpecialEffectConfigs[i].ufix);
            OV7725_WriteReg(i2c, OV7725_VFIX_REG, ov7725SpecialEffectConfigs[i].vfix);

            return OV7725_ModifyReg(i2c, OV7725_DSP_CTRL1_REG, 1 << 5, 1 << 5);
        }
    }

    /* No configuration found. */
    return kStatus_InvalidArgument;
}

status_t OV7725_SetContrast(camera_device_handle_t *handle, int32_t contrast)
{
    if ((contrast < -4) || (contrast > 4))
    {
        return kStatus_InvalidArgument;
    }

    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    return OV7725_WriteReg(i2c, OV7725_CNST_REG, 0x20 + (0x04 * contrast));
}

status_t OV7725_SetBrightness(camera_device_handle_t *handle, int32_t brightness)
{
    if ((brightness < -4) || (brightness > 4))
    {
        return kStatus_InvalidArgument;
    }

    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    if (brightness >= 0)
    {
        OV7725_WriteReg(i2c, OV7725_BRIGHT_REG, 0x08 + (0x10 * brightness));
        return OV7725_WriteReg(i2c, OV7725_SIGN_REG, 0x06);
    }
    else
    {
        brightness = -brightness - 1;
        OV7725_WriteReg(i2c, OV7725_BRIGHT_REG, 0x08 + (0x10 * brightness));
        return OV7725_WriteReg(i2c, OV7725_SIGN_REG, 0x0e);
    }
}

status_t OV7725_SetSaturation(camera_device_handle_t *handle, int32_t saturation)
{
    if ((saturation < -4) || (saturation > 4))
    {
        return kStatus_InvalidArgument;
    }

    uint8_t val = (saturation + 4) * 0x10;
    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    OV7725_WriteReg(i2c, OV7725_USAT_REG, val);
    return OV7725_WriteReg(i2c, OV7725_VSAT_REG, val);
}

status_t OV7725_SetLightMode(camera_device_handle_t *handle, int32_t lightMode)
{
    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(ov7725LightModeConfigs); i++)
    {
        if (lightMode == ov7725LightModeConfigs[i].lightMode)
        {
            OV7725_WriteReg(i2c, OV7725_COM8_REG, ov7725LightModeConfigs[i].com8);
            OV7725_WriteReg(i2c, OV7725_BLUE_REG, ov7725LightModeConfigs[i].blue);
            OV7725_WriteReg(i2c, OV7725_RED_REG, ov7725LightModeConfigs[i].red);
            OV7725_WriteReg(i2c, OV7725_COM5_REG, ov7725LightModeConfigs[i].com5);
            OV7725_WriteReg(i2c, OV7725_ADVFL_REG, 0);
            return OV7725_WriteReg(i2c, OV7725_ADVFH_REG, 0);
        }
    }

    /* No configuration found. */
    return kStatus_InvalidArgument;
}

status_t OV7725_SetNightMode(camera_device_handle_t *handle, int32_t nightMode)
{
    ov7725_resource_t *resource = (ov7725_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(ov7725NightModeConfigs); i++)
    {
        if (nightMode == ov7725NightModeConfigs[i].nightMode)
        {
            return OV7725_ModifyReg(i2c, OV7725_COM5_REG, 0xF0, ov7725NightModeConfigs[i].com5);
        }
    }

    /* No configuration found. */
    return kStatus_InvalidArgument;
}

status_t OV7725_Deinit(camera_device_handle_t *handle)
{
    ((ov7725_resource_t *)(handle->resource))->pullPowerDownPin(true);

    return kStatus_Success;
}

status_t OV7725_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(ov7725CmdFuncMap); i++)
    {
        if (ov7725CmdFuncMap[i].cmd == cmd)
        {
            return ov7725CmdFuncMap[i].func(handle, arg);
        }
    }

    return kStatus_InvalidArgument;
}

status_t OV7725_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV7725_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV7725_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return OV7725_Init(handle, config);
}
