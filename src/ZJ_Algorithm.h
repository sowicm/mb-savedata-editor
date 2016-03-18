#pragma once

#ifndef ZJ_ALGORITHM
#define ZJ_ALGORITHM

void InitAlgorithm();


int KMP(unsigned char* strSource, int nSourceLength, unsigned char* strTemplete, int nTempleteLength, int nStartPos = 0);


int wtoi(const wchar_t *pw);
wchar_t* itow(int n);

int wtou(const wchar_t *pw);
wchar_t* utow(unsigned n);


const char chHex[] = "0123456789ABCDEF";
unsigned char* GetUszFromHex(const char* strHex);
char* GetHexFromQWORD(QWORD qw);


extern char lpszByteToHex[256][3];

#endif