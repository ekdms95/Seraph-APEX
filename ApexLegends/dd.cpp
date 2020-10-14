#include "dd.h"

dd::dd() : m_hModule(NULL)
, m_pfDD_btn(NULL)
, m_pfDD_mov(NULL)
, m_pfDD_key(NULL)
, m_pfDD_movR(NULL)
, m_pfDD_str(NULL)
, m_pfDD_whl(NULL)
, m_pfDD_todc(NULL)
{
}

dd::dd(LPCWSTR lpszDllPath) : m_hModule(NULL)
, m_pfDD_btn(NULL)
, m_pfDD_mov(NULL)
, m_pfDD_key(NULL)
, m_pfDD_movR(NULL)
, m_pfDD_str(NULL)
, m_pfDD_whl(NULL)
, m_pfDD_todc(NULL)
{
	if (this->Init(lpszDllPath) != 0)
	{
		MessageBox(NULL, TEXT("DD_초기화 실패"), TEXT("Error"), MB_OK);
	}
}

dd::~dd()
{
	this->DisPose();
}

INT dd::Init(LPCWSTR lpszDllPath)
{
	if (_waccess_s(lpszDllPath, 0))
	{
		return -1;
	}

	this->m_hModule = LoadLibraryW(lpszDllPath);
	if (this->m_hModule == NULL)
	{
		return -2;
	}


	//함수를 못 찾았을 때 에러처리 추가할 것.
	this->m_pfDD_btn = (PFDD_BTN)GetProcAddress(this->m_hModule, "DD_btn");
	this->m_pfDD_mov = (PFDD_MOV)GetProcAddress(this->m_hModule, "DD_mov");
	this->m_pfDD_key = (PFDD_KEY)GetProcAddress(this->m_hModule, "DD_key");
	this->m_pfDD_movR = (PFDD_MOVR)GetProcAddress(this->m_hModule, "DD_movR");
	this->m_pfDD_str = (PFDD_STR)GetProcAddress(this->m_hModule, "DD_str");
	this->m_pfDD_whl = (PFDD_WHL)GetProcAddress(this->m_hModule, "DD_whl");
	this->m_pfDD_todc = (PFDD_TODC)GetProcAddress(this->m_hModule, "DD_todc");

	return 0;
}

VOID dd::DisPose()
{
	if (this->m_hModule != NULL)
	{
		FreeLibrary(this->m_hModule);
		this->m_hModule = NULL;
	}
}

INT dd::DD_btn(INT nParam)
{
	if (this->m_hModule == NULL || this->m_pfDD_mov == NULL)
	{
		return -2;
	}

	return this->m_pfDD_btn(nParam);
}

INT dd::DD_mov(INT x, INT y)
{
	if (this->m_hModule == NULL || this->m_pfDD_mov == NULL)
	{
		return -2;
	}

	return this->m_pfDD_mov(x, y);
}

INT dd::DD_movR(INT dx, INT dy)
{
	if (this->m_hModule == NULL || this->m_pfDD_movR == NULL)
	{
		return -2;
	}

	return this->m_pfDD_movR(dx, dy);
}

INT dd::DD_whl(INT nParam)
{
	if (this->m_hModule == NULL || this->m_pfDD_whl == NULL)
	{
		return -2;
	}
	return this->m_pfDD_whl(nParam);
}

INT dd::DD_key(INT nParam1, INT nParam2)
{
	if (this->m_hModule == NULL || this->m_pfDD_key == NULL)
	{
		return -2;
	}

	return this->m_pfDD_key(nParam1, nParam2);
}

INT dd::DD_str(LPCSTR lpszSendString)
{
	if (this->m_hModule == NULL || this->m_pfDD_str == NULL)
	{
		return -2;
	}

	return this->m_pfDD_str(lpszSendString);
}

INT dd::DD_todc(INT nVK_Code)
{
	if (this->m_hModule == NULL || this->m_pfDD_todc == NULL)
	{
		return -2;
	}

	return this->m_pfDD_todc(nVK_Code);
}
