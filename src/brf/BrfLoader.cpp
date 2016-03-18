#include "StdAfx.h"
#include "BrfLoader.h"

#include "..\..\CZJGUI\CZJGUI\ZJManager.h"

CBrfLoader::CBrfLoader(void)
{
    m_pDevice = NULL;
    m_pManager = NULL;
}


CBrfLoader::~CBrfLoader(void)
{
    for (int i = 0; i < m_Meshes.GetSize(); ++i)
        SAFE_DELETE(m_Meshes[i]);
}


bool CBrfLoader::Load(wchar_t *strFilePath)
{
    FILE *fp;
    _wfopen_s(&fp, strFilePath, L"rb");

    if (!fp)
        return false;

    m_Version = 0;

    char str[256];
    while (true)
    {
        if (!LoadString(fp, str))
        {
            fclose(fp);
            return false;
        }
        if (0 == strcmp(str, "end"))
        {
            break;
        }
        else if (0 == strcmp(str, "rfver "))
        {
            LoadInt(fp, &m_Version);
        }
        else if (0 == strcmp(str, "mesh"))
        {
            LoadMeshes(fp, &m_Meshes);
        }
        else if (0 == strcmp(str, "texture"))
        {
            LoadTextures(fp, &m_Textures);
        }
        else if (0 == strcmp(str, "shader"))
        {
            LoadShaders(fp, &m_Shaders);
        }
        else if (0 == strcmp(str, "material"))
        {
            LoadMaterials(fp, &m_Materials);
        }
        else if (0 == strcmp(str, "skeleton"))
        {
            LoadSkeletons(fp, &m_Skeletons);
        }
        else if (0 == strcmp(str, "skeleton_anim"))
        {
            LoadAnimations(fp, &m_Animations);
        }
        else if (0 == strcmp(str, "body"))
        {
            LoadBodies(fp, &m_Bodies);
        }
        else
        {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}


bool CBrfLoader::LoadString(FILE *fp, char *str)
{
    int c;
    fread(&c, 4, 1, fp);
    if (c < 1 || c >= 256)
        return false;
    fread(str, 1, c, fp);
    str[c] = 0;

    return true;
}


void CBrfLoader::LoadMeshes(FILE *fp, CZJArray <CBrfMesh*> *meshes)
{
    int c;
    LoadInt(fp, &c);
    int i = meshes->GetSize();
    meshes->Increase(c);
    for (c += i; i < c; ++i)
    {
        meshes->GetAt(i) = new CBrfMesh;
        LoadMesh(fp, meshes->GetAt(i));
    }
}



void CBrfLoader::LoadRiggings(FILE *fp, CZJArray <stTmpRigging> *pRigs)
{
    int c;
    LoadType(fp, &c);
    int i = pRigs->GetSize();
    pRigs->Increase(c);
    int cPairs, iPair;
    for (c += i; i < c; ++i)
    {
        LoadType(fp, &pRigs->GetAt(i).iBone);
        LoadType(fp, &cPairs);
        pRigs->GetAt(i).Pairs.Increase(cPairs);
        for (iPair = 0; iPair < cPairs; ++iPair)
        {
            LoadType(fp, &pRigs->GetAt(i).Pairs[iPair]);
            /*
            LoadType(fp, &pRigs->GetAt(i).Pairs[iPair].iVertex);
            LoadType(fp, &pRigs->GetAt(i).Pairs[iPair].Weight);
            */
        }
    }
}

// Vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL2[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};

void CBrfLoader::LoadMesh(FILE *fp, CBrfMesh* pMesh)
{
    HRESULT hr;

    LoadString(fp, pMesh->m_Name);

    LoadType(fp, &pMesh->m_Flags);

    LoadString(fp, pMesh->m_MaterialName);

    if (m_Version != 0)
    {
        m_Version = (pMesh->m_Flags >> 16 == 2) ? 2 : 1;
    }

    CBrfFrame *pFrame = new CBrfFrame;
    LoadVectors(fp, &pFrame->m_Positions);

    CZJArray <stTmpRigging> tmpRigs;
    LoadRiggings(fp, &tmpRigs);

    int cFrame;
    LoadType(fp, &cFrame);
    pMesh->m_Frames.Increase(1 + cFrame);

    for (int i = 1; i <= cFrame; ++i)
    {
        pMesh->m_Frames[i] = new CBrfFrame;
        LoadFrame(fp, pMesh->m_Frames[i]);
    }

    LoadVertices(fp, &pMesh->m_Vertices);
    LoadArray(fp, &pMesh->m_Faces);

    if (pMesh->m_Faces.GetSize() > 0)
    {
        D3DXCreateMesh(pMesh->m_Faces.GetSize(), pMesh->m_Faces.GetSize() * 3,//pMesh->m_Vertices.GetSize(),
            D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL2,
            m_pDevice, &pMesh->m_pXMesh);

        stVertexPosNorTex *pVertex;
        V( pMesh->m_pXMesh->LockVertexBuffer(0, (void**)&pVertex) );
        int iVertex = 0;
        for (int iFace = 0; iFace < pMesh->m_Faces.GetSize(); ++iFace)
        {
            for (int iIndex = 0; iIndex < 3; ++iIndex, ++iVertex)
            {
                pVertex[iVertex].position = pFrame->m_Positions[pMesh->m_Vertices[pMesh->m_Faces[iFace].index[iIndex]].id];
                pVertex[iVertex].normal = pMesh->m_Vertices[pMesh->m_Faces[iFace].index[iIndex]].normal;
                pVertex[iVertex].texcoord = pMesh->m_Vertices[pMesh->m_Faces[iFace].index[iIndex]].ta;
            }
            /*
            pVertex[iVertex].position = pFrame->m_Positions[iVertex];
            pVertex[iVertex].normal = pMesh->m_Vertices[iVertex].normal;
            pVertex[iVertex].texcoord = pMesh->m_Vertices[iVertex].ta;
            */
        }
        //memcpy(pVertex, pMesh->m_Vertices.GetData(), pMesh->m_Vertices.GetSize() * sizeof( stVertexPosNorTex ) );
        pMesh->m_pXMesh->UnlockVertexBuffer();
        //pMesh->m_Vertices.RemoveAll();

        /*
        int *pIndex;
        V( pMesh->m_pXMesh->LockIndexBuffer(0, (void**)&pIndex) );
        memcpy(pIndex, pMesh->m_Faces.GetData(), pMesh->m_Faces.GetSize() * sizeof(CBrfFace));
        pMesh->m_pXMesh->UnlockIndexBuffer();
        //pMesh->m_Faces.RemoveAll();
        */
    }

    int c = pMesh->m_Vertices.GetSize();
    pFrame->m_Normals.Increase(c);
    for (int i = 0; i < c; ++i)
    {
        pFrame->m_Normals[i] = pMesh->m_Vertices[i].normal;
    }
    
    pMesh->m_Frames[0] = pFrame;
    pFrame = NULL;
}


void CBrfLoader::LoadVectors(FILE *fp, CZJArray <D3DXVECTOR3> *pVectors)
{
    int c;
    LoadType(fp, &c);
    pVectors->Increase(c);
    for (int i = 0; i < c; ++i)
    {
        LoadType(fp, &pVectors->GetAt(i));
    }
}


void CBrfLoader::LoadFrame(FILE *fp, CBrfFrame *pFrame)
{
    LoadType(fp, &pFrame->time);

    LoadVectors(fp, &pFrame->m_Positions);
    LoadVectors(fp, &pFrame->m_Normals);
}

void CBrfLoader::LoadVertices(FILE *fp, CZJArray <CBrfVertex> *pVertices)
{
    int cVertices;
    LoadType(fp, &cVertices);
    pVertices->Increase(cVertices);

    switch (m_Version)
    {
    case 0: // M&B files
        {
            for (int i = 0; i < cVertices; ++i)
            {
                LoadType(fp, &pVertices->GetAt(i).id);
                LoadType(fp, &pVertices->GetAt(i).color);
                LoadType(fp, &pVertices->GetAt(i).normal);
                LoadType(fp, &pVertices->GetAt(i).ta);
                pVertices->GetAt(i).ta[1] = 1 - pVertices->GetAt(i).ta[1];
                LoadType(fp, &pVertices->GetAt(i).tb);
                pVertices->GetAt(i).tb[1] = 1 - pVertices->GetAt(i).tb[1];
            }
        }
        break;
    case 1: // warband files
        {
            for (int i = 0; i < cVertices; ++i)
            {
                LoadType(fp, &pVertices->GetAt(i).id);
                LoadType(fp, &pVertices->GetAt(i).color);
                LoadType(fp, &pVertices->GetAt(i).normal);

                //---
                LoadType(fp, &pVertices->GetAt(i).tandir);
                LoadType(fp, &pVertices->GetAt(i).iTexture);
                //---

                LoadType(fp, &pVertices->GetAt(i).ta);
                pVertices->GetAt(i).ta[1] = 1 - pVertices->GetAt(i).ta[1];
                pVertices->GetAt(i).tb = pVertices->GetAt(i).ta;
            }
        }
        break;
    case 2: // warband files
        {
            for (int i = 0; i < cVertices; ++i)
            {
                LoadType(fp, &pVertices->GetAt(i).id);
                LoadType(fp, &pVertices->GetAt(i).color);
                LoadType(fp, &pVertices->GetAt(i).normal);
                LoadType(fp, &pVertices->GetAt(i).ta);
                pVertices->GetAt(i).ta[1] = 1 - pVertices->GetAt(i).ta[1];
                pVertices->GetAt(i).tb = pVertices->GetAt(i).ta;
            }
        }
        break;
    }
}


void CBrfLoader::LoadTextures(FILE *fp, CZJArray <CBrfTexture> *pTextures)
{
    int c;
    LoadType(fp, &c);
    int i = pTextures->GetSize();
    pTextures->Increase(c);

    for (c += i; i < c; ++i)
    {
        LoadString(fp, pTextures->GetAt(i).m_Name);
        LoadType(fp, &pTextures->GetAt(i).m_Flags);
    }
}


void CBrfLoader::LoadShaders(FILE *fp, CZJArray <CBrfShader> *pShaders)
{
    int c;
    LoadType(fp, &c);
    int i = pShaders->GetSize();
    pShaders->Increase(c);

    for (c += i; i < c; ++i)
    {
        LoadString(fp, pShaders->GetAt(i).m_Name);
        LoadType(fp, &pShaders->GetAt(i).m_Flags);
        LoadType(fp, &pShaders->GetAt(i).m_Requires);
        LoadString(fp, pShaders->GetAt(i).m_strTechnique);
        int B;
        LoadType(fp, &B);
        if (B)
        {
            LoadString(fp, pShaders->GetAt(i).m_strFallback);
        }
        else
        {
            pShaders->GetAt(i).m_strFallback[0] = '\0';
        }

        int cOpt;
        LoadType(fp, &cOpt);
        pShaders->GetAt(i).m_Opts.Increase(cOpt);
        for (int iOpt = 0; iOpt < cOpt; ++iOpt)
        {
            LoadType(fp, &pShaders->GetAt(i).m_Opts[iOpt]);
        }
    }
}


void CBrfLoader::LoadMaterials(FILE *fp, CZJArray <CBrfMaterial> *pMaterials)
{
    int c;
    LoadType(fp, &c);
    int i = pMaterials->GetSize();
    pMaterials->Increase(c);

    for (c += i; i < c; ++i)
    {
        LoadString(fp, pMaterials->GetAt(i).m_Name);
        LoadType(fp, &pMaterials->GetAt(i).m_Flags);
        
        pMaterials->GetAt(i).m_RenderOrder = (pMaterials->GetAt(i).m_Flags >> 24) & 0xF;
        if (pMaterials->GetAt(i).m_RenderOrder > 7)
            pMaterials->GetAt(i).m_RenderOrder -= 16;
        pMaterials->GetAt(i).m_Flags &= 0xF0FFFFFF;

        LoadString(fp, pMaterials->GetAt(i).m_Shader);
        LoadString(fp, pMaterials->GetAt(i).m_DiffuseA);
        LoadString(fp, pMaterials->GetAt(i).m_DiffuseB);
        LoadString(fp, pMaterials->GetAt(i).m_Bump);
        LoadString(fp, pMaterials->GetAt(i).m_Enviro);

        if (!LoadString(fp, pMaterials->GetAt(i).m_Spec))
        {
            fseek(fp, -4, SEEK_CUR);
            strcpy_s(pMaterials->GetAt(i).m_Spec, "none");
        }

        LoadType(fp, &pMaterials->GetAt(i).m_Specular);
        LoadType(fp, &pMaterials->GetAt(i).r);
        LoadType(fp, &pMaterials->GetAt(i).g);
        LoadType(fp, &pMaterials->GetAt(i).b);
    }
}


void CBrfLoader::LoadSkeletons(FILE *fp, CZJArray <CBrfSkeleton> *pSkeletons)
{
    int c;
    LoadType(fp, &c);
    int i = pSkeletons->GetSize();
    pSkeletons->Increase(c);

    for (c += i; i < c; ++i)
    {
        LoadString(fp, pSkeletons->GetAt(i).m_Name);
        LoadBones(fp, &pSkeletons->GetAt(i).m_Bones);
    }
}


void CBrfLoader::LoadBones(FILE *fp, CZJArray <CBrfBone> *pBones)
{
    int c;
    LoadType(fp, &c);
    pBones->Increase(c);

    for (int i = 0; i < c; ++i)
    {
        LoadType(fp, &pBones->GetAt(i).attach);

        if (LoadString(fp, pBones->GetAt(i).m_Name))
        {
            LoadType(fp, &pBones->GetAt(i).b);
        }
        else
        {
            fseek(fp, -4, SEEK_CUR);
            strcpy_s(pBones->GetAt(i).m_Name, "none");
        }

        LoadType(fp, &pBones->GetAt(i).x);
        LoadType(fp, &pBones->GetAt(i).z); // Is this right ???!!!
        LoadType(fp, &pBones->GetAt(i).y);
        LoadType(fp, &pBones->GetAt(i).t);
    }
}


void CBrfLoader::LoadAnimations(FILE *fp, CZJArray <CBrfAnimation> *pAnimations)
{
    int c;
    LoadType(fp, &c);
    int i = pAnimations->GetSize();
    pAnimations->Increase(c);

    for (c += i; i < c; ++i)
    {
        LoadString(fp, pAnimations->GetAt(i).m_Name);

        CZJArray <stTmpBone4> tmpBone4v;
        int cBones;
        LoadType(fp, &cBones);
        tmpBone4v.Increase(cBones);
        for (int iBone = 0; iBone < cBones; ++iBone)
        {
            int cCas;
            LoadType(fp, &cCas);
            tmpBone4v[iBone].cas.Increase(cCas);
            for (int iCas = 0; iCas < cCas; ++iCas)
            {
                LoadType(fp, &tmpBone4v[iBone].cas[iCas]);
            }
        }

        CZJArray <stTmpCas3> tmpCas3v;
        int nCas;
        LoadType(fp, &nCas);
        tmpCas3v.Increase(nCas);
        for (int iCas = 0; iCas < nCas; ++iCas)
        {
            LoadType(fp, &tmpCas3v[iCas]);
        }
    }
}


void CBrfLoader::LoadBodies(FILE *fp, CZJArray <CBrfBody> *pBodies)
{
    int c;
    LoadType(fp, &c);
    int i = pBodies->GetSize();
    pBodies->Increase(c);

    char str[256];
    for (c += i; i < c; ++i)
    {
        LoadString(fp, pBodies->GetAt(i).m_Name);

        LoadString(fp, str);

        int nParts;
        if (0 == strcmp(str, "composite"))
        {
            LoadType(fp, &nParts);
            LoadString(fp, str);
        }
        else
        {
            nParts = 1;
        }
        pBodies->GetAt(i).m_Parts.Increase(nParts);
        for (int iPart = 0; iPart < nParts; ++iPart)
        {
            if (iPart > 0)
                LoadString(fp, str);

            if (0 == strcmp(str, "manifold"))
            {
                pBodies->GetAt(i).m_Parts[iPart].type = MANIFOLD;

                LoadArray(fp, &pBodies->GetAt(i).m_Parts[iPart].m_Positions);

                int nFaces;
                LoadType(fp, &nFaces);
                pBodies->GetAt(i).m_Parts[iPart].m_Faces.Increase(nFaces);
                CZJArray <int> *v;
                for (int iFace = 0; iFace < nFaces; ++iFace)
                {
                    LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].orientation); 
                    if (pBodies->GetAt(i).m_Parts[iPart].orientation != -1)
                        pBodies->GetAt(i).m_Parts[iPart].orientation = 1;

                    int nvertexes;
                    LoadType(fp, &nvertexes);
                    ASSERT( nvertexes == 0 );
                    
                    v = new CZJArray <int>;
                    LoadArray(fp, v);

                    pBodies->GetAt(i).m_Parts[iPart].m_Faces[iFace] = v;
                }
            }
            else if (0 == strcmp(str, "capsule"))
            {
                pBodies->GetAt(i).m_Parts[iPart].type = CAPSULE;
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].radius);
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].center);
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].dir);
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].m_Flags);
            }
            else if (0 == strcmp(str, "sphere"))
            {
                pBodies->GetAt(i).m_Parts[iPart].type = SPHERE;
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].radius);
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].center);
                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].m_Flags);
            }
            else if (0 == strcmp(str, "face"))
            {
                pBodies->GetAt(i).m_Parts[iPart].type = FACE;
                LoadArray(fp, &pBodies->GetAt(i).m_Parts[iPart].m_Positions);

                int nPositions = pBodies->GetAt(i).m_Parts[iPart].m_Positions.GetSize();

                CZJArray <int> *v = new CZJArray <int>;
                v->Increase(nPositions);

                for (int iPosition = 0; iPosition < nPositions; ++iPosition)
                {
                    v->GetAt(iPosition) = iPosition;
                }

                pBodies->GetAt(i).m_Parts[iPart].m_Faces.Increase();
                pBodies->GetAt(i).m_Parts[iPart].m_Faces[0] = v;

                LoadType(fp, &pBodies->GetAt(i).m_Parts[iPart].m_Flags);
            }
            else
            {
                ASSERT( NULL );
            }
        }
    }
}