#pragma once

#include <d3dx9.h>
#include "..\..\CZJGUI\CZJGUI\ZJArray.h"

class CBrfBone
{
public:
    int attach;

    char m_Name[256];
    
    int b;

    D3DXVECTOR3 x, z, y, t;
};

class CBrfSkeleton
{
public:
    char m_Name[256];
    CZJArray <CBrfBone> m_Bones;
};