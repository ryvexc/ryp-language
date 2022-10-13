#include<iostream>
#include<vector>

template<typename T> void print_tokens(T __TK) {
    std::cout << "[";
    for(int i=0; i<(__TK).size(); i++)
        std::cout << (__TK)[i].name << ":" << (__TK)[i].value << (i < (__TK).size() - 1 ? "," : ""); 
    std::cout << "]\n";
}

template<typename T> void print_raw_tokens(T __TK) {
    std::cout << "[";
    for(int i=0; i<(__TK).size(); i++)
        std::cout << (__TK)[i].value << (i < (__TK).size() - 1 ? "," : ""); 
    std::cout << "]\n";
}

template<typename T> void print_function_args(T __TK) {
    for(int i=0; i<__TK.size(); i++) {
        std::cout << "name: " << __TK[i].name << std::endl;
        std::cout << "type: " << __TK[i].type << std::endl;
    }
}

template<typename T> void print_var_info(T __V) {
    std::cout << "{\n";
    std::cout << "\tnama: " << __V.name << std::endl;
    std::cout << "\tdata_type: " << __V.__data_type << std::endl;
    std::cout << "\tvalue_stack" << __V.value_stack << std::endl;
    std::cout << "\tvalue: " << __V.value << std::endl;
    std::cout << "\tnullable: " << __V.nullable << std::endl;
    std::cout << "\tblock_stack: " << __V.block_level << std::endl;
    std::cout << "}";
}