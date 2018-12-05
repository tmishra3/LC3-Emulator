The purpose of this mp was to take an array of LC-3 machine commands and
emulating them through an LC-3 simulator. THe simulator runs through a certain
configuration and a defined number of steps. Firstly, the simualator takes the
starting address given within the arguments and starts to copy the program into
an array representing the memory. Another array, which represents the registers
,is set to 111 in binary for the PSR and set the starting location for the PC.
The PC is presented by location 8 and the PSR is represented by location 9.Once
the program is copied into memory,a while loop starts to pick out each line and
starts to execute them. The opcode is first determined by taking the >> by 12.
Once that is found, we can run the appropriate execute_... function. The
function will then do what it needs to do by manipulating the register and
memory array and by using the get_... functions. The PC is changed at the end
depending on whether the function is linear or can branch. When the program
returns back to the simulate function via return, its return value is checked.
If the return value is 0, the while loop is broken out of with an appropriate
error being displayed from the terminate() function. If the while loop detects
the value of 0x0F025, it means we have detected a trap and the while loop is
quit of.
