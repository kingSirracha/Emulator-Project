#ifndef CACHE_DEF
#define CACHE_DEF

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include "Memory.h"
using namespace std;


class Cache{
      private:
            Memory *mem;
            // if the cache is/isn't on
            bool enabled;
            bool task_complete;
            bool more_cycle_work;
            //I = invalid   V = valid  W = written
            enum memFlags{I,V,W};
            enum States{IDLE,FETCH,WAIT,EXECUTE};
            enum Tasks{STORE,RETREIVE,NA};
            //true if data was written. is set as false when a new block is obtained
            bool dataWritten;
            //stores 8-bytes of information. These 8-bits will be 8-bit segments of data memory
            //does not take from instruct memory
            uint8_t cache[8];
            memFlags flags[8];
            States state;
            Tasks task;
            //keeps track of the memory index which Cache is on
            //0 = 0-7, 1 = 8-15, 2 = 16-23, etc ...
            uint8_t CLO;
            uint8_t retreived_val;
            uint8_t ref_addr;
            uint8_t ref_val;
            //0 = 0x00-0x07, 1 = 0x08-0x0E, 2 = 0x0F-0x27
            //writes to mem where data has been written
            void flush();
            void MemToCache();
      public:
            Cache(Memory *mem_ref);
            //cache is disabled and CLO = 0
            void end_store();
            bool is_enabled();
            bool is_retreive_complete();
            bool is_store_complete();
            void start_retrieve(uint8_t addr);
            uint8_t get_retrieve();
            void start_store(uint8_t value, uint8_t addr);
            void reset();
            //cycle related commands
            bool isMoreCycleWorkNeeded();
            void do_cycle_work();
            void start_tick();
            //now keeps copies of data transfered between CPU and Memory
            void on();
            //Back to assignment 3
            //data written in cache gets written to mem
            void off();
            void dump();

};



#endif