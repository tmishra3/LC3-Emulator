/*
mp4.2
Tanmay Mishra
ECE 190
Section AD3
*/

#include <stdio.h>
#include "mp4.2.h"
#include <stdlib.h>


/*void simulate (int start_addr, int num_instructions, unsigned short
instructions[]);*/
unsigned short Get_Address (int bits, unsigned short instruction);
int Check_Correct_Format (int opcode, unsigned short instruction);
int Check_Memory_Bound (int base_address, int offset);
int Check_Memory_Bound_DR (int base_address, int offset);
int get_Signed (int number, int bits);/*Returns the signed value of a number
when an unsigned number is entered.*/
unsigned short Get_NZP (unsigned short instruction);
unsigned short Get_Destination_Register (unsigned short instruction);
unsigned short Get_Source1_Register (unsigned short instruction);
unsigned short Get_Source2_Register (unsigned short instruction);
unsigned short Get_Source_Register (unsigned short instruction);
unsigned short Get_TrapVect8 (unsigned short instruction);
unsigned short Get_Imm5 (unsigned short instruction);
void set_PSR (int number);/*Sets the value of the PSR depending on the last
value obtained.*/
void Terminate (int error);/*Displays the various error codes or displays
the HALT display.*/
int execute_ADD (unsigned short instruction);/*DONE*/
int execute_AND (unsigned short instruction);/*DONE*/
int execute_BR (unsigned short instruction);/*DONE*/
int execute_JMP (unsigned short instruction);/*DONE*/
int execute_JSR (unsigned short instruction);/*DONE*/
int execute_JSRR (unsigned short instruction);/*DONE*/
int execute_LD (unsigned short instruction);/*DONE*/
int execute_LDI (unsigned short instruction);/*DONE*/
int execute_LDR (unsigned short instruction);/*DONE*/
int execute_LEA (unsigned short instruction);/*DONE*/
int execute_NOT (unsigned short instruction);/*DONE*/
int execute_RET (unsigned short instruction);/*DONE*/
int execute_ST (unsigned short instruction);/*DONE*/
int execute_STI (unsigned short instruction);/*DONE*/
int execute_STR (unsigned short instruction);/*DONE*/
int execute_TRAP (unsigned short instruction);/*DONE*/

/*
*******************************************************************************
***************************Global Variable Declarations.***********************
*******************************************************************************

Contains memory locations x0000 to xFFFF.
*/
int memory[65536] = {0};

/*
Contains registers R0 to R7, PC, and PSR sequentially.
PC = register[8]
PSR = regsiter[9]
*/
int registers[10] = {0};

int  *current_location;/*Points at the current location we are reading in the
array.*/
int *start_location;/*Points at the variable, start_addr.*/
int *num_location;/*Points at the variable, num_instruction.*/

/*
*******************************************************************************
*******************************************************************************
*******************************************************************************
*/

/*Used for the memory dump.*/
unsigned short read_memory (unsigned short addr)        {
       return memory[addr];
}

/*Returns the 5 least significant bits.*/
unsigned short Get_Imm5 (unsigned short instruction) {

       int temp = instruction & 31;
       return temp;

}

/*Returns the 8 least significant bits.*/
unsigned short Get_TrapVect8 (unsigned short instruction) {

       int temp = instruction & 255;
       return temp;

}

/*Returns bits 8 to 6*/
unsigned short Get_Source_Register (unsigned short instruction) {

       int temp = instruction & 448;
       temp = temp >> 6;
       return temp;

}

/*
Returns the bits 2 to 0 as a decimal.
*/

unsigned short Get_Source2_Register (unsigned short instruction) {

       int temp = instruction & 7;
       return temp;

}

/*
Returns the bits 8 to 6 as a decimal.
*/


unsigned short Get_Source1_Register (unsigned short instruction) {

       int temp = instruction & 448;
       temp = temp >> 6;
       return temp;

}

/*
Returns the bits 11 to 9 as a decimal.
*/


unsigned short Get_Destination_Register (unsigned short instruction) {

       int temp = instruction & 3584;
       temp = temp >> 9;
       return temp;

}

/*
Returns bits 11 to 9.
*/
unsigned short Get_NZP (unsigned short instruction) {

       int temp = instruction & 3584;
       temp = temp >> 9;
       return temp;

}

unsigned short Get_Address (int bits, unsigned short instruction) {

       int temp;

       if (bits==9)
       {
               temp = instruction & 511;
               temp = get_Signed (temp, 9);
       }

       if (bits==11)
       {
               temp = instruction & 2047;
               temp = get_Signed (temp, 11);
       }

       return (*start_location+*current_location+temp+1);


}

/*
Sets the PSR depending on what the value of the Destination registers was.
*/
void set_PSR (int number) {

       if (number<0)
               registers[9] = 4;
       else if (number==0)
               registers[9] = 2;
       else if (number>0)
               registers[9] = 1;

}

/*
Returns the value of an integer in its signed format as a 2's complement's
number if it is entered as a regular unsigned number.
*/

int get_Signed (int number, int bits) {

       int power=1;

       int cycle;

       for (cycle=1;cycle<bits;cycle++)
       {
               power*=2;
       }

       int temp = number & power;
       int temp2 = 0xFFFFFFFF;
       temp2 = temp2 >> (bits);
       temp2 = temp2 << (bits);

       temp = temp >> (bits-1);

       if (temp==0)
               return number;
       else
       {
               number=(~(number+temp2)*-1)-1;
               return number;
       }
}
/*
Allows you to display the various error messages and also displays the
LC-3 Halt display.
*/
void Terminate (int error) {

       switch (error)  {

               case 0:
                       printf("Illegal instruction.\n");
                       break;
               case 1:
                       printf("Illegal TRAP.\n");
                       break;
               case 2:
                       printf("Illegal memory access.\n");
                       break;
               case 3:
                       printf("Halting the LC-3.\n");
       }
}


/*
Once the opcode and the instruction is entered, this function will return an
integer that signifies whether the function is in its primary form, its
secondary form or if it is in an incorrect form. If it is in its primary form
the function returns 1, if in its secondary form, it returns 2 and if it is in
the incorrect form, returns a 0.
*/

int Check_Correct_Format (int opcode, unsigned short instruction) {

       unsigned short temp = instruction;
       unsigned short temp2 = instruction;
       unsigned short temp3;
       unsigned short temp4;

       switch (opcode) {

               case 1:
                       temp = temp & 32;
                       temp = temp >> 5;
                       temp2 = temp2 & 24;
                       temp2 = temp2 >> 3;
                       if ((temp==0) && (temp2==0))/*bits 5 to 3 are being
checked.Regular ADD function.*/
                               return 1;/*bit 5 is being checked. Alternate
ADD function.*/
                       else if (temp==1)
                               return 2;
                       else
                               return 0;
                       break;
               case 5:
                       temp = temp & 32;
                       temp = temp >> 5;
                       temp2 = temp2 & 24;
                       temp2 = temp2 >> 3;
                       if ((temp==0) && (temp2==0))/*bits 5 to 3 are being
checked.Regular ADD function.*/
                               return 1;/*bit 5 is being checked. Alternate
ADD function.*/
                       else if (temp==1)
                               return 2;
                       else
                               return 0;
                       break;
               case 12:
                       temp = temp & 3584;
                       temp = temp >> 9;
                       temp2 = temp2 & 63;
                       if ((temp==0) && (temp2==0))/*bits 11 to 9 and bits
5 to 0 are being checked to see if they are 0.*/
                               return 1;
                       else
                               return 0;
                       break;
               case 4:
                       temp3 = temp;
                       temp4 = temp;

                       temp3 = temp3 & 3584;
                       temp3 = temp3 >> 9;
                       temp4 = temp4 & 63;

                       temp = temp & 2048;
                       temp = temp >> 11;
                       if (temp==1)/*If bit 11 is 1, we are looking at JSR.*/
                               return 1;
                       else if ((temp3==0) && (temp4==0))/*If bits 11 to 9 and
bits bits 5 to 0 are 0, we are looking at JSRR.*/
                               return 2;
                       else
                               return 0;
                       break;
               case 9:
                       temp = temp & 63;
                       if (temp==63)/*Checking bits 5 to 0 to see if they are
1.*/
                               return 1;
                       else
                               return 0;
                       break;
               case 15:
                       temp = temp & 3840;
                       temp = temp >> 8;
                       if (temp==0)/*Checking to see whether or not bits 11 to
8 are 0.*/
                               return 1;
                       else
                               return 0;
                       break;
               default:
                       return 0;
                       break;

       }

}

/*
Checks to see whether out offset will be within the range of the LC-3 memory.
If it is not, we return a 0.
*/

int Check_Memory_Bound (int base_address, int offset) {

       /*int max = *num_location+*start_location-1;*/
       int bound = base_address+1+offset;

       if ((bound>0xFFFF) || (bound<0x0))
               return 0;
       else
               return 1;

}

/*
Special modification of the Check_Memory_Bound for the
LDR and STR functions.
*/

int Check_Memory_Bound_DR (int base_address, int offset) {

       /*int max = *num_location+*start_location-1;*/
       int bound = base_address+offset;

       if ((bound>0xFFFF) || (bound<0x0))
               return 0;
       else
               return 1;

}

/*
This function takes in the arguments as start address, the number of
instructions, and an array of instructions. It first starts out by
copying all the instructions from the array of instructions into the
valid locations into memory. From there, the register array value that
represents the PSR is set to the 111 and the PC is set to the starting
address. A while loop is initiated that checks whether we have or have
not reached 0xF025 (HALT). As long as we have not, we continue to process
the instructions.
*/


void simulate (int start_addr, int num_instruction, unsigned short
instruction[]){

       int cycle=0;

       for (cycle=0; cycle<num_instruction; cycle++)   {
               memory[start_addr+cycle]=instruction[cycle];
       }

       registers[8]=start_addr;

       unsigned short single_instruction;
       unsigned short opcode;
       int PC = start_addr;
       current_location = &PC;
       start_location = &start_addr;
       num_location = &num_instruction;
       int error;
       registers[9]=7;


/************************************************/
/*int a=0;*/
/************************************************/

       while (memory[PC]!=0xF025)
       {
               single_instruction = memory[PC];/*Copying one instruction
from the memory.*/
               opcode = single_instruction >> 12;/*Isolating the 4 most
significant bits.*/

               switch (opcode)
               {
                       case 1:
                               error = execute_ADD(single_instruction);
                               break;
                       case 5:
                               error = execute_AND(single_instruction);
                               break;
                       case 0:
                               error = execute_BR(single_instruction);
                               break;
                       case 12:
                               error = execute_JMP(single_instruction);
                               break;
                       case 4:
                               error = execute_JSR(single_instruction);
                               break;
                       case 2:
                               error = execute_LD(single_instruction);
                               break;
                       case 10:
                               error = execute_LDI(single_instruction);
                               break;
                       case 6:
                               error = execute_LDR(single_instruction);
                               break;
                       case 14:
                               error = execute_LEA(single_instruction);
                               break;
                       case 9:
                               error = execute_NOT(single_instruction);
                               break;
                       case 3:
                               error = execute_ST(single_instruction);
                               break;
                       case 11:
                               error = execute_STI(single_instruction);
                               break;
                       case 7:
                               error = execute_STR(single_instruction);
                               break;
                       case 15:
                               error = execute_TRAP(single_instruction);
                               break;
                       default:
                               Terminate(0);
                               error = 0;
               }
                       if (error==0)
                               break;

                       PC = registers[8];

/************************************************/

/*
TEST CODE
for (a=0; a<11; a++)
       printf("\n%X",registers[a]);
       printf("\n\n");*/
/************************************************/

       }

       if (memory[PC]==0xF025) {
               Terminate(3);

       }


/************************************************/

/*
TEST CODE
for (a=0; a<11; a++)
       printf("\n%X",registers[a]);
       printf("\n");*/
/************************************************/


       dump_memory (start_addr, start_addr+num_instruction);

}
/*
Executes the ADD function.
*/
int execute_ADD (unsigned short instruction) {

       int DR=0, SR1=0, SR2=0, temp=0;

       int immed = Check_Correct_Format (1, instruction);
       if (immed==0)/*If in the incorrect format, quit.*/
       {
               Terminate(0);
               return 0;
       }

       DR = Get_Destination_Register (instruction);
       SR1 = Get_Source1_Register (instruction);

       if (immed==2) {
               temp = Get_Imm5 (instruction);
               temp = get_Signed (temp, 5);/*Sign the Imm5 bits.*/
               registers[DR]=registers[SR1]+temp;
       }
       else
       {
               SR2 = Get_Source2_Register (instruction);
               registers[DR]=registers[SR1]+registers[SR2];
       }
       set_PSR (registers[DR]);
       registers[8]=registers[8]+1;
       return 1;
}
/*
Executes the AND function.
*/
int execute_AND (unsigned short instruction) {

       int DR=0, SR1=0, SR2=0, temp;

       int immed = Check_Correct_Format (1, instruction);
       if (immed==0)/*If in the incorrect format, quit out.*/
       {
               Terminate(0);
               return 0;
       }

       DR = Get_Destination_Register (instruction);
       SR1 = Get_Source1_Register (instruction);

       if (immed==2) {
               temp = Get_Imm5 (instruction);
               temp = get_Signed (temp, 5);/*Sign the Imm5 bits.*/
               registers[DR]=registers[SR1]&temp;
       }
       else
       {
               SR2 = Get_Source2_Register (instruction);
               registers[DR]=registers[SR1]&registers[SR2];
       }
       set_PSR (registers[DR]);/*Setting the PSR depending upon the
destination register value.*/
       registers[8]=registers[8]+1;/*Increment the PC*/
       return 1;
}

int execute_LD (unsigned short instruction) {
       int DR=0;
       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);/*Obtaining and signing the
Offset9.*/
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);/*Checking memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       int to_load_from = *current_location+1+temp3;
       registers[DR] = memory[to_load_from];
       set_PSR (registers[DR]);
       registers[8]=registers[8]+1;/*Increment the PC*/
       return 1;

}

int execute_ST (unsigned short instruction) {
       int DR=0;
       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);/*Obtaining and signing the
Offset9.*/
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);/*Checking memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       int to_load_from = *current_location+1+temp3;
       memory[to_load_from]=registers[DR];
       registers[8]=registers[8]+1;/*Increment the PC*/
       return 1;

}

int execute_JMP (unsigned short instruction) {

       int temp = Check_Correct_Format (12, instruction);
       if (temp==0)/*Check for the correct format.*/
       {
               Terminate(0);
               return 0;
       }
       int BR = Get_Source1_Register(instruction);

       registers[8] = registers[BR];/*Jump to the location
specified by the register BR.*/
       return 1;

}
/*
Special case of JMP function except it will always use R7.*/

int execute_RET (unsigned short instruction) {

       int temp = Check_Correct_Format (12, instruction);
       if (temp==0)
       {
               Terminate(0);
               return 0;
       }
       registers[8] = registers[7];
       return 1;

}


int execute_LDR (unsigned short instruction) {
       int DR=0, BR=0;
       int temp3 = instruction & 63;
       temp3 = get_Signed(temp3, 6);/*Obtaining and signing the Offset6
value.*/
       BR = Get_Source_Register(instruction);
       int temp2 = Check_Memory_Bound_DR(registers[BR],
       temp3);
       if (temp2==0)
       {
               Terminate(2);/*Display memory error.*/
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       int to_load_from = registers[BR]+temp3;
       registers[DR] = memory[to_load_from];/*Loading from a register
plus offset.*/
       set_PSR (registers[DR]);
       registers[8]=registers[8]+1;/*Incrementing the PC*/
       return 1;

}

int execute_STR (unsigned short instruction) {
       int SR=0, BR=0;
       int temp3 = instruction & 63;
       temp3 = get_Signed(temp3, 6);/*Obtaining and signing the Offset6.*/
       BR = Get_Source_Register(instruction);
       int temp2 = Check_Memory_Bound_DR (registers[BR],
       temp3);/*Checking memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       SR = Get_Destination_Register (instruction);
       int to_load_from = registers[BR]+temp3;
       memory[to_load_from] = registers[SR];/*Storing value of SR to the
memory of BR plus the offset.*/
       registers[8]=registers[8]+1;/*Incrementing the PC.*/
       return 1;

}

int execute_LDI (unsigned short instruction) {
       int DR=0;
       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);/*Obtaining Offset9 as a signed number.*/
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);/*Checking memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       int to_load_from = *current_location+1+temp3;
       int mem_to_load = memory[to_load_from];
       registers[DR] = memory[mem_to_load];/*Getting the memory of the memory
at a given offset.*/
       set_PSR (registers[DR]);
       registers[8]=registers[8]+1;/*Incrementing the PC.*/
       return 1;

}

int execute_STI (unsigned short instruction) {
       int SR=0;
       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);/*Obtaining Offset9 as a signed number.*/
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);/*Checking memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       SR = Get_Destination_Register (instruction);
       int to_load_from = *current_location+1+temp3;
       int mem_to_load = memory[to_load_from];
       memory[mem_to_load] = registers[SR];
       registers[8]=registers[8]+1;/*Incrementing the PC.*/
       return 1;

}

int execute_LEA (unsigned short instruction) {
       int DR=0;
       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);/*Checking the memory bound.*/
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       int to_load_from = *current_location+1+temp3;
       registers[DR] = to_load_from;/*Simply loading the address into the
register.*/
       set_PSR (registers[DR]);/*Setting the PSR.*/
       registers[8]=registers[8]+1;/*Incrementing the PC.*/
       return 1;

}

int execute_NOT (unsigned short instruction) {

       int DR=0, SR=0;
       int temp = Check_Correct_Format (9, instruction);/*Checking for correct
form.*/
       if (temp==0)
       {
               Terminate(0);
               return 0;
       }
       DR = Get_Destination_Register (instruction);
       SR = Get_Source_Register (instruction);
       int number = registers[SR];
       number=-number-1;/*Taking the NOT of the number.*/
       registers[DR]=number;
       set_PSR (registers[DR]);/*Setting the PSR.*/
       registers[8]=registers[8]+1;/*Incrementing the PC.*/
       return 1;
}

int execute_JSR (unsigned short instruction) {
       int check = Check_Correct_Format (4, instruction);/*Checking for the
correct format.*/
       int BR=0;
       if (check==2)/*If in the alternate form.*/
       {
               registers[7]=registers[8]+1;/*Saving the PC to R7.*/
               BR = Get_Source1_Register (instruction);
               registers[8]=registers[BR];/*Setting PC to value of BR.*/
               return 1;
       }
       else if (check==1)/*If in the first form.*/
       {
               registers[7]=registers[8]+1;
               int temp3 = instruction & 2047;/*Finding the offset11 and
signing it.*/
               temp3 = get_Signed(temp3, 11);
               int temp2 = Check_Memory_Bound (*current_location,temp3);
               if (temp2==0)/*Checking the memory bound.*/
               {
                       Terminate(2);
                       return 0;
               }
               registers[8]=(1+temp3+*current_location);/*Setting the PC
equal to the offset.*/
               return 1;
       }
       else
       {
               Terminate(0);
               return 0;
       }

}

/*
Our JSR function contains the code for JSRR so
we just go to JSR and use its return value and
return it.*/

int execute_JSRR (unsigned short instruction) {
       int ret = execute_JSR (instruction);
       return ret;
}

/*
Uses the value of NZP and PSR and ands them together. If atleast one
of them match (the value of z with z or p with p or n with n), then we get
a non zero value. This means we must skip to location specified by the
BR. If we receive a zero for NZP & PSR, it means that we have no conditions
in common so we just increment the PC by one.
*/

int execute_BR (unsigned short instruction) {
       int NZP = Get_NZP (instruction);
       if (NZP==0)/*The case of obtaining a BR, we just assume it to be an
NZP.*/
               NZP = 7;
       int PSR = registers[9];
       int decision = NZP&PSR;

       int temp3 = instruction & 511;
       temp3 = get_Signed(temp3, 9);
       int temp2 = Check_Memory_Bound (*current_location,
       temp3);
       if (temp2==0)
       {
               Terminate(2);
               return 0;
       }

       if (decision==0)        {
               registers[8] = registers[8] + 1;
               return 1;
       }
       else    {
               registers[8] = (*current_location+1+temp3);
               return 1;
       }
}

int execute_TRAP (unsigned short instruction) {

       int temp = Check_Correct_Format (15, instruction);
       if (temp==0)
       {
               Terminate(0);
               return 0;
       }


       int trap = Get_TrapVect8 (instruction);
       trap = trap&0xFF;

       if (trap==0x20) {/*GETC*/
               /*registers[8] = registers[8] + 1;*/
               char input;
               scanf("%c",&input);
               input=input&0x00FF;
               registers[0] = input;
               /*registers[8] = registers[7];*/
               registers[8] = registers[8] + 1;
               return 1;
       }
       else if (trap==0x21)    {/*OUT*/
/*              registers[7] = registers[8] + 1;*/
               printf("%c",registers[0]);
/*              registers[8] = registers[7];*/
               registers[8] = registers[8] + 1;
               return 1;
       }
       else if (trap==0x22)    {/*PUTS*/
/*              registers[7] = registers[8] + 1;*/
               int out_char = memory [registers[0]];
               int mem = registers[0];
               while (out_char!=0x0000)        {
                       printf("%c",out_char);
                       mem++;
                       out_char = memory[mem];
               }
               /*registers[8] = registers[7];*/
               registers[8] = registers[8] + 1;
               return 1;
       }
       else if (trap==0x23)    {/*IN*/
/*              registers[7] = registers[8] + 1;*/
               char input;
               scanf("%c",&input);
               input=input&0x00FF;
               printf("%c",input);
               registers[0] = input;
               registers[8] = registers[8] + 1;
/*              registers[8] = registers[7];*/
               return 1;
       }
       else if (trap==0x30)    {/*RAND*/
/*              registers[7] = registers[8] + 1;*/
               registers[0] = rand();
/*              registers[8] = registers[7];*/
               registers[8] = registers[8] + 1;
               return 1;
       }
       else if (trap==0x31)    {/*SEED*/
/*              registers[7] = registers[8] + 1;*/
               srand (registers[0]);
/*              registers[8] = registers[7];*/
               registers[8] = registers[8] + 1;
               return 1;
       }

       else    {/*If nothing else, display invalid Trap and quit.*/
               Terminate(1);
               return 0;
       }

}