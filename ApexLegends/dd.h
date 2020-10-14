#pragma once
#include <Windows.h>
#include <iostream>

#define DD_BTN_LBUTTONDOWN 1
#define DD_BTN_LBUTTONUP 2
#define DD_BTN_RBUTTONDOWN 4
#define DD_BTN_RBUTTONUP 8
#define DD_BTN_MBUTTONDOWN 16
#define DD_BTN_MBUTTONUP 32
#define DD_BTN_4BUTTONDOWN 64
#define DD_BTN_4BUTTONUP 128
#define DD_BTN_5BUTTONDOWN 256
#define DD_BTN_5BUTTONUP 512

#define DD_KEY_DOWN 1
#define DD_KEY_UP 2

typedef INT(*PFDD_BTN)(INT nParam);
typedef INT(*PFDD_MOV)(INT x, INT y);
typedef INT(*PFDD_MOVR)(INT dx, INT dy);
typedef INT(*PFDD_WHL)(INT nParam);
typedef INT(*PFDD_KEY)(INT nParam1, INT nParam2);
typedef INT(*PFDD_STR)(LPCSTR lpszSendString);
typedef INT(*PFDD_TODC)(INT nVK_Code);

class dd
{
private:
	HMODULE m_hModule;
	PFDD_BTN m_pfDD_btn = NULL;
	PFDD_MOV m_pfDD_mov = NULL;
	PFDD_MOVR m_pfDD_movR = NULL;
	PFDD_WHL m_pfDD_whl = NULL;
	PFDD_KEY m_pfDD_key = NULL;
	PFDD_STR m_pfDD_str = NULL;
	PFDD_TODC m_pfDD_todc = NULL;

public:
	dd();
	dd(LPCWSTR lpszDllPath);
	~dd();

	/*
		DD를 초기화 하는 함수 입니다.
		반드시 한번은 호출해야합니다.

		인자 :
		lpszDllPath : DLL의 경로입니다.

		반환값 :
		-1 : DLL을 찾을 수 없음
		-2 : LoadLibrary를 실패함. GetLastError를 통해서 추가 정보를 얻으세요
	*/
	INT Init(LPCWSTR lpszDllPath);

	/*
		DD를 풀어주는 함수
		굳이 호출 안해도 소멸자에서 호출함
	*/
	VOID DisPose();

	/*
	=============================================================================




	DD_로 시작하는 함수들은 리턴값이 -2일 경우엔 초기화가 안됐을 떄 입니더



	=============================================================================
	*/

	/*
		마우스 버튼을 누르는 함수

		인자 :
		#define DD_BTN_LBUTTONDOWN 1	//좌클릭 누름
		#define DD_BTN_LBUTTONUP 2		//좌클릭 땜
		#define DD_BTN_RBUTTONDOWN 4	//우클릭 누름
		#define DD_BTN_RBUTTONUP 8		//우클릭 땜
		#define DD_BTN_MBUTTONDOWN 16
		#define DD_BTN_MBUTTONUP 32
		#define DD_BTN_4BUTTONDOWN 64
		#define DD_BTN_4BUTTONUP 128
		#define DD_BTN_5BUTTONDOWN 256
		#define DD_BTN_5BUTTONUP 512
	*/
	INT DD_btn(INT nParam);

	INT DD_mov(INT x, INT y);

	/*
		마우스를 이동시키는 함수

		인자 :
		x : 화면상 절대 x 좌표
		y : 화면상 절대 y 좌표
	*/
	//INT DD_mov(INT x, INT y);

	/*
		마우스를 상대적으로 이동시키는 함수
		현재 마우스 커서 위치에서 인자 값 만큼 이동시킵니다.

		인자 :
		dx : dx만큼 해당 위치에서 움직입니다.
		dy : dy만큼 해당 위치에서 움직입니다.
	*/
	INT DD_movR(INT dx, INT dy);

	/*
		마우스 휠을 움직이는 함수 입니다.

		쓸일 없을거 같아서 테스트 안해봄
	*/
	INT DD_whl(INT nParam);

	/*
		키를 누르는 함수 입니다.

		인자 :
		nParam1 : DD코드입니다. 이 코드는 DD_todc함수로 구할 수 있습니다.
		nParam2 : 키보드의 동작 상태입니다. 1 (누름) , 2(땜)

		#define DD_KEY_DOWN 1
		#define DD_KEY_UP 2
	*/
	INT DD_key(INT nParam1, INT nParam2);

	/*
		문자열을 입력받는 함수입니다.
		한글은 사용할 수 없습니다.

		인자 :
		lpszSendString : 입력할 문자열
	*/
	INT DD_str(LPCSTR lpszSendString);

	/*
		가상 키코드를 DD코드로 변환하는 함수입니다.

		인자 :
		nVK_Code : 변환시킬 가상 키코드
	*/
	INT DD_todc(INT nVK_Code);
};