//
//  main.cpp
//  RISC-V translator
//
//  Created by Daniel Tai on 2021/4/10.
//  Copyright © 2021 Daniel Tai. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "risc-v.h"
using namespace std;

//needs: opcode, operands, offset
//string translateInst(string opcode, string operands, int offset);
string replaceInst(Inst *&);

int main(int argc, const char * argv[]) {
    // insert code here...
    string inputs[100];
    string labels[100];
    string inputo;
    string output = "nothing";
    int linesTotal = 0;
    
    //cout << "begun" << endl;
    
    //test area
    /*
    string in = "2";//"x2,x2,x23";
    int num = stoi(in);
    cout << num << endl;
    
    //*/
    
    ifstream inFile;
    inFile.open(argv[1]);
    //cin >> inputo;//debug
    //inFile.open(inputo);
    //cout << "begun 1" << endl;
    
    if(!inFile.is_open())
    {
        cout << "unable to open file" << endl;
        return 0;
    }
    
    while(getline(inFile, inputo)) //while(getline(cin, inputo))
    {
        //cout << inputo << endl;
        inputs[linesTotal] = inputo;
        
        int col = -1;
        for(int l = 0; l < inputo.length(); l++)
        {
            if(inputo[l] == ':')
            {
                col = l;
                break;
            }
        }
        if(col != -1)
            labels[linesTotal] = inputo.substr(0,col);
        else
            labels[linesTotal] = "x";
        
        linesTotal++;
        
        if(linesTotal > 99)
        {
            cout << "This program doesn't currently support programs with more than 100 lines. Sorry!" << endl;
            return 0;
        }
    }
    /*
    cout << "labels?" << endl;
    for(int a = 0; a < linesTotal; a++)
    {
        cout << a << ": " << labels[a] << endl;
    }
    */
    //debug
    /*
    for(int i = 0; i < linesTotal; i++)
    {
        cout << inputs[i] << endl;
    }
    */
    //separation + decide if branch occurs
    string parts[3];
    for (int i = 0; i < linesTotal; i++)
    {
        string opcode = "";
        string operands = "";
        int offset = 0;
        
        stringstream input(inputs[i]);
        string part;
        int partCount = 0;
        
        while(input >> part)
        {
            parts[partCount] = part;
            partCount++;
        }
        
        //cout << parts[0] << endl;
        
        //separate
        int o = 0;
        if(partCount == 3)
        {
            o = 1;
        }
        opcode = parts[0+o];
        operands = parts[1+o];
        
        //find offset !! note that this is NOT the literal distance
        if(opcode[0] == 'b' || opcode == "jal")
        {
            //cout << "!find offset (currently unsupported)!/n offset set to -1" << endl;//
            //offset = -1;
            
            //find your label
            int loc = -1;
            int c = int(operands.length()-1);
            for(; c >= 0; c--)
            {
                if(operands[c] == ',')
                {
                    loc = c;
                    break;
                }
            }
            
            int len = int(operands.length() - loc);
            loc++;
            string targL = operands.substr(loc, len);
            //cout << targL << endl;//
            //find where it is
            int curr = i;
            int targ = -1;
            for(int s = 0; s < linesTotal; s++)
            {
                if(labels[s] == targL)
                {
                    targ = s;
                }
            }
            if(targ == -1)
            {
                cout << "label couldn't be found, whoops" << endl;
                cout << "maybe check input or sth, idk" << endl;
            }
            
            //calculate distance
            //store value
            offset = (targ - curr)*4; //already divided by 2
        }
        
        //cout << "opcode: |" << opcode << "| operands: |" << operands << "| offset: " << offset << endl;//
        
        //translate
        //cout << opcode << " " << operands << " " << offset << endl;
        //string output = translateInst(opcode, operands, offset);
        Inst * target = new Inst(opcode, operands, offset);
        //Inst instructi(opcode, operands, offset);
        //cout << replaceInst(target) << endl;
        replaceInst(target);
        output = target->generateBinary();
        //Inst * rinst = new RInst(target);//
        //cout << rinst->generateBinary() << endl;//
        //output
        //cout << i << ": " << output << endl;//
        cout << output << endl;
        //cout << "00000001011100010000000100110011" << endl;
        
    }
    
    inFile.close();
}

//should put this into risc-v.h if possible, shouldn't be in main
string replaceInst(Inst * &obj)
{
    string opcode = obj->getOpcode();
    string result = "nope";
    string types[37] = {"lui","auipc","jal","jalr","beq","bne","blt","bge","brtu","bgeu","lb","lh","lw","lbu","lhu","sb","sh","sw","addi","slti","sltiu","xori","ori","andi","slli","srli","srai","add","sub","sll","slt","sltu","xor","srl","sra","or","and"};
    
    int a = -1;
    for(int i = 0; i < 37; i++)
    {
        if(opcode == types[i])
        {
            //found = true;
            a = i;
            break;
        }
    }
    Inst * obja;
    switch (a)
    {
        case 0 ... 1://2
        {
            result = "u";//expression
            //obj = new
            obja = new UInst(obj);
        }
        break;
        
        case 2 ... 3://2
        {
            result = "uj";//expression
            obja = new UJInst(obj);
        }
        break;
        
        case 4 ... 9://6
        {
            result = "sb";//expression
            obja = new SBInst(obj);
        }
        break;
        
        case 10 ... 14://5
        {
            result = "i";//expression
            obja = new IInst(obj);
        }
        break;
        
        case 15 ... 17://3
        {
            result = "s";//expression
            obja = new SInst(obj);
        }
        break;
        
        case 18 ... 26://9
        {
            result = "i";//expression
            obja = new IInst(obj);
        }
        break;
        
        case 27 ... 36://10
        {
            result = "r";//expression
            obja = new RInst(obj);
        }
        break;
        
        default:
        {
            cout << "error: type not found" << endl;
            return 0;
        }
        break;
        }
        //delete obj;
        //cout << obja->generateBinary() << endl;
        obj = obja;
        //cout << result << en
    return result;
}
