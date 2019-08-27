/** \example priority_queue/sample_priority_queue1.cpp
 * This is an example of how to use the PriorityQueue class.
 */
#include <iostream>
#include "PriorityQueue.hpp"

using namespace std;
using namespace mini_utils;

int main()
{
    PriorityQueue<float> q;

    // insert three elements into the priority queue
    q.push(66.6);
    q.push(22.2);
    q.push(44.4);

    // read and print two elements
    cout << q.top() << ' ';
    q.pop();
    cout << q.top() << endl;
    q.pop();

    // insert three more elements
    q.push(11.1);
    q.push(55.5);
    q.push(33.3);

    // skip one element
    q.pop();

    // pop and print remaining elements
    while (!q.isEmpty()) {
        cout << q.top() << ' ';
        q.pop();
    }
    cout << endl;
}
