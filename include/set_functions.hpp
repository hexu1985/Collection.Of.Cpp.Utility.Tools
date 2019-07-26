#ifndef MINI_UTILS_SET_FUNCTIONS_INC
#define MINI_UTILS_SET_FUNCTIONS_INC

#include <vector>
#include <algorithm>

namespace MiniUtils {

template <typename T>
std::vector<T> set_intersection(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(std::min(n1, n2));
    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            i1++;
        } else if (v2[i2] < v1[i1]) {
            i2++;
        } else {
            res.push_back(v1[i1]);
            i1++;
            i2++;
        }
    }

    return res;
}

template <typename T>
std::vector<T> set_union(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1+n2);
    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            res.push_back(v2[i2]);
            i2++;
        } else {
            res.push_back(v1[i1]);
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    for ( ; i2 < n2; i2++) {
        res.push_back(v2[i2]);
    }

    return res;
}

template <typename T>
std::vector<T> set_difference(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1);

    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            i2++;
        } else {
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    return res;
}

template <typename T>
std::vector<T> set_symmetric_difference(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1+n2);

    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            res.push_back(v2[i2]);
            i2++;
        } else {
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    for ( ; i2 < n2; i2++) {
        res.push_back(v2[i2]);
    }

    return res;
}

}   // namespace MiniUtils

#endif	// HEAP_FUNCTIONS
