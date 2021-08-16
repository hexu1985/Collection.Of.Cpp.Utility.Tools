/**
 * @file dump_functions.hpp
 * @brief dump数据的函数
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-09-17
 *
 * @see sentinel c runtime lib
 */
#ifndef MINI_UTILS_DUMP_FUNCTIONS_INC
#define MINI_UTILS_DUMP_FUNCTIONS_INC

#include <stdio.h>
#include <stdint.h>

namespace mini_util {

/**
 * @brief 每行打印字符个数
 */
static const int DUMP_COLS_PER_ROW = 16; 

/**
 * @brief 每组打印字符个数
 */
static const int DUMP_SPACE_COLS = 8; 

/**
 * @brief dump指定数据块成十六进制格式
 *
 * @param data 数据块地址
 * @param datalen 数据块长度
 * @param margin 边界
 */
static
void dump_hex(const uint8_t *data, unsigned int datalen, const char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump character in hex */
        printf("%02X ", data[i]);
    }
} /* dump_hex */

/**
 * @brief dump指定数据块成ASCII码格式, 不可打印字符显示为"."
 *
 * @param data 数据块地址
 * @param datalen 数据块长度
 * @param margin 边界
 */
static
void dump_ascii(const uint8_t *data, unsigned int datalen, const char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump printable character in ascii */
        printf("%c", ((data[i] > 31) && (data[i] < 128)) ? data[i] : '.');
    }
} /* dump_ascii */

/**
 * @brief dump指定数据块成十六进制和ASCII码格式(不可打印字符显示为".")
 *
 * @param data 数据块地址
 * @param datalen 数据块长度
 * @param margin 十六进制和ASCII码之间的边界
 */
static
void dump(const uint8_t *data, unsigned int datalen, const char *margin)
{
    unsigned int i, icols;

    for (i = 0; i < datalen; )
    {
        icols = datalen - i;

        if (icols > DUMP_COLS_PER_ROW)
            icols = DUMP_COLS_PER_ROW;

        dump_hex(&data[i], icols, margin);
        dump_ascii(&data[i], icols, "  ");
        printf("\n");

        i += icols;
    }
} /* dump */

}   // namespace mini_util

#endif
