#include <iostream>
#include <fstream>
#include <string>
//#include <stdlib.h>
//#include <iomanip>
#include "CPU.h"
#include "Memory.h"
#include "Clock.h"
#include "IMemory.h"
using namespace std;

string parse_line(string &line){
      string output;
      int split1 = line.find(" ");
      output = line.substr(0, split1);
      //chopping the used command off the line so it's easier to get the next param
      line = line.substr(split1 + 1, line.size() - split1 + 1);
      char hex_check = output[1];
      char x = 'x';
      if (hex_check == 'x'){
            output = output.substr(2,output.size() - 2);
            //cout << output << endl;
      }
      return output;
};

uint16_t string_to_hex(string in_string){
      int temp = stoi(in_string, 0, 16);
      return temp;
};


int main(int argc, char *argv[]) {
      Memory mem;
      IMemory imem;
      CPU cpu(&imem, &mem);


      Clock clock(&mem, &cpu);
      //cout << &cpu << endl;

      if (argc < 2) return 1; //to make sure there's an arg

      ifstream myFile (argv[1]); //creates an input file from the first arg
      string line;
      string command; //will hold the command information
      string sub_command;


      if (myFile.is_open()) {
            while (getline (myFile, line)){
                  int split1;
                  command = parse_line(line);
                  sub_command = parse_line(line);

                  if  (command == "clock"){
                        //do clock related commands
                        if (sub_command == "reset"){
                              clock.reset();
                        } else if (sub_command == "tick"){
                              //made an edit since the tick command takes in a decimal value not an int
                              int arg1 = stoi(parse_line(line),0,10);
                              clock.tick(arg1);
                        } else if (sub_command == "dump"){
                              clock.dump();
                        }
                  } else if  (command == "memory"){
                        //do memeory related commands
                        if (sub_command == "create"){
                              string first_arg = parse_line(line);

                              uint16_t temp = string_to_hex(first_arg);

                              mem.create(temp);
                        } else if (sub_command == "reset"){
                              mem.reset();
                        } else if (sub_command == "dump"){
                              uint8_t arg1 = string_to_hex(parse_line(line));
                              uint8_t arg2 = string_to_hex(parse_line(line));

                              mem.dump(arg1,arg2);

                              //uint8_t test = arg2;
                              //cout <<  (int)((uint8_t) 123) << endl;
                        } else if (sub_command == "set"){
                              uint8_t arg1 = string_to_hex(parse_line(line));
                              uint8_t arg2 = string_to_hex(parse_line(line));
                              uint8_t values[arg2];
                              for (int i = 0; i < arg2; i++){
                                    values[i] = string_to_hex(parse_line(line));
                              }
                              //uint8_t test[] = {0x43, 0xa3, 0x4b, 0x99, 0x11, 0x11};
                              //cout << "arg 1 is: "<< hex << (int)arg1 << endl;
                              //cout << "arg 2 is: "<< hex << (int)arg2 << endl;
                              mem.set(arg1,arg2,values);
                        }
                  } else if  (command == "cpu"){
                        //do cpu related commands
                        if (sub_command == "reset"){
                              cpu.reset();
                        } else if (sub_command == "set"){
                              parse_line(line);
                              string arg1 = parse_line(line);
                              uint8_t arg2 = string_to_hex(parse_line(line));
                              cpu.set_reg(arg1,arg2);

                        } else if (sub_command == "dump"){
                              cpu.dump();
                        }


                  } else if (command == "imemory"){
                        if (sub_command == "create"){
                              string first_arg = parse_line(line);

                              uint16_t temp = string_to_hex(first_arg);

                              imem.create(temp);
                        } else if (sub_command == "reset"){
                            
                              imem.reset();
                        } else if (sub_command == "dump"){
                              uint16_t arg1 = string_to_hex(parse_line(line));
                              uint16_t arg2 = string_to_hex(parse_line(line));

                              imem.dump(arg1,arg2);

                              //uint8_t test = arg2;
                              //cout <<  (int)((uint8_t) 123) << endl;

                        } else if (sub_command == "set"){

                              //TODO: Allow IMemory to accept a file for instructions

                              //input should be set(addr,filename)
                              uint16_t arg1 = string_to_hex(parse_line(line));
                              parse_line(line);
                              string testing = parse_line(line);
                              //cout << "filename is: " << testing << endl;
                              imem.set(arg1, testing);
                        }

                  }
                  //cout << "a command was ran" << endl;

            }

      } else {
            cerr << "error opening file" << argv[1] << endl;
            return 1;
      }

      //cout << "end of program" << endl;
}

