#include "AppWindow.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
};

AppWindow::AppWindow()
{

}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	this->m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//for drawing a triangle
	vertex list[] = 
	{//    X     Y     Z
		{ 0.0f, 0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f }, 
	};

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	this->m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);


}

void AppWindow::onUpdate()
{
	Window::onUpdate();                                                               //  R  G  B  A
	GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 1, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->setShaders();

	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vb);
	GraphicsEngine::get()->getDeviceContext()->drawTriangleList(this->m_vb->getSizeVertexList(), 0);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_vb->release();
	this->m_swap_chain->release();
	GraphicsEngine::get()->release();
}

AppWindow::~AppWindow()
{

}