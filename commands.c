/**********************************************************************************************************************************************
The commands.c file is for commands that will be displayed through the serial terminal. 
In order to add a command you must create a function as seen below.
Then function must be added to the "const CMD_SPEC cmd_tbl[]={{"help"," [command]",helpCmd}" table at the end of the file.
**********************************************************************************************************************************************/
#include <msp430.h>   // prune these 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <terminal.h>
#include <commandLib.h>
#include <stdlib.h>
#include <ARCbus.h>
#include <SDlib.h>
#include <i2c.h>

//*********************************************************** passing arguments over the terminal *********************************************
int example_command(char **argv,unsigned short argc){
  int i,j;
  //TODO replace printf with puts ? 
  printf("This is an example command that shows how arguments are passed to commands.\r\n""The values in the argv array are as follows : \r\n");
  for(i=0;i<=argc;i++){
    printf("argv[%i] = 0x%p\r\n\t""string = \"%s\"\r\n",i,argv[i],argv[i]);
    //print out the string as an array of hex values
    j=0;
    printf("\t""hex = {");
    do{
      //check if this is the first element
      if(j!='\0'){
        //print a space and a comma
        printf(", ");
      }
      //print out value
      printf("0x%02hhX",argv[i][j]);
    }while(argv[i][j++]!='\0');
    //print a closing bracket and couple of newlines
    printf("}\r\n\r\n");
  }
  return 0;
}



//table of commands with help
const CMD_SPEC cmd_tbl[]={{"help"," [command]",helpCmd},
                   {"ex","[arg1] [arg2] ...\r\n\t""Example command to show how arguments are passed",example_command},

                   ARC_COMMANDS,CTL_COMMANDS,ERROR_COMMANDS, // add lib functions to the help list 
                   //end of list
                   {NULL,NULL,NULL}};

