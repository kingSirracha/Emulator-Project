#ifndef MEM_DEF
#define MEM_DEF



#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;


class Memory{
      private:
            uint8_t *mem;
            uint16_t size;
            enum memStates {IDLE,WAIT,MOVEDATA};
            enum RequestTypes {FETCH,STORE};
            RequestTypes taskRequested;
            memStates currentState;
            //determines if more work needs to be done in the cycle
            bool moreCycleWork;
            //used to communicate with the CPU if it can give requested information on a cycle
            bool fetchComplete;
            bool storeComplete;
            //values used to carry func args across different methods
            uint8_t fetchVal[8];
            uint8_t storeVal[8];
            uint8_t storeAddr;

            int data_ammount;

            int ticksToComplete;
            int counter;
      public:
            Memory();
            //allocates mem
            void create(uint16_t mem_size);
            //sets all vals to 0
            void reset();
            void store_direct(uint8_t addr, uint8_t val);
            //tick processes
            void start_tick();
            void doCycleWork();
            bool isMoreCycleWorkNeeded();
            //retreives a mem value
            void memStartFetch(uint8_t hex_addr, int count);
            void memStartStore(uint8_t hex_addr, uint8_t* value, int count);
            bool isFetchComplete();
            bool isStoreComplete();
            uint8_t* endFetch();
            void endStore();
            void set(uint8_t hex_addr, uint8_t hex_count, uint8_t* values);
            void dump(uint8_t hex_addr, uint8_t hex_count);


};

#endif