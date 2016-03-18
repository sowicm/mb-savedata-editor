#pragma once

#include "Troop.h"
#include "MainFrm.h"

class CMainFrame;

class CWSE
{
public:
    CWSE(void);
    ~CWSE(void);

    void OnFileOpen(void);
    void OnSaveFile(void);
    void OnSaveFileAs(void);

    int GetTroopInfo(WSE::CTroop& trp);
    void GetItemPlayerInfo(void);

    void PutTroopInfo(WSE::CTroop& trp);
    void PutItemPlayerInfo(void);

private:
    template <typename T>
    inline void GetType(int &nIndex, T *p)
    {
        *p = *(T*)(&m_puFileData[nIndex]);
    }


protected:
    CString        m_PathName;
    int            m_nFileLength;
    unsigned char *m_puFileData;
    unsigned       m_uIndexOfQuitWithoutSave;

    bool           m_bBackUp;
    

public:
    WSE::CTroop     *pCurTroop, trpPlayer, *trpNpc;
    CZJGUIListBox   *m_pNPCList;

    bool           m_bCannotQuitWithoutSave;

public:    
    bool IsEmpty()  { return m_PathName.IsEmpty(); }
};

extern CWSE theWSE;