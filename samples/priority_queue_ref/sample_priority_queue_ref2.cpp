/** \example priority_queue_ref/sample_priority_queue_ref2.cpp
 * This is an example of how to use the PriorityQueueRef class.
 */
#include <iostream>
#include "PriorityQueueRef.hpp"

using namespace std;
using namespace MiniUtils;

int main()
{
    vector<int> keyList = {10,60,50,20};
    PriorityQueueRef<int> q(keyList);

    for (size_t i = 0; i < keyList.size(); i++)
        q.push(i);

    cout << "top index: " << q.top() << ", value: " << keyList[q.top()] << endl;

    cout << "after keyList[3] += 60\n";
    keyList[3] += 60;
    q.update(3);
    cout << "top index: " << q.top() << ", value: " << keyList[q.top()] << endl;

    keyList[2] -= 20;
    q.update(2);

    cout << "now keyList is: ";
    for (auto key: keyList)
        cout << key << ' ';
    cout << endl;

    cout << "priority order is: ";
    while (!q.isEmpty())
        cout << keyList[q.pop()] << ' ';
    cout << endl;

    return 0;
}
