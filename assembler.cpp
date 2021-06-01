#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include "assembler.h"
using namespace std;

map<string, string> registerToBinaryNumber;
map<string, string> iOpToBinaryNumber;
map<string, string> rFunctToBinaryNumber;
map<string, string> jOpToBinaryNumber;
map<string, int> labelToAddress;

void buildRegisterMap() {
    registerToBinaryNumber["$zero"] = "00000";
    registerToBinaryNumber["$at"] = "00001";
    registerToBinaryNumber["$v0"] = "00010";
    registerToBinaryNumber["$v1"] = "00011";
    registerToBinaryNumber["$a0"] = "00100";
    registerToBinaryNumber["$a1"] = "00101";
    registerToBinaryNumber["$a2"] = "00110";
    registerToBinaryNumber["$a3"] = "00111";
    registerToBinaryNumber["$t0"] = "01000";
    registerToBinaryNumber["$t1"] = "01001";
    registerToBinaryNumber["$t2"] = "01010";
    registerToBinaryNumber["$t3"] = "01011";
    registerToBinaryNumber["$t4"] = "01100";
    registerToBinaryNumber["$t5"] = "01101";
    registerToBinaryNumber["$t6"] = "01110";
    registerToBinaryNumber["$t7"] = "01111";
    registerToBinaryNumber["$s0"] = "10000";
    registerToBinaryNumber["$s1"] = "10001";
    registerToBinaryNumber["$s2"] = "10010";
    registerToBinaryNumber["$s3"] = "10011";
    registerToBinaryNumber["$s4"] = "10100";
    registerToBinaryNumber["$s5"] = "10101";
    registerToBinaryNumber["$s6"] = "10110";
    registerToBinaryNumber["$s7"] = "10111";
    registerToBinaryNumber["$t8"] = "11000";
    registerToBinaryNumber["$t9"] = "11001";
    registerToBinaryNumber["$k0"] = "11010";
    registerToBinaryNumber["$k1"] = "11011";
    registerToBinaryNumber["$gp"] = "11100";
    registerToBinaryNumber["$sp"] = "11101";
    registerToBinaryNumber["$fp"] = "11110";
    registerToBinaryNumber["$ra"] = "11111";
}

void buildIOpMap() {
    iOpToBinaryNumber["addi"] = "001000";
    iOpToBinaryNumber["addiu"] = "001001";
    iOpToBinaryNumber["andi"] = "001100";
    iOpToBinaryNumber["ori"] = "001101";
    iOpToBinaryNumber["xori"] = "001110";
    iOpToBinaryNumber["lui"] = "001111";
    iOpToBinaryNumber["slti"] = "001010";
    iOpToBinaryNumber["sltiu"] = "001011";
    iOpToBinaryNumber["teqi"] = "01100";
    iOpToBinaryNumber["tgei"] = "01000";
    iOpToBinaryNumber["tgeiu"] = "01001";
    iOpToBinaryNumber["tlti"] = "01010";
    iOpToBinaryNumber["tltiu"] = "01011";
    iOpToBinaryNumber["tnei"] = "01110";
    iOpToBinaryNumber["beq"] = "000100";
    iOpToBinaryNumber["bgez"] = "000001";
    iOpToBinaryNumber["bgezal"] = "000001";
    iOpToBinaryNumber["bgtz"] = "000111";
    iOpToBinaryNumber["blez"] = "000110";
    iOpToBinaryNumber["bltzal"] = "000001";
    iOpToBinaryNumber["bltz"] = "000001";
    iOpToBinaryNumber["bne"] = "000101";
    iOpToBinaryNumber["lb"] = "100000";
    iOpToBinaryNumber["bu"] = "100100";
    iOpToBinaryNumber["lh"] = "100001";
    iOpToBinaryNumber["lbu"] = "100101";
    iOpToBinaryNumber["lw"] = "100011";
    iOpToBinaryNumber["lwl"] = "100010";
    iOpToBinaryNumber["lwr"] = "100110";
    iOpToBinaryNumber["ll"] = "110000";
    iOpToBinaryNumber["sb"] = "101000";
    iOpToBinaryNumber["sh"] = "101001";
    iOpToBinaryNumber["sw"] = "101011";
    iOpToBinaryNumber["swl"] = "101010";
    iOpToBinaryNumber["swr"] = "101110";
    iOpToBinaryNumber["sc"] = "111000";
}

void buildJOpMap() {
    jOpToBinaryNumber["j"] = "000010";
    jOpToBinaryNumber["jal"] = "000011";
}

void buidRFunctMap() {
    rFunctToBinaryNumber["add"] = "100000";
    rFunctToBinaryNumber["addu"] = "100001";
    rFunctToBinaryNumber["and"] = "100100";
    rFunctToBinaryNumber["clo"] = "100001";
    rFunctToBinaryNumber["clz"] = "100000";
    rFunctToBinaryNumber["div"] = "011010";
    rFunctToBinaryNumber["divu"] = "011011";
    rFunctToBinaryNumber["mult"] = "011000";
    rFunctToBinaryNumber["multu"] = "011001";
    rFunctToBinaryNumber["mul"] = "000010";
    rFunctToBinaryNumber["madd"] = "000000";
    rFunctToBinaryNumber["maddu"] = "000001";
    rFunctToBinaryNumber["msub"] = "000100";
    rFunctToBinaryNumber["msubu"] = "000101";
    rFunctToBinaryNumber["nor"] = "100111";
    rFunctToBinaryNumber["or"] = "100101";
    rFunctToBinaryNumber["sll"] = "000000";
    rFunctToBinaryNumber["sllv"] = "000100";
    rFunctToBinaryNumber["sra"] = "000011";
    rFunctToBinaryNumber["srav"] = "000111";
    rFunctToBinaryNumber["srl"] = "000010";
    rFunctToBinaryNumber["srlv"] = "000110";
    rFunctToBinaryNumber["sub"] = "100010";
    rFunctToBinaryNumber["subu"] = "100011";
    rFunctToBinaryNumber["xor"] = "100110";
    rFunctToBinaryNumber["slt"] = "101010";
    rFunctToBinaryNumber["sltu"] = "101011";
    rFunctToBinaryNumber["jalr"] = "001001";
    rFunctToBinaryNumber["jr"] = "001000";
    rFunctToBinaryNumber["tne"] = "110110";
    rFunctToBinaryNumber["teq"] = "110100";
    rFunctToBinaryNumber["tge"] = "110000";
    rFunctToBinaryNumber["tgeu"] = "110001";
    rFunctToBinaryNumber["tlt"] = "110010";
    rFunctToBinaryNumber["tltu"] = "110011";
    rFunctToBinaryNumber["mfhi"] = "010000";
    rFunctToBinaryNumber["mflo"] = "010010";
    rFunctToBinaryNumber["mthi"] = "010001";
    rFunctToBinaryNumber["mtlo"] = "010011";
    rFunctToBinaryNumber["syscall"] = "001100";
}


string decimalToBinary(string str, int l) {
    string mcode;
    string a = "0";
    int s = atoi(str.c_str()) + 4 - 4;
    int arr[32];
    int i = 0;
    while (s >= 1) {
        arr[i] = s % 2;
        s = s/2;
        i++;
    }
    
    for (int j = i-1; j >= 0; j--) {
        mcode += to_string(arr[j]);
    }
    
    for (int k = l - (int)mcode.length() ; k > 0; k--) {
        mcode = a + mcode;
    }
    return mcode;
}

string toTwosComplement16(int num) {
    if (num >= 0) {
        string str = to_string(num);
        return decimalToBinary(str, 16);
    } else {
        //turn to 1's complement
        int abs_num = abs(num);
        string str = decimalToBinary(to_string(abs_num), 16);
        str[0] = '1';
        for (int i = 1; i <= 15; i++) {
            if (str[i] == '1') {
                str[i] = '0';
            } else {
                str[i] = '1';
            }
        }
        //turn to 2's complement
        int carry = 1;
        int j = 15;
        do {
            if ((str[j] == '0') && (carry == 1)) {
                str[j] = '1';
                carry = 0;
            } else if ((str[j] == '1') && (carry == 1)) {
                str[j] = '0';
            }
            j--;
        } while ((carry == 1) && (j != 0));
        return str;
    }
}

bool is_operation(string str) {
    if ((rFunctToBinaryNumber.find(str) != rFunctToBinaryNumber.end()) || (iOpToBinaryNumber.find(str) != iOpToBinaryNumber.end()) || (jOpToBinaryNumber.find(str) != jOpToBinaryNumber.end())) {
        return true;
    } else {
        return false;
    }
}

void toMachineCode(istream & is, ostream & os) {
    string line;
    int address = 0x400000 - 4;
    
    while (getline(is, line)) {
        int num = 0;
        int j = 0;
        string arr[10];

        while (j < (int)line.length()) {
            while ((line[j] == ' ') || (line[j] == ',') || (line[j] == '(') || (line[j] == ')') || (line[j] == '\t')) {
                if (j == (int)line.length()) {
                    break;
                }
                j++;
            }
            string str;
            while ((line[j] != ' ') && (line[j] != ',') && (line[j] != '(') && (line[j] != ')')) {
                if (j == (int)line.length()) {
                    break;
                }
                str.push_back(line[j]);
                j++;
            }
            arr[num] = str;
            num += 1;
        }
        
//        os << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << ";"<< endl;
        int fs = 0;
        if (is_operation(arr[0])) {
            fs = 0;
            address += 4;
        } else if (is_operation(arr[1])) {
            fs = 1;
            address += 4;
        }
        string mcode;
        string s = arr[fs];
        if (rFunctToBinaryNumber.find(arr[fs]) != rFunctToBinaryNumber.end()) {
            if ((s == "add") || (s == "addu") || ( s == "and") || ( s == "nor") || (s == "or") || (s == "sub") || (s == "subu") || (s =="xor") || (s == "slt") || (s == "sltu")) {
                string a = "000000";
                string b = "00000";
                mcode = a + registerToBinaryNumber[arr[fs+2]] + registerToBinaryNumber[arr[fs+3]] + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "clo") || (s == "clz")) {
                string a = "011100";
                string b = "00000";
                mcode = a + registerToBinaryNumber[arr[fs+2]] + b + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "div") || (s == "divu") || (s == "mult") || (s == "multu") || (s == "teq") || (s == "tge") || (s == "tgeu") || (s == "tlt") || (s == "tltu") || (s == "tne")){
                string a = "000000";
                string b = "0000000000";
                mcode = a + registerToBinaryNumber[arr[fs+1]] + registerToBinaryNumber[arr[fs+2]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if (s == "mul") {
                string a = "011100";
                string b = "00000";
                mcode = a + registerToBinaryNumber[arr[fs+2]] + registerToBinaryNumber[arr[fs+3]] + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "madd") || (s == "maddu") || (s == "msub") || (s == "msubu")) {
                string a = "011100";
                string b = "0000000000";
                mcode = a + registerToBinaryNumber[arr[fs+1]] + registerToBinaryNumber[arr[fs+2]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "sllv") || (s == "srav") || (s == "srlv")) {
                string a = "000000";
                string b = "00000";
                mcode = a + registerToBinaryNumber[arr[fs+3]] + registerToBinaryNumber[arr[fs+2]] + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "sll") || (s == "sra") || (s == "srl")) {
                string a = "00000000000";
                mcode = a + registerToBinaryNumber[arr[fs+2]] + registerToBinaryNumber[arr[fs+1]] + decimalToBinary(arr[fs+3], 5) + rFunctToBinaryNumber[arr[fs]];
            } else if (s == "jalr") {
                string a = "000000";
                string b = "00000";
                mcode = a + registerToBinaryNumber[arr[fs+1]] + b + registerToBinaryNumber[arr[fs+2]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "jr") || (s == "mthi") || (s == "mtlo")) {
                string a = "000000";
                string b = "000000000000000";
                mcode = a + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if ((s == "mfhi") || (s == "mflo")) {
                string a = "0000000000000000";
                string b = "00000";
                mcode = mcode + a + registerToBinaryNumber[arr[fs+1]] + b + rFunctToBinaryNumber[arr[fs]];
            } else if (s == "syscall") {
                string a = "00000000000000000000000000";
                mcode = a + rFunctToBinaryNumber[arr[fs]];
            }
        } else if (iOpToBinaryNumber.find(arr[fs]) != iOpToBinaryNumber.end()) {
            if ((s == "addi") || (s == "addiu") || (s == "andi") || (s == "ori") || (s == "xori") || (s == "slti") || (s == "sltiu")) {
                int imm = atoi(arr[fs+3].c_str());
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+2]] + registerToBinaryNumber[arr[fs+1]] + toTwosComplement16(imm);
            } else if ((s == "teqi") || (s == "tgei") || (s == "tgeiu") || (s == "tlti") || (s == "tltiu") || (s == "tnei")) {
                string a = "000001";
                int imm = atoi(arr[fs+2].c_str());
                mcode = a +  registerToBinaryNumber[arr[fs+1]] + iOpToBinaryNumber[arr[fs]] + toTwosComplement16(imm);
            } else if (s == "lui") {
                string a = "00000";
                int imm = atoi(arr[fs+2].c_str());
                mcode = iOpToBinaryNumber[arr[fs]] + a + registerToBinaryNumber[arr[fs+1]] + toTwosComplement16(imm);
            } else if ((s == "beq") || (s == "bne")){
                string a = ":";
                string label = arr[fs+3] + a;
                int labelAddress = labelToAddress[label];
                int diff = (labelAddress - address) / 4;
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+1]] + registerToBinaryNumber[arr[fs+2]] + toTwosComplement16(diff);
            } else if ((s == "bgtz") || (s == "blez") || (s == "bltz")) {
                string a = "00000";
                string b = ":";
                string label = arr[fs+2] + b;
                int labelAddress = labelToAddress[label];
                int diff = (labelAddress - address) / 4;
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+1]] + a + toTwosComplement16(diff);
            } else if (s == "bgez") {
                string a = "00001";
                string b = ":";
                string label = arr[fs+2] + b;
                int labelAddress = labelToAddress[label];
                int diff = (labelAddress - address) / 4;
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+1]] + a + toTwosComplement16(diff);
            } else if (s == "bgezal") {
                string a = "10001";
                string b = ":";
                string label = arr[fs+2] + b;
                int labelAddress = labelToAddress[label];
                int diff = (labelAddress - address) / 4;
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+1]] + a + toTwosComplement16(diff);
            } else if (s == "bltzal") {
                string a = "10000";
                string b = ":";
                string label = arr[fs+2] + b;
                int labelAddress = labelToAddress[label];
                int diff = (labelAddress - address) / 4;
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+1]] + a + toTwosComplement16(diff);
            } else if ((s == "lb") || (s == "lbu") || (s == "lh") || (s == "lhu") || (s == "lw") || (s == "lwl") || (s == "lwr") || (s == "ll") || (s == "sb") || (s == "sh") || (s == "sw") || (s == "swl") || (s == "swr") || (s == "sc")) {
                int offset = atoi(arr[fs+2].c_str());
                mcode = iOpToBinaryNumber[arr[fs]] + registerToBinaryNumber[arr[fs+3]] + registerToBinaryNumber[arr[fs+1]] + toTwosComplement16(offset);
            }
        } else if (jOpToBinaryNumber.find(arr[fs]) != jOpToBinaryNumber.end()){
            string a = ":";
            string label = arr[fs+1] + a;
            int decimalAddress = (labelToAddress[label] + 4) / 4;
            mcode = jOpToBinaryNumber[arr[fs]] + decimalToBinary(to_string(decimalAddress), 26);
        }
        
        if (mcode != "") {
            os << mcode << endl;
        }
    }
}

void removeComments(istream & is, ostream & os) {
    //remove comments
    char ch1;
    char ch2;
    while (is.get(ch1)) {
        if (ch1 == '#') {
            while (is.get(ch2)) {
                if (ch2 == '\n') {
                    os.put(ch2);
                    break;
                }
            }
            ch1 = ch2;
        }
        os.put(ch1);
    }
}

void recordAddress(istream & is) {
    //Record the address of label
    string line;
    int labelAddress = 0x400000 - 4;
    while (getline(is, line)) {
        int num = 0;
        int j = 0;
        string arr[100];

        while (j < (int)line.length()) {
            while ((line[j] == ' ') || (line[j] == ',') || (line[j] == '\t')) {
                if (j == (int)line.length()) {
                    break;
                }
                j++;
            }
            string str;
            while ((line[j] != ' ') && (line[j] != ',')) {
                if (j == (int)line.length()) {
                    break;
                }
                str.push_back(line[j]);
                j++;
            }
            arr[num] = str;
            num += 1;
        }

        if (arr[0][(int)arr[0].length() - 1] == ':') {
            labelToAddress[arr[0]] = labelAddress;
            if (is_operation(arr[1])) {
                labelAddress += 4;
            }
        } else if (is_operation(arr[0])) {
            labelAddress += 4;
        }
    }
}

int main1() {
    buildRegisterMap();
    buildIOpMap();
    buildJOpMap();
    buidRFunctMap();

    return 0;
}

