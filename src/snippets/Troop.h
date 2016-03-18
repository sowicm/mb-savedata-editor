#pragma once
class CTroop
{
public:
    CTroop(void);
    ~CTroop(void);
public:
    unsigned nIndex;
    DWORD nLevel;
    DWORD nMoney;
    DWORD nXP;
    DWORD nAttributePoints, nSkillPoints, nWeaponPoints;
    DWORD attSTR, attAGI, attINT, attCHA;
    DWORD sklIronflesh,
        sklPStrike,
        sklPThrow,
        sklPDraw,
        sklWMaster,
        sklShield,
        sklAthletics,
        sklRiding,
        sklHArchery,
        sklLooting,
        sklTrainer,
        sklTracking, // not yet ?
        sklTactics,
        sklPathfinding,
        sklSpotting,
        sklIManagement,
        sklWTreatment,
        sklSurgery,
        sklFirstAid,
        sklEngineer,
        sklPersuation,
        sklPManagement,
        sklLeadership,
        sklTrade,
        sklR[100];    
    CString *pszFaceKey1, *pszFaceKey2;
    CString *pName;
};

