#include "stdafx.h"
#include "ZJ_Algorithm.h"

char lpszByteToHex[256][3];

void InitAlgorithm()
{
    static bool bInit = false;
    if (bInit)
        return;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
        {
            lpszByteToHex[i * 16 + j][0] = chHex[i];
            lpszByteToHex[i * 16 + j][1] = chHex[j];
            lpszByteToHex[i * 16 + j][2] = 0;
        }
    bInit = true;
}

int KMP(unsigned char* strSource, int nSourceLength, unsigned char* strTemplete, int nTempleteLength, int nStartPos)
{
    int *shift = new int[nTempleteLength];
    int i = 0, j = -1;
    shift[0] = -1;
    while (i < nTempleteLength - 1)
    {
        if (j == -1 || strTemplete[i] == strTemplete[j])
        {
            ++i;
            ++j;
            if (strTemplete[i] != strTemplete[j])
                shift[i] = j;
            else
                shift[i] = shift[j];
        }
        else
            j = shift[j];
    }
    i = nStartPos, j = 0;
    while (i < nSourceLength && j < nTempleteLength)
    {
        if (j == -1 || strSource[i] == strTemplete[j])
        {
            ++i;
            ++j;
        }
        else
        {
            j = shift[j];
        }
    }
    delete shift;
    return j >= nTempleteLength ? i : -1;
}


int wtoi(const wchar_t *pw)
{
    int n = 0;
    int i;
    bool bNeg;
    if (pw[0] == '-')
    {
        bNeg = true;
        i = 1;
    }
    else
    {
        bNeg = false;
        i = 0;
    }
    for (; pw[i]; ++i)
    {
        n = n * 10 + pw[i] - '0';
    }
    if (bNeg)
        n = -n;
    return n;
}


wchar_t* itow(int n)
{
    bool bNeg;
    int len;
    if (n < 0)
    {
        bNeg = true;
        n &= 0x7ffffffff;
        len = 1;
    }
    else
    {
        bNeg = false;
        len = 0;
    }
    for (int i = n; i > 0; i /= 10, ++len);
    wchar_t *pw = new wchar_t[len + 1];
    for (int i = len - 1; n > 0; n /= 10, --i)
    {
        pw[i] = n % 10 + '0';
    }
    if (bNeg)
        pw[0] = '-';
    pw[len] = 0;
    return pw;
}


int wtou(const wchar_t *pw)
{
    int n = 0;
    for (int i = 0; pw[i]; ++i)
    {
        n = n * 10 + pw[i] - '0';
    }
    return n;
}


wchar_t* utow(unsigned n)
{
    int len = 0;
    int i = n;
    do
    {
        i /= 10;
        ++len;
    } while (i > 0);

    wchar_t *pw = new wchar_t[len + 1];
    i = len - 1;
    do
    {
        pw[i] = n % 10 + '0';
        n /= 10;
        --i;
    } while (n > 0);
    pw[len] = 0;
    return pw;
}


unsigned char* GetUszFromHex(const char* strHex)
{
    int nLength = strlen(strHex);
    ASSERT(!(nLength & 1));
    unsigned char* usz = new unsigned char[nLength >> 1];
    for (int i = 0; strHex[i]; i += 2)
    {
        for (int j = 0; j < 16; ++j)
        {
            if (strHex[i] == chHex[j])
            {
                usz[i >> 1] = j * 16;
                for (j = 0; j < 16; ++j)
                {
                    if (strHex[i + 1] == chHex[j])
                    {
                        usz[i >> 1] += j;
                        break;
                    }
                }
                break;
            }
        }
    }
    return usz;
}


char* GetHexFromQWORD(QWORD qw)
{
    char *str = new char[2 * 2 * 4 + 1];
    int n;
    for (int i = 0; i < 2 * 4; ++i)
    {
        n = int((qw >> (i << 3)) & 0xFF);
        str[i << 1] = lpszByteToHex[n][0];
        str[(i << 1) + 1] = lpszByteToHex[n][1];
    }
    str[2 * 2 * 4] = '\0';
    return str;
}