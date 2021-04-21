//
//  risc-v.cpp
//  RISC-V translator
//
//  Created by Daniel Tai on 2021/4/10.
//  Copyright © 2021 Daniel Tai. All rights reserved.
//

#include "risc-v.h"

IInst::IInst(Inst * obj)
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "0010011";
    //int rd;
    int rs1;
    int imm;
    int f3 = 0;
    bool odd = 0;
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',' || operands[i+1] == '(' || operands[i+1] == ')' || i == (operands.length() - 1))
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            buffer = 0;
            rCount++;
        }
        
        buffer *= 10;
    }
    
    if(opcode[0] == 'l')
    {
        op[2] = '0';
        //read erands
        rs1 = reg[2];
        imm = reg[1];
        
        //f3
        string rs[5] = {"lb","lh","lw","lbu","lhu"};
        for(int i = 0; i < 5; i++)
        {
            if(opcode == rs[i])
            {
                f3 = i;
                if(i >= 3)
                f3 = i+1;
            }
        }
    }
    else
    {
        //read erands
        rs1 = reg[1];
        imm = reg[2];
        
        //f3
        string rs[9] = {"addi","slli","slti","sltiu","xori","srli","srai""ori","andi",};
        for(int i = 0; i < 9; i++)
        {
            if(opcode == rs[i])
            {
                f3 = i;
                if(i > 5)
                    f3 = i-1;
            }
        }
        
        if(f3 == 1 || f3 == 5)
            odd = true;
    }
    
    binary.replace(25, 7, op);
    
    string r = regiB(reg[0]); //rd, 5bits
    //cout << r << endl;
    binary.replace(20,5,r);
    r = regiB(rs1); //rs1
    //cout << r << endl;
    binary.replace(12,5,r);
    
    string func3 = bitset<3>(f3).to_string();
    binary.replace(17,3,func3);
    
    
    //start with the final imm
    string fimm = bitset<12>(imm).to_string();
    binary.replace(0,12,fimm);
    
    if(odd)
    {
        fimm = bitset<5>(imm).to_string();
        binary.replace(7,5,fimm);
        string mt = "0000000";
        binary.replace(0,7,mt);
        if(opcode == "srai")
            binary[1] = '1';
    }
}
/*
string IInst::generateBinary()
{
    
    binary =  "I1111111111111111111111111111111"; //to indicate
    return binary;
}
*/


SInst::SInst(Inst * obj)
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "0100011";//7
    binary.replace(25, 7, op);
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',' || operands[i+1] == '(' || operands[i+1] == ')')
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            buffer = 0;
            rCount++;
        }
        
        buffer *= 10;
    }
    
    string r = regiB(reg[0]); //rs2, 5bits
    //cout << r << endl;
    binary.replace(7,5,r);
    
    r = bitset<12>(reg[1]).to_string();//offset 12bits
    string imm1 = r.substr(0,7);//0 to 6
    string imm2 = r.substr(7,5);//7 to 12
    //cout << r << endl;
    binary.replace(0,7,imm1);
    binary.replace(20,5,imm2);
    
    r = regiB(reg[2]); //rs1
    //cout << r << endl;
    binary.replace(12,5,r);
    
    int f3 = 0;
    string rs[3] = {"sb","sh","sw"};
    for(int i = 0; i < 3; i++)
    {
        if(opcode == rs[i])
        {
            f3 = i;
        }
    }
    string func3 = bitset<3>(f3).to_string();
    binary.replace(17,3,func3);
}
/*
string SInst::generateBinary()
{
    return "S1111111111111111111111111111111"; //to indicate
    
}
*/


RInst::RInst(Inst * obj)//done
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "0110011";//7
    binary.replace(25, 7, op);
    string fun = "0000000";//7
    binary.replace(0, 7, fun);
    if(opcode == "sub" || opcode == "sra")
        binary[1] = '1';
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',' || i == (operands.length() - 1))
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            buffer = 0;
            rCount++;
        }
        
        buffer *= 10;
    }
    
    string r = regiB(reg[0]); //rd, 5bits
    //cout << r << endl;
    binary.replace(20,5,r);
    r = regiB(reg[1]); //rs1
    //cout << r << endl;
    binary.replace(12,5,r);
    r = regiB(reg[2]); //rs2
    //cout << r << endl;
    binary.replace(7,5,r);
    
    int f3 = 0;
    string rs[8] = {"sll","slt","sltu","xor","srl","sra","or","and"};
    for(int i = 0; i < 8; i++)
    {
        if(opcode == rs[i])
        {
            //found = true;
            if(i <= 5)
                f3 = i + 1;
            else
                f3 = i;
            break;
        }
    }
    
    string func3 = bitset<3>(f3).to_string();
    binary.replace(17,3,func3);
}
/*
string RInst::generateBinary()
{
    return binary; //to indicate
}
*/


UInst::UInst(Inst * obj)
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "0110111";//lui
    if(opcode == "auipc")
        op[1] = '0';
        
    binary.replace(25, 7, op);
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        //cout << operands << endl;
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << operands[i] << endl;
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',' || i == (operands.length() - 1) )
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            //cout << i << endl;
            //cout << '9' - 0 << endl;
            buffer = 0;
            rCount++;
            //break;
        }
        buffer *= 10;
    }
    
    //cout << reg[0] << ' ' << reg[1] << endl;
    string r = regiB(reg[0]); //rd, 5bits
    //cout << r << endl;
    binary.replace(20,5,r);
    
    string imm = bitset<20>(reg[1]).to_string();
    //cout << "u imm: " << imm << endl;//
    
    binary.replace(0,20,imm);
}
/*
string UJInst::generateBinary()
{
    return "UJ111111111111111111111111111111"; //to indicate
}
*/


SBInst::SBInst(Inst * obj)
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "1100011";
    binary.replace(25, 7, op);
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',')
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            buffer = 0;
            rCount++;
        }
        
        buffer *= 10;
    }
    
    string r = regiB(reg[0]); //rs1
    //cout << r << endl;
    binary.replace(12,5,r);
    r = regiB(reg[1]); //rs2
    //cout << r << endl;
    binary.replace(7,5,r);
    
    string imm = bitset<13>(offset).to_string();
    //cout << "branch imm: " << imm << endl;//
    binary[0] = imm[0];//12
    binary[24] = imm[1];//11
    string part = imm.substr(2,6);//10 to 5
    string part2 = imm.substr(8,4);//4 to 1
                                //0 is ignored
    binary.replace(1,6,part);
    binary.replace(20,4,part2);
    
    int f3 = 0;
    string rs[6] = {"beq","bne","blt","bge","brtu","bgeu"};
    for(int i = 0; i < 6; i++)
    {
        if(opcode == rs[i])
        {
            f3 = i;
            if(i >= 1)
                i += 2;
        }
    }
    string func3 = bitset<3>(f3).to_string();
    binary.replace(17,3,func3);
}
/*
string SBInst::generateBinary()
{
    return "SB111111111111111111111111111111"; //to indicate
}
*/


UJInst::UJInst(Inst * obj) //jal, jalr //jalr x0,0(x1)
:Inst(obj->getOpcode(), obj->getOperands(), obj->getOffset())
{
    string op = "1101111";
    //binary.replace(25, 7, op);
    //cout << "ui" << endl;
    if(opcode == "jalr")
        op[3] = '0';
    binary.replace(25, 7, op);
    
    //string func3 = bitset<3>(0).to_string();
    binary.replace(17,3,"000");
    
    int buffer = 0;
    int rCount = 0;
    for (int i = 0; i < operands.length(); i++) //shouldn't this be a function? //later
    {
        //int buffer;
        //cout << "oper: " << operands[i] << ' ' << operands[i] - 0 << endl;//
        if(operands[i] - 0 <= 57 && operands[i]- 0 >= 48)//if its a number
        {
            buffer += (operands[i] - 0 - 48);
            //cout << buffer << "opopo" << endl;
        }
        
        if(operands[i+1] == ',' || operands[i+1] == '(' || operands[i+1] == ')')
        {
            reg[rCount] = buffer;
            //cout << buffer << endl;//
            buffer = 0;
            rCount++;
        }
        
        buffer *= 10;
    }
    
    string r = regiB(reg[0]); //rd, 5bits
    //cout << r << endl;
    binary.replace(20,5,r);
    
    if(opcode == "jalr")
    {
        r = regiB(reg[2]); //rs1
        //cout << reg[0] << ' ' << reg[1] << ' ' << reg[2] << endl;
        //cout << r << endl;
        binary.replace(12,5,r);
        
        string imm = bitset<12>(reg[1]).to_string();
        binary.replace(0,12,imm);
    }
    else //jal
    {
        string imm = bitset<21>(offset).to_string();
        binary[0] = imm[0];
        binary[11] = imm[9];
        string part = imm.substr(10,10);//10 to 19
        string part2 = imm.substr(1,8);//1 to 8
                                    //0 is ignored
        binary.replace(1,10,part);
        binary.replace(12,8,part2);
        
    }
}
/*
string UInst::generateBinary()
{
    return "U1111111111111111111111111111111"; //to indicate
}
*/
