#include"dfe/base64.hpp"
#include<iostream>
#include<fstream>
#include<stdexcept>



bool decodeFile(const std::string& path){
    std::ifstream ifs(path);
    if(!ifs.good()){
        std::cerr << "\tfailed to open the input file : " << path << std::endl;
        return false;
    }
    std::string data=std::string(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
    ifs.close();
    
    std::cout << "\tsize : " << data.size() << " bytes" << std::endl;
    
    try{
        data=dfe::base64::decode(data);
    }catch(std::exception& e){
        std::cerr << "\t" << e.what() << std::endl;
        return false;
    }

    std::string outputPath=path+".base64decoded";
    std::ofstream ofs(outputPath,std::ios::binary);
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
        if(decodeFile(fileName)){
            std::cout << std::endl;
        }else{
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}