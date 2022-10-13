#include<string>
#include<vector>
#include<iostream>
#include<unistd.h>

#include "token.cpp"
#include "../tools/compare.cpp"

/////////////////////////////////////////////////////

std::vector<std::string> KEYWORDS {
    "fn", 
    "import", 
    "default",
    "var",
    "return",
    "if",
    "else",
    "class",
    "public",
    "namespace",
    "list",
    "typename"
};

std::vector<std::string> DATA_TYPES {
    "i8",   // byte   (-128, 127)
    "i16",  // short  (-32768, 32767)
    "i32",  // int    (-2147483648, 2147483647)
    "i64",  // long   (-9223372036854775808, 9223372036854775807)
    "f32",  // float  
    "f64",  // double
    "ui8",   // u_byte   (255)
    "ui16",  // u_short  (65535)
    "ui32",  // u_int    (4294967295)
    "ui64",  // u_long   (18446744073709551615)
    "byte",
    "bool",
    "str",
    "cstr",  // const char
    "cptrstr", // "const char*"
    "char",
    "void"
};

/////////////////////////////////////////////////////

int __pos = -1;
std::string ___stream = "";
char next_chr() {
    __pos++;
    if(__pos < ___stream.size()) 
        return ___stream[__pos];
    else 
        return 27;
}

bool is_keyword(std::string* str) {
    for(int i=0; i<KEYWORDS.size(); i++)
        if(KEYWORDS[i] == (*str)) return true;
    return false;
}

bool is_datatype(std::string* str) {
    for(int i=0; i<DATA_TYPES.size(); i++)
        if(DATA_TYPES[i] == (*str)) return true;
    return false;
}

bool is_number(char __C) {
    char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(char a : numbers)
        if(a == __C) return true;
    return false; 
}

bool is_identifieric(char __C) {
    std::string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    for(char a : alpha)
        if(a == __C) return true;
    return false;
}

token generate_identifier(char __C) {
    char current_char = __C;
    std::string stream = "";
    std::string type_name = "IDENTIFIER";
    int size = 0;
    while((is_identifieric(current_char) || is_number(current_char)) && current_char != ' ' && current_char != '\t' && current_char != '\n') {
        stream.push_back(current_char);
        size += 1;
        current_char = next_chr();
    }

    for(std::string key : KEYWORDS) 
        if(stream == key) type_name = "KEYWORD";

    for(std::string type : DATA_TYPES) 
        if(stream == type) type_name = "DATA_TYPE";

    if(stream == "true" || stream == "false") type_name = "DATA_TYPE";

    return {type_name, stream};
}

token generate_number(char __C) {
    std::string number_stream = "";
    bool is_float = false;
    char current_char = __C;
    while(is_number(current_char) || current_char == '.') {
        number_stream.push_back(current_char);
        if(current_char == '.') is_float = true;
        current_char = next_chr();
    }
    if(is_float)
        return {"FLOAT", number_stream}; 
    else 
        return {"INT", number_stream};
}

token generate_chrstr_literals(char __C) {
    char current_char = __C;     
    std::string type_identf = current_char == '"' ? "STRING" : "CHARACTER";
    current_char = next_chr();
    std::string stream = "";

    if(type_identf == "STRING") {
        stream.push_back('"');
        while(current_char != '"') {
            stream.push_back(current_char);
            current_char = next_chr();
        }
        stream.push_back('"');
    } else {
        stream.push_back('\'');
        stream.push_back(current_char);
        current_char = next_chr();
        if(current_char != '\'') {
            std::cout << "Ryp.cmpl.lexical_analysis.ge...literal(): Char out of bounds";
            exit(0);
        } else stream.push_back(current_char);
    }

    return {type_identf, stream};
}

token generate_special_op(char __C) {
    std::string stream = "";
    std::string _tok_name = "MINUS";
    char c_token = __C;
    stream.push_back(__C);
    if(c_token == '-') {
        c_token = next_chr();
        if(c_token == '>') {
            stream += ">";
            _tok_name = "ARROW_R";
        } else if(c_token == '-') {
            stream += "-";
            _tok_name = "DECREMENT";
        } else if(c_token == '=') {
            stream += "=";
            _tok_name == "DECREMENT_BY";
        } else {
            __pos--;
        }
    } else if(c_token == '!') {
        c_token = next_chr();
        while(is_number(c_token) || is_identifieric(c_token) || c_token == '=') {
            stream += stringize(c_token);
            c_token = next_chr();
        }

        if(stream == "!=") _tok_name = "NOT_EQUALS";
        else if(stream == "!i32") {
            stream = "ui32";
            _tok_name = "DATA_TYPE";
        } 
        else if(stream == "!i64") {
            stream = "ui64";
            _tok_name = "DATA_TYPE";
        }
        else if(stream == "!i16") {
            stream = "ui16";
            _tok_name = "DATA_TYPE";
        }
        else if(stream == "!i8") {
            stream = "ui8";
            _tok_name = "DATA_TYPE";
        }
        else if(stream == "!i8") {
            stream = "ui8";
            _tok_name = "DATA_TYPE";
        }
        else if(stream == "!str") {
            stream = "cptrstr";
            _tok_name = "DATA_TYPE";
        } else {
            stream = "!";
            _tok_name = "NOT_OP";
        }
    }

    return {_tok_name, stream};
}

std::vector<token> lexical_analyzer(std::string __stream) {
    ___stream = __stream;
    char current_char = next_chr();
    std::vector<token> TOKENS;
    
    while(current_char != 27) {
        if(current_char == '\t'|| current_char == ' ');
        else if(current_char == '\n')
            TOKENS.push_back({"NEWLINE", stringize(current_char)});
        else if(current_char == '+')
            TOKENS.push_back({"PLUS", stringize(current_char)});
        else if(current_char == '-')
            TOKENS.push_back(generate_special_op(current_char));
        else if(current_char == '*')
            TOKENS.push_back({"MULTIPLY", stringize(current_char)});
        else if(current_char == '/')
            TOKENS.push_back({"SLASH", stringize(current_char)});
        else if(current_char == '\\')
            TOKENS.push_back({"BACKSLASH", stringize(current_char)});
        else if(current_char == '(')
            TOKENS.push_back({"LPAREN", stringize(current_char)});
        else if(current_char == ')')
            TOKENS.push_back({"RPAREN", stringize(current_char)});
        else if(current_char == '=')
            TOKENS.push_back({"EQUAL", stringize(current_char)});
        else if(current_char == ')')
            TOKENS.push_back({"RPAREN", stringize(current_char)});
        else if(current_char == '{')
            TOKENS.push_back({"LBRACKET", stringize(current_char)});
        else if(current_char == '}')
            TOKENS.push_back({"RBRACKET", stringize(current_char)});
        else if(current_char == '>')
            TOKENS.push_back({"MORE_OP", stringize(current_char)});
        else if(current_char == '<')
            TOKENS.push_back({"LESS_OP", stringize(current_char)});
        else if(current_char == ':')
            TOKENS.push_back({"COLON", stringize(current_char)});
        else if(current_char == ';')
            TOKENS.push_back({"SEMICOLON", stringize(current_char)});
        else if(current_char == ',')
            TOKENS.push_back({"COMMA", stringize(current_char)});
        else if(current_char == '?')
            TOKENS.push_back({"QUESTION_MARK", stringize(current_char)});
        else if(current_char == '&')
            TOKENS.push_back({"AND_OP", stringize(current_char)});
        else if(current_char == '.')
            TOKENS.push_back({"DOT", stringize(current_char)});
        else if(current_char == '!')
            TOKENS.push_back(generate_special_op(current_char));
        else if(current_char == '[')
            TOKENS.push_back({"L_SBRACKET", stringize(current_char)});
        else if(current_char == ']')
            TOKENS.push_back({"R_SBRACKET", stringize(current_char)});
        else if(is_identifieric(current_char)) {
            TOKENS.push_back(generate_identifier((char)current_char));
            __pos--;
        }
        else if(is_number(current_char)) {
            TOKENS.push_back(generate_number((char)current_char));
            __pos--;
        }
        else if(current_char == '\'' || current_char == '"') {
            TOKENS.push_back(generate_chrstr_literals((char)current_char));
        }
        else {
            std::cout << "Ryp.cmpl.lexical_analyzer(): Invalid Syntax!" << std::endl;
        }
        
        current_char = next_chr();
    }

    __pos = -1;
    return TOKENS;
}