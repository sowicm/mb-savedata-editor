#pragma once

/*
#include "..\..\CZJGUI\CZJGUI\ZJGUI.h"

#include <set>
using namespace std;

// TEMPLATE STRUCT NameLess
template<class _Ty>
	struct NameLess
		: public binary_function<_Ty, _Ty, bool>
	{	// functor for operator<
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator< to operands
		return (strcmp(_Left.name, _Right.name) < 0);
		}
	};
//--------------------------------------------------------------------

class CTextureNode
{
public:
    CTextureNode() : texture(NULL) {}
    ~CTextureNode() { SAFE_RELEASE(texture); }
    char name[MAX_PATH];
    LPDIRECT3DTEXTURE9 texture;
};

class CMeshNode
{
public:
    char name[MAX_PATH];
    //CMyMeshLoader mesh;
};

class CZJManager
{
public:
            CZJManager(void);
    virtual ~CZJManager(void);

    void    AddMeshFromObj(const char* strFileName, UINT* piMesh);

protected:
    set <CTextureNode, NameLess<CTextureNode>> m_Textures;
    set <CMeshNode, NameLess<CMeshNode>>       m_Meshes;
};

*/