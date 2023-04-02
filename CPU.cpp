#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include "CPU.h"
#include "Memory.h"
#include "Cache.h"
#include "Clock.h"
using namespace std;


CPU::CPU(IMemory *imemory, Memory *memory, Cache *incache){
      cache = incache;
      imem = imemory;
      mem = memory;
      currentState = IDLE;
      //true when an instruction only takes a cycle to complete
      bool isSameCycle = false;
      //true when an instruction will take multiple cycles to run on the cpu itself (does not inclued cmds using other devices)
      bool needsInternalCycles = false;
      int internalCycleCount = 0;
      int requiredCycles = 0;
      TC = 0;
}
//uint8_t PC;
//Sets all registers to 0x00
void CPU::reset(){
      PC = 0x00;
      for(int i = 0; i < 8; i++){
            regs[i] = 0x00;
      }

}

uint8_t CPU::get_reg(string reg){
      if (reg == "PC"){
            return PC;
      } else {
            int temp = int(reg[1]); //converts last char to an ascii value
            temp -= 65; //subtract asci value so it can translate to an array position
            return regs[temp];
      }
}

void CPU::set_reg(string reg, uint8_t hex_byte){
      if (reg == "PC"){
            PC = hex_byte;
      } else {
            int temp = int(reg[1]); //converts last char to an ascii value
            temp -= 65; //subtract asci value so it can translate to an array position
            regs[temp] = hex_byte;
      }
}

//displays the contents of CPU
void CPU::dump(){
      cout << "PC: 0x";
      if(PC < 16){
            cout << "0"; //adds an extra 0 if the number is too low
      }
      cout << uppercase << hex << (int)PC <<endl;
      int asci_count = 65;
      for(int i = 0; i < 8; i++){
            int temp = (int)regs[i];
            cout << "R" << (char)asci_count << ": 0x"; //prints out reg num and the indicator for hex
            if(temp < 16){
                  cout << "0"; //adds an extra 0 if the number is too low
            }
            cout << uppercase << hex << (int)regs[i] << endl;
            asci_count ++;
      }
      cout << "TC: " << dec <<(int) TC << endl;
      cout << endl;
}

void CPU::start_tick(){
      moreCycleWork = true;
      TC ++;
      //cout << "CPU Tick started" << endl;
      //if prevents unwarrented state change
      if (currentState == IDLE){
            //vars needed for cpu exclusive cmds
            isSameCycle = false;
            needsInternalCycles = false;
            internalCycleCount = 0;
            requiredCycles = 0;
            //takes the state off of idle
            currentState = FETCH_INSTUCT;
      }
}

void CPU::doCycleWork(){
      switch (currentState){
      case IDLE:
            moreCycleWork = false;
            /* Waiting for CPU_StartTick */
            break;
      case FETCH_INSTUCT:
            //gets instruction from instruction mem
            input = (imem) -> memStartFetch(PC);
            //onto the next state
            currentState = DECODE;
            PC++;
            //cout << "Fetched Instruction" << endl;
            break;
      case DECODE:
            //shifting and masking the value in instruction mem for decoding
            instruction = input >> 17;
            designated_reg = (input & 0x1FFFF ) >> 14;
            source_reg = (input & 0x03FFF ) >> 11;
            //old target_reg code (input & 0x007FF ) >> 8
            target_reg = (input >> 8) & 0x07;
            immediate_val = (input & 0x000FF);
            currentState = EXECUTE;
            //cout << "finished decoding" << endl;
            //cout << uppercase << hex << input << endl;
            //cout << "instruction code: " << (int)instruction << "  d_reg: " << (int)designated_reg << "   s_reg: " << (int)source_reg << endl;
            //cout << "PC: " << (int)PC << endl;
            break;
      case EXECUTE:
            switch(instruction){
                  case 0b000:
                        //add
                        //$d = $s + $t or store the results of adding two registers into a designated register
                        //cout << "Designated_reg: " << designated_reg << endl;
                        regs[designated_reg] = regs[target_reg] + regs[source_reg];
                        isSameCycle = true;
                        break;
                  case 0b001:
                        //addi
                        //$d = $s + imm
                        //cout << "adding_i together " << (int)regs[source_reg] << " + " << (int)immediate_val << endl;
                        //cout << "source_reg: " << (int)source_reg << endl;
                        regs[designated_reg] = regs[source_reg] + immediate_val;
                        isSameCycle = true;
                        //cout << "storing reg: " << (int)designated_reg << " end result: " << (int)regs[designated_reg]<< endl;
                        //this->dump();
                        break;
                  case 0b010:
                        //mul
                        //takes the upper and lower 4 bits of the source reg and multiplies those vals together
                        //storing the result in the designated reg
                        uint8_t val_1, val_2;
                        //uint8_t val_2;
                        val_1 = regs[source_reg] >> 4 ;
                        val_2 = regs[source_reg] & 0x0F;
                        regs[designated_reg] = val_1 * val_2;
                        needsInternalCycles = true;
                        requiredCycles = 2;
                        break;
                  case 0b011:
                        //inv
                        //$d = !($s)
                        regs[designated_reg] = ~(regs[source_reg]);
                        isSameCycle = true;
                        break;
                  case 0b100:
                        //branching functions
                        switch(designated_reg){
                              case 0b000:
                                    //beq
                                    //if $s == $ t then PC = imm
                                    if(regs[source_reg] == regs[target_reg]){
                                          PC = immediate_val;
                                          //cout << "branched to instuction: " << (int)PC << endl;
                                          needsInternalCycles = true;
                                          requiredCycles = 2;
                                    } else {
                                          isSameCycle = true;
                                    }
                                    break;
                              case 0b001:
                                    //bneq
                                    //if $s != $ t then PC = imm
                                    if(regs[source_reg] != regs[target_reg]){
                                          PC = immediate_val;
                                          needsInternalCycles = true;
                                          requiredCycles = 2;
                                    } else {
                                          isSameCycle = true;
                                    }
                                    break;
                              case 0b010:
                                    //blt
                                    //if $s == $ t then PC = imm
                                    if((int8_t)regs[source_reg] < (int8_t)regs[target_reg]){
                                          PC = immediate_val;
                                          needsInternalCycles = true;
                                          requiredCycles = 2;
                                    } else {
                                          isSameCycle = true;
                                    }
                                    break;
                              default:
                                    cout << "This branching instruction does not exist" << endl;
                                    cout << "$t :" << (int)designated_reg << endl;
                                    break;
                        }
                        break;
                  case 0b101:
                        //load word (lw)
                        //loads value from mem location t_reg into d_reg
                        //cout << "loading word \n";
                        (mem) -> memStartFetch(regs[target_reg],1);
                        break;
                  case 0b110:
                        //store word (sw)
                        //stores the value from s_reg into the mem location at t_reg
                        //cout << "storing word \n";
                        //A very hacky way of only having one val stored
                        uint8_t in_data[1];
                        in_data[0] = regs[source_reg];
                        (mem) -> memStartStore(regs[target_reg],in_data,1);
                        break;
                  case 0b111:
                        //halt
                        currentState = HALT;
                        break;
                  default:
                        cout << "This instruction does not exist" << endl;
                        cout << "PC :" << (int)PC << endl;
                        break;
            }

            //cout << "now waiting" << endl;
            if (currentState != HALT){
                  currentState = WAIT;
            }
            break;
      case WAIT:
            moreCycleWork = false;

            if (isSameCycle){
                  currentState = IDLE;
            }else if (needsInternalCycles){
                  internalCycleCount ++;
                  if (internalCycleCount >= requiredCycles){
                        currentState = IDLE;
                  }
            }else if ((mem) -> isFetchComplete()){
                  //returns value
                  //cout << "FETCH TASK COMPLETE" << endl;
                  uint8_t* fetch_results = (mem) -> endFetch();
                  regs[designated_reg] = fetch_results[0];
                  currentState = IDLE;
            } else if ((mem) -> isStoreComplete()){
                  //returns value
                  //cout << "STORE TASK COMPLETE" <<endl;
                  (mem) -> endStore();
                  currentState = IDLE;
            }
            break;
      case HALT:
            //cpu will no longer read instructions during ticks
            moreCycleWork = false;
            break;
      default:
            //something went wrong if you're here
            break;
      }
}

bool CPU::isMoreCycleWorkNeeded(){
      return moreCycleWork;
}





// Old doCycleWorkCode

//for (int i = 7; i >= 0; i--){
            //regs[i] = regs[i-1];
      //}
      //regs[0] = (mem) -> memStartFetch(PC);

      //cout << "Do Cycle Work ran: "<< (int)PC << endl;

      //PC ++;
