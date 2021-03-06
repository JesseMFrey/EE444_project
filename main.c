//*****************************************************************************************
//  Using ARC_lib <-- uses port 1
//
//
//*****************************************************************************************
#include <msp430.h> // prune these 
//#include <msp430f6779a.h>
#include <ctl.h>
#include <i2c.h>              
#include <ARCbus.h>
#include <Error.h>
#include <terminal.h>      
#include <string.h>           // for memset function
#include <UCA2_uart.h>        // UART setup 



CTL_TASK_t terminal_task; // name your task (first thing to do when setting up a new task (1))

//********************************************* allocate mem for tasks (2)
//stack for terminal
unsigned terminal_stack[2000];

//******************************************** redefine putchar and getchar 
//make printf and friends use async
int __putchar(int c){
  return UCA2_TxChar(c);
}

//make printf and friends use async
int __getchar(void){
  return UCA2_Getc();
}

//******************************************* Example_Bare_bones main loop
void main(void){
  //DO this first
  ARC_setup();

  //turn on LED's this will flash the LED's during startup
  P7DIR=0xFF;
  //init complete turn on LED0 and all others off
  P7OUT=0xFF;

  //TESTING: set log level to report everything by default
  set_error_level(0);

  //initialize UART
  //UCA2_init_UART(UART_PORT,UART_TX_PIN_NUM,UART_RX_PIN_NUM);
  UCA2_init_UART(3,5,6);

  //setup bus interface
  initARCbus(0x14);   // Default addr for "SYS" subsystem, should be changed for specific subsystems.
  
  //init I2C on P4.5 SDA and P4.4 SCL
  initI2C(4,5,4);

  // initialize stacks (3) 
  memset(terminal_stack,0xcd,sizeof(terminal_stack));                                           //write known values into the stack 
  terminal_stack[0]=terminal_stack[sizeof(terminal_stack)/sizeof(terminal_stack[0])-1]=0xfeed;  //put marker values at the words before/after the stack


  // creating the tasks
  ctl_task_run(&terminal_task,BUS_PRI_LOW,terminal,"EE444 IMU Project code","terminal",sizeof(terminal_stack)/sizeof(terminal_stack[0])-2,terminal_stack-1,0);

  //main loop <-- this is an ARCbus function 
  mainLoop(); 

}

//decode errors
char *err_decode(char buf[150], unsigned short source,int err, unsigned short argument){
  sprintf(buf,"source = %i, error = %i, argument = %i",source,err,argument);
  return buf;
}
