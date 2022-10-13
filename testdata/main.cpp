#include<iostream>
#include<fstream>

#include "../compiler/cvt_cpp.cpp"

int main(int argc, char* argv[]) {
    if(argc > 1) {
        ifstream inputfile(argv[1]);
        std::string line, input_stack;
        while(getline(inputfile, line)) input_stack += (line + "\n");
        std::vector<token> tokens = lexical_analyzer(input_stack);
        CVT_CPP::convert(&tokens);
    } else {
        std::cout << "Need Arguments!" << std::endl;
    }

}