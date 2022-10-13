#include<vector>
#include<string>
#include<bits/stdc++.h>

struct __variable {
    std::string name;
    std::string __data_type;
    std::string value_stack;
    std::string value;
    bool nullable = false;
    int block_level;
};

class __STORAGE {
    public: std::vector<__variable> __DATA;
};

__STORAGE __variables;

class Variables {
    public:
    static std::string load_value(std::string __var_name, int __block_location) {
        int pos = 0;
        while(__block_location >= 0) {
            if(pos >= __variables.__DATA.size()) break;
            if(__variables.__DATA[pos].name == __var_name && __block_location) 
                return __variables.__DATA[pos].value;
            else __block_location--;
            pos++;
        }
        std::cout << "Ryp.register.Variables.load_value(): variables not defined." << std::endl;
        exit(0);
    }

    static void add_var(__variable new_var) {
        __variables.__DATA.push_back(new_var);
    }
};