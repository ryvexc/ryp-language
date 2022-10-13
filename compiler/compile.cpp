#include<iostream>

void compile() {
    system("clang++ dmp.cmpl.__tmp.cpp -o dump");
    system("./dump");
    system("rm -f dump");
    // system("rm -f dmp.cmpl.__tmp.cpp");
}