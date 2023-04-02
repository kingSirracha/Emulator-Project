#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "Memory.h"
using namespace std;


Memory::Memory(){
      currentState = IDLE;
      counter = 0;
      ticksToComplete = 5;
      for(int i = 0; i < 8; i++){
            fetchVal[i] = 0;
            storeVal[i] = 0;
      }
}

void Memory::create(uint16_t mem_size){
      mem = (uint8_t *) malloc(mem_size);
      size = mem_size;
}
void Memory::reset(){
      for(int i = 0; i < size; i++){
            mem[i] = 0x00;
      }
}
void Memory::memStartFetch(uint8_t hex_addr, int count){
      taskRequested = FETCH;
      fetchComplete = false;
      storeComplete = false;
      data_ammount = count;
      //would have been false by now so it needs another kickstart to get going again
      moreCycleWork = true;
      for(int i = 0; i < count; i++){
            fetchVal[i] = mem[hex_addr + i];
      }
      //now waits for n ticks
      currentState = WAIT;
      //return mem[hex_addr];
}
uint8_t* Memory::endFetch(){
      fetchComplete = false;
      return fetchVal;
}

bool Memory::isFetchComplete(){
      return fetchComplete;
}

void Memory::memStartStore(uint8_t hex_addr , uint8_t* value, int count){
      taskRequested = STORE;
      storeComplete = false;
      fetchComplete = false;
      moreCycleWork = true;
      storeAddr = hex_addr;
      data_ammount = count;
      for(int i = 0; i < count; i++){
            storeVal[i] = value[i];
      }
      //now waits for n ticks
      currentState = WAIT;
      //return mem[hex_addr];
}

void Memory::endStore(){
      storeComplete = false;
      for(int i = 0; i < data_ammount; i++){
            mem[storeAddr + i] = storeVal[i];
      }
}

bool Memory::isStoreComplete(){
      return storeComplete;
}

void Memory::set(uint8_t hex_addr, uint8_t hex_count, uint8_t* values){
      //note, for the values var a temp var is needed to hold the input array when putting in the arg
      int val_count = 0;
      for(int i = hex_addr; i < (hex_addr + hex_count); i++){
            //cout << "setting address " << hex << i << " to " << hex << (int)values[val_count] << endl;
            mem[i] = values[val_count];
            val_count++;
      }
}
void Memory::dump(uint8_t hex_addr, uint8_t hex_count){
      uint8_t count = 0x00;
      uint8_t indent_check = -1;
      cout << "Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F";
      while (count < hex_count + hex_addr){
            if ((count >> 4) != indent_check){
                  cout << endl;
                  indent_check += 1;
                  cout << "0x" << uppercase << hex << (int)indent_check << "0 ";
            }
            //hex_count --;
            if (count >= hex_addr && count < size){
                  int temp = (int)mem[count]; //what will be used for printing values
                  if (temp < 16) {
                        cout << 0;
                  }
                  cout << uppercase << hex << temp << " ";
            } else {
                  cout << "   ";
            }
            count ++;
      }
      cout << "\n";
      cout << endl;
}
void Memory::start_tick(){
      //gets mem off of the Idle state to start processes
      moreCycleWork = true;
      //cout << "Mem Tick started" << endl;
      if (currentState == IDLE){
            //cout << "Mem Tick started" << endl;
            //we actually don't want to change the state here
            //we want it to change when cpu calls on a function of mem
            //currentState = WAIT;
      }
      //if waiting, increments the counter by 1
      if (currentState == WAIT){
            //cout << "+1 :" << counter << endl;
            counter ++;
            if (counter >= 4){
                  //cout << "Mem finished! \n";
                  counter = 0;
                  currentState = MOVEDATA;
            }
      }
}

void Memory::doCycleWork(){
      switch (currentState){
            case IDLE:
                  //waiting for start_tick to be called
                  moreCycleWork = false;
            break;
            case WAIT:
                  if(counter >= 7){
                        currentState = MOVEDATA;
                  } else {
                        moreCycleWork = false;
                  }
            break;
            case MOVEDATA:
                  switch (taskRequested){
                        case FETCH:
                              fetchComplete = true;
                        break;
                        case STORE:
                              storeComplete = true;
                        break;
                        default:
                              //program should not use this
                        break;
                  }
                  //cout << "mem moved data!" << endl;
                  //counter = 0;
                  moreCycleWork = false;
                  currentState = IDLE;
            break;
            default:

            break;
      }
}

bool Memory::isMoreCycleWorkNeeded(){
      return moreCycleWork;
}



//uint8_t temp_array[15] = {0x2A,0x11,0x9E,0x23,0x55,0x2A,0x15,0x9E,0x03,0x55,0x2A,0x01,0x9E,0x23,0xA5};
