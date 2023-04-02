#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Cache.h"
#include "Memory.h"
#include <cmath>
using namespace std;


Cache::Cache(Memory *in_mem){
      mem = in_mem;
      CLO = 0;
      enabled = false;
      dataWritten = false;
      task_complete = false;
      state = IDLE;
      task = NA;
}

bool Cache::is_enabled(){
      return enabled;
}

void Cache::reset(){
      CLO = 0;
      //set all flags to invalid
      for(int i = 0; i < 8; i++){
            flags[i] = I;
      }
}

void Cache::on(){
      enabled = true;
}

//TODO should take as many ticks as mem needs so 5
void Cache::flush(){
      if (dataWritten) {
            for(int i = 0; i < 8; i++){
                  if (flags[i] == W){
                        (mem) -> store_direct(CLO * 8 + i, cache[i]);
                  }
            }
      }
}

void Cache::start_retrieve(uint8_t addr){
      task = RETREIVE;
      //if in correct range
      if (addr >= CLO * 8 || addr < (CLO + 1) * 8){
            retreived_val = cache[addr - (CLO * 8)];
      } else {
            //since contents are about to be erased we flush to mem
            flush();
            CLO = floor(addr/8);
            //start a mem Fetch for the given mem area
            (mem) -> memStartFetch(addr - addr % 8, 8);
      }
}

void Cache::start_store(uint8_t value, uint8_t addr){

}

void Cache::off(){
      flush();
      enabled = false;
}

void Cache::do_cycle_work(){
      switch(state){
            case IDLE:
                  //must be called by an outside device to exit this state
            break;
            case FETCH:
                  //fetched instruction
            break;
            case WAIT:
                  //waiting usually for mem
            break;
            case EXECUTE:
                  //running instruction
            break;
      }
}