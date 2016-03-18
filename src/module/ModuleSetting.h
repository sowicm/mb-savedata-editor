#pragma once
class CModuleSetting
{
public:
    CModuleSetting(void);
    ~CModuleSetting(void);    

    static const int nNumModifier = 43;
    static const int nNumItem = 728;

    CString szModifierNames[nNumModifier];
    CString szItemNames[nNumItem];
};

