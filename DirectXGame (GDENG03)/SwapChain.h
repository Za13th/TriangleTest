#pragma once

#include <d3d11.h>

class DeviceContext;
class SwapChain
{
	public:
		SwapChain();
		//Initialize stuff
		bool init(HWND hwnd, UINT width, UINT height);

		bool present(bool vsync);

		//Releases used resources
		bool release();

		~SwapChain();

	private:	
		IDXGISwapChain* m_swap_chain;
		ID3D11RenderTargetView* m_rtv;

		friend class DeviceContext;

};

