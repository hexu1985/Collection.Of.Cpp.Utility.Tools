/**
 * @file LCS.hpp
 * @brief 计算最长公共子序列的算法(longest-common-subsequence)
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-23
 *
 * @see 算法导论第三版, 15.4节
 */
#ifndef MINI_UTILS_LCS_INC
#define MINI_UTILS_LCS_INC

#include <cassert>
#include <vector>
#include <algorithm>

namespace mini_utils {

/**
 * @brief 计算最长公共子序列的算法(longest-common-subsequence)
 *
 * @tparam Sequence 序列类型
 */
template <typename Sequence>
class LCS {
private:
    enum { LEFT = '-', UP = '|', LEFT_UP = '\\' };
	std::vector<std::vector<int>> cTbl_;
	std::vector<std::vector<char>> bTbl_;
    const Sequence &seqX_;
    const Sequence &seqY_;

	template <typename T>
	static void initTable(std::vector<std::vector<T>> &v, int row, int col)
	{
        v.clear();
		v.resize(row+1);
		for (int i = 0; i <= row; i++) {
			v[i].resize(col+1);
		}	
	}

	void calLCS()
	{
		int row = seqX_.size();
		int col = seqY_.size();
		initTable(cTbl_, row, col);
		initTable(bTbl_, row, col);

		for (int i = 1; i <= row; ++i) {
			for (int j = 1; j <= col; ++j) {
				if (seqX_[i-1] == seqY_[j-1]) {
					cTbl_[i][j] = cTbl_[i-1][j-1]+1;
					bTbl_[i][j] = LEFT_UP;
				} else if (cTbl_[i-1][j] >= cTbl_[i][j-1]) {
					cTbl_[i][j] = cTbl_[i-1][j];
					bTbl_[i][j] = UP;
				} else {
					cTbl_[i][j] = cTbl_[i][j-1];
					bTbl_[i][j] = LEFT;
				}
			}
		}
	}

public:
    /**
     * @brief 初始化LCS类, 并计算LCS
     *
     * @param x 序列1
     * @param y 序列2
     */
	LCS(const Sequence &x, const Sequence &y): seqX_(x), seqY_(y)
	{
		calLCS();
	}

    /**
     * @brief 获取LCS长度
     *
     * @return LCS长度
     */
	int length() const 
    {
		return cTbl_[seqX_.size()][seqY_.size()];
	}

    /**
     * @brief 获取LCS在原始序列中的子序列下标
     *
     * @param subX 获取LCS在序列1中的子序列下标
     * @param subY 获取LCS在序列2中的子序列下标
     */
    void getLCS(std::vector<int> &subX, std::vector<int> &subY)
    {
        subX.clear(); subY.clear();
        int i = seqX_.size();
        int j = seqY_.size();

        while (i != 0 && j != 0) {
            if (bTbl_[i][j] == LEFT_UP) {
                subX.push_back(i-1);
                subY.push_back(j-1);
                i--;
                j--;
            } else if (bTbl_[i][j] == UP) {
                i--;
            } else {    // bTbl_[i][j] == LEFT
                assert(bTbl_[i][j] == LEFT);
                j--;
            }
        }

        std::reverse(subX.begin(), subX.end());
        std::reverse(subY.begin(), subY.end());
    }
};

}   // namespace mini_utils

#endif

