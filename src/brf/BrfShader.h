#pragma once

#include "..\..\CZJGUI\CZJGUI\ZJArray.h"

class CBrfShaderOpt
{
public:
    int map;
    DWORD colorOp, alphaOp;
    DWORD flags;
};

class CBrfShader
{
public:
    char m_Name[256];
    DWORD m_Flags;
    DWORD m_Requires;
    char m_strTechnique[256];
    char m_strFallback[256];

    CZJArray <CBrfShaderOpt> m_Opts;
};