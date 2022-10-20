#include<iostream>
#include<string>

void compile(std::string root_path) {
    std::cout << "root_path: " << root_path << std::endl;
    std::cout << "compiler_loc: " << root_path << "/bin" << std::endl;
    std::cout << "executing code..." << std::endl;
    std::cout << "=======================================================" << std::endl;
    system("clang++ dmp.cmpl.__tmp.cpp -o dump");
    system("./dump");
    system("rm -f dump");
    // system("rm -f dmp.cmpl.__tmp.cpp");
}