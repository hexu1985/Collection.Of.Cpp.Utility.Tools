/** \example sample_priority_queue1.cpp
 * This is an example of how to use the PriorityQueue class.
 */
#include <iostream>
#include "PriorityQueueRef.hpp"

using namespace std;
using namespace MiniUtils;

int main()
{
    vector<int> keyList = {10,60,50,20};
    PriorityQueueRef<int> q(keyList);

    for (int i = 0; i < keyList.size(); i++)
        q.push(i);

    while (!q.isEmpty()) {
        cout << keyList[q.top()] << ' ';
        q.pop();
    }

    cout << endl;
}
