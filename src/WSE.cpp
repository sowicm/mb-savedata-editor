#include "StdAfx.h"
#include "WSE.h"

#include "ZJ_Algorithm.h"
#include "GUIResource.h"
#include "MainFrm.h"
#include "MainCore.h"

using namespace WSE;

CWSE::CWSE(void)
    : m_PathName(_T(""))
    , m_puFileData(NULL)
    , m_nFileLength(0)
    , m_bCannotQuitWithoutSave(false)
{
    pCurTroop = &trpPlayer;
    InitAlgorithm();

    trpNpc = NULL;
}


CWSE::~CWSE(void)
{
    SAFE_DELETE_ARRAY(trpNpc);
}

CWSE theWSE;

void CWSE::OnFileOpen(void)
{
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("M&B存档 (*.sav)|*.sav||"));
    fileDlg.DoModal();
    if (!fileDlg.GetPathName().IsEmpty())
    {
        m_PathName = fileDlg.GetPathName();
        m_bBackUp = true;
#if 1
        FILE *fp;
        _tfopen_s(&fp, m_PathName.GetBuffer(), _T("rb"));

        fseek(fp, 0L, SEEK_END);
        m_nFileLength = ftell(fp);

        fseek(fp, 0L, SEEK_SET);

#else
        
        CFileStatus status;
        CFile::GetStatus(m_PathName, status);

        m_nFileLength = (int)status.m_size;

#endif

        SAFE_DELETE_ARRAY(m_puFileData);
        m_puFileData = new unsigned char[m_nFileLength];
        
#if 1
        fread(m_puFileData, m_nFileLength + 1, 1, fp);
        fclose(fp);
#else

        ifstream file(m_PathName, ios::binary);
        file.read((char *)m_puFileData, m_nFileLength);
        file.close();

#endif

        /*
        char pFileHead[6];
        memcpy(pFileHead, m_puFileData, 5);
        pFileHead[5] = 0;
        if (strcmp(pFileHead, "WRDRn"))
        {
            ::AfxMessageBox(_T("此文件不是战团存档！"));
            m_PathName.Empty();
            return;
        }
        */

        unsigned char puFlag[] = {255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0};
        
        const int nFlagLength = 48;
        
        int i = KMP(m_puFileData, m_nFileLength, puFlag, nFlagLength);
        if (i == -1)
        {
            ::AfxMessageBox(_T("此文件不是战团存档！"));
            m_PathName.Empty();
            return;
        }

        /*
        // For others
        unsigned char *uszflag = GetUszFromHex("FFFFFFFFFFFFFFFF");
        i = KMP(m_puFileData, m_nFileLength, uszflag, 8, i);
        if (i == -1)
        {
            ::AfxMessageBox(_T("此文件不是战团存档！1"));
            m_szPathName.Empty();
            return;
        }
        i = KMP(m_puFileData, m_nFileLength, uszflag, 8, i);
        if (i == -1)
        {
            ::AfxMessageBox(_T("此文件不是战团存档！2"));
            m_szPathName.Empty();
            return;
        }
        i = KMP(m_puFileData, m_nFileLength, uszflag, 8, i);
        if (i == -1)
        {
            ::AfxMessageBox(_T("此文件不是战团存档！3"));
            m_szPathName.Empty();
            return;
        }*/

        char PlayerName[64];
        int j;
        for (j = 16; m_puFileData[j]; ++j);
        j += 3;
        int k;
        for (k = j; m_puFileData[k]; ++k);
        if (k - j > 63)
        {
            AfxMessageBox(_T("您取的角色名比作者预料中的长...读档失败！"));
            m_PathName.Empty();
            return;
        }
        memcpy_s(PlayerName, 64, m_puFileData + j, k - j + 1);
        PlayerName[k - j - 1] = 0;
        wchar_t wszPlayerName[64];
        int nNameLength = MultiByteToWideChar(CP_UTF8, 0, PlayerName, -1, NULL, 0);
        MultiByteToWideChar(CP_UTF8, 0, PlayerName, -1, wszPlayerName, nNameLength);
        *trpPlayer.pName = wszPlayerName;
        trpPlayer.pName->Replace(_T(" "), _T(""));

        m_uIndexOfQuitWithoutSave = k + 19;
        m_bCannotQuitWithoutSave = !m_puFileData[m_uIndexOfQuitWithoutSave];

        trpPlayer.nIndex = i + 1320;
        i = GetTroopInfo(trpPlayer);

        m_pNPCList->AddItem(*trpPlayer.pName);

        //NPC
        unsigned char *usz;
        const int nFaceKeyLength = 11;
        int nStartPos = i;

        trpNpc = new CTroop[theSMS.m_NPCs.GetSize()];
        for (int iNPC = 0; iNPC < theSMS.m_NPCs.GetSize(); ++iNPC)
        {
            *trpNpc[iNPC].pName = theSMS.m_NPCs[iNPC].text;
            usz = GetUszFromHex(theSMS.m_NPCs[iNPC].facekey);
            i = KMP(m_puFileData, m_nFileLength, usz, nFaceKeyLength, nStartPos);
            delete[] usz;
            if (i == -1)
            {
                AfxMessageBox(_T("该存档NPC人物容貌被改动，无法继续。请关注更新。"));
                return;
            }
            trpNpc[iNPC].nIndex = i - 1187; /* 498h 1176 */
            nStartPos = GetTroopInfo(trpNpc[iNPC]);
            m_pNPCList->AddItem(theSMS.m_NPCs[iNPC].text);
        }

        m_pNPCList->UpdateScrollBar();

        GetItemPlayerInfo();

        theCore.m_pMainFrm->Update(false);
    }
}

void CWSE::OnSaveFile(void)
{
    if (m_PathName.IsEmpty())
        return;
    theCore.m_pMainFrm->Update(true);
    m_puFileData[m_uIndexOfQuitWithoutSave] = !m_bCannotQuitWithoutSave;
    PutTroopInfo(trpPlayer);
    PutItemPlayerInfo();
    for (int j = 1; j <= theSMS.m_NPCs.GetSize(); ++j)
        PutTroopInfo(trpNpc[j]);

    if (m_bBackUp)
    {
        m_bBackUp = false;
        CopyFile(m_PathName, m_PathName + _T(".bak"), FALSE);
    }

    ofstream file(m_PathName, ios::binary);
    file.write((char *)m_puFileData, m_nFileLength);
    file.close();

    //m_pMainFrm->m_guiTop.Trace(_T("成功保存..."));
}



void CWSE::OnSaveFileAs(void)
{
    CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("M&B存档 (*.sav)|*.sav||"));
    fileDlg.DoModal();    
    if (!fileDlg.GetPathName().IsEmpty())
    {
        m_PathName = fileDlg.GetPathName();
        OnSaveFile();
    }
}


int CWSE::GetTroopInfo(CTroop& trp)
{
    int i = trp.nIndex;
    ASSERT(i != 0);
    trp.attSTR = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.attAGI = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.attINT = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.attCHA = *(unsigned*)(&m_puFileData[i]);

    //
    unsigned t;

    i += 32;
    t = m_puFileData[i];
    trp.sklTrade = t & 0xF;
    trp.sklLeadership = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklPManagement = t & 0xF;
    trp.sklR[1] = (t & 0xF0) >> 4;        

    t = m_puFileData[++i];
    trp.sklR[2] = t & 0xF;
    trp.sklR[3] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[4] = t & 0xF;
    trp.sklPersuation = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklEngineer = t & 0xF;
    trp.sklFirstAid = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklSurgery = t & 0xF;
    trp.sklWTreatment = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklIManagement = t & 0xF;
    trp.sklSpotting = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklPathfinding = t & 0xF;
    trp.sklTactics = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklTracking = t & 0xF;
    trp.sklTrainer = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[5] = t & 0xF;
    trp.sklR[6] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[7] = t & 0xF;
    trp.sklR[8] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklLooting = t & 0xF;
    trp.sklHArchery = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklRiding = t & 0xF;
    trp.sklAthletics = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklShield = t & 0xF;
    trp.sklWMaster = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[9] = t & 0xF;
    trp.sklR[10] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[11] = t & 0xF;
    trp.sklR[12] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[13] = t & 0xF;
    trp.sklPDraw = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklPThrow = t & 0xF;
    trp.sklPStrike = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklIronflesh = t & 0xF;
    trp.sklR[14] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[15] = t & 0xF;
    trp.sklR[16] = (t & 0xF0) >> 4;

    t = m_puFileData[++i];
    trp.sklR[17] = t & 0xF;
    trp.sklR[18] = (t & 0xF0) >> 4;

    //at FD109H

    i += 224;
    trp.nSkillPoints = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.nAttributePoints = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.nWeaponPoints = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.nLevel = *(unsigned*)(&m_puFileData[i]);

    i += 4;
    trp.nMoney = *(unsigned*)(&m_puFileData[i]);    //++i;trp.nMoney |= (m_puFileData[i] << 8);

    i += 4;
    trp.nXP = *(unsigned*)(&m_puFileData[i]);//++i;trp.nXP |= (m_puFileData[i] << 8);++i;trp.nXP |= (m_puFileData[i] << 16);++i;trp.nXP |= (m_puFileData[i] << 24);

    i += 860;
    // 89 67  5 23 41
    char str[17] = {0};
    str[7] = lpszByteToHex[m_puFileData[i]][0];
    str[8] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[5] = lpszByteToHex[m_puFileData[i]][0];
    str[6] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[4] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[1] = lpszByteToHex[m_puFileData[i]][0];
    str[2] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[3] = lpszByteToHex[m_puFileData[i]][0];
    str[0] = lpszByteToHex[m_puFileData[i]][1];
    *trp.pszFaceKey1 = str;
    trp.pszFaceKey1->MakeLower();

    i += 4;
    char str2[17];
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    *trp.pszFaceKey2 = str2;
    trp.pszFaceKey2->MakeLower();
    return i;
}



void CWSE::GetItemPlayerInfo(void)
{
    /*
    ((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->
        m_ItemList.DeleteAllItems();
    ((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->
        m_ImageList.Create(IDB_WRITELARGE, 32, 1, 0);
    ((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->
        m_ItemList.SetImageList(&((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->m_ImageList, LVSIL_NORMAL);

    int i = trpPlayer.nIndex + 320;
    for (int j = 0; j < CItem::Number; ++j)
    {
        itmPlayer[j].nID = *(unsigned*)(&m_puFileData[i]);
        i += 4;
        itmPlayer[j].nConsumed = m_puFileData[i];
        i += 3;
        itmPlayer[j].nModifier = m_puFileData[i];
        ++i;
        if (itmPlayer[j].nID != -1)
        {
            itmPlayer[j].szName = pCurMod->szModifierNames[itmPlayer[j].nModifier] +
                pCurMod->szItemNames[itmPlayer[j].nID];
            ((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->
                m_ItemList.InsertItem(j, itmPlayer[j].szName, 0);//, IDB_MAIN);
        }
        else
        {
            ((CItemView*)((CMainFrame*)m_pMainWnd)->m_pItemView)->
                m_ItemList.InsertItem(j, _T("空物品栏"), 0);//, IDB_MAIN);
        }
    }*/
}


void CWSE::PutTroopInfo(CTroop& trp)
{
    int i = trp.nIndex;
    ASSERT(i != 0);
    *(unsigned*)(&m_puFileData[i]) = trp.attSTR;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.attAGI;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.attINT;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.attCHA;

    //
    unsigned t;

    i += 32;
    t = trp.sklTrade | (trp.sklLeadership << 4);
    m_puFileData[i] = t;


    t = trp.sklPManagement | (trp.sklR[1] << 4);
    m_puFileData[++i] = t;

     t = trp.sklR[2] | (trp.sklR[3] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[4] | (trp.sklPersuation << 4);
    m_puFileData[++i] = t;

    t = trp.sklEngineer | (trp.sklFirstAid << 4);
    m_puFileData[++i] = t;

    t = trp.sklSurgery | (trp.sklWTreatment << 4);
    m_puFileData[++i] = t;

    t = trp.sklIManagement | (trp.sklSpotting << 4);
    m_puFileData[++i] = t;

    t = trp.sklPathfinding | (trp.sklTactics << 4);
    m_puFileData[++i] = t;

    t = trp.sklTracking | (trp.sklTrainer << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[5] | (trp.sklR[6] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[7] | (trp.sklR[8] << 4);
    m_puFileData[++i] = t;

    t = trp.sklLooting | (trp.sklHArchery << 4);
    m_puFileData[++i] = t;

    t = trp.sklRiding | (trp.sklAthletics << 4);
    m_puFileData[++i] = t;

    t = trp.sklShield | (trp.sklWMaster << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[9] | (trp.sklR[10] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[11] | (trp.sklR[12] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[13] | (trp.sklPDraw << 4);
    m_puFileData[++i] = t;

    t = trp.sklPThrow | (trp.sklPStrike << 4);
    m_puFileData[++i] = t;

    t = trp.sklIronflesh | (trp.sklR[14] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[15] | (trp.sklR[16] << 4);
    m_puFileData[++i] = t;

    t = trp.sklR[17] | (trp.sklR[18] << 4);
    m_puFileData[++i] = t;

    //at FD109H

    i += 224;
    *(unsigned*)(&m_puFileData[i]) = trp.nSkillPoints;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.nAttributePoints;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.nWeaponPoints;

    i += 4;
    //m_puFileData[i] = trp.nLevel;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.nMoney;

    i += 4;
    *(unsigned*)(&m_puFileData[i]) = trp.nXP;

    i += 860;
    // 89 67  5 23 41
    /*
    char str[17] = {0};
    str[7] = lpszByteToHex[m_puFileData[i]][0];
    str[8] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[5] = lpszByteToHex[m_puFileData[i]][0];
    str[6] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[4] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[1] = lpszByteToHex[m_puFileData[i]][0];
    str[2] = lpszByteToHex[m_puFileData[i]][1];
    ++i;
    str[3] = lpszByteToHex[m_puFileData[i]][0];
    str[0] = lpszByteToHex[m_puFileData[i]][1];
    *trp.pszFaceKey1 = str;
    trp.pszFaceKey1->MakeLower();

    i += 4;
    char str2[17];
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);
    ++i;
    strcpy(str, lpszByteToHex[m_puFileData[i]]);
    strcat(str, str2);
    ++i;
    strcpy(str2, lpszByteToHex[m_puFileData[i]]);
    strcat(str2, str);*/
    //*trp.pszFaceKey2 = str2;
    //trp.pszFaceKey2->MakeLower();
}


void CWSE::PutItemPlayerInfo(void)
{
    /*
    int i = trpPlayer.nIndex + 320;
    for (int j = 0; j < CItem::Number; ++j)
    {
        *(unsigned*)(&m_puFileData[i]) = itmPlayer[j].nID;
        i += 4;
        m_puFileData[i] = itmPlayer[j].nConsumed;
        i += 3;
        m_puFileData[i] = itmPlayer[j].nModifier;
        ++i;
    }*/
}
