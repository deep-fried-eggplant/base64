#ifndef DFE_BASE64_HPP
#define DFE_BASE64_HPP

#include<string>
#include<vector>
#include<stdexcept>

#ifdef __GNUC__
#define INLINE inline __attribute__((always_inline))
#define NOINLINE inline __attribute__((noinline))
#else
#define INLINE inline
#define NOINLINE inline
#endif

/**
 * @namespace dfe
 * @brief deep-fried-eggplant.com
*/
namespace dfe{
    /**
     * @namespace base64
     * @brief namespace for base64 utils
    */
    namespace base64{
        
        /**
         * @fn encode
         * @param binaryData std::vector<T> which sizeof T is 1 byte and contains binary data
         * @return base64 encoded string
        */
        template<typename T>
        std::string encode(const std::vector<T> &binaryData) noexcept;
        
        /**
         * @fn encode
         * @param binaryData std::string which contains binary data
         * @return base64 encoded string
        */
        std::string encode(const std::string &binaryData) noexcept;
        
        /**
         * @fn decode
         * @param base64str base64 encoded string
         * @return binary data as std::vector<T> which sizeof T is 1 byte
        */
        template<typename T>
        std::vector<T> decode(const std::string &base64str);
        
        /**
         * @fn decode
         * @param base64str base64 encoded string
         * @return binary data as std::string
        */
        std::string decode(const std::string &base64str);

    }

    namespace exception{
        class Base64Exception : public std::runtime_error{
            public:
            Base64Exception(const char* message):runtime_error(message){}
        };
    }

    namespace detail{
        INLINE char byteToBase64char(const uint8_t b){
            return
                (b<26)?('A'+b)
                :(b<52)?('a'+(b-26))
                :(b<62)?('0'+(b-52))
                :(b==62)?'+'
                :(b==63)?'/'
                :0;
        }
        
        INLINE uint8_t base64charToByte(const char c){
            return
                (c>='A'&&c<='Z')?(c-'A')
                :(c>='a'&&c<='z')?(c-'a'+26)
                :(c>='0'&&c<='9')?(c-'0'+52)
                :(c=='+')?62
                :(c=='/')?63
                :(c=='=')?64
                :0xff;
        }
    }

    namespace base64{
        using Exception=exception::Base64Exception;

        template<typename T>
        NOINLINE std::string encode(const std::vector<T> &binaryData) noexcept{
            static_assert(sizeof(T)==1,"sizeof(vector<T>::valuetype) must be 1");
            std::string res;
            uint8_t phase=0;
            uint8_t b=0;
            for(T byt : binaryData){
                char ch=char(byt);
                switch(phase){
                    case 0:
                    {
                        b = ch>>2 & 0b111111;
                        res+=detail::byteToBase64char(b);
                        b = (ch&0b11)<<4;
                    } break;
                    case 1:
                    {
                        b += ch>>4 & 0b1111;
                        res+=detail::byteToBase64char(b);
                        b = (ch&0b1111)<<2;
                    } break;
                    case 2:
                    {
                        b += ch>>6 & 0b11;
                        res+=detail::byteToBase64char(b);
                        b = ch&0b111111;
                        res+=detail::byteToBase64char(b);
                        b = 0;
                    } break;
                }
                phase=(phase+1)%3;
            }
            if(phase>0){
                res+=detail::byteToBase64char(b);
            }
            if(res.length()%4>0){
                for(int i=res.length()%4; i<4; i++){
                    res+='=';
                }
            }
            return res;
        }
        NOINLINE std::string encode(const std::string &binaryData) noexcept{
            std::string res;
            uint8_t phase=0;
            uint8_t b=0;
            for(auto ch : binaryData){
                switch(phase){
                    case 0:
                    {
                        b = ch>>2 & 0b111111;
                        res+=detail::byteToBase64char(b);
                        b = (ch&0b11)<<4;
                    } break;
                    case 1:
                    {
                        b += ch>>4 & 0b1111;
                        res+=detail::byteToBase64char(b);
                        b = (ch&0b1111)<<2;
                    } break;
                    case 2:
                    {
                        b += ch>>6 & 0b11;
                        res+=detail::byteToBase64char(b);
                        b = ch&0b111111;
                        res+=detail::byteToBase64char(b);
                        b = 0;
                    } break;
                }
                phase=(phase+1)%3;
            }
            if(phase>0){
                res+=detail::byteToBase64char(b);
            }
            if(res.length()%4>0){
                for(int i=res.length()%4; i<4; i++){
                    res+='=';
                }
            }
            return res;
        }
        
        
        template<typename T>
        NOINLINE std::vector<T> decode(const std::string &base64str){
            static_assert(sizeof(T)==1,"sizeof(vector<T>::value_type) must be 1");
            std::vector<T> res;
            uint8_t phase=0;
            char b=0;
            for(char ch : base64str){
                uint8_t tmp=detail::base64charToByte(ch);
                if(tmp==64){
                    if(phase==1){
                        res.push_back((T)b);
                    }
                    break;
                }else if(tmp==0xff){
                    throw Exception("invalid character in base64 string");
                }

                switch(phase){
                    case 0:
                    {
                        b = tmp<<2;
                    }break;
                    case 1:
                    {
                        b += tmp>>4 & 0b11;
                        res.push_back((T)b);
                        b = (tmp&0b1111)<<4;
                    }break;
                    case 2:
                    {
                        b += tmp>>2 & 0b1111;
                        res.push_back((T)b);
                        b = (tmp&0b11)<<6;
                    }break;
                    case 3:
                    {
                        b += tmp & 0b111111;
                        res.push_back((T)b);
                    }break;
                }
                phase=(phase+1)%4;
            }
            return res;
        }
        NOINLINE std::string decode(const std::string &base64str){
            std::string res;
            uint8_t phase=0;
            char b=0;
            for(char ch : base64str){
                uint8_t tmp=detail::base64charToByte(ch);
                if(tmp==64){
                    if(phase==1){
                        res+=b;
                    }
                    break;
                }else if(tmp==0xff){
                    throw Exception("invalid character in base64 string");
                }
                
                switch(phase){
                    case 0:
                    {
                        b = tmp<<2;
                    }break;
                    case 1:
                    {
                        b += tmp>>4 & 0b11;
                        res += b;
                        b = (tmp&0b1111)<<4;
                    }break;
                    case 2:
                    {
                        b += tmp>>2 & 0b1111;
                        res += b;
                        b = (tmp&0b11)<<6;
                    }break;
                    case 3:
                    {
                        b += tmp & 0b111111;
                        res += b;
                    }break;
                }
                phase=(phase+1)%4;
            }
            return res;
        }

    }
}

#endif