#ifndef _zf_common_fifo_h_
#define _zf_common_fifo_h_
#define NULL 0  
typedef unsigned char       uint8;                                              // ???  8 bits
typedef unsigned short int  uint16;                                             // ??? 16 bits
typedef unsigned int        uint32;                                             // ??? 32 bits
typedef unsigned long long  uint64;                                             // ??? 64 bits

typedef signed char         int8;                                               // ???  8 bits
typedef signed short int    int16;                                              // ??? 16 bits
typedef signed int          int32;                                              // ??? 32 bits
typedef signed long long    int64;                                              // ??? 64 bits

typedef enum
{
    FIFO_SUCCESS,

    FIFO_WRITE_UNDO,
    FIFO_CLEAR_UNDO,
    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_IDLE       = 0x00,
    FIFO_CLEAR      = 0x01,
    FIFO_WRITE      = 0x02,
    FIFO_READ       = 0x04,
}fifo_execution_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

typedef enum
{
    FIFO_DATA_8BIT,
    FIFO_DATA_16BIT,
    FIFO_DATA_32BIT,
}fifo_data_type_enum;

typedef struct
{
    uint8               execution;                                              
    fifo_data_type_enum type;                                                   
    void               *buffer;                                                 
    uint32              head;                                                   
    uint32              end;                                                    
    uint32              size;                                                   
    uint32              max;                                                    
}fifo_struct;

fifo_state_enum fifo_clear              (fifo_struct *fifo);
uint32          fifo_used               (fifo_struct *fifo);

fifo_state_enum fifo_write_element      (fifo_struct *fifo, uint32 dat);
fifo_state_enum fifo_write_buffer       (fifo_struct *fifo, void *dat, uint32 length);
fifo_state_enum fifo_read_element       (fifo_struct *fifo, void *dat, fifo_operation_enum flag);
fifo_state_enum fifo_read_buffer        (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag);
fifo_state_enum fifo_read_tail_buffer   (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag);

fifo_state_enum fifo_init               (fifo_struct *fifo, fifo_data_type_enum type, void *buffer_addr, uint32 size);

#endif
