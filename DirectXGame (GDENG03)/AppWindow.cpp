#include "AppWindow.h"
#include "structs.h"
#include <Windows.h>
#include <iostream>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Quad.h"
#include "DirectionalLight.h"

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	DirectX::XMMATRIX m_proj_inv;
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

void AppWindow::updateQuadPosition()
{
	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();
	m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;


	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f) m_delta_pos = 0.0f;
	

	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 10.f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_delta_scale) + 1.0f)/2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5, -1.5, 0), Vector3D(1.5, 1.5, 0), m_delta_pos));

	//cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(1, 1, 1));



			temp.setRotationZ(m_delta_scale);
			cc.m_world *= temp;
			temp.setRotationY(m_delta_scale);
			cc.m_world *= temp;
			temp.setRotationX(m_delta_scale);
			cc.m_world *= temp;


			cc.m_view.setIdentity();

			RECT rc = this->getClientWindowRect();
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;

			cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);



			cc.m_proj_inv = cc.m_proj.getInverse();

			this->m_cb->update(GraphicsEngine::get()->getDeviceContext(), &cc);

			DirectionalLight d_light;
			d_light.direction = { 0, 100, 0, 1.0f };
			this->m_cb_light->update(GraphicsEngine::get()->getDeviceContext(), &d_light);
		


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
	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
     	{ Vector3D(-0.5f, -0.5f, -0.5f) ,Vector3D(-0.577f, -0.577f, -0.577f) , Vector3D(0.1f),  Vector3D(0.4)},
		{ Vector3D(-0.5f, 0.5f, -0.5f) , Vector3D(-0.577f,  0.577f, -0.577f) ,  Vector3D(0.2f),   Vector3D(0.4) },
		{ Vector3D(0.5f, 0.5f, -0.5f) , Vector3D(0.577f,  0.577f, -0.577f), Vector3D(0.4) },
		{ Vector3D(0.5f, -0.5f, -0.5f),  Vector3D(0.577f, -0.577f, -0.577f) ,Vector3D(0.4f),    Vector3D(0.4)},

		{ Vector3D(0.5f, -0.5f, 0.5f) , Vector3D(0.577f, -0.577f,  0.577f),  Vector3D(0.4) },
		{ Vector3D(0.5f, 0.5f, 0.5f) ,  Vector3D(0.577f,  0.577f,  0.577f) , Vector3D(0.6f),  Vector3D(0.4) },
		{ Vector3D(-0.5f, 0.5f, 0.5f) , Vector3D(-0.577f,  0.577f,  0.577f) ,Vector3D(0.7f), Vector3D(0.4) },
		{ Vector3D(-0.5f, -0.5f, 0.5f),  Vector3D(-0.577f, -0.577f,  0.577f) ,Vector3D(0.8f),    Vector3D(0.4)}
	};

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
	};

	this->m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	this->m_ib->load(index_list, size_index_list);

	this->m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	DirectionalLight d_light;
	d_light.direction = { 0, 100, 0, 1.0f };
	this->m_cb_light = GraphicsEngine::get()->createConstantBuffer();
	this->m_cb_light->load(&d_light, sizeof(DirectionalLight));

}

void AppWindow::onCreate()
{

}

void AppWindow::onUpdate()
{
	Window::onUpdate();                                                               //  R  G  B  A
	GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(135.f/255.f), (float)(206.f /255.f), (float)(255.f /255.f), 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	this->updateQuadPosition();


	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getDeviceContext()->setLightBuffer(this->m_cb_light);
	GraphicsEngine::get()->getDeviceContext()->setVertexShader(this->m_vs);

	GraphicsEngine::get()->getDeviceContext()->setPixelShader(this->m_ps);
	

	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vb);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_ib);
	
	//Rectangle:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_ib->getSizeIndexList(), 0, 0);


	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta)?((m_new_delta - m_old_delta) / 1000.0f):0; // Calculate delta time
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	if(this->m_vb != nullptr) //vertex buffers are part of the quad class instead
	this->m_vb->release();
	this->m_ib->release();
	this->m_cb->release();
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