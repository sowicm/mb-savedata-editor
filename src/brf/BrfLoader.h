#pragma once

#include "..\..\CZJGUI\CZJGUI\ZJArray.h"
#include "BrfMesh.h"
#include "BrfTexture.h"
#include "BrfShader.h"
#include "BrfMaterial.h"
#include "BrfSkeleton.h"
#include "BrfAnimation.h"
#include "BrfBody.h"

#include "VertexStruct.h"

#include "..\..\CZJGUI\CZJGUI\ZJManager.h"

class CBrfLoader
{
public:
    CBrfLoader(void);
    ~CBrfLoader(void);

    bool Load(wchar_t *strFilePath);

    template <typename T>
    static inline void LoadType(FILE *fp, T *p)
    {
        fread(p, sizeof(T), 1, fp);
    }
    template <typename T>
    static inline void LoadArray(FILE *fp, CZJArray <T> *pA)
    {
        int n;
        LoadType(fp, &n);
        pA->Increase(n);
        for (int i = 0; i < n; ++i)
            LoadType(fp, &pA->GetAt(i));
    }
    static inline void LoadInt(FILE *fp, int *pi)
    {
        fread(pi, 4, 1, fp);
    }
    static inline void LoadUINT(FILE *fp, UINT *pu)
    {
        fread(pu, 4, 1, fp);
    }
    static bool LoadString(FILE *fp, char *str);

    //---- Meshes
    void LoadMeshes(FILE *fp, CZJArray <CBrfMesh*> *meshes);
    void LoadMesh(FILE *fp, CBrfMesh* pMesh);

    static void LoadVectors(FILE *fp, CZJArray <D3DXVECTOR3> *pVectors);

    static void LoadFrame(FILE *fp, CBrfFrame *pFrame);    
    
    void LoadRiggings(FILE *fp, CZJArray <stTmpRigging> *pRigs);
    
    void LoadVertices(FILE *fp, CZJArray <CBrfVertex> *pVertices);

    //void LoadFaces(FILE *fp, CZJArray <CBrfFace> *pFaces);

    //---- Textures
    void LoadTextures(FILE *fp, CZJArray <CBrfTexture> *pTextures);

    //---- Shaders
    void LoadShaders(FILE *fp, CZJArray <CBrfShader> *pShaders);

    //---- Materials
    void LoadMaterials(FILE *fp, CZJArray <CBrfMaterial> *pMaterials);

    //---- Skeletons
    void LoadSkeletons(FILE *fp, CZJArray <CBrfSkeleton> *pSkeletons);
    void LoadBones(FILE *fp, CZJArray <CBrfBone> *pBones);

    //---- Animations
    void LoadAnimations(FILE *fp, CZJArray <CBrfAnimation> *pAnimations);

    //---- Bodies
    void LoadBodies(FILE *fp, CZJArray <CBrfBody> *pBodies);

    
    LPDIRECT3DDEVICE9                   m_pDevice;
    CZJManager                         *m_pManager;

private:

    int                                 m_Version;
    CZJArray <CBrfMesh*>                m_Meshes;
    CZJArray <CBrfTexture>              m_Textures;
    CZJArray <CBrfShader>               m_Shaders;
    CZJArray <CBrfMaterial>             m_Materials;
    CZJArray <CBrfSkeleton>             m_Skeletons;
    CZJArray <CBrfAnimation>            m_Animations;
    CZJArray <CBrfBody>                 m_Bodies;
};

