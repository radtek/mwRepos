
/*Author: Neilchang
* Date: 2007-4.28
* Subject: OCI_select_demo
*   test.c
*/
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <oci.h>
typedef struct test_t
{
    char USER_ID[10];
    char USER_NAME[5];
    int AGE;
}test_t;
static int ProcessInit = 0;
typedef struct OCIConnection{
    char uid[100];
    char    pwd[30];
    char cstr[50];
}CONN_INFO,*PCONN_INFO;
typedef struct OCIHandle
{
    OCIEnv    *envhp;     /*OCI 环境句柄*/
    OCISvcCtx *svchp;     /*OCI 服务上下文句柄*/
    OCIError *errhp;     /*OCI 错误句柄*/
    OCIServer *srvhp;     /*OCI 服务器句柄*/
    OCISession *authp;     /*OCI 会话句柄*/
    OCIStmt    *stmthp;    /*OCI */
    int    loggedon;    /*记录是否处于登录状态*/
}HOCI,*POCI;
int oci_init(POCI pocih, int mode )
{
    pocih->loggedon = 0;
    if(!ProcessInit)
    {
        if (OCIInitialize((ub4) mode, (dvoid *)0, (dvoid * (*)(dvoid *, size_t)) 0,
            (dvoid * (*)(dvoid *, dvoid *, size_t))0, (void (*)(dvoid *, dvoid *)) 0 ))
        {
            (void) printf("FAILED: OCIInitialize().\n");
            return -1;
        }
        ProcessInit = 1;
    }
    /* Inititialize the OCI Environment */
    if (OCIEnvInit((OCIEnv **) &pocih->envhp, (ub4) OCI_DEFAULT,
        (size_t) 0, (dvoid **) 0 ))
    {
        (void) printf("FAILED: OCIEnvInit()\n");
        return OCI_ERROR;
    }
    /* Allocate a service handle */
    if (OCIHandleAlloc((dvoid *) pocih->envhp, (dvoid **) &pocih->svchp,
        (ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIHandleAlloc() on svchp\n");
        return OCI_ERROR;
    }
    /* Allocate an error handle */
    if (OCIHandleAlloc((dvoid *) pocih->envhp, (dvoid **) &pocih->errhp,
        (ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIHandleAlloc() on errhp\n");
        return OCI_ERROR;
    }
    /* Allocate a server handle */
    if (OCIHandleAlloc((dvoid *) pocih->envhp, (dvoid **) &pocih->srvhp,
        (ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIHandleAlloc() on srvhp\n");
        return OCI_ERROR;
    }
    /* Allocate a authentication handle */
    if (OCIHandleAlloc((dvoid *) pocih->envhp, (dvoid **) &pocih->authp,
        (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIHandleAlloc() on authp\n");
        return OCI_ERROR;
    }
    if (OCIHandleAlloc((dvoid *)pocih->envhp, (dvoid **) &pocih->stmthp,
        (ub4)OCI_HTYPE_STMT, (CONST size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIHandleAlloc() on stmthp\n");
        return OCI_ERROR;
    }
    return OCI_SUCCESS;
}

int oci_server_attach(POCI pocih,int mode,char* cstring)
{
    if (OCIServerAttach(pocih->srvhp, pocih->errhp, (text *) cstring,
        (sb4) strlen(cstring), (ub4) mode))
    {
        (void) printf("FAILED: OCIServerAttach()\n");
        return OCI_ERROR;
    }

    /* Set the server handle in the service handle */
    if (OCIAttrSet((dvoid *) pocih->svchp, (ub4) OCI_HTYPE_SVCCTX,
        (dvoid *) pocih->srvhp, (ub4) 0, (ub4) OCI_ATTR_SERVER, pocih->errhp))
    {
        (void) printf("FAILED: OCIAttrSet() server attribute\n");
        return OCI_ERROR;
    }
    return OCI_SUCCESS;
}

int oci_login (POCI pocih,char* uid, char* pwd, int credt, int mode)
{
    /* Set attributes in the authentication handle */
    if (OCIAttrSet((dvoid *) pocih->authp, (ub4) OCI_HTYPE_SESSION,
        (dvoid *)(text *) uid, (ub4) strlen(uid),
        (ub4) OCI_ATTR_USERNAME, pocih->errhp))
    {
        return OCI_ERROR;
    }
    if (OCIAttrSet((dvoid *) pocih->authp, (ub4) OCI_HTYPE_SESSION,
        (dvoid *)(text *) pwd, (ub4) strlen(pwd),
        (ub4) OCI_ATTR_PASSWORD, pocih->errhp))
    {
        return OCI_ERROR;
    }
    if (OCISessionBegin(pocih->svchp, pocih->errhp, pocih->authp, (ub4)credt, (ub4)mode))
    {
        return OCI_ERROR;
    }

    /* Set the authentication handle in the Service handle */
    if (OCIAttrSet((dvoid *) pocih->svchp, (ub4) OCI_HTYPE_SVCCTX,
        (dvoid *) pocih->authp, (ub4) 0, (ub4) OCI_ATTR_SESSION, pocih->errhp))
    {
        return OCI_ERROR;
    }
    pocih->loggedon = 1;
    return OCI_SUCCESS;
}

int ocilink(POCI pocih,PCONN_INFO pocic,int mode)
{
    if(oci_init(pocih, mode ) == OCI_ERROR)
        return -1;
    if(oci_server_attach(pocih,OCI_DEFAULT,pocic->cstr) == OCI_ERROR)
        return -1;
    if(oci_login(pocih,pocic->uid, pocic->pwd, OCI_CRED_RDBMS,OCI_DEFAULT) == OCI_ERROR)
        return -1;
    return 0;
}

int oracle_conn_init(int init_mode)
{
    if (OCIInitialize((ub4) init_mode, (dvoid *)0, (dvoid * (*)(dvoid *, size_t)) 0,
        (dvoid * (*)(dvoid *, dvoid *, size_t))0, (void (*)(dvoid *, dvoid *)) 0 ))
    {
        printf("conn db InitOCIProcess error!\n");
        return -1;
    }
    ProcessInit = 1;
    return 0;
}

int oracle_connect(POCI pdbc)
{
    CONN_INFO ocic;//登录数据必须的信息
    strcpy(ocic.uid, "smsacc");
    strcpy(ocic.pwd, "123456");
    strcpy(ocic.cstr, "smsacc");
    if (ocilink(pdbc, &ocic, OCI_DEFAULT) != OCI_SUCCESS)
    {
        printf("conn db ConstructOciLink error!\n");
        return -1;
    }
    return 0;
}

int select_test(POCI p_db_conn)
{
/*    int stat = 0;
    OCIBind* bndhp[1];
    ub1 buffer[256];
    char *select_sql = "SELECT USER_NAME FROM Test where USER_ID = 'a'";
    OCIStmtPrepare((OCIStmt *)p_db_conn->stmthp, p_db_conn->errhp, select_sql, strlen((char*)select_sql),
        (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    OCIDefineByPos((OCIStmt *)p_db_conn->stmthp, &bndhp[0], p_db_conn->errhp, 1, (dvoid *)buffer, 256,
        SQLT_LNG, 0, 0, 0, (ub4) OCI_DEFAULT);stat = OCIStmtExecute(p_db_conn->svchp, p_db_conn->stmthp, p_db_conn->errhp, 1, 0, NULL, NULL, OCI_BATCH_ERRORS | OCI_COMMIT_ON_SUCCESS);
    if(stat != OCI_SUCCESS)
    {
        printf("select error!\n");
        return -1;
    }
    buffer[255] = '\0';
    printf("test--------->buf\n%s\n", buffer);*/
    return 0;
}

void handle()
{
    int stat = 0;
    HOCI    ocih;
    memset(&ocih, 0, sizeof(HOCI));
    stat = oracle_conn_init(1);
    if(stat != 0)
    {
        printf("oracle_conn_init error!\n");
        return ;
    }
    stat = oracle_connect(&ocih);
    if(stat != 0)
    {
        printf("oracle_connect error!\n");
        return ;
    }
    stat = select_test(&ocih);
    if(stat != 0)
    {
        printf("process_test_select error!\n");
        return ;
    }return;
}


int main(int argc, char* argv[])
{
    handle();
    return 0;
}
