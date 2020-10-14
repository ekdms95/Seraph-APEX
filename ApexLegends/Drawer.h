#pragma once
#include "main.h"

class Renderer
{
public:
	void Initialize();
	void BeginScene();
	void DrawScene();
	void EndScene();

	static Renderer* GetInstance();

	Var_ChaoticUIFont* m_pFont;

	Renderer();
	~Renderer();

	float DrawMyText(Var_ChaoticUIFont* pFont, PCHAR text, const DirectX::XMFLOAT2& pos, float size, DirectX::XMFLOAT4 color, bool center);
	void DrawLine(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& to, DirectX::XMFLOAT4 color, float thickness);
	void DrawBox(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& size, DirectX::XMFLOAT4 color, float rounding, float thickness);
	void DrawBoxFilled(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& size, DirectX::XMFLOAT4 color, float rounding);
	void DrawCircle(const DirectX::XMFLOAT2& from, float radius, DirectX::XMFLOAT4 color, float thickness = 1.f);
	void DrawCircleFilled(const DirectX::XMFLOAT2& from, float radius, DirectX::XMFLOAT4 color);
private:
	static Renderer* m_pInstance;
};