#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Cache.h"
#include "Memory.h"
using namespace std;


Cache::Cache(Memory *in_mem){
      mem = in_mem;
      CLO = 0;
      enabled = false;
      dataWritten = false;
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

void Cache::off(){
      //TODO should also flush to memory
      enabled = false;
}