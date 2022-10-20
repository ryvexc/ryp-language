#include<string>
#include<vector>
#include "../syntaxes/interpreter.cpp"
#include "var/var_handler.cpp"
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include "../scripts/default_type_value.cpp"
#include "compile.cpp"
#include "config.cpp"

int __block_stack = 0;

std::vector<token> ignore_space(int pos, std::vector<token> tokens) {
    std::vector<token>* dmp = &tokens;
    std::cout << "[BEFORE]" << std::endl;
    int it = 0;
    for(it = pos; (*dmp)[it].name != "SEMICOLON" && (*dmp)[it].name != "LBRACKET"; it++) {
        if((*dmp)[it].name == "SPACE") {
            (*dmp).erase((*dmp).begin() + it);
            it--;
        } 
    }
    std::cout << "[AFTER]" << std::endl;
    std::cout << "iterator: " << it << std::endl;
    return *dmp;
}

void assign_cpp_code(std::vector<std::string> __str_line) {
    // include data_types
    ifstream __data_type_data(ryp_path+"/scripts/runtime/lang_default_datatype.cpp");
    std::string line, line_stack;
    while(getline(__data_type_data, line)) line_stack += (line + "\n");
    __data_type_data.close();

    ifstream __check_value_data(ryp_path+"../scripts/runtime/store_value.check.cpp");
    while(getline(__check_value_data, line)) line_stack += (line + "\n");
    __check_value_data.close();

    for(std::string str : __str_line) line_stack += (str);
    
    fstream cpp_dump_file;
    cpp_dump_file.open("dmp.cmpl.__tmp.cpp", ios::out);
    if(!cpp_dump_file.is_open()) std::cout << "ERROR OPEN FILE" << std::endl;
    cpp_dump_file << line_stack;
    cpp_dump_file.close();
}

std::string line_temp = "";
std::vector<std::string> converted_code_stack;

class CVT_CPP {
    public:
    static void convert(std::vector<token>* tokens, bool silent = false) {
        int __line = 1;

        for(int i=0; i<(*tokens).size(); i++) {
            if((*tokens)[i].name == "NEWLINE") __line++;
            else if((*tokens)[i].name == "SPACE") 
                converted_code_stack.push_back(" ");
            else if((*tokens)[i].name == "LBRACKET") {
                converted_code_stack.push_back("{");
                converted_code_stack.push_back("\n");
                __block_stack++;
            }
            else if((*tokens)[i].name == "RBRACKET") {
                converted_code_stack.push_back("}");
                converted_code_stack.push_back("\n");
                __block_stack--;
            }
            else if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "var") {
                *tokens = ignore_space(i, *tokens);
                bool has_value = false;
                std::vector<std::string> var_stack;
                __variable var_dmp;
                bool is_list = false;

                // ADDING VAR STACK
                for(;(*tokens)[i].name != "COLON"; i++) {
                    if((*tokens)[i].name == "IDENTIFIER") var_stack.push_back((*tokens)[i].value);
                } i++;

                if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "list") {
                    i++;
                    if((*tokens)[i].name == "ARROW_R") {
                        is_list = true;
                        i++;
                    }
                }

                if((*tokens)[i].name == "DATA_TYPE" && (*tokens)[i].value != "void") {
                    var_dmp.__data_type = (*tokens)[i].value;
                    i++;
                } 
                // else {
                //     std::cout << "please define the variable type." << std::endl;
                //     exit(0);
                // }

                if((*tokens)[i].name == "QUESTION_MARK") {
                    var_dmp.nullable = true;
                    i++;
                }
                if((*tokens)[i].name == "MULTIPLY") {
                    var_dmp.__data_type += "*";
                    i++;
                }
                if((*tokens)[i].name == "EQUAL") {
                    i++;
                }
                
                // LOAD VAR VALUE
                std::string list_len = "";
                if(is_list && (*tokens)[i].name == "L_SBRACKET") {
                    i++;
                    
                    for(;(*tokens)[i].name != "R_SBRACKET"; i++)
                        list_len += (*tokens)[i].value;
                }
                else for(;(*tokens)[i].name != "SEMICOLON"; i++) {
                    var_dmp.value_stack += (*tokens)[i].value;
                    has_value = true;
                }

                std::cout << var_dmp.value_stack << std::endl;
                std::string tmp_value = var_dmp.value_stack;
                var_dmp.value = tmp_value;

                var_dmp.block_level = __block_stack;
                for(int i=0; i<var_stack.size(); i++) {
                    var_dmp.name = var_stack[i];
                    if(i < var_stack.size() - 1) var_dmp.value = "";
                    var_dmp.value = tmp_value;
                    print_var_info(var_dmp);

                    if(!has_value && !var_dmp.nullable) {
                        if(!is_list) {
                            std::cout << "Ryp.cmpl.parsing: variable "+var_dmp.name+" isn't nullable.";
                            exit(0);
                        }
                    }

                    if(var_dmp.__data_type[0] == 'u') {
                        if(to_int(var_dmp.value) < 0) {
                            std::cout << "Ryp.err.unsigned: unsigned type shouldn't have negative value." << std::endl;
                            exit(0);
                        }
                    }

                    std::string default_value = "";
                    if(is_keyword(&(var_dmp.__data_type))) {
                        if(var_dmp.__data_type.find("str") != std::string::npos)
                            default_value = NOT_NUMERIC;
                        else 
                            default_value = NUMERIC;
                    }

                    std::string cpp_format;
                    if(is_list) cpp_format = "std::array<"+ var_dmp.__data_type +", "+list_len+"> " + var_dmp.name; 
                    else cpp_format = var_dmp.__data_type + " " + var_dmp.name + (!is_list && !var_dmp.nullable? " = ": "") + (has_value ? var_dmp.value_stack : default_value) + ";";

                    converted_code_stack.push_back("\n");
                    converted_code_stack.push_back(cpp_format);
                }
            } 
            else if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "fn") {
                *tokens = ignore_space(i, *tokens);

                struct var_tok {
                    std::string name;
                    std::string type;
                };

                std::string function_name = "";
                std::vector<var_tok> function_args;
                std::string return_type = "";
                bool return_nullable = false;
                std::vector<std::string> fn_line;

                // get function name
                for(; (*tokens)[i].name != "LPAREN"; i++)
                    function_name = ((*tokens)[i].value);

                // get function args
                std::string arg_name;
                std::string arg_type;
                bool is_list = false;
                while(i < (*tokens).size()) {
                    if((*tokens)[i].name == "IDENTIFIER") {
                        arg_name = (*tokens)[i].value;
                    } else if((*tokens)[i].name == "DATA_TYPE") {
                        arg_type = (*tokens)[i].value;
                    } else if((*tokens)[i].name == "MULTIPLY") {
                        arg_type += "*";
                    } else if((*tokens)[i].name == "LESS_OP" && (*tokens)[i+1].name == "IDENTIFIER") {
                        arg_type = (*tokens)[i+1].value;
                        i+=2;
                    }

                    if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "list") {
                        i++;
                        if((*tokens)[i].name == "ARROW_R")
                            is_list = true;
                    }
                    
                    if((*tokens)[i].name == "COMMA") {
                        function_args.push_back({arg_name + (is_list ? "[]" : ""), arg_type});
                    } else if((*tokens)[i].name == "RPAREN") {
                        function_args.push_back({arg_name + (is_list ? "[]" : ""), arg_type});
                        break;
                    }

                    i++;
                }

                i++;
                if((*tokens)[i].name == "ARROW_R") {
                    i++;
                    if((*tokens)[i].name != "DATA_TYPE") {
                        std::cout << "return type isn't data type." << std::endl;
                        exit(0);
                    }
                    return_type = (*tokens)[i].value;
                    if((*tokens)[i+1].name == "QUESTION_MARK") i++;
                } else {
                    std::cout << "function required a return type, atleast void." << std::endl;
                    exit(0);
                }

                std::string arg_cpp_format = "";
                for(int i=0; i<function_args.size(); i++) {
                    arg_cpp_format += (function_args[i].type + " " + function_args[i].name + (i < function_args.size() - 1 ? ", " : ""));
                }
                std::string cpp_format = return_type+" "+function_name+"("+arg_cpp_format+")";
                converted_code_stack.push_back("\n");
                converted_code_stack.push_back(cpp_format);
            }
            else if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "import") {
                *tokens = ignore_space(i, *tokens);
                i++;
                if((*tokens)[i].name == "KEYWORD" && (*tokens)[i].value == "default") {
                    i++;
                    std::string file_to_include;
                    if((*tokens)[i].name == "IDENTIFIER") {
                        for(;(*tokens)[i].name != "SEMICOLON"; i++)
                            file_to_include += (*tokens)[i].value;
                    }
                    else {
                        std::cout << "please define what you need to include!" << std::endl;
                        exit(0);
                    }
                    std::string cpp_format = "#include<"+file_to_include+">\n";
                    converted_code_stack.push_back(cpp_format);
                } else if ((*tokens)[i].name == "IDENTIFIER") {
                    system("pwd");
                    std::string def_lib_path = ryp_path+"/scripts/builtins/";
                    std::string __include_path = def_lib_path + (*tokens)[i].value + "/" +(*tokens)[i].value+ ".ryp";
                    ifstream to_import(__include_path);
                    if(!to_import.is_open()) std::cout << "no std lib for " << (*tokens)[i].value << std::endl;
                    std::string line;
                    while(getline(to_import, line)) {
                        std::vector<token> line_lex_res = lexical_analyzer(line);
                        CVT_CPP::convert(&line_lex_res, true);
                    }
                }
            }
            else if((*tokens)[i].name == "SLASH")
                while((*tokens)[i].name != "NEWLINE" && i < (*tokens).size()) i++;
            else {
                std::string line_stack = "";
                line_stack += (*tokens)[i].value;
                if(is_keyword(&line_stack) || is_datatype(&line_stack)) line_stack += " ";
                converted_code_stack.push_back(line_stack);
            }
        }

        if(!silent) {
            assign_cpp_code(converted_code_stack);    
            compile(ryp_path);
        }
    }
};