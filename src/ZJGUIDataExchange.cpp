#include "StdAfx.h"
#include "ZJGUIDataExchange.h"
#include "ZJ_Algorithm.h"

void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, CString& value)
{
    CZJGUIView *pView = gui.GetView(iView);
    for (int i = 0; i < pView->GetNumControls(); ++i)
    {
        if (pView->GetControl(i)->GetID() == nIDC)
        {
            if (pDX->m_bSaveAndValidate)
            {
                value = pView->GetControl(i)->GetText();
            }
            else
            {
                pView->GetControl(i)->SetText(value.GetBuffer());
            }
            break;
        }
    }
}
void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, DWORD& value)
{
    CZJGUIView *pView = gui.GetView(iView);
    for (int i = 0; i < pView->GetNumControls(); ++i)
    {
        if (pView->GetControl(i)->GetID() == nIDC)
        {
            if (pDX->m_bSaveAndValidate)
            {
                value = wtou(pView->GetControl(i)->GetText());
            }
            else
            {
                pView->GetControl(i)->SetTextDirectly(utow(value));
            }
            break;
        }
    }
}
void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, int& value)
{
    CZJGUIView *pView = gui.GetView(iView);
    for (int i = 0; i < pView->GetNumControls(); ++i)
    {
        if (pView->GetControl(i)->GetID() == nIDC)
        {
            if (pDX->m_bSaveAndValidate)
            {
                value = wtoi(pView->GetControl(i)->GetText());
            }
            else
            {
                pView->GetControl(i)->SetTextDirectly(itow(value));
            }
            break;
        }
    }
}

void DDX_Check(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, bool& value)
{
    CZJGUIView *pView = gui.GetView(iView);
    for (int i = 0; i < pView->GetNumControls(); ++i)
    {
        if (pView->GetControl(i)->GetID() == nIDC)
        {
            if (pDX->m_bSaveAndValidate)
            {
                value = ((CZJGUICheckBox*)pView->GetControl(i))->GetChecked();
            }
            else
            {
                ((CZJGUICheckBox*)pView->GetControl(i))->SetChecked(value);
            }
            break;
        }
    }
}