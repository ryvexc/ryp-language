#include<string>
#include<sstream>

struct number_node {
    std::string value;
    std::string repr() {
        return value;
    }
};

struct add_node {
    std::string left;
    std::string right;
    std::string repr() {
        return "("+left+"+"+right+")";
    }
};

struct subtract_node {
    std::string left;
    std::string right;
    std::string repr() {
        return "("+left+"-"+right+")";
    }
};

struct multiply_node {
    std::string left;
    std::string right;
    std::string repr() {
        return "("+left+"*"+right+")";
    }
};

struct divide_node {
    std::string left;
    std::string right;
    std::string repr() {
        return "("+left+"/"+right+")";
    }
};

struct signed_node {
    std::string value;
    std::string repr() {
        return "(+"+value+")";
    }
};

struct unsigned_node {
    std::string value;
    std::string repr() {
        return "(-"+value+")";
    }
};