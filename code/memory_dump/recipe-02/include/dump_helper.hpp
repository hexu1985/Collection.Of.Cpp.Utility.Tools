/**
 * @file dump_helper.hpp
 * @brief dump数据的帮助类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-23
 *
 * @see dump_functions.hpp
 */
#ifndef MINI_UTILS_DUMP_HELPER_INC
#define MINI_UTILS_DUMP_HELPER_INC

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>

namespace mini_util {

class DumpHelper {
private:
	std::ostream &out_;			// 输出流
	int dump_cols_per_row_ = 16;// 每行打印字符个数
	int dump_space_cols_ = 8;	// 每组打印字符个数

public:
	DumpHelper(std::ostream &out = std::cout): out_(out) {}

	DumpHelper(std::ostream &out, int dump_cols_per_row, int dump_space_cols):
		out_(out), dump_cols_per_row_(dump_cols_per_row), dump_space_cols_(dump_space_cols) 
	{
	}

	/**
	 * @brief dump指定数据块成十六进制格式
	 *
	 * @param data 数据块地址
	 * @param datalen 数据块长度
	 * @param margin 边界
	 */
	void dumpHex(const uint8_t *data, int datalen, const char *margin = NULL)
	{
		int i;

		for (i = 0; i < datalen; i++)
		{
			if (((i % dump_space_cols_) == 0) && (i != 0))
				out_ << " ";

			if ((i % dump_cols_per_row_) == 0)
			{
				if (i != 0)
					out_ << "\n";

				if (margin != NULL)
					out_ << margin;
			}

			/* dump character in hex */
			out_ << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << (int) data[i] << ' ';
		}
	} /* dumpHex */

	/**
	 * @brief dump指定数据块成ASCII码格式, 不可打印字符显示为"."
	 *
	 * @param data 数据块地址
	 * @param datalen 数据块长度
	 * @param margin 边界
	 */
	void dumpAscii(const uint8_t *data, int datalen, const char *margin = NULL)
	{
		int i;

		for (i = 0; i < datalen; i++)
		{
			if (((i % dump_space_cols_) == 0) && (i != 0))
				out_ << " ";

			if ((i % dump_cols_per_row_) == 0)
			{
				if (i != 0)
					out_ << "\n";

				if (margin != NULL)
					out_ << margin;
			}

			/* dump printable character in ascii */
			out_ << (char) (((data[i] > 31) && (data[i] < 128)) ? data[i] : '.');
		}
	} /* dumpAscii */

	/**
	 * @brief dump指定数据块成十六进制和ASCII码格式(不可打印字符显示为".")
	 *
	 * @param data 数据块地址
	 * @param datalen 数据块长度
	 * @param margin 十六进制和ASCII码之间的边界
	 */
	void dump(const uint8_t *data, int datalen, const char *margin = NULL)
	{
		int i, icols;

		for (i = 0; i < datalen; )
		{
			icols = datalen - i;

			if (icols > dump_cols_per_row_)
				icols = dump_cols_per_row_;

			dumpHex(&data[i], icols, margin);
			dumpAscii(&data[i], icols, "  ");
			out_ << "\n";

			i += icols;
		}
	} /* dump */
};

}   // namespace mini_util

#endif
