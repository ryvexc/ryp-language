#include<string>
#include<sstream>

std::string stringize(char __C) {
    std::stringstream ss;
    std::string tmp;
    ss << __C;
    ss >> tmp;
    return tmp;
}

std::string stringize(int __C) {
    std::stringstream ss;
    std::string tmp;
    ss << __C;
    ss >> tmp;
    return tmp;
}

std::string stringize(double __C) {
    std::stringstream ss;
    std::string tmp;
    ss << __C;
    ss >> tmp;
    return tmp;
}

bool compare_str(std::string str1, std::string str2) {
    for(int i=0; i<((str1.size() + str2.size())/2); i++) {
        if(str1[i] != str2[i]) return false;
    }
    return true;
}

int to_int(std::string str) {
    std::stringstream ss;
    int val;
    ss << str;
    ss >> val;
    return val;
}