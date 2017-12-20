//
//  main.cpp
//  SMC-Assember
//
//  Created by MuMhu on 02/8/2560 BE.
//  Copyright © 2560 MuMhu. All rights reserved.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

//ตรวจสอบว่า parameter เป็นเลขหรือเป็น symbolic
bool isNumber(string s){
    return s.find_first_not_of( "-0123456789" ) == string::npos;
}

int getValue(string temp, map<string,string> label){
    int value;
    while(!isNumber(temp)){
        auto pos = label.find(temp);
        if(pos == label.end())
        {
            cout<<"undefine label : "<<temp<<endl;
            //undfine label
            exit(1);
        }
        else temp = label.at(temp);
    }
    value = stoi(temp);
    if(value>=0){
        return value;
    }else{
        return 65536+value;
    }
}

//ตรวจว่าparameterหน้าสุดเป็น opcode ที่ถูกต้องหรือไม่
bool isNotOp(string token){
    string opcode[8] = {"add","nand","lw","sw","beq","jalr","halt","noop"};
    for(int i=0;i<8;i++){
        if(token==opcode[i])return false;
    }
    return true;
    
}

bool isLabel(string token, map<string,string> label){
    auto pos = label.find(token);
    if(pos == label.end())return false;
    else return true;
}


int main(int argc, const char * argv[]) {
    map<string,string> label;
    ifstream file;
    file.open("/Users/WaveToMe/Desktop/SMC-Assember/test1.txt");
    string line;
    string partial;
    string parameter[5];
    int count = 0;
    int address = 0;
    int output;
    while(getline(file, line)) {
        string temp;
        istringstream iss(line);
        string token;
        count = 0;
        //อ่านรอบแรกเก็บ Label
        while(iss >> token){
            parameter[count]=token;
            count++;
            if(count>=3)break;
        }
        if(isNotOp(parameter[0])){
            auto pos = label.find(parameter[0]);
            if(pos == label.end())
            {
                label.insert(pair<string,string>(parameter[0],to_string(address)));
            }
            else {
                cout<<"Used label."<<endl;
                exit(1);
            }

        }
        address++;
    }
    
    //อ่านรอบที่สองเพือ compile
    file.close();
    file.open("/Users/WaveToMe/Desktop/SMC-Assember/test1.txt");
    ofstream myfile ("/Users/WaveToMe/Desktop/SMC-Assember/output.txt");
    address = 0;
    while(getline(file, line)) {
        istringstream iss(line);
        string token;
        count = 0;
        output = 0;
        while(iss >> token){
            if(!(isLabel(token, label)&&count==0)){
                parameter[count]=token;
                count++;
            }
            if(count>=5)break;
        }
        if(parameter[0]=="add"){
            output = (0<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16)+(getValue(parameter[3], label));
            myfile<<output<<endl;
        }
        else if(parameter[0]=="nand"){
            output = (1<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16)+(getValue(parameter[3], label));
            myfile<<output<<endl;
        }
        else if(parameter[0]=="lw"){
            if((getValue(parameter[3], label)) > 32767 || (getValue(parameter[3], label)) < -32768){
                //offset out of range
                cout<<"invalid offset"<<endl;
                exit(1);
            }
            output = (2<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16)+(getValue(parameter[3], label));
            myfile<<output<<endl;
        }
        else if(parameter[0]=="sw"){
            if((getValue(parameter[3], label)) > 32767 || (getValue(parameter[3], label)) < -32768){
                cout<<"invalid offset"<<endl;
                //offset out of range
                exit(1);
            }
            if((getValue(parameter[3], label)) == address){
                cout<<"same label"<<endl;
                // use same label
                exit(1);
            }
            output = (3<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16)+(getValue(parameter[3], label));
            myfile<<output<<endl;
        }
        else if(parameter[0]=="beq"){
            if((getValue(parameter[3], label)) > 32767 || (getValue(parameter[3], label)) < -32768){
                cout<<"invalid offset"<<endl;
                exit(1);
            }
            output = (4<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16);
            
            if(isLabel(parameter[3], label)){
                output+=getValue(to_string(getValue(parameter[3], label)-address-1), label);
            }
            else{
                output+=(getValue(parameter[3], label));
            }
            myfile<<output<<endl;
        }
        else if(parameter[0]=="jalr"){
            if((getValue(parameter[3], label)) == address){
                cout<<"same label"<<endl;
                exit(1);
            }
            output = (5<<22)+(getValue(parameter[1], label)<<19)+(getValue(parameter[2], label)<<16);
            myfile<<output<<endl;
        }
        else if(parameter[0]=="halt"){
            output = (6<<22);
            myfile<<output<<endl;
        }
        else if(parameter[0]=="noop"){
            output = (7<<22);
            myfile<<output<<endl;
        }
        else if(parameter[0]==".fill")myfile<<parameter[1]<<endl;
        else {
            cout<<"invalid opcode"<<endl;
            exit(1);
        }
        address++;
    }
    file.close();
    myfile.close();
    //program ended.
    exit(0);
    return 0;
}


