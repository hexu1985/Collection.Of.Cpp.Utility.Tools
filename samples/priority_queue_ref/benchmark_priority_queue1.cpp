/** \example priority_queue_ref/benchmark_priority_queue1.cpp
 * This is an example of how to use the std::priority_queue class.
 */
#include <iostream>
#include <queue>

using namespace std;

int main()
{
    vector<int> keyList = {10,60,50,20};
    priority_queue<int> q;

    for (auto key: keyList)
        q.push(key);

    while (!q.empty()) {
        cout << q.top() << ' ';
        q.pop();
    }

    cout << endl;
}
