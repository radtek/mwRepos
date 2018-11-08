#ifndef __BIGDECIMAL_H__
#define __BIGDECIMAL_H__



struct BigDecimal
{
private:
    std::string  strInteger;          // 整数位
    std::string strDecimal;           // 小数位
    int nScale;						  // 小数点后的位数
    int nSign;
public:
    BigDecimal(){
        strInteger = "";
        strDecimal = "";
        nScale = 0;
        nSign = 1;
    }
    BigDecimal(const std::string& strInteger, const std::string& strDecimal, int nScale)
    {
        if (!strInteger.empty())
        {
            if ('-' == strInteger.at(0))
            {
                nSign = -1;
                this->strInteger = strInteger.substr(1, strInteger.length() - 1);
            }
            else
            {
                nSign = 1;
                this->strInteger = strInteger;
            }
        }


        this->strDecimal = strDecimal;
        this->nScale = nScale;
    }

    int CalcPow(int nPow)
    {
        int nInit = 1;
        while (nPow-- > 0)
        {
            nInit *= 10;
        }
        return nInit;
    }

    // 获取保留小数点位数
    int getScale()
    {
        if (0 == nScale)
        {
            nScale = strDecimal.length();
        }
        return nScale;
    }
    // 获取小数位实际位数
    int getDecimalPlaces()
    {
        int n = strDecimal.length();
        if (n > nScale)
        {
            n = nScale;
        }
        return n < nScale ? nScale - n : n;
    }

    // 返回转换为INT64的最终数据
    INT64 divide()
    {
        if (0 == atoll(strInteger.c_str()) && getDecimalPlaces() != nScale)
        {
            return  atoll(strDecimal.c_str())*CalcPow(getDecimalPlaces());
        }
        else
        {
            return  nSign * (atoll(strInteger.c_str())*CalcPow(getScale()) + atoll(strDecimal.c_str()));
        }
    }
};

#endif !__BIGDECIMAL_H__