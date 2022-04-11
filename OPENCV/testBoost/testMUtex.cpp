
#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <atomic>
#include <unistd.h>
#include <condition_variable>

using namespace std;
using namespace std::chrono;

template<typename T>
class send_receive_one_object {
    std::atomic<T *> a_ptr;
    condition_variable cv_;
    mutex lock_;
public:

    void send(T const& _obj) {
        T *new_ptr = new T;
        *new_ptr = _obj;
        std::unique_ptr<T> old_ptr(a_ptr.exchange(new_ptr));
        cv_.notify_one();
    }

    T receive() {
        std::unique_ptr<T> ptr;
        do {
            std::unique_lock<mutex> l(lock_);
            cv_.wait(l, [&]{return a_ptr != NULL;});
            ptr.reset(a_ptr.exchange(NULL));
        } while (!ptr);
        T obj = *ptr;
        return obj;
    }

    send_receive_one_object() : a_ptr(NULL)
    {
        cout << "================> init send_receive_one_object" << endl;
    }
};

struct Job{
    int id;
}; // 收发数据结构体，可以写视频帧或者其他有用的数据

const bool sync_ = true;
static send_receive_one_object<Job> read_write;

void frame_write(){
    cout << "this is frame_write !!!" << endl;
    sleep(3);
    Job jj;
    int cnt = 30;
    while(cnt --){
        jj.id = 30-cnt;
        read_write.send(jj);
        usleep(10000); // 模拟取流30帧，并设置时间间隔
    }
}

void frame_read(){
    cout << "this is frame_read !!!" << endl;
    Job jj;
    int cnt = 200;
    lock_.lock();
    while(cnt --){
        jj = read_write.receive();
        cout << jj.id << endl;
        usleep(30000); // 模拟算法处理速度是取流速度的1/3
    lock_.unlock();
    }
}

int main(int argc, char** argv)
{
    std::thread tw(frame_write);
    std::thread tr(frame_read);

    tw.join();
    tr.join();

    return 0;
}