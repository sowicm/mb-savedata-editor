#pragma once

#include <d3dx9.h>

const DWORD D3DFVF_POSNORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
struct stVertexPosNorTex
{    
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
};

const DWORD D3DFVF_POSTEX = D3DFVF_XYZ | D3DFVF_TEX2;
struct stVertexPosTex
{
    D3DXVECTOR3 Position;
    D3DXVECTOR2 Tex;
};


const DWORD D3DFVF_XYZRHWTEX = D3DFVF_XYZRHW | D3DFVF_TEX2;
struct stVertexXYZRHWTex
{    
    D3DXVECTOR4 XYZRHW;
    D3DXVECTOR2 Tex;
};

const DWORD D3DFVF_XYZNOR = D3DFVF_XYZ | D3DFVF_NORMAL;
struct stVertexXYZNor
{
    D3DXVECTOR3 XYZ;
    D3DXVECTOR3 Normal;
};