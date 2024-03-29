#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Clock.h"
#include "Memory.h"
#include "Clock.h"
#include "Cache.h"
using namespace std;


Clock::Clock(Memory *memin, CPU *cpuin, Cache *cachein){
       mem = memin;
       cpu = cpuin;
       cache = cachein;
};

void  Clock::reset(){
       counter = 0;
}
//test for github
void Clock::tick(int num){
       //cout << "clock ticking: " << num << endl;
       for(int i = 0; i < num; i++){
              counter ++;
              bool workToDo = true;
              //cout << "cycle Start, on cycle: " << (int)i << endl;

              //initializes tick
              (mem) -> start_tick();
              (cpu) -> start_tick();

              //runs while devices have tasks to perform within the tick
              while (workToDo) {
                     (mem) -> doCycleWork();
                     (cache) -> do_cycle_work();
                     (cpu) -> doCycleWork();
                     workToDo = ((cpu) -> isMoreCycleWorkNeeded() || (mem) -> isMoreCycleWorkNeeded() || (cache) -> isMoreCycleWorkNeeded());
              }

       }
}
void Clock::dump(){
       cout << "Clock: " << counter;
}
int Clock::get_counter(){
       return counter;
}




