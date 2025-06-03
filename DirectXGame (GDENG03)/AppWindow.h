#pragma once
#include <vector>

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Quad.h"


class AppWindow : public Window
{
	public:
		static AppWindow* getInstance();
		static void initialize();
		static void destroy();

		virtual void createGraphicsWindow();
		virtual void onCreate() override;
		virtual void onUpdate() override;
		virtual void onDestroy() override;

	private:
		AppWindow();
		
		void updateQuadPosition();

		~AppWindow();
		AppWindow(const AppWindow&) {};
		AppWindow& operator=(const AppWindow&) {};
		static AppWindow* sharedInstance;

		SwapChain* m_swap_chain;
		VertexBuffer* m_vb;
		IndexBuffer* m_ib;
		ConstantBuffer* m_cb;
		VertexShader* m_vs;
		PixelShader* m_ps;

		float m_old_delta;
		float m_new_delta;
		float m_delta_pos = 0.0f;
		float m_delta_scale = 0.0f;

		unsigned long m_old_time = 0;
		float m_delta_time = 0;
		float m_angle = 0;

		std::vector<Quad> quads;
		ConstantBuffer* m_cb_light;
};

