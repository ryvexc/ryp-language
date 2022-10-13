#include<vector>
#include<iostream>

#include "lexer.cpp"
#include "node.cpp"
#include "../tools/printable.cpp"

class Parser {
    private:
    std::vector<token> tokens;
    token current_token;
    int __pos = -1;
    
    public:
    Parser(std::vector<token> tokens) {
        this->tokens = tokens;
        this->next_token();
    }

    void next_token() {
        this->__pos++;
        if(this->__pos < this->tokens.size())
            this->current_token = this->tokens[this->__pos];
        else 
            this->current_token = {"",""};
    }

    std::string parse() {
        // std::cout << this->current_token.name << ":" << this->current_token.value << std::endl;

        if(token_empty_check(this->current_token)) 
            return "";
        print_raw_tokens(this->tokens);
        
        std::string result = this->expr();

        if(!token_empty_check(this->current_token))
            std::cout << "Ryp.cmpl.Parser->parse(): c_token<std::string _N, std::string _V> should be null" << std::endl;

        return result;
    }

    std::string expr() {
        std::string result = this->term();

        while(!token_empty_check(this->current_token) && (this->current_token.name == "PLUS" || this->current_token.name == "MINUS")) {
            if(this->current_token.name == "PLUS") {
                this->next_token();
                result = add_node { result, this->term() }.repr();
            } else if(this->current_token.name == "MINUS") {
                this->next_token();
                result = subtract_node { result, this->term() }.repr();
            }
        }

        return result;
    }

    std::string term() {
        std::string result = this->factor();

        while(!token_empty_check(this->current_token) && (this->current_token.name == "MULTIPLY" || this->current_token.name == "SLASH")) {
            if(this->current_token.name == "MULTIPLY") {
                this->next_token();
                result = multiply_node { result, this->factor() }.repr();
            } else if(this->current_token.name == "SLASH") {
                this->next_token();
                result = divide_node { result, this->factor() }.repr();
            }
        }

        return result;
    }

    std::string factor() {
        token c_token = this->current_token;
        // std::cout << c_token.name << ":" << c_token.value << std::endl;

        if(c_token.name == "LPAREN") {
            this->next_token();
            std::string result = this->expr();

            if(this->current_token.name != "RPAREN") 
                std::cout << "Ryp.cmpl.Parser->factor(): expected \")\"" << std::endl;

            this->next_token();
            return result;
        } else if(c_token.name == "INT" || c_token.name == "FLOAT") {
            this->next_token();
            return number_node {c_token.value}.repr();
        } else if(c_token.name == "PLUS") {
            this->next_token();
            return signed_node {this->factor()}.repr();
        } else if(c_token.name == "MINUS") {
            this->next_token();
            return unsigned_node {this->factor()}.repr();
        }

        std::cout << "Ryp.cmpl.Parser->factor(): parser confused, Illegal instruction?" << std::endl;
        return "";
    }
};