#include "StdAfx.h"
#include "Troop.h"

using namespace WSE;

CTroop::CTroop(void)
{
    memset(this, 0, sizeof(CTroop));
    pszFaceKey1 = new CString;
    pszFaceKey2 = new CString;
    pName = new CString;
}


CTroop::~CTroop(void)
{
    delete pszFaceKey1;
    delete pszFaceKey2;
    delete pName;
}
