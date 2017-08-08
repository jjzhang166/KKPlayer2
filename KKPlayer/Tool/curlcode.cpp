#include "curlcode.h"

std::string CURLCode::UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}
std::string CURLCode::UrlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}
std::string CURLCode::URLDecode(const std::string &sIn)
{
    std::string sOut;
              for( size_t ix = 0; ix < sIn.size(); ix++ )
              {
                  BYTE ch = 0;
                  if(sIn[ix]=='%')
                  {
                      ch = (fromHex(sIn[ix+1])<<4);
                      ch |= fromHex(sIn[ix+2]);
                      ix += 2;
                  }
                  else if(sIn[ix] == '+')
                  {
                      ch = ' ';
                  }
                  else
                  {
                      ch = sIn[ix];
                  }
                  sOut += (char)ch;
              }
              return sOut;
}
