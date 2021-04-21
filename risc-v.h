//
//  risc-v.h
//  RISC-V translator
//
//  Created by Daniel Tai on 2021/4/10.
//  Copyright © 2021 Daniel Tai. All rights reserved.
//

#ifndef risc_v_h
#define risc_v_h

#include <iostream>
#include <string>
#include <bitset>
using namespace std;

class Inst
{
    public:
        Inst(string a, string b, int c)
        {
            opcode = a;
            operands = b;
            offset = c;
        }
        
        virtual string generateBinary()
        {
            return binary;
        }
        
        string getOpcode()
        {
            return opcode;
        }
        
        string getOperands()
        {
            return operands;
        }
        
        int getOffset()
        {
            return offset;
        }
        
        string regiB(int regi)//5 bits //this function is fine
        {
            //regi = 1;
            string res = bitset<5>(regi).to_string();
            //cout << "regiB test " << regi << " " << res << endl;
            return res;
        }
        
    protected:
        string opcode;
        string operands;
        int offset;
        string binary = "0000000000000inst000000000000000";
        int reg[4] = {0,0,0,0}; //rd, rs1, rs2 //added xtra incase of weird labels
        //               01234567890123456789012345678901
};

class IInst : public Inst
{
public:
    IInst(Inst *);
    //virtual string generateBinary();
};

class SInst : public Inst // sb, sh, sw
{
public:
    SInst(Inst *);
    //virtual string generateBinary();
};

class RInst : public Inst
{
public:
    RInst(Inst *);
    //virtual string generateBinary();
//private:
    //int reg[3] = 0; //rd, rs1, rs2
    //int rs2;
    //int rd;
    //string funct3;
};

class UJInst : public Inst
{
public:
    UJInst(Inst *);
    //virtual string generateBinary();
};

class SBInst : public Inst
{
public:
    SBInst(Inst *);
    //virtual string generateBinary();
};

class UInst : public Inst
{
public:
    UInst(Inst *);
    //virtual string generateBinary();
};
#endif /* risc_v_h */

