#ifndef CPU_DEF
#define CPU_DEF


#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include "IMemory.h"
#include "Memory.h"
using namespace std;

class CPU{
      private:
      //pointers to both memory objects for acess
      IMemory *imem;
      Memory *mem;

      uint32_t input;
      //Values that will be set by instructions
      uint8_t instruction;
      uint8_t designated_reg;
      uint8_t source_reg;
      uint8_t target_reg;
      uint8_t immediate_val;
      //determines if more work needs to be done in the cycle
      bool moreCycleWork;
      //true when an instruction only takes a cycle to complete
      bool isSameCycle;
      //true when an instruction will take multiple cycles to run on the cpu itself (does not inclued cmds using other devices)
      bool needsInternalCycles;
      int internalCycleCount;
      int requiredCycles;


      public:
      enum CpuStates {IDLE,WAIT,FETCH_INSTUCT,DECODE,EXECUTE,HALT};
      CpuStates currentState;
      uint8_t PC;
      uint8_t regs[8];
      CPU(IMemory *imemory, Memory *memory);
      void start_tick();
      void reset();
      uint8_t get_reg(string reg);
      void set_reg(string reg, uint8_t hex_byte);
      //prints out register information
      void dump();
      void doCycleWork();
      bool isMoreCycleWorkNeeded();


};

#endif //CPU_DEF