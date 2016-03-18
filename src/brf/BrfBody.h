#pragma once

#include <d3dx9.h>
#include "..\..\CZJGUI\CZJGUI\ZJArray.h"
#include "..\..\CZJGUI\CZJGUI\ZJPointerArray.h"

enum BrfBodyPartType
{
    MANIFOLD,
    FACE,
    CAPSULE,
    SPHERE,
    N_TYPE
};

class CBrfBodyPart
{
public:
    CBrfBodyPart();
    ~CBrfBodyPart();

    CZJArray <D3DXVECTOR3> m_Positions;
    BrfBodyPartType type;
    int orientation;

    CZJPointerArray < CZJArray <int>* > m_Faces;

    float radius;
    D3DXVECTOR3 center, dir;
    
    DWORD m_Flags;
};

class CBrfBody
{
public:
    char m_Name[256];
    CZJArray <CBrfBodyPart> m_Parts;
};