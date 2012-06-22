#ifndef TIMER_H
#define TIMER_H

#include <ctime>
//--
#include "utils/Assert.h"

class Timer{
private:
    clock_t begin, end;
    bool started;
public:
    Timer(bool startNow = false){
        started = startNow;
        if(startNow){
            begin = clock();
            Assert(begin != -1);//Indicates error condition
        }else{
            begin = end = -1;
        }
    }

    void start(){
        Assert(!started);

        begin = clock();
        started = true;
    }

    void stop(){
        Assert(started);

        end = clock();
        started = false;
    }

    inline unsigned long getElapsedMillis()const{
        Assert(! started);
        Assert(begin != -1 && end != -1);

        long double elapsedSecsLD = getElapsedSeconds();
        const long double millisPerSecondLD = 1000.0;
        return elapsedSecsLD * millisPerSecondLD;
    }

    inline long double getElapsedSeconds()const{
        Assert(! started);
        Assert(begin != -1 && end != -1);

        clock_t elapsed = end - begin;
        long double elapsedLD = (long double)elapsed;
        long double clocksPerSecLD = (long double) CLOCKS_PER_SEC;

        return elapsedLD / clocksPerSecLD;
    }

};

#endif //TIMER_H
