#include "str.cpp"

template<typename T> bool compare_tokens(T token1, T token2) {
    bool conditions[2] = {0, 0};
    conditions[0] = compare_str(token1.name, token2.name);
    conditions[1] = compare_str(token1.value, token2.value);
    return conditions[0] && conditions[1];
}

template<typename T> bool token_empty_check(T token) {
    return compare_str(token.name, "") && compare_str(token.value, "");
}