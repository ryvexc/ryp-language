#include<string>
#include<array>
#include<utility>
#include<memory>
#include<type_traits>

typedef __INT8_TYPE__ i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

typedef __UINT8_TYPE__ ui8;
typedef __UINT16_TYPE__ ui16;
typedef __UINT32_TYPE__ ui32;
typedef __UINT64_TYPE__ ui64;

typedef float f32;
typedef double f64;

typedef __INT8_TYPE__ byte;

typedef std::string str;
typedef const char* cptrstr;
typedef const char cstr;

template<typename T> i32 lenof(T arr) {
    return arr.size();
}
#include<iostream>
namespace  io {
    template<typename  T> 
void println(T arg){
        std::cout << arg << std::endl;    }
}
;
i32 main(void ){
    
std::array<i32, 10> arr;    io::println(lenof(arr));    io::println(arr.size());    const 
i32 i = 5;    io::println(i);    i = 9;    io::println(i);}
