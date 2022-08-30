#include"dfe/base64.hpp"
#include<iostream>
#include<fstream>
#include<stdexcept>

bool encodeFile(const std::string& path){
    std::ifstream ifs(path,std::ios::binary);
    if(!ifs.good()){
        std::cerr << "\tfailed to open the input file : " << path << std::endl;
        return false;
    }
    std::string data=std::string(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
    ifs.close();
    
    std::cout << "\tsize : " << data.size() << " bytes" << std::endl;
    
    data=dfe::base64::encode(data);
    
    std::string outputPath=path+".base64encoded.txt";
    std::ofstream ofs(outputPath);
    if(!ofs.good()){
        std::cerr << "\tfailed to open the output file : " << outputPath << std::endl;
    }
    ofs.write(data.c_str(),data.size());
    ofs.close();
    std::cout << "\tfinish" << std::endl;
    return true;
}

int main(int argc,char* argv[]){
    for(int i=1; i<argc; i++){
        std::string fileName(argv[i]);
        std::cout << "# " << fileName << std::endl;
        if(encodeFile(fileName)){
            std::cout << std::endl;
        }else{
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}