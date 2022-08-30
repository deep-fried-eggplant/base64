#include"dfe/base64.hpp"
#include<iostream>
#include<string>
#include<vector>
#include<type_traits>
#include<random>
#include<ctime>

template<typename T,typename std::enable_if<sizeof(T)==1,std::nullptr_t>::type=nullptr>
void printBytes(const std::vector<T>& bytes){
    for(T bt : bytes){
        uint8_t b=uint8_t(bt);
        printf("%02x",b);
    }
    printf("\n");
}
void printBytes(const std::string& bytes){
    for(char bt : bytes){
        uint8_t b=uint8_t(bt);
        printf("%02x",b);
    }
    printf("\n");
}

std::string makeRandomData(const std::size_t length){
    std::mt19937_64 rnd(time(nullptr));
    std::string str;
    str.reserve(length);
    for(std::size_t i=0; i<length; i++){
        str.push_back(uint8_t(rnd()%256));
    }
    return str;
}

std::string makeRandomString(const std::size_t length){
    std::string str=makeRandomData(length);
    for(char& c : str){
        c=0x20+c%(0x7f-0x20);
    }
    return str;
}

int main(){
    using uchar=unsigned char;
    const std::size_t length=30+time(nullptr)%10;
    std::cout << "create " << length << " bytes random data\n" << std::endl;
    std::string         srcStr=makeRandomData(length);
    std::vector<char>   srcCharVec (srcStr.begin(),srcStr.end());
    std::vector<uchar>  srcUcharVec(srcStr.begin(),srcStr.end());
    
    std::string enStr     =dfe::base64::encode(srcStr);
    std::string enCharVec =dfe::base64::encode(srcCharVec);
    std::string enUcharVec=dfe::base64::encode(srcUcharVec);

    auto deStr     =dfe::base64::decode(enStr);
    auto deCharVec =dfe::base64::decode<char>(enCharVec);
    auto deUcharVec=dfe::base64::decode<uchar>(enUcharVec);

    std::cout << "[Original]\n";
    printBytes(srcStr);
    printBytes(srcCharVec);
    printBytes(srcUcharVec);
    std::cout << std::endl;

    std::cout << "[Encoded]\n"
        << enStr       << '\n'
        << enCharVec  << '\n'
        << enUcharVec << '\n'
        << std::endl;

    std::cout << "[Decoded]\n";
    printBytes(srcStr);
    printBytes(srcCharVec);
    printBytes(srcUcharVec);
    std::cout << std::endl;

    return 0;
}