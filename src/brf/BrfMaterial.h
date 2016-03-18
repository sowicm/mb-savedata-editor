#pragma once

class CBrfMaterial
{
public:
    char m_Name[256];
    DWORD m_Flags;
    char m_Shader[256];
    char m_DiffuseA[256], m_DiffuseB[256];
    char m_Bump[256];
    char m_Enviro[256];
    char m_Spec[256];
    float m_Specular;
    float r, g, b;

    int m_RenderOrder;
};