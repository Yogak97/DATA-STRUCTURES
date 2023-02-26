/*
author : Yoga K
Desc : Deadlock example
*/

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

std::mutex m1, m2;

void thread1()
{
    m1.lock();
    std::this_thread::sleep_for(chrono::seconds(1));
    m2.lock(); // m2 is already locked by t2 and t1 is also trying to lock it.
    cout << "critical section of thread 1" << endl;
    m1.unlock();
    m2.unlock();
}
void thread2()
{
    m2.lock();
    std::this_thread::sleep_for(chrono::seconds(1));
    m1.lock(); // m1 is already locked by t1 and t2 is also trying to lock it.
    cout << "critical section of thread 2" << endl;
    m1.unlock();
    m2.unlock();
}
int main()
{
    std::thread t1(thread1);
    std::thread t2(thread2);

    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();

    cout << " main() ended" << endl;
    return 0;
}