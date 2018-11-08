/*****************************************************************************************************
copyright (C),2017-2050,montnets .Co.,Ltd.

FileName     : mw_check.h 
Author       : Shijh      Version : 1.0    Date: 2016年07月20日
Description  : 梦网检验类
Version      : 1.0
Function List:

History      :
<author>       <time>             <version>            <desc>
Shijh       2016年07月20日          1.0          build this moudle
******************************************************************************************************/
#ifndef __MW_CHECK_H__
#define __MW_CHECK_H__

// standard header 
#include <string>
#include <vector>
using namespace std;

#include "utility/common/Global.h"
#include "utility/common/dstring.h"

//////////////////////////////////////////////////////////////////////////
// 配置错误码
#define  DB_NAME_NULL               -1          // 数据库名称为空
#define  DB_NAME_LENGTH_SLOP_OVER   -2          // 数据库名长度超长(最大长度64)
#define  DB_SVR_NULL                -3          // 服务IP为空
#define  DB_USER_NULL               -4          // 数据库用户名称为空
#define  DB_USER_LENGTH_SLOP_OVER   -5          // 数据库用户名长度超长(最大长度32)
#define  DB_PWD_NULL                -6          // 数据库密码为空
#define  DB_PWD_LENGTH_SLOP_OVER    -7          // 数据库密码超长(最大长度256)
#define  DB_CONNECT_STRING_ERR      -8          // 数据库连接字串错误
#define  INI_PATH_FILE_EXISTS       -250        // 配置路径文件不存在
#define  INI_TRY_CATCH_ERR          -251        // try...catch

#pragma once
class CMWCheck
{
public:
    CMWCheck(){};
    ~CMWCheck(){};

    /**************************************************************************************
    * Function      : CheckDbParams
    * Description   : 校验数据库连接参数
    * Input         : 无
    * Output        : int
    * Return        : nRet - [0] 成功 [<0] 错误代码
    * Author        : Shijh      Version : 1.0    Date: 2016年07月20日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static int CheckDbParams(const string& strDbName, const string &strDbSvr, const string &strDbUser, const string &strDbPwd)
    {
        int nRet = 0;

        if (strDbName.empty())
        {
            nRet = DB_NAME_NULL;
        }
        if (strDbName.length() > 64)
        {
            nRet = DB_NAME_LENGTH_SLOP_OVER;
        }
        if (strDbSvr.empty())
        {
            nRet = DB_SVR_NULL;
        }
        if (strDbUser.empty())
        {
            nRet = DB_USER_NULL;
        }
        if (strDbName.length() > 32)
        {
            nRet = DB_USER_LENGTH_SLOP_OVER;
        }
        if (strDbPwd.empty())
        {
            nRet = DB_PWD_NULL;
        }
        if (strDbPwd.length() > 128 * 2)
        {
            nRet = DB_PWD_LENGTH_SLOP_OVER;
        }

        return nRet;
    }

    /**************************************************************************************
    * Function      : CheckDbParams
    * Description   : 校验数据库连接参数
    * Input         : 无
    * Output        : int
    * Return        : nRet - [0] 成功 [<0] 错误代码
    * Author        : Shijh      Version : 1.0    Date: 2016年07月20日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static int CheckDbParams(const string& strConnectString)
    {
        int nRet = 0;
        try
        {
            string strDbName = "", strDbSvr = "", strDbUser = "", strDbPwd = "";
            DString strValue = "";
            std::vector <std::string> vParam;
            int nCount = CGlobal::SplitStr(strConnectString.c_str(), ";", vParam);
            if (5 <= nCount)
            {
                // Driver
                //         strValue = vParam.at(0).c_str();

                // Server
                strValue = vParam.at(1).c_str();
                int nPos = strValue.find("=");
                if (nPos >= 0)
                {
                    strValue = strValue.substr(nPos + 1, strValue.length());
                    strDbSvr = strValue.c_str();
                }

                // Database
                strValue = vParam.at(2).c_str();
                nPos = strValue.find("=");
                if (nPos >= 0)
                {
                    strValue = strValue.substr(nPos + 1, strValue.length());
                    strDbName = strValue.c_str();
                }

                // Uid
                strValue = vParam.at(3).c_str();
                nPos = strValue.find("=");
                if (nPos >= 0)
                {
                    strValue = strValue.substr(nPos + 1, strValue.length());
                    strDbUser = strValue.c_str();
                }

                // Pwd
                strValue = vParam.at(4).c_str();
                nPos = strValue.find("=");
                if (nPos >= 0)
                {
                    strValue = strValue.substr(nPos + 1, strValue.length());
                    strValue.trim();
                    strDbPwd = strValue.c_str();
                }
            }
            else
            {
                return nRet = DB_CONNECT_STRING_ERR;
            }

            nRet = CheckDbParams(strDbName, strDbSvr, strDbUser, strDbPwd);
        }
        catch (...)
        {
            nRet = INI_TRY_CATCH_ERR;
        }
        return nRet;
    }

};  // !CMWCheck

#endif  // !__MW_CHECK_H__

