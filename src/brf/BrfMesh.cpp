#include "StdAfx.h"
#include "BrfMesh.h"

#include "..\..\CZJGUI\CZJGUI\ZJManager.h"

CBrfMesh::CBrfMesh(void)
{
    m_pXMesh = NULL;
}


CBrfMesh::~CBrfMesh(void)
{
    SAFE_RELEASE(m_pXMesh);
}
