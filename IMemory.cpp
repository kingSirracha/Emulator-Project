#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include "IMemory.h"
using namespace std;


void IMemory::create(uint16_t mem_size){
      mem = (uint32_t *) calloc(mem_size, sizeof(uint32_t));
      size = mem_size;
}
void IMemory::reset(){
      for(int i = 0; i < size; i++){
            mem[i] = 0;
      }
      //cout << "The mem size is 0x" << uppercase << hex << (int)size << endl;
}
uint32_t IMemory::memStartFetch(uint16_t hex_addr){
      return mem[hex_addr];
}

//TODO Change the inputs of this here
void IMemory::set(uint16_t hex_addr, string filename){
      ifstream myFile (filename);
      if (myFile.is_open()) {
            string line;
            int i = 0;
            while (getline (myFile, line)){
                  int temp = stoi(line, 0, 16);
                  mem[i] = temp;
                  //cout << "0x" << uppercase << hex << (int)temp << endl;
                  i++;
            }

      }

}
void IMemory::dump(uint16_t hex_addr, uint16_t hex_count){
      uint8_t count = 0x00;
      uint8_t indent_check = -1;
      uint8_t address_tracker = 0x10;
      cout << "Addr     0     1     2     3     4     5     6     7";
      while (count < hex_count + hex_addr){
            if ((count >> 3) != indent_check){
                  cout << endl;
                  indent_check += 1;
                  cout << "0x" << uppercase << hex << (int)address_tracker << " ";
                  address_tracker += 8;
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


//uint8_t temp_array[15] = {0x2A,0x11,0x9E,0x23,0x55,0x2A,0x15,0x9E,0x03,0x55,0x2A,0x01,0x9E,0x23,0xA5};
