/**************************************************************************************
 * 
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 *
**************************************************************************************/

This folder includes files for use with the Arduino DUE:
"node_2_linux" include the files neccecarry to get started with ATsam3X8E on Linux 
"uart_and_printf" includes a simple interface for using printf via UART on the Arduino DUE. 
"can" include a simple can library for ATsam3X8E, feel free to adapt it to your needs.

NOTE: A known bug in the printf library is that the compiler crashes if you put "\n", "\r" etc. in the string. 
A quick fix is to use printf(" ... %c\n\r",' '), which insert a space at the end of the line.  

Remember to import the files in your code (and include them in the Makefile if you are using make)