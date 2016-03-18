#pragma once

#include <d3dx9.h>
#include "..\..\CZJGUI\CZJGUI\ZJArray.h"
#include "..\..\CZJGUI\CZJGUI\ZJPointerArray.h"

struct stTmpRiggingPair
{
    int iVertex;
    float Weight;
};

struct stTmpRigging
{
    int iBone;
    CZJArray <stTmpRiggingPair> Pairs;
};

class CBrfVertex
{
public:
    int id;
    DWORD color;
    D3DXVECTOR3 normal;
    D3DXVECTOR3 tandir; // tangent dir
    BYTE iTexture;
    D3DXVECTOR2 ta, tb;
};

class CBrfFace
{
public:
    int index[3];
};

class CBrfFrame
{
public:
    int time;
    CZJArray <D3DXVECTOR3> m_Positions;
    CZJArray <D3DXVECTOR3> m_Normals;
};

class CBrfMesh
{
public:
    CBrfMesh(void);
    ~CBrfMesh(void);

    char m_Name[256];
    DWORD m_Flags;

    char m_MaterialName[256];

    CZJPointerArray <CBrfFrame*> m_Frames;

    CZJArray <CBrfVertex> m_Vertices;

    CZJArray <CBrfFace>   m_Faces;

    ID3DXMesh *m_pXMesh;
};