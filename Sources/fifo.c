#include "fifo.h"

//-------------------------------------------------------------------------------------------------------------------
// ????     FIFO ?????
// ????     *fifo               FIFO ????
// ????     offset              ???
// ????     void
// ????     fifo_head_offset(fifo, 1);
// ????     ?????????? ?????? ?????
//-------------------------------------------------------------------------------------------------------------------
static void fifo_head_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->head += offset;
    
    while(fifo->max <= fifo->head)                                              // ????????????? ???????????
    {
        fifo->head -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// ????     FIFO ?????
// ????     *fifo               FIFO ????
// ????     offset              ???
// ????     void
// ????     fifo_end_offset(fifo, 1);
// ????     ?????????? ?????? ?????
//-------------------------------------------------------------------------------------------------------------------
static void fifo_end_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->end += offset;
    
    while(fifo->max <= fifo->end)                                               // ????????????? ???????????
    {
        fifo->end -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// ????     FIFO ?????
// ????     *fifo               FIFO ????
// ????     void
// ????     fifo_clear(fifo);
// ????     ???? FIFO ?????
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_clear (fifo_struct *fifo)
{
    //assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;
    do
    {
        if(FIFO_CLEAR & fifo->execution)
        {
            return_state = FIFO_CLEAR_UNDO;
            break;
        }
        fifo->execution |= FIFO_CLEAR;
        fifo->head      = 0;
        fifo->end       = 0;
        fifo->size      = fifo->max;
        switch(fifo->type)
        {
            case FIFO_DATA_8BIT:    memset(fifo->buffer, 0, fifo->max);     break;
            case FIFO_DATA_16BIT:   memset(fifo->buffer, 0, fifo->max * 2); break;
            case FIFO_DATA_32BIT:   memset(fifo->buffer, 0, fifo->max * 4); break;
        }
//        memset(fifo->buffer, 0, fifo->max);
        fifo->execution &= ~FIFO_CLEAR;
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     FIFO ????????
// ????     *fifo               FIFO ????
// ????     uint32              ?????
// ????     uint32 len = fifo_used(fifo);
// ????     
//-------------------------------------------------------------------------------------------------------------------
uint32 fifo_used (fifo_struct *fifo)
{
    //assert(fifo != NULL);
    return (fifo->max - fifo->size);
}

//-------------------------------------------------------------------------------------------------------------------
// ????     ? FIFO ?????
// ????     *fifo               FIFO ????
// ????     dat                 ??
// ????     fifo_state_enum     ????
// ????     zf_log(fifo_write_element(&fifo, data) == FIFO_SUCCESS, "fifo_write_byte error");
// ????     
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_element (fifo_struct *fifo, uint32 dat)
{
    //assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;
    
    do
    {
        if(FIFO_WRITE & fifo->execution)
        {
            return_state = FIFO_WRITE_UNDO;
            break;
        }
        fifo->execution |= FIFO_WRITE;

        if(1 <= fifo->size)                                                     // ????????????
        {
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:    ((uint8 *)fifo->buffer)[fifo->head] = dat;  break;
                case FIFO_DATA_16BIT:   ((uint16 *)fifo->buffer)[fifo->head] = dat; break;
                case FIFO_DATA_32BIT:   ((uint32 *)fifo->buffer)[fifo->head] = dat; break;
            }
            fifo_head_offset(fifo, 1);                                          // ?????
            fifo->size -= 1;                                                    // ?????????
        }
        else
        {
            return_state = FIFO_SPACE_NO_ENOUGH;
        }
        fifo->execution &= ~FIFO_WRITE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     ? FIFO ?????
// ????     *fifo               FIFO ????
// ????     *dat                ?????????
// ????     length              ?????????
// ????     fifo_state_enum     ????
// ????     zf_log(fifo_write_buffer(&fifo, data, 32) == FIFO_SUCCESS, "fifo_write_buffer error");
// ????     
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_buffer (fifo_struct *fifo, void *dat, uint32 length)
{
   // zf_assert(fifo != NULL);
    fifo_state_enum return_state = FIFO_SUCCESS;
    uint32 temp_length = 0;
    
    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
            break;
        }
        if(FIFO_WRITE & fifo->execution)
        {
            return_state = FIFO_WRITE_UNDO;
            break;
        }
        fifo->execution |= FIFO_WRITE;

        if(length <= fifo->size)                                                 // ????????????
        {
            temp_length = fifo->max - fifo->head;                               // ?????????????????

            if(length > temp_length)                                            // ?????????????? ?????????
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]),
                            dat, temp_length);                                  // ???????
                        fifo_head_offset(fifo, temp_length);                    // ?????
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]), 
                            &(((uint8 *)dat)[temp_length]),
                            length - temp_length);                              // ???????
                        fifo_head_offset(fifo, length - temp_length);           // ?????
                        break;
                    case FIFO_DATA_16BIT:
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            dat, temp_length * 2);                              // ???????
                        fifo_head_offset(fifo, temp_length);                    // ?????
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            &(((uint16 *)dat)[temp_length]),
                            (length - temp_length) * 2);                        // ???????
                        fifo_head_offset(fifo, length - temp_length);           // ?????
                        break;
                    case FIFO_DATA_32BIT:
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            dat, temp_length * 4);                              // ???????
                        fifo_head_offset(fifo, temp_length);                    // ?????
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            &(((uint32 *)dat)[temp_length]),
                            (length - temp_length) * 4);                        // ???????
                        fifo_head_offset(fifo, length - temp_length);           // ?????
                        break;
                }
            }
            else
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]),
                            dat, length);                                       // ??????
                        fifo_head_offset(fifo, length);                         // ?????
                        break;
                    case FIFO_DATA_16BIT:
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            dat, length * 2);                                   // ??????
                        fifo_head_offset(fifo, length);                         // ?????
                        break;
                    case FIFO_DATA_32BIT:
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            dat, length * 4);                                   // ??????
                        fifo_head_offset(fifo, length);                         // ?????
                        break;
                }
//                memcpy(&fifo->buffer[fifo->head], dat, length);                 // ??????
//                fifo_head_offset(fifo, length);                                 // ?????
            }

            fifo->size -= length;                                               // ?????????
        }
        else
        {
            return_state = FIFO_SPACE_NO_ENOUGH;
        }
        fifo->execution &= ~FIFO_WRITE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     ? FIFO ????
// ????     *fifo               FIFO ????
// ????     *dat                ???????
// ????     flag                ???? FIFO ?? ????????????
// ????     fifo_state_enum     ????
// ????     zf_log(fifo_read_element(&fifo, data, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_byte error");
// ????     
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_element (fifo_struct *fifo, void *dat, fifo_operation_enum flag)
{
    //assert(fifo != NULL);
    fifo_state_enum return_state = FIFO_SUCCESS;

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
            break;
        }
        fifo->execution |= FIFO_READ;

        if(1 > fifo_used(fifo))
        {
            return_state = FIFO_DATA_NO_ENOUGH;                                 // ??????
        }

        switch(fifo->type)
        {
            case FIFO_DATA_8BIT:
                *((uint8 *)dat) = ((uint8 *)fifo->buffer)[fifo->end];
                break;
            case FIFO_DATA_16BIT:
                *((uint16 *)dat) = ((uint16 *)fifo->buffer)[fifo->end];
                break;
            case FIFO_DATA_32BIT:
                *((uint32 *)dat) = ((uint32 *)fifo->buffer)[fifo->end];
                break;
        }
        
        if(flag == FIFO_READ_AND_CLEAN)                                         // ????????? FIFO ??
        {
            if(FIFO_CLEAR & fifo->execution)
            {
                return_state = FIFO_CLEAR_UNDO;
                break;
            }
            fifo->execution |= FIFO_CLEAR;
            fifo_end_offset(fifo, 1);                                           // ?? FIFO ???
            fifo->size += 1;
            fifo->execution &= ~FIFO_CLEAR;
        }
    }while(0);
    fifo->execution &= FIFO_READ;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     ? FIFO ????
// ????     *fifo               FIFO ????
// ????     *dat                ???????
// ????     *length             ??????? ???????????????
// ????     flag                ???? FIFO ?? ????????????
// ????     fifo_state_enum     ????
// ????     zf_log(fifo_read_buffer(&fifo, data, &length, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_buffer error");
// ????     
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_buffer (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag)
{
    //assert(fifo != NULL);
    //assert(length != NULL);
    fifo_state_enum return_state = FIFO_SUCCESS;
    uint32 temp_length;
    uint32 fifo_data_length;

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
            break;
        }
        fifo->execution |= FIFO_READ;

        fifo_data_length = fifo_used(fifo);
        if(*length > fifo_data_length)
        {
            *length = fifo_data_length;                                         // ???????
            return_state = FIFO_DATA_NO_ENOUGH;                                 // ??????
        }

        temp_length = fifo->max - fifo->end;                                    // ?????????????????
        if(*length <= temp_length)                                              // ?????????
        {
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:
                    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->end]), *length);
                    break;
                case FIFO_DATA_16BIT:
                    memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->end]), *length * 2);
                    break;
                case FIFO_DATA_32BIT:
                    memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->end]), *length * 4);
                    break;
            }
        }
        else
        {
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:
                    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->end]), temp_length);
                    memcpy(&(((uint8 *)dat)[temp_length]), fifo->buffer, *length - temp_length);
                    break;
                case FIFO_DATA_16BIT:
                    memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->end]), temp_length * 2);
                    memcpy(&(((uint16 *)dat)[temp_length]), fifo->buffer, (*length - temp_length) * 2);
                    break;
                case FIFO_DATA_32BIT:
                    memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->end]), temp_length * 4);
                    memcpy(&(((uint32 *)dat)[temp_length]), fifo->buffer, (*length - temp_length) * 4);
                    break;
            }
        }
        
        if(flag == FIFO_READ_AND_CLEAN)                                         // ????????? FIFO ??
        {
            if(FIFO_CLEAR & fifo->execution)
            {
                return_state = FIFO_CLEAR_UNDO;
                break;
            }
            fifo->execution |= FIFO_CLEAR;
            fifo_end_offset(fifo, *length);                                     // ?? FIFO ???
            fifo->size += *length;
            fifo->execution &= ~FIFO_CLEAR;
        }
    }while(0);
    fifo->execution &= FIFO_READ;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     ? FIFO ???????? buffer
// ????     *fifo               FIFO ????
// ????     *dat                ???????
// ????     *length             ??????? ???????????????
// ????     flag                ???? FIFO ?? ????????????
// ????     fifo_state_enum     ????
// ????     zf_log(fifo_read_tail_buffer(&fifo, data, &length, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_buffer error");
// ????     ???? FIFO_READ_AND_CLEAN ?? ????????????? FIFO
//              ???? FIFO_READ_AND_CLEAN ?? ????????????? FIFO
//              ???? FIFO_READ_AND_CLEAN ?? ????????????? FIFO
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_tail_buffer (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag)
{
    //assert(fifo != NULL);
    //assert(length != NULL);
    fifo_state_enum return_state = FIFO_SUCCESS;
    uint32 temp_length;
    uint32 fifo_data_length;

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
            break;
        }
        fifo->execution |= FIFO_READ;

        fifo_data_length = fifo_used(fifo);
        if(*length > fifo_data_length)
        {
            *length = fifo_data_length;                                         // ???????
            return_state = FIFO_DATA_NO_ENOUGH;                                 // ??????
        }

        if((fifo->head > fifo->end) || (fifo->head >= *length))
        {
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:
                    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->head - *length]), *length);
                    break;
                case FIFO_DATA_16BIT:
                    memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->head - *length]), *length * 2);
                    break;
                case FIFO_DATA_32BIT:
                    memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->head - *length]), *length * 4);
                    break;
            }
        }
        else
        {
            temp_length = *length - fifo->head;                                 // ?????????????????
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:
                    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->max - temp_length]), temp_length);
                    memcpy(&(((uint8 *)dat)[temp_length]), &(((uint8 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length));
                    break;
                case FIFO_DATA_16BIT:
                    memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->max - temp_length]), temp_length * 2);
                    memcpy(&(((uint16 *)dat)[temp_length]), &(((uint16 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length) * 2);
                    break;
                case FIFO_DATA_32BIT:
                    memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->max - temp_length]), temp_length * 4);
                    memcpy(&(((uint32 *)dat)[temp_length]), &(((uint32 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length) * 4);
                    break;
            }
        }
        
        if(flag == FIFO_READ_AND_CLEAN)                                         // ????????? FIFO ??
        {
            if(FIFO_CLEAR & fifo->execution)
            {
                return_state = FIFO_CLEAR_UNDO;
                break;
            }
            fifo->execution |= FIFO_CLEAR;
            fifo_end_offset(fifo, (fifo->max - fifo->size));
            fifo->size = fifo->max;
            fifo->execution &= ~FIFO_CLEAR;
        }
    }while(0);
    fifo->execution &= FIFO_READ;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// ????     FIFO ??? ???????
// ????     *fifo               FIFO ????
// ????     type                FIFO ????
// ????     *buffer_addr        ???????
// ????     size                ?????
// ????     fifo_state_enum     ????
// ????     fifo_init(&user_fifo, user_buffer, 64);
// ????     
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_init (fifo_struct *fifo, fifo_data_type_enum type, void *buffer_addr, uint32 size)
{
    //assert(fifo != NULL);
    fifo_state_enum return_value = FIFO_SUCCESS;
    do
    {
        if(NULL == buffer_addr)
        {
            return_value = FIFO_BUFFER_NULL;
            break;
        }
        fifo->buffer            = buffer_addr;
        fifo->execution         = FIFO_IDLE;
        fifo->type              = type;
        fifo->head              = 0;
        fifo->end               = 0;
        fifo->size              = size;
        fifo->max               = size;
    }while(0);
    return return_value;
}
