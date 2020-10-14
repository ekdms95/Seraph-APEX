struct ID3D11Device;
struct ID3D11DeviceContext;

Var_ChaoticUI_API bool        Var_ChaoticUI_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
Var_ChaoticUI_API void        Var_ChaoticUI_ImplDX11_Shutdown();
Var_ChaoticUI_API void        Var_ChaoticUI_ImplDX11_NewFrame();
Var_ChaoticUI_API void        Var_ChaoticUI_ImplDX11_RenderDrawData(Var_ChaoticUIDrawData* draw_data);

// Use if you want to reset your rendering device without losing Var_ChaoticUI state.
Var_ChaoticUI_API void        Var_ChaoticUI_ImplDX11_InvalidateDeviceObjects();
Var_ChaoticUI_API bool        Var_ChaoticUI_ImplDX11_CreateDeviceObjects();
