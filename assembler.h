#ifndef Assembler_H
#define Assembler_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


int main1();
string decimalToBinary(string str, int l);
void removeComments(istream & is, ostream & os);
void recordAddress(istream & is);
void toMachineCode(istream & is, ostream & os);

#endif
