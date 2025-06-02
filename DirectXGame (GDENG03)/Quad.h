#pragma once
#include <Windows.h>
#include "structs.h"

class VertexBuffer;

class Quad
{
	public:
		Quad();
		Quad(float width, float height, Vector3D center);
		

		void Create(void** shader_byte_code, size_t* size_shader);
		void setColor(Vector3D color);
		void setColor(Vector3D color1, Vector3D color2, Vector3D color3, Vector3D color4);
		void Draw();
		void Release();


	private:
		VertexBuffer* m_vbuffer;
		vertex vertexes[4];
		UINT size_vertexes;



};

