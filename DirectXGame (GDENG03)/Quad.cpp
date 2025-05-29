#include "Quad.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

Quad::Quad()
{   //default rainbow quad but smaller
	this->vertexes[0] = { -0.1f, -0.1f, 0.0f, -0.32f, -0.11f, 0.0f ,1,0,0,  1,0,1 };
	this->vertexes[1] = { -0.1f, 0.1f, 0.0f,  -0.11f, 0.78f, 0.0f, 0,1,0,   0.5,0.5,1 };
	this->vertexes[2] = { 0.1f, -0.1f, 0.0f,  0.75f, -0.73f, 0.0f, 0,0,0.5, 0.4,0.4,0.4 };
	this->vertexes[3] = { 0.1f, 0.1f, 0.0f,   0.88f, 0.77f, 0.0f, 1,1,0,    0,1,1 };

	this->size_vertexes = ARRAYSIZE(vertexes);
}

Quad::Quad(float width, float height, vec3 center)
{
	this->vertexes[0] = { center.x - width / 2, center.y - height / 2, center.z, -0.32f, -0.11f, 0.0f ,1,0,0,  1,0,1 };
	this->vertexes[1] = { center.x - width / 2, center.y + height / 2, center.z,  -0.11f, 0.78f, 0.0f, 0,1,0,   0.5,0.5,1 };
	this->vertexes[2] = { center.x + width / 2, center.y - height / 2, center.z,  0.75f, -0.73f, 0.0f, 0,0,0.5, 0.4,0.4,0.4 };
	this->vertexes[3] = { center.x + width / 2, center.y + height / 2, center.z,   0.88f, 0.77f, 0.0f, 1,1,0,    0,1,1 };

	this->size_vertexes = ARRAYSIZE(vertexes);
}

void Quad::Create(void** shader_byte_code, size_t* size_shader)
{
	this->m_vbuffer = GraphicsEngine::get()->createVertexBuffer();

	m_vbuffer->load(this->vertexes, sizeof(vertex), this->size_vertexes, *shader_byte_code, *size_shader);
}

void Quad::setColor(vec3 color)
{
	this->vertexes[0].color = color;
	this->vertexes[1].color = color;
	this->vertexes[2].color = color;
	this->vertexes[3].color = color;
}

void Quad::setColor(vec3 color1, vec3 color2, vec3 color3, vec3 color4)
{
	this->vertexes[0].color = color1;
	this->vertexes[1].color = color2;
	this->vertexes[2].color = color3;
	this->vertexes[3].color = color4;
}

void Quad::Draw()
{
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vbuffer);
	GraphicsEngine::get()->getDeviceContext()->drawTriangleStrip(this->m_vbuffer->getSizeVertexList(), 0);
}

void Quad::Release()
{
	this->m_vbuffer->release();
}