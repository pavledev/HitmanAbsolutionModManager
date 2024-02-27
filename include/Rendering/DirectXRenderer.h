#pragma once

#include <d3d11.h>

#include "SwapChain.h"
#include "Logger.h"

class DirectXRenderer
{
public:
	DirectXRenderer();
	bool Setup(HWND hwnd, WNDCLASSEXW* wc);
	bool CreateD3DDevice(HWND hWnd);
	void CleanupD3DDevice();
	const HWND GetHWND() const;
	const ID3D11Device* GetD3D11Device() const;
	ID3D11Device* GetD3D11Device();
	const ID3D11DeviceContext* GetD3D11DeviceContext() const;
	ID3D11DeviceContext* GetD3D11DeviceContext();
	SwapChain* GetSwapChain();
	void SetBackBufferAsRenderTarget();
	void ClearBackBuffer();

private:
	HWND hwnd;
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
	SwapChain* swapChain;
};
