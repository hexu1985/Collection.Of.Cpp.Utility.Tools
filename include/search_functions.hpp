#ifndef MINI_UTILS_SEARCH_FUNCTIONS_INC
#define MINI_UTILS_SEARCH_FUNCTIONS_INC

namespace MiniUtils {

template <typename T>
int sequence_search(const std::vector<T> &arr, int first, int last, const T &target)
{
    for (int i = first; i < last; i++) {
        if (arr[i] == target)
            return i;
    }
    return last;
}

template <typename T>
int sequence_search(const std::vector<T> &arr, const T &target)
{
    return sequence_search(arr, 0, arr.size(), target);
}

template <typename T>
int binary_search(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	int origLast = last;		// save original value of last
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		if (target == arr[mid])
			return mid;			// have a match
		// determine which sublist to search
		else if (target < arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return origLast;			// target not found
}

template <typename T>
int binary_search(const std::vector<T> &arr, const T &target)
{
    return binary_search(arr, 0, arr.size(), target);
}

template <typename T>
int lower_bound(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		// determine which sublist to search
		if (target <= arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return first;
}

template <typename T>
int lower_bound(const std::vector<T> &arr, const T &target)
{
    return lower_bound(arr, 0, arr.size(), target);
}

template <typename T>
int upper_bound(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		// determine which sublist to search
		if (target < arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return first;
}

template <typename T>
int upper_bound(const std::vector<T> &arr, const T &target)
{
    return upper_bound(arr, 0, arr.size(), target);
}

}	// namespace MiniUtils

#endif


