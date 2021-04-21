# Code for Project 1: Implementing RISC-V Assembler

## Code description: 
  Convert risc-v assembly code to machine language.


### Details:
1. Only supports RV32I instructions (minus the Fence instructions). 
2. Reads input from a text file, outputs the results to screen.
3. Format: [label:] (space) name (space) operands,operands,operands,...
    for example: L2: addi, x1,x1,x0    (note that the operands don't have spaces between them)
    also: this code only supports jalr instructions written as: jalr rd,imm(rs1)
4. Does NOT support assembly programs larger than 100 lines; this amount is arbitrary and exists simply because I didn't bother to make dynamic storage

Note! This program DOES support all other programs! (In theory, I have not done intensive testing to ensure the accuracy of the conversions)

Terminal command: ./a.out riscv.txt
(replace "riscv.txt" with whatever filepath you use)

    example code (as given in the homework description):   
        add x2,x2,x23   
        addi x24,x24,2   
        bne x24,x23,L2   
        sw x27,0(x10)   
        beq x0,x0,L1   
    L2: sw x1,0(x2)   
    L1: addi x24,x24,1   

   
------------ Everything above this line is what you need to operate the code. ------------

### How this program works: Uses class constructors to generate the correct machine code for each instruction.
<pre>
Summary for main.cpp: 
    1. Read entire text file, store within an array. While reading, detect if there are labels present in front, save labels into a separate array.  
    2. Iterate through the saved instructions. In each iteration, do the following:
        i. Separate each line of code into 2 ~ 3 parts (label, name, operands)         
        ii. If the instruction uses labels, calculate the offset (using the label array)    
        iii. Create a Inst object, then call the replaceInst() function and immediately swap it with object of the appropriate subclass    
        iv. Call the generateBinary() function and output the machine code.     
When the constructor for a subclass object of Inst is called, it immediately generates the correct machine code.  
######(Cleanup of the code should happen as soon as is convenient)   

