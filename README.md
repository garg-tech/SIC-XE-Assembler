# SIC-XE-Assembler

> The objective is to implement a version of a two-pass SIC/XE assembler. The Assembler implemented includes all the SIC/XE instructions and supports all four formats 1, 2, 3, 4, addressing modes, and program relocation. It also supports a machine-independent feature i.e., program blocks.

## Table of Contents
* [Source Files](#source-files)
* [Input](#input)
* [Output](#output)
* [How to run](#how-to-run)
* [Contact](#contact)

## Source Files
There are four source files:
* Tables.cpp (Contains OPTAB, SYMTAB, REGTAB and Block Table)
* Utility.cpp (Contains functions that would be useful during pass1 and pass2 implementation)
* Pass1.cpp (Execution of pass 1)
* Pass2.cpp (Execution of pass 2)

## Input

Assembler source program using the instruction set of SIC/XE stored as a text file i.e., 'input1.txt'

## Output

The Assembler will generate the following files as output:

#### After Pass1 : 
  1. Symbol Table (SYMTAB) i.e., 'tables_input1.txt'
  2. Intermediate file i.e., 'intermediate_input1.txt'

#### After Pass2 : 
  1. Listing file (it will contain the input assembly code with address, block number, and object code of each instruction) i.e., 'listing_input1.txt'
  2. Object program (includes H, T, M, and E records) i.e., 'object_input1.txt'
  3. Error file (displays the errors in the assembly program, if any) i.e., 'error_input1.txt'

## How To Run

1. Open command prompt/terminal in the same directory as the source files.
2. Type the following command - g++ -std=c++11 pass2.cpp -o ./assembler.out  
   (It will generate an object file named 'assembler.out' in the current directory)  
   (You can generate the 'assembler.out' file in any directory as long as it is in the same directory as the input file)
4. Now run the assembler.out file by typing the following command - '.\assembler.out' (For cmd - windows)  
   &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&nbsp;'./assembler.out' (For terminal - MAC/Linux)
5. Type the name of the input file (with extension), in our case 'input1.txt', to generate the object code file i.e., 'object_input1.txt' and all the additional files.  
   (The files will be generated in the same folder)

## Contact
Created by Devansh Garg - feel free to contact me!  
dgarg1806@gmail.com
