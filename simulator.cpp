#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "assembler.h"
using namespace std;

map<string, string> binaryNumberToRegister;
map<string, int*> findRegister;

int start_pc = 0x400000;
int startText = 0x500000;
int endText = 0x500000;
int stackTop = 0x1000000;

int pc = 0x400000;
char *text = NULL;
char *startofMemory = NULL;
char *staticData = NULL;
int *lo = new int;
int *hi = new int;

char *test = NULL;


void buildMap1() {
    binaryNumberToRegister["00000"] = "$zero";
    binaryNumberToRegister["00001"] = "$at";
    binaryNumberToRegister["00010"] = "$v0";
    binaryNumberToRegister["00011"] = "$v1";
    binaryNumberToRegister["00100"] = "$a0";
    binaryNumberToRegister["00101"] = "$a1";
    binaryNumberToRegister["00110"] = "$a2";
    binaryNumberToRegister["00111"] = "$a3";
    binaryNumberToRegister["01000"] = "$t0";
    binaryNumberToRegister["01001"] = "$t1";
    binaryNumberToRegister["01010"] = "$t2";
    binaryNumberToRegister["01011"] = "$t3";
    binaryNumberToRegister["01100"] = "$t4";
    binaryNumberToRegister["01101"] = "$t5";
    binaryNumberToRegister["01110"] = "$t6";
    binaryNumberToRegister["01111"] = "$t7";
    binaryNumberToRegister["10000"] = "$s0";
    binaryNumberToRegister["10001"] = "$s1";
    binaryNumberToRegister["10010"] = "$s2";
    binaryNumberToRegister["10011"] = "$s3";
    binaryNumberToRegister["10100"] = "$s4";
    binaryNumberToRegister["10101"] = "$s5";
    binaryNumberToRegister["10110"] = "$s6";
    binaryNumberToRegister["10111"] = "$s7";
    binaryNumberToRegister["11000"] = "$t8";
    binaryNumberToRegister["11001"] = "$t9";
    binaryNumberToRegister["11010"] = "$k0";
    binaryNumberToRegister["11011"] = "$k1";
    binaryNumberToRegister["11100"] = "$gp";
    binaryNumberToRegister["11101"] = "$sp";
    binaryNumberToRegister["11110"] = "$fp";
    binaryNumberToRegister["11111"] = "$ra";
}

void buildMap2() {
    findRegister["$zero"] = new int;
    findRegister["$at"] = new int;
    findRegister["$v0"] = new int;
    findRegister["$v1"] = new int;
    findRegister["$a0"] = new int;
    findRegister["$a1"] = new int;
    findRegister["$a2"] = new int;
    findRegister["$a3"] = new int;
    findRegister["$t0"] = new int;
    findRegister["$t1"] = new int;
    findRegister["$t2"] = new int;
    findRegister["$t3"] = new int;
    findRegister["$t4"] = new int;
    findRegister["$t5"] = new int;
    findRegister["$t6"] = new int;
    findRegister["$t7"] = new int;
    findRegister["$s0"] = new int;
    findRegister["$s1"] = new int;
    findRegister["$s2"] = new int;
    findRegister["$s3"] = new int;
    findRegister["$s4"] = new int;
    findRegister["$s5"] = new int;
    findRegister["$s6"] = new int;
    findRegister["$s7"] = new int;
    findRegister["$t8"] = new int;
    findRegister["$t9"] = new int;
    findRegister["$k0"] = new int;
    findRegister["$k1"] = new int;
    findRegister["$gp"] = new int;
    findRegister["$sp"] = new int;
    findRegister["$fp"] = new int;
    findRegister["$ra"] = new int;
    
    * findRegister["$zero"] = 0;
    * findRegister["$at"] = 0;
    * findRegister["$v0"] = 0;
    * findRegister["$v1"] = 0;
    * findRegister["$a0"] = 0;
    * findRegister["$a1"] = 0;
    * findRegister["$a2"] = 0;
    * findRegister["$a3"] = 0;
    * findRegister["$t0"] = 0;
    * findRegister["$t1"] = 0;
    * findRegister["$t2"] = 0;
    * findRegister["$t3"] = 0;
    * findRegister["$t4"] = 0;
    * findRegister["$t5"] = 0;
    * findRegister["$t6"] = 0;
    * findRegister["$t7"] = 0;
    * findRegister["$s0"] = 0;
    * findRegister["$s1"] = 0;
    * findRegister["$s2"] = 0;
    * findRegister["$s3"] = 0;
    * findRegister["$s4"] = 0;
    * findRegister["$s5"] = 0;
    * findRegister["$s6"] = 0;
    * findRegister["$s7"] = 0;
    * findRegister["$t8"] = 0;
    * findRegister["$t9"] = 0;
    * findRegister["$k0"] = 0;
    * findRegister["$k1"] = 0;
    * findRegister["$gp"] = 0;
    * findRegister["$sp"] = 0xa00000;
    * findRegister["$fp"] = 0x500000;
    * findRegister["$ra"] = 0;
}


void allocateMemory() {
    startofMemory = (char*) malloc(0x600000);
    text = startofMemory;
    staticData = startofMemory + 0x100000;
}

string toBinary(string str) {
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
    
    return mcode;
}

unsigned int calculate_u(int signed_n){
    string str = to_string(signed_n);
    string binary = decimalToBinary(str, 32);
    int n = 1;
    unsigned int result = 0;
    for (int i = 31; i >= 0; i--){
        result += (binary[i] - '0') *n;
        n *= 2;
    }
    return result;
}

string toTwosComplement(int num) {
    if (num >= 0) {
        string str = to_string(num);
        return decimalToBinary(str, 32);
    } else {
        //turn to 1's complement
        int abs_num = abs(num);
        string str = decimalToBinary(to_string(abs_num), 32);
        str[0] = '1';
        for (int i = 1; i <= 31; i++) {
            if (str[i] == '1') {
                str[i] = '0';
            } else {
                str[i] = '1';
            }
        }
        //turn to 2's complement
        int carry = 1;
        int j = 31;
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

int toDecimalNum(string binaryString){
    return stoi(binaryString, nullptr, 2);
}

int readTwosComplement(string binaryCode, int l) {
    string str = binaryCode.substr(1, l-1);
    if (binaryCode[0] == '0') {
        return toDecimalNum(binaryCode);
    } else {
        int index=0;
        string sub;
        while(index <= l-2){
            sub += "1";
            index += 1;
        }
        
        return toDecimalNum(str) - toDecimalNum(sub) - 1;
    }
}


void storeMachineCode(istream & is) {
    string line;
    while (getline(is, line)) {
        int binaryNum = readTwosComplement(line, 32);
        *(int*) text = binaryNum;
        text = text + 4;
    }
}


void storeData(istream & is) {
    string line;
    while (getline(is, line)) {
        int j = 0;
        int i = 0;
        string str;
        
        while (i < (int)line.length()) {
            if (line[i] == '.') {
                while ((line[i] != ' ') && (i < (int)line.length())) {
                    str.push_back(line[i]);
                    i++;
                }
                i++;
                break;
            }
            i++;
        }

        j = i;
        while (j < (int)line.length()) {
            if (line[j] == '"') {
                j++;
                int num = 0;
                if (str == ".ascii") {
                    while (line[j] != '"') {
                        if ((line[j] == '\\' ) && (line[j+1] == 'n')) {
                            * staticData = '\n';
                            staticData = staticData + 2;
                            j = j + 2;
                            num = num + 2;
                            continue;
                        } else if ((line[j] == '\\' ) && (line[j+1] == 't')) {
                            * staticData = '\t';
                            staticData = staticData + 2;
                            j = j + 2;
                            num = num + 2;
                            continue;
                        }
                        * staticData = line[j];
                        staticData++;
                        j++;
                        num++;
                    }
                    while (num % 4 != 0) {
                        num++;
                        staticData++;
                    }
                } else if (str == ".asciiz") {
                    while (line[j] != '"') {
                        if ((line[j] == '\\' ) && (line[j+1] == 'n')) {
                            * staticData = '\n';
                            staticData = staticData + 2;
                            j = j + 2;
                            num = num + 2;
                            continue;
                        } else if ((line[j] == '\\' ) && (line[j+1] == 't')) {
                            * staticData = '\t';
                            staticData = staticData + 2;
                            j = j + 2;
                            num = num + 2;
                            continue;
                        }
                        * staticData = line[j];
                        staticData++;
                        j++;
                        num++;
                    }
                    * staticData = '\0';
                    staticData++;
                    num++;
                    while (num % 4 != 0) {
                        num++;
                        staticData++;
                    }
                }
            } else {
                int arr[4];
                int num = 0;
                int t = 0;
                while (j < (int)line.length()) {
                    while ((line[j] == ' ') || (line[j] == '\t') || (line[j] == ',')) {
                        if (j == (int)line.length()) {
                            break;
                        }
                        j++;
                    }
                    string str;
                    while ((line[j] != ' ') && (line[j] != ',') && (line[j] != '\t')) {
                        if (j == (int)line.length()) {
                            break;
                        }
                        str.push_back(line[j]);
                        j++;
                    }
                    arr[num] = atoi(str.c_str());
                    num += 1;
                }
                
                if (str == ".byte") {
                    while (t < num) {
                        *(int *) (staticData) = arr[t];
                        staticData++;
                        t++;
                    }
                } else if (str == ".half") {
                    while (t < num) {
                        *(int *) (staticData) = arr[t];
                        staticData = staticData + 2;
                        t++;
                    }
                } else if (str == ".word") {
                    while (t < num) {
                        *(int *) (staticData) = arr[t];
                        staticData = staticData + 4;
                        t++;
                    }
                }
            }
            j++;
        }
    }
}



int determineOverflow(int num1, int num2, string arith_type){
    if (arith_type == "add"){
        if (num1>0 && num2>0 && num1+num2<0){
            return 1;
        }
        else if (num1<0 && num2<0 && num1+num2>0){
            return 1;
        }
    }
    else if (arith_type == "divide"){
        if (num1==-2147483648 && num2==-1){
            return 1;
        }
    }
    else if (arith_type == "substract"){
        if (num2 == ~num2+1){
            return int(num1<0);
        }
        else{
            return determineOverflow(num1, -num2, "add");
        }
    }
    else if (arith_type == "multiply"){
        int product = num1 * num2;
        return int(!num1 || num1==product/num2);
    }
    return 0;
}


    
void _add(string rd, string rs, string rt){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rs]] + * findRegister[binaryNumberToRegister[rt]];
}

void _addu(string rd, string rs, string rt){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rs]] + * findRegister[binaryNumberToRegister[rt]];
}

void _addi(string rt, string rs, string imm){
    * findRegister[binaryNumberToRegister[rt]] = * findRegister[binaryNumberToRegister[rs]] + readTwosComplement(imm, 16);
}

void _addiu(string rt, string rs, string imm){
    * findRegister[binaryNumberToRegister[rt]] = * findRegister[binaryNumberToRegister[rs]] + readTwosComplement(imm, 16);
}

void _and(string rd, string rs, string rt){
    string a = "0";
    string b = "1";
    string rd_num;
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    string rt_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    for (int j = 0; j < 32; j++) {
        if ((rs_num[j] == '1') && (rt_num[j] == '1')) {
            rd_num = rd_num + b;
        } else {
            rd_num = rd_num + a;
        }
    }
    * findRegister[binaryNumberToRegister[rd]] = toDecimalNum(rd_num);
}

void _andi(string rt, string rs, string imm){
    string a = "0";
    string b = "1";
    string rt_num;
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    string imm_num = decimalToBinary(to_string(readTwosComplement(imm, 16)), 32);
    for (int j = 0; j < 32 ; j++) {
        if ((rs_num[j] == '1') && (imm_num[j] == '1')) {
            rt_num = rt_num + b;
        } else {
            rt_num = rt_num + a;
        }
    }
    * findRegister[binaryNumberToRegister[rt]] = toDecimalNum(rt_num);
}

void _clo(string rs, string rd){
    string rs_num = toBinary(to_string(* findRegister[binaryNumberToRegister[rs]]));
    int num = 0;
    while ((rs_num[num] == '1') && (num < (int)rs_num.length())) {
        num++;
    }
    * findRegister[binaryNumberToRegister[rd]] = num;
}

void _clz(string rd, string rs){
    string rs_num = toBinary(to_string(* findRegister[binaryNumberToRegister[rs]]));
    int num = 0;
    while ((rs_num[num] == '0') && (num < (int)rs_num.length())) {
        num++;
    }
    * findRegister[binaryNumberToRegister[rd]] = num;
}

void _div(string rs, string rt){
    int rs_num = * findRegister[binaryNumberToRegister[rs]];
    int rt_num = * findRegister[binaryNumberToRegister[rt]];
    * lo = rs_num / rt_num;
    * hi = rs_num % rt_num;
}

void _divu(string rs, string rt){
    int rs_num = * findRegister[binaryNumberToRegister[rs]];
    int rt_num = * findRegister[binaryNumberToRegister[rt]];
    * lo = rs_num / rt_num;
    * hi = rs_num % rt_num;
}

void _mult(string rs, string rt){
    if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo += * findRegister[binaryNumberToRegister[rt]] << j;
                * hi += * findRegister[binaryNumberToRegister[rt]] >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _multu(string rs, string rt){
    if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo += * findRegister[binaryNumberToRegister[rt]] << j;
                * hi += * findRegister[binaryNumberToRegister[rt]] >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _mul(string rd, string rs, string rt){
    int i = 0;
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    for (int j = 31; j >= 0; j--){
        if (rs_num[j] == '1'){
            * findRegister[binaryNumberToRegister[rd]] += * findRegister[binaryNumberToRegister[rt]] << i;
        }
        i += 1;
    }
}

void _madd(string rs, string rt){
    if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo += * findRegister[binaryNumberToRegister[rt]] << j;
                * hi += * findRegister[binaryNumberToRegister[rt]] >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _msub(string rs, string rt){
    if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo -= * findRegister[binaryNumberToRegister[rt]] << j;
                * hi -= * findRegister[binaryNumberToRegister[rt]] >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _maddu(string rs, string rt){
    if (determineOverflow(uint32_t(* findRegister[binaryNumberToRegister[rs]]), uint32_t(* findRegister[binaryNumberToRegister[rt]]), "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(uint32_t(* findRegister[binaryNumberToRegister[rs]])), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo += uint32_t(* findRegister[binaryNumberToRegister[rt]]) << j;
                * hi += uint32_t(* findRegister[binaryNumberToRegister[rt]]) >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(uint32_t(* findRegister[binaryNumberToRegister[rs]]), uint32_t(* findRegister[binaryNumberToRegister[rt]]), "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _msubu(string rs, string rt){
    if (determineOverflow(uint32_t(* findRegister[binaryNumberToRegister[rs]]), uint32_t(* findRegister[binaryNumberToRegister[rt]]), "multiply") == 0) {
        int j = 0;
        * hi = 0;
        * lo = 0;
        string rs_num = decimalToBinary(to_string(uint32_t(* findRegister[binaryNumberToRegister[rs]])), 32);
        for (int i = 31; i >= 0; i--) {
            if (rs_num[i] == '1') {
                * lo -= uint32_t(* findRegister[binaryNumberToRegister[rt]]) << j;
                * hi -= uint32_t(* findRegister[binaryNumberToRegister[rt]]) >> (i+1);
            }
            j++;
        }
    } else if (determineOverflow(uint32_t(* findRegister[binaryNumberToRegister[rs]]), uint32_t(* findRegister[binaryNumberToRegister[rt]]), "multiply") == 1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _nor(string rd, string rs, string rt){
    string rd_num;
    string rt_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    for (int i = 0; i <= 31; i++){
        if (rt_num[i]=='0' && rs_num[i]=='0'){
            rd_num += "1";
        }else{
            rd_num += "0";
        }
    }
    * findRegister[binaryNumberToRegister[rd]] = toDecimalNum(rd_num);
}

void _or(string rd, string rs, string rt){
    string rd_num;
    string rt_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    for (int i = 0; i <= 31; i++){
        if (rt_num[i]=='0' && rs_num[i]=='0'){
            rd_num += "0";
        }else{
            rd_num += "1";
        }
    }
    * findRegister[binaryNumberToRegister[rd]] = toDecimalNum(rd_num);
}

void _ori(string rt, string rs, string imm){
    string rt_num;
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    imm = decimalToBinary(to_string(readTwosComplement(imm, 16)), 32);
    for (int i = 0; i <= 31; i++){
        if (imm[i]=='0' && rs_num[i]=='0'){
            rt_num += "0";
        }else{
            rt_num += "1";
        }
    }
    * findRegister[binaryNumberToRegister[rt]] = toDecimalNum(rt_num);
}

void _sll(string rd, string rt, string shamt){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rt]] << toDecimalNum(shamt);
}

void _sllv(string rd, string rt, string rs){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rt]] << * findRegister[binaryNumberToRegister[rs]];
}

void _sra(string rd, string rt, string shamt){
    int shift_num = uint16_t(toDecimalNum(shamt));
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rt]] >> shift_num;
}

void _srav(string rd, string rt, string rs){
    int shift_num = uint16_t(toDecimalNum(rs));
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rt]] >> shift_num;
}

void _srl(string rd, string rt, string shamt){
    * findRegister[binaryNumberToRegister[rd]]  = * findRegister[binaryNumberToRegister[rt]]  >> toDecimalNum(shamt);
}

void _srlv(string rd, string rt, string rs){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rt]] >> * findRegister[binaryNumberToRegister[rs]];
}

void _sub(string rd, string rs, string rt){
    if (determineOverflow(* findRegister[binaryNumberToRegister[rs]], * findRegister[binaryNumberToRegister[rt]], "substract")==1) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
    else{
        * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rs]] - * findRegister[binaryNumberToRegister[rt]];
    }
}

void _subu(string rd, string rs, string rt){
    * findRegister[binaryNumberToRegister[rd]] = * findRegister[binaryNumberToRegister[rs]] - * findRegister[binaryNumberToRegister[rt]];
}

void _xor(string rd, string rs, string rt){
    string rd_num;
    string rt_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    for (int i = 0; i <= 31 ; i++) {
        if (rt_num[i] == rs_num[i]) {
            rd_num += "0";
        }else{
            rd_num += "1";
        }
    }
    * findRegister[binaryNumberToRegister[rd]] = toDecimalNum(rd_num);
}

void _xori(string rs, string rt, string imm){
    string rt_num;
    string rs_num = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rs]]), 32);
    imm =  decimalToBinary(to_string(readTwosComplement(imm, 16)), 32);
    for (int i = 0; i <= 31; i++){
        if (imm[i] == rs_num[i]){
            rt_num += "0";
        }else{
            rt_num += "1";
        }
    }
    * findRegister[binaryNumberToRegister[rt]] = toDecimalNum(rt_num);
}

void _lui(string rt, string imm){
    * findRegister[binaryNumberToRegister[rt]] = readTwosComplement(imm, 16) << 16;
}

void _slt(string rd, string rs, string rt){
    if (* findRegister[binaryNumberToRegister[rt]] > * findRegister[binaryNumberToRegister[rs]]) {
        * findRegister[binaryNumberToRegister[rd]] = 1;
    }
    else{
        * findRegister[binaryNumberToRegister[rd]] = 0;
    }
}

void _sltu(string rd, string rs, string rt){
    int rt_num = uint32_t(* findRegister[binaryNumberToRegister[rt]]);
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    if (rt_num > rs_num) {
        * findRegister[binaryNumberToRegister[rd]] = 1;
    }
    else{
        * findRegister[binaryNumberToRegister[rd]] = 0;
    }
}

void _slti(string rt, string rs, string imm){
    if (* findRegister[binaryNumberToRegister[rs]] < readTwosComplement(imm, 16)) {
        * findRegister[binaryNumberToRegister[rt]] = 1;
    }
    else{
        * findRegister[binaryNumberToRegister[rt]] = 0;
    }
}

void _sltiu(string rt, string rs, string imm){
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    int imm_num = uint16_t(readTwosComplement(imm, 16));
    if (rs_num < imm_num) {
        * findRegister[binaryNumberToRegister[rt]] = 1;
    }
    else{
        * findRegister[binaryNumberToRegister[rt]] = 0;
    }
}

void _beq(string rs, string rt, string label){
    if ( * findRegister[binaryNumberToRegister[rs]] ==  * findRegister[binaryNumberToRegister[rt]]){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bgez(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] >= 0){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bgezal(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] >= 0){
        * findRegister["$ra"] = (uintptr_t)(pc + 1);
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bgtz(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] > 0){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _blez(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] <= 0){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bltzal(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] <= 0){
        * findRegister["$ra"] = (uintptr_t)(pc + 1);
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bltz(string rs, string label){
    if (* findRegister[binaryNumberToRegister[rs]] < 0){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _bne(string rs,string rt, string label){
    if (* findRegister[binaryNumberToRegister[rs]] != * findRegister[binaryNumberToRegister[rt]]){
        pc += readTwosComplement(label, 16) * 4;
    }
}

void _j(string target){
    pc = toDecimalNum(target) * 4 - 4;
}

void _jal(string target){
    * findRegister["$ra"] = (uintptr_t)(pc + 4);
    int address = toDecimalNum(target) * 4 - 4;
    pc = address;
}

void _jalr(string rs, string rd){
    * findRegister[binaryNumberToRegister[rd]] = (uintptr_t) (pc + 1);
    pc = * findRegister[binaryNumberToRegister[rs]] - 4;
}

void _jr(string rs){
    pc = * findRegister[binaryNumberToRegister[rs]] - 4;
}

void _teq(string rs, string rt){
    if (* findRegister[binaryNumberToRegister[rs]] == * findRegister[binaryNumberToRegister[rt]]){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _teqi(string rs, string imm){
    if (* findRegister[binaryNumberToRegister[rs]] == readTwosComplement(imm, 16)){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _tne(string rs, string rt){
    if (* findRegister[binaryNumberToRegister[rs]] != * findRegister[binaryNumberToRegister[rt]]){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _tnei(string rs, string imm){
    if (* findRegister[binaryNumberToRegister[rs]] == readTwosComplement(imm, 16)){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _tge(string rs, string rt){
    if (* findRegister[binaryNumberToRegister[rs]] >= * findRegister[binaryNumberToRegister[rt]]){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _tgeu(string rs, string rt){
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    int rt_num = uint32_t(* findRegister[binaryNumberToRegister[rt]]);
    if ( rs_num >= rt_num){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _tgei(string rs, string imm){
    if (* findRegister[binaryNumberToRegister[rs]] >= readTwosComplement(imm, 16)){
        cout << "Warning: overflow!"  << endl;
        exit(0);
    }
}
void _tgeiu(string rs, string imm){
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    int imm_num = uint16_t(readTwosComplement(imm, 16));
    if ( rs_num >= imm_num){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}
void _tlt(string rs, string rt){
    if (* findRegister[binaryNumberToRegister[rs]] < * findRegister[binaryNumberToRegister[rt]]){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}
void _tltu(string rs, string rt){
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    int rt_num = uint32_t(* findRegister[binaryNumberToRegister[rt]]);
    if ( rs_num < rt_num) {
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}
void _tlti(string rs, string imm){
    if (* findRegister[binaryNumberToRegister[rs]] < readTwosComplement(imm, 16)){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}
void _tltiu(string rs, string imm){
    int rs_num = uint32_t(* findRegister[binaryNumberToRegister[rs]]);
    int imm_num = uint16_t(readTwosComplement(imm, 16));
    if ( rs_num < imm_num){
        cout << "Warning: overflow!" << endl;
        exit(0);
    }
}

void _lb(string rs, string rt, string address){
    char* ch = (char*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    * findRegister[binaryNumberToRegister[rt]] = *ch;
}

void _lbu(string rs, string rt, string address){
    char* ch = (char*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    int8_t value = *ch;
    * findRegister[binaryNumberToRegister[rt]] = calculate_u(value);
}

void _lh(string rs, string rt, string address){
    int16_t* half = (int16_t*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    * findRegister[binaryNumberToRegister[rt]] = *half;
}

void _lhu(string rs, string rt, string address){
    int16_t* half = (int16_t*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    * findRegister[binaryNumberToRegister[rt]] = calculate_u(*half);
}

void _lw(string rs, string rt, string address){
    int* value = (int*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
//    cout << value << endl;
//    cout << *value << endl;
    * findRegister[binaryNumberToRegister[rt]] = * value;
}

void _lwl(string rs, string rt, string address){
    int address_s = (readTwosComplement(address, 16) + * findRegister[binaryNumberToRegister[rs]]);
    int remainder = address_s % 4;
    int load = *(int*)(address_s - 0x400000 + startofMemory - remainder + * findRegister[binaryNumberToRegister[rs]]);
    string load_str = decimalToBinary(to_string(load), 32);
    string rt_str = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    string result = "";
    for (int i = 0; i < 8 * remainder; i++){
        result.push_back(rt_str[i]);
    }
    for (int i = 8 * remainder; i < 32; i++){
        result.push_back(load_str[i]);
    }
    * findRegister[binaryNumberToRegister[rt]] = toDecimalNum(result);
}

void _lwr(string rs, string rt, string address){
    int address_s = (readTwosComplement(address, 16) + * findRegister[binaryNumberToRegister[rs]]);
    int remainder = address_s % 4;
    int load = *(int*)(address_s - 0x400000 + startofMemory  - remainder + * findRegister[binaryNumberToRegister[rs]]);
    string load_str = decimalToBinary(to_string(load), 32);
    string rt_str = decimalToBinary(to_string(* findRegister[binaryNumberToRegister[rt]]), 32);
    string result = "";
    for (int i = 0; i < 8* (remainder+1); i++){
        result.push_back(rt_str[i]);
    }
    for (int i = 0; i < 32-(remainder+1)*8; i++){
        result.push_back(load_str[i]);
    }
    * findRegister[binaryNumberToRegister[rt]] = toDecimalNum(result);
}

void _ll(string rs, string rt, string address){
    int* value = (int*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    * findRegister[binaryNumberToRegister[rt]] = * value;
}

void _sb(string rs, string rt, string address){
    char* address_t = (char*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    char ch;
    ch = * findRegister[binaryNumberToRegister[rt]];
    *address_t = ch;
}

void _sh(string rs, string rt, string address){
    int address_i = readTwosComplement(address, 16) + * findRegister[binaryNumberToRegister[rs]];;
    int remainder = address_i % 2;
    int16_t* address_t = (int16_t*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]] + remainder);
    * address_t =* findRegister[binaryNumberToRegister[rt]];
}

void _sw(string rs, string rt, string address){
    int address_i = readTwosComplement(address, 16) + * findRegister[binaryNumberToRegister[rs]];
    int remainder = address_i % 4;
    int * address_t = (int *) (readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]] - remainder);
    * address_t = * findRegister[binaryNumberToRegister[rt]];
}

void _swl(string rs, string rt, string address){
    int* address_t = (int*)(readTwosComplement(address, 16) - 0x400000 + startofMemory - 3 + * findRegister[binaryNumberToRegister[rs]]);
    *address_t = * findRegister[binaryNumberToRegister[rt]];
}

void _swr(string rs, string rt, string address){
    int* address_t = (int*)(readTwosComplement(address, 16) - 0x400000 + startofMemory + * findRegister[binaryNumberToRegister[rs]]);
    *address_t = * findRegister[binaryNumberToRegister[rt]];
}

void _sc(string rs, string rt, string address){
    int address_i = readTwosComplement(address, 16) + * findRegister[binaryNumberToRegister[rs]];
    int remainder = address_i % 4;
    int* address_t = (int*)(readTwosComplement(address, 16) - 0x400000 + startofMemory +  * findRegister[binaryNumberToRegister[rs]] + (4-remainder));
    *address_t = * findRegister[binaryNumberToRegister[rt]];
}

void _mfhi(string rd){
    * findRegister[binaryNumberToRegister[rd]] =  * hi;
}
void _mflo(string rd){
    * findRegister[binaryNumberToRegister[rd]] =  * lo;
}
void _mthi(string rs){
    * hi = * findRegister[binaryNumberToRegister[rs]];
}
void _mtlo(string rs){
    * lo = * findRegister[binaryNumberToRegister[rs]];
}

void _syscall(istream& is, ostream& os){
    if (* findRegister["$v0"] == 1) {
        os << * findRegister["$a0"] ;
    } else if (* findRegister["$v0"] == 4) {
        int address = * findRegister["$a0"];
        char ch = *(char*)(address + startofMemory - 0x400000);
        while (ch != '\0'){
            os << ch;
            address += 1;
            ch = *(char*)(address + startofMemory - 0x400000);
        }
        os << '\0';
        os.flush();
    } else if (* findRegister["$v0"] == 5) {
        int ins;
        string input;
        getline(is, input);
        ins = stoi(input, nullptr, 0);
        * (int *)findRegister["$v0"] = ins;
    } else if (* findRegister["$v0"] == 8) {
        string read_str;
        int length = *(int*)(findRegister["$a1"]);
        getline(is, read_str);
        char * ch = (char*)(* findRegister["$a0"] + startofMemory - 0x400000);
        for (int i = 0; i < length; i++){
            * ch = read_str[i];
            ch += 1;
        }
        *ch = '\0';
    } else if (* findRegister["$v0"] == 9) {
        * findRegister["$v0"] = * findRegister["$fp"];
        * findRegister["$fp"] =  * findRegister["$fp"] + * findRegister["$a0"];
    } else if (* findRegister["$v0"] == 10) {
        pc = 0x7fffffff;
    } else if (* findRegister["$v0"] == 11) {
        os << *(char*)(findRegister["$a0"]);
    } else if (* findRegister["$v0"] == 12) {
        string input;
        getline(is, input);
        char ch = input[0];
        *(char*)(findRegister["$v0"]) = ch;
    } else if (* findRegister["$v0"] == 13) {
        string mode = "";
        string filename = "";
        int n = 0;
        char* a0 = (char*)( * findRegister["$a0"] + startofMemory - 0x400000);
        char* a2 = (char*)( * findRegister["$a2"] + startofMemory - 0x400000);
        while (*a0 != 0){
            filename += *(a0 + n);
            n += 1;
        }
        n = 0;
        while (*a2 != 0){
            mode += *(a2 + n);
            n += 1;
        }

        int f = open(filename.c_str(),*(char*)(findRegister["$a1"]), mode.c_str());
        *(int*)( findRegister["$v0"]) = f;

    } else if (* findRegister["$v0"] == 14) {
        int fd = *(int*)findRegister["$a0"];
        char* buffer = startofMemory + *(int*)(findRegister["$a1"]) - 0x400000;
        int length = *(int*)findRegister["$a2"];
        int num = int(write(fd, buffer, length));
        *(int*)findRegister["$v0"] = num;
    } else if (* findRegister["$v0"] == 15) {
        int fd = *(int*)findRegister["$a0"];
        char* buffer = startofMemory + *(int*)(findRegister["$a1"]) - 0x400000;
        int length = *(int*)findRegister["$a2"];
        int num = int(write(fd, buffer, length));
        *(int*)findRegister["$v0"] = num;
    } else if (* findRegister["$v0"] == 16) {
        close(*(int*)(findRegister["$v0"]));
    } else if (* findRegister["$v0"] == 17) {
        pc = 0x7fffffff;
    }
}



//Tell the binary code of instructions
void typeofInstruction(string functCode, string opCode, string rtCode, string decimalCode, istream& is, ostream& os){
    if (opCode == "000000") {
        if (functCode == "100000") {
            _add(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100001") {
            _addu(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100100") {
            _and(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "011010") {
            _div(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "011011") {
            _divu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "011000") {
            _mult(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "011001") {
            _multu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100111") {
            _nor(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100101") {
            _or(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "000000") {
            _sll(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(21, 5));
        } else if (functCode == "000100") {
            _sllv(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(6, 5));
        } else if (functCode == "000011") {
            _sra(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(21, 5));
        } else if (functCode == "000111") {
            _srav(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(6, 5));
        } else if (functCode == "000010") {
            _srl(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(21, 5));
        } else if (functCode == "000110") {
            _srlv(decimalCode.substr(16, 5), decimalCode.substr(11, 5), decimalCode.substr(6, 5));
        } else if (functCode == "100010") {
            _sub(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100011") {
            _subu(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "100110") {
            _xor(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "101010") {
            _slt(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "101011") {
            _sltu(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "001001") {
            _jalr(decimalCode.substr(6, 5), decimalCode.substr(16, 5));
        } else if (functCode == "001000") {
            _jr(decimalCode.substr(6, 5));
        } else if (functCode == "110110") {
            _tne(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "110100") {
            _teq(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "110000") {
            _tge(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "110001") {
            _tgeu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "110010") {
            _tlt(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "110011") {
            _tltu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "010000") {
            _mfhi(decimalCode.substr(16, 5));
        } else if (functCode == "010010") {
            _mflo(decimalCode.substr(16, 5));
        } else if (functCode == "010001") {
            _mthi(decimalCode.substr(6, 5));
        } else if (functCode == "010011") {
            _mtlo(decimalCode.substr(6, 5));
        } else if (functCode == "001100") {
            _syscall(is, os);
        }
    } else if (opCode == "101100") {
        if (functCode == "100001") {
            _clo(decimalCode.substr(16, 5), decimalCode.substr(6, 5));
        } else if (functCode == "100000") {
            _clz(decimalCode.substr(16, 5), decimalCode.substr(6, 5));
        } else if (functCode == "000010") {
            _mul(decimalCode.substr(16, 5), decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "000000") {
            _madd(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "000001") {
            _maddu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "000100") {
            _msub(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        } else if (functCode == "000101") {
            _msubu(decimalCode.substr(6, 5), decimalCode.substr(11, 5));
        }
    } else if (opCode == "000001") {
        if (rtCode == "01100") {
            _teqi(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "01000") {
            _tgei(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "01001") {
            _tgeiu(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "01010") {
            _tlti(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "01011") {
            _tltiu(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "01110") {
            _tnei(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "00001") {
            _bgez(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "10001") {
            _bgezal(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "10000") {
            _bltzal(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        } else if (rtCode == "00000") {
            _bltz(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
        }
    } else if (opCode == "001000") {
        _addi(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001001") {
        _addiu(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001100") {
        _andi(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001101") {
        _ori(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001110") {
        _xori(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001111") {
        _lui(decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001010") {
        _slti(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "001011") {
        _sltiu(decimalCode.substr(11, 5), decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "000100") {
        _beq(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "000111") {
        _bgtz(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "000110") {
        _blez(decimalCode.substr(6, 5), decimalCode.substr(16, 16));
    } else if (opCode == "000101") {
        _bne(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100000") {
        _lb(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100100") {
        _lbu(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100001") {
        _lh(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100101") {
        _lbu(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100011") {
        _lw(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100010") {
        _lwl(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "100110") {
        _lwr(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "110000") {
        _ll(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "101000") {
        _sb(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "101001") {
        _sh(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "101011") {
        _sw(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "101010") {
        _swl(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "101110") {
        _swr(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "111000") {
        _sc(decimalCode.substr(6, 5), decimalCode.substr(11, 5), decimalCode.substr(16, 16));
    } else if (opCode == "000010") {
        _j(decimalCode.substr(6, 26));
    } else if (opCode == "000011") {
        _jal(decimalCode.substr(6, 26));
    }
}


void simulate(istream& is, ostream& os) {
    while (pc < (text - startofMemory + 0x400000)) {
        int code = * (int *)(pc - 0x400000 + startofMemory);
        string decimalCode = toTwosComplement(code);
        string functCode = decimalCode.substr(26, 6);
        string opCode = decimalCode.substr(0, 6);
        string rtCode = decimalCode.substr(11, 5);
        typeofInstruction(functCode, opCode, rtCode, decimalCode, is, os);
        pc = pc + 4;
    }
}


int main(int argc, char** argv) {
    if(argc < 4){
        printf("wrong number of arguments");
        
    }
    
    
    buildMap1();
    buildMap2();
    allocateMemory();

    main1();
    ifstream infile1;
    ifstream infile3;
    ofstream outfile1;
    ofstream outfile2;
    ofstream code_file;

    infile1.open(argv[1]);
    infile3.open(argv[2]);
    outfile2.open(argv[3]);
    outfile1.open("a_put.txt");
    code_file.open("code_put.txt");
    
    removeComments(infile1, outfile1);
    infile1.close();
    outfile1.close();
    
    ifstream infile4;
    infile4.open("a_put.txt");
    recordAddress(infile4);
    infile4.close();
    
    ifstream infile6("a_put.txt");
    toMachineCode(infile6, code_file);
    infile6.close();
    code_file.close();
    
    ifstream infile7("a_put.txt");
    storeData(infile7);
    infile7.close();

    ifstream infile5("code_put.txt");
    storeMachineCode(infile5);
    infile5.close();

    simulate(infile3, outfile2);
    
    infile3.close();
    outfile2.close();

    delete lo;
    delete hi;
    delete findRegister["$zero"];
    delete findRegister["$at"];
    delete findRegister["$v0"];
    delete findRegister["$v1"];
    delete findRegister["$a0"];
    delete findRegister["$a1"];
    delete findRegister["$a2"];
    delete findRegister["$a3"];
    delete findRegister["$t0"];
    delete findRegister["$t1"];
    delete findRegister["$t2"];
    delete findRegister["$t3"];
    delete findRegister["$t4"];
    delete findRegister["$t5"];
    delete findRegister["$t6"];
    delete findRegister["$t7"];
    delete findRegister["$s0"];
    delete findRegister["$s1"];
    delete findRegister["$s2"];
    delete findRegister["$s3"];
    delete findRegister["$s4"];
    delete findRegister["$s5"];
    delete findRegister["$s6"];
    delete findRegister["$s7"];
    delete findRegister["$t8"];
    delete findRegister["$t9"];
    delete findRegister["$k0"];
    delete findRegister["$k1"];
    delete findRegister["$gp"];
    delete findRegister["$sp"];
    delete findRegister["$fp"];
    delete findRegister["$ra"];
    
    return 0;
}

