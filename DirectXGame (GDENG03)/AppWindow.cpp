#include "AppWindow.h"
#include "structs.h"
#include <Windows.h>
#include <iostream>
#include "Quad.h"

__declspec(align(16))
struct constant
{
	float m_angle;
};

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow* AppWindow::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new AppWindow();
		sharedInstance->init();
	}
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
	sharedInstance->init();

}

void AppWindow::destroy()
{
	if (sharedInstance != NULL)
		sharedInstance->release();
}

AppWindow::AppWindow()
{

}

void AppWindow::createGraphicsWindow()
{
	GraphicsEngine::initialize();

	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	std::cout << "Window rect width: " << width << std::endl;
	std::cout << "Window rect height: " << height << std::endl;

	this->m_swap_chain->init(this->m_hwnd, width, height);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//for drawing a quad
	/*vertex list[] =
	{//    X     Y     Z
		//Rainbow
     	{ -0.5f, -0.5f, 0.0f, -0.32f, -0.11f, 0.0f ,1,0,0,  1,0,1 },
		{ -0.5f, 0.5f, 0.0f,  -0.11f, 0.78f, 0.0f, 0,1,0,   0.5,0.5,1 },
		{ 0.5f, -0.5f, 0.0f,  0.75f, -0.73f, 0.0f, 0,0,0.5, 0.4,0.4,0.4 },
		{ 0.5f, 0.5f, 0.0f,   0.88f, 0.77f, 0.0f, 1,1,0,    0,1,1}
	};

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	this->m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();*/

	quads.push_back(Quad(0.5f, 0.5f, { 0,0,0 }));
	quads.push_back(Quad(0.3f, 0.5f, { 0.5,-0.5,0 }));
	quads.push_back(Quad(0.2f, 0.4f, { 0.2,0.7,0 }));

	quads[1].setColor({ 1.f,0.5,1.f });
	quads[2].setColor({ 1.f,0.f,1.f }, {0.5,0.5,1.f},{ 0.3f,0.3f,0.3f},{ 0.f,1.f,1.f });

	for (int i = 0; i < quads.size(); i++)
	{
		quads[i].Create(&shader_byte_code, &size_shader);
	}

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onCreate()
{

}

void AppWindow::onUpdate()
{
	Window::onUpdate();                                                               //  R  G  B  A
	GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 1, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();
	m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;

	this->m_cb->update(GraphicsEngine::get()->getDeviceContext(), &cc);

	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(this->m_ps);
	

	/*GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vb);
	
	//Rectangle:
	GraphicsEngine::get()->getDeviceContext()->drawTriangleStrip(this->m_vb->getSizeVertexList(), 0);*/
	
	for (int i = 0; i < quads.size(); i++)
	{
		quads[i].Draw();
	}


	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	if(this->m_vb != nullptr) //vertex buffers are part of the quad class instead
	this->m_vb->release();
	this->m_swap_chain->release();
	this->m_vs->release();
	this->m_ps->release();
	GraphicsEngine::get()->release();

	for (int i = 0; i < quads.size(); i++)
	{
		quads[i].Release();
	}
}

AppWindow::~AppWindow()
{

}