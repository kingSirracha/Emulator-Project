#ifndef CLOCK_DEF
#define CLOCK_DEF

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Memory.h"
#include "CPU.h"
using namespace std;


class Clock{
      private:
            int counter;
            Memory *mem;
            CPU *cpu;
      public:
            Clock(Memory *memin, CPU *cpuin);
            int get_counter();
            //lets devices know when a new cycle has begun
            void startTick();
            //gives devices a chance to do some work on the given cycle
            void doCycleWork();
            //gives devices more time before all the devices finish their work on the given cycle
            bool isMoreCycleWorkNeeded();
            //resets the clock timer
            void reset();
            //allows the clock to progress by some number of ticks
            void tick(int num);
            void dump();
};



#endif