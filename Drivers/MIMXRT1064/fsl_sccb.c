/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
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
#include "include.h"
#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_lpi2c.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t SCCB_WriteReg(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t value)
{
    uint8_t data[3];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (reg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)(reg);
    data[size++] = value;


    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    return LPI2C_MasterStop(i2c);
}

status_t SCCB_WriteMultiRegs(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t startReg, const uint8_t *value, uint32_t len)
{
    uint8_t data[2];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (startReg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)(startReg);

    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    LPI2C_MasterSend(i2c, value, len);

    return LPI2C_MasterStop(i2c);
}

status_t SCCB_ReadReg(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t *value)
{
    status_t status;
    uint8_t data[2];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (reg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)reg;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    LPI2C_MasterStop(i2c);

    LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Read);

    LPI2C_MasterReceive(i2c, value, 1);

    return LPI2C_MasterStop(i2c);
}
status_t SCCB_ReadMultiRegs(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t startReg, const uint8_t *value, uint32_t len)
{
    uint8_t data[2];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (startReg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)(startReg);

    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    LPI2C_MasterStop(i2c);

    LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Read);

    LPI2C_MasterReceive(i2c,(void*) value, len);    
    
    return LPI2C_MasterStop(i2c);
}
status_t SCCB_ModifyReg(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t clrMask, uint8_t value)
{
    status_t status;

    uint8_t regVal;

    status = SCCB_ReadReg(i2c, i2cAddr, addrType, reg, &regVal);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~((uint32_t)clrMask)) | (value & clrMask);

    return SCCB_WriteReg(i2c, i2cAddr, addrType, reg, regVal);
}
