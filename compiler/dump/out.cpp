#include<string>

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
#include<iostream>
namespace io{

void println(i8 arg){
std::cout<<arg<<std::endl;}

void println(i16 arg){
std::cout<<arg<<std::endl;}

void println(i32 arg){
std::cout<<arg<<std::endl;}

void println(i64 arg){
std::cout<<arg<<std::endl;}

void println(bool arg){
std::cout<<arg<<std::endl;}

void println(str arg){
std::cout<<arg<<std::endl;}

void println(cstr arg){
std::cout<<arg<<std::endl;}

void println(cstr arg[]){
std::cout<<arg<<std::endl;}

void println(cptrstr arg[]){
std::cout<<arg<<std::endl;}
template<typename T>
void println(T vec){
std::cout<<"[";for(
i32 i = 0;i<vec.size();i++)std::cout<<vec[i]<<(i<vec.size()-1?",":"");std::cout<<"]"<<std::endl;}
}

i32 global_list[5];;
i32 main(i32 argc, char* argv[]){
io::println("Hello Wolrd!");}
