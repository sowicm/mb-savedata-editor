#pragma once

#include <d3dx9.h>
#include "..\..\CZJGUI\CZJGUI\ZJArray.h"

template <typename VectorT>
struct stTmpCas
{
    int iFrame;
    VectorT rot;
};

typedef stTmpCas <D3DXVECTOR3> stTmpCas3;
typedef stTmpCas <D3DXVECTOR4> stTmpCas4;

struct stTmpBone4
{
    CZJArray <stTmpCas4> cas;
};

class CBrfAnimation
{
public:
    char m_Name[256];
};