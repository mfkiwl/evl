#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include <tuple>

#include <boost/circular_buffer.hpp>
#include "unistd.h"
#include <evl/core/types.hpp>

//typedef std::tuple<int32_t, uint16_t, uint16_t, bool> EventTuple;
//typedef boost::circular_buffer<EventTuple> EventBuffer;

std::mutex test_mtx;

void disp_(EventTuple x ) {
    std::cout << std::get<0>(x) << ' ' << std::get<1>(x) << ' ' << std::get<2>(x) << std::endl;
}

// TUPLE VERSION
void loop_task3(EventBuffer *buffer){
    // int i = 0;
    int32_t _ts = 0;
    uint16_t _x = 100;
    uint16_t _y = 120;
    bool _pol = true;
    while (1){
        usleep(2);
        EventTuple tup = std::make_tuple(_ts, _x, _y, _pol);
        test_mtx.lock();
        (*buffer).push_front(tup);
        _ts += 1;
        test_mtx.unlock();
    }
}

std::vector<EventTuple> readLifetimeData(EventBuffer *buffer, int lifetime){
    std::vector<EventTuple> v;
    test_mtx.lock();

    EventTuple tup = (*buffer).front();    // get first element
    int starttime = std::get<0>(tup);
    (*buffer).pop_front();                  // remove first element
    v.push_back(tup);
    for (int i=0; i<(*buffer).size(); i++){
        EventTuple tup = (*buffer).front();    // get first element
        if (std::get<0>(tup) > starttime - lifetime){
            (*buffer).pop_front();                  // remove first element
            v.push_back(tup);
        }
        else{test_mtx.unlock(); break;}
    }
    return v;
}

void loop_task4(EventBuffer *buffer, int lifetime){
    for (int i = 0; i < 10000; i++) {
        usleep(100000);      // micro sec
        std::vector<EventTuple> v = readLifetimeData(buffer, lifetime);

        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), disp_);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}


int main(){
    int lifetime = 100;
    EventBuffer buffer(50000);

    // std::thread t1(loop_task1, &ts, &x, &y, &pol);
    std::thread t1(loop_task3, &buffer);
    std::thread t2(loop_task4, &buffer, lifetime);

    t1.join();
    t2.join();
    return 0;
}


// SEPARATE VERSION
// typedef boost::circular_buffer<int32_t> TimeBufer;
// typedef boost::circular_buffer<uint16_t> XBuffer;
// typedef boost::circular_buffer<uint16_t> YBuffer;
// typedef boost::circular_buffer<bool> PolBuffer;

// void loop_task1(TimeBufer *ts, XBuffer *x, YBuffer *y, PolBuffer *pol){
//     // int i = 0;
//     int32_t _ts = 0;
//     uint16_t _x = 100;
//     uint16_t _y = 120;
//     bool _pol = true;
//     while (1){
//         usleep(2);
//         test_mtx.lock();
//         // (*v).push_front(std::make_tuple(ts, x, y, pol));
//         (*ts).push_front(_ts);
//         (*x).push_front(_x);
//         (*y).push_front(_y);
//         (*pol).push_front(_pol);
//         // std::cout << "[Thread1] Append: "<< i << std::endl;
//         _ts += 1;
//         test_mtx.unlock();
//     }
// }

// EventTuple task2(TimeBufer *ts, XBuffer *x, YBuffer *y, PolBuffer *pol){
//     // get first element
//     test_mtx.lock();

//     int32_t _ts = (*ts).front();
//     uint16_t _x = (*x).front();
//     uint16_t _y = (*y).front();
//     bool _pol = (*pol).front();

//     // remove first element
//     (*ts).pop_front();
//     (*x).pop_front();
//     (*y).pop_front();
//     (*pol).pop_front();
//     test_mtx.unlock();   

//     // std::cout << "[Thread2] Remaining >>> ";
//     // std::for_each((*ts).begin(), (*ts).end(), disp_);
//     // std::cout << std::endl;
//     return std::make_tuple(_ts, _x, _y, _pol);
// }

// void loop_task2(TimeBufer *ts, XBuffer *x, YBuffer *y, PolBuffer *pol,
//     std::vector<EventTuple> *v){

//     for (int i = 0; i < 20; i++) {
//         usleep(10000);      // micro sec
//         EventTuple tup = task2(ts, x, y, pol);
//         (*v).push_back(tup);
//     }
// }


