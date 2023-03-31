#ifndef IMEM_DEF
#define IMEM_DEF



#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
using namespace std;


class IMemory{
      private:
            uint32_t *mem;
            uint16_t size;
      public:
            void create(uint16_t mem_size);
            void reset();
            uint32_t memStartFetch(uint16_t hex_addr);
            void set(uint16_t hex_addr, string filename);
            void dump(uint16_t hex_addr, uint16_t hex_count);


};

#endif