# Emulator-Project
Emulator from my Computer Organization class which is meant to read in commands from a fantasy MTU computer. The main goal of the project was to come out with a better understanding of the inner workings of a computer by creating a simulation of them. This emulator has a working Instruction Memory, Memory, Cache, and CPU being able to take in a text file of commands and output what changes were made to the "device".
This was also my first major program written in C++, I tried to keep the code organized but there might be some segments that are less than ideal for either performance or readability.

# Running the Program
The code has already been compiled into out.exe but if you wish to compile again the command I used was "g++ *.cpp *.h -o out" without the quotes. If you wish to run the program it does take a text file as an argument so in powershell you'd use a command like "./out test4/test1.txt". For the input feel free to use any of the test text files I have floating around. I was not able to find the documentation given on the command structure so apologies if you wish to try running your own program.
