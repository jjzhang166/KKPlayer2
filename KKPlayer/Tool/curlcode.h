#ifndef CURLCODE_H
#define CURLCODE_H
#include <string>
#include <assert.h>
typedef unsigned char BYTE;
class CURLCode
{
public:
      CURLCode(){};
      virtual ~CURLCode(){};
      static  std::string UrlEncode(const std::string& str);
      static std::string UrlDecode(const std::string& str);
      static std::string URLDecode(const std::string &sIn);
private:
 inline static unsigned char ToHex(unsigned char x)
 {
     return  x > 9 ? x + 55 : x + 48;
 }
 inline static unsigned char FromHex(unsigned char x)
 {
     unsigned char y;
     if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
     else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
     else if (x >= '0' && x <= '9') y = x - '0';
     else assert(0);
     return y;
 }
 inline static BYTE fromHex(const BYTE &x)
 {
         return isdigit(x) ? x-'0' : x-'A'+10;
 }
};

#endif // CURLCODE_H
