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

bool Cache::is_retreive_complete(){
      if (task == RETREIVE){
            return task_complete;
      }
      return false;
}

bool Cache::is_store_complete(){
      if (task == STORE){
            return task_complete;
      }
      return false;
}

void Cache::reset(){
      enabled = false;
      dataWritten = false;
      task_complete = false;
      state = IDLE;
      task = NA;
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
            dataWritten = false;
      }
}

void Cache::start_retrieve(uint8_t addr){
      task = RETREIVE;
      //if in correct range
      if (addr >= CLO * 8 || addr < (CLO + 1) * 8){
            retreived_val = cache[addr - (CLO * 8)];
            state = EXECUTE;
      } else {
            //since contents are about to be erased we flush to mem
            flush();
            CLO = floor(addr/8);
            //start a mem Fetch for the given mem area
            (mem) -> memStartFetch(addr - addr % 8, 8);
            ref_addr = addr - (CLO * 8);
            state = WAIT;
      }
}

uint8_t Cache::get_retrieve(){
      return retreived_val;
      task_complete = false;
      task = NA;
}

void Cache::end_store(){
      task_complete = false;
      task = NA;
}

void Cache::start_store(uint8_t value, uint8_t addr){
      task = STORE;
      if (addr >= CLO * 8 || addr < (CLO + 1) * 8){
            cache[addr - (CLO * 8)] = value;
            flags[addr - (CLO * 8)] = W;
            dataWritten = true;
            state = EXECUTE;
      } else {
            flush();
            CLO = floor(addr/8);
            //start a mem Fetch for the given mem area
            (mem) -> memStartFetch(addr - addr % 8, 8);
            ref_val = value;
            ref_addr = addr - (CLO * 8);
            state = WAIT;
      }
}

void Cache::off(){
      flush();
      enabled = false;
}

void Cache::start_tick(){
      more_cycle_work = true;
}

void Cache::do_cycle_work(){
      switch(state){
            case IDLE:
                  //must be called by an outside device to exit this state
                  more_cycle_work = false;
            break;
            case FETCH:
                  //fetched instruction
            break;
            case WAIT:
                  //waiting usually for mem
                  if ((mem) -> isFetchComplete()){
                        //fetched mem vals go into cache
                        uint8_t* temp = (mem) -> endFetch();
                        for(int i = 0; i < 8; i++){
                              cache[i] = temp[i];
                        }
                        if(task = RETREIVE){
                              retreived_val = cache[ref_addr];
                        } else if (task = STORE){
                              cache[ref_addr] = ref_val;
                              flags[ref_addr] = W;
                              dataWritten = true;
                        }
                        state = EXECUTE;
                  } else {
                        more_cycle_work = false;
                  }
            break;
            case EXECUTE:
                  switch (task){
                        case RETREIVE:
                        break;
                        case STORE:
                        break;
                        case NA:
                              cout << "this line shouldn't be reached \n";
                        break;
                  }
                  task_complete = true;
                  state = IDLE;
                  //running instruction
            break;
      }
}

bool Cache::isMoreCycleWorkNeeded(){
      return more_cycle_work;
}

void Cache::dump(){
      if (CLO < 16){
            //if the hex number is too small
            cout << "CLO        : 0x0" << hex << (int) CLO << endl;
      }else{
            cout << "CLO        : 0x" << hex << (int) CLO << endl;
      }
      cout << "cache data : ";
      for(int i = 0; i < 8; i++){
            int temp = (int)cache[i];
            cout <<"0x"; 
            if(temp < 16){
                  cout << "0"; //adds an extra 0 if the number is too low
            }
            cout << uppercase << hex << (int)cache[i] << " ";
      }
      cout << endl;
      cout << "Flags      :";
      for(int i = 0; i < 8; i++){
            switch (flags[i]) {
                  //converts the enums into strings
                  case I:
                        cout << "   I";
                  break;
                  case V:
                        cout << "   V";
                  break;
                  case W:
                        cout << "   W";
                  break;
            }
      }
      cout << endl;
}