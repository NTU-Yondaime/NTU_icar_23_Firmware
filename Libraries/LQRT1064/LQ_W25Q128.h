#ifndef __LQ_W25Q128_H
#define __LQ_W25Q128_H


/* FLASH 信息 */
#define FLASH_SIZE 0x4000   /* 16Mb/KByte */
#define FLASH_PAGE_SIZE 256 /* 一页256字节 */
#define SECTOR_SIZE 0x1000  /* 4K */

/* FLASH 命令 */
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL 0
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST 1
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD 2
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS 3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 5
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD 7
#define NOR_CMD_LUT_SEQ_IDX_READID 8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG 9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI 10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI 11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG 12
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP 13

#define CUSTOM_LUT_LENGTH 60
#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0


extern flexspi_device_config_t deviceconfig;

extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];


/* flash 底层驱动 */
status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);
status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);
status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);



#endif
