#pragma once
#include <Windows.h>
#include "structs.h"

class VertexBuffer;
class Quad
{
	public:
		Quad();
		Quad(float width, float height, vec3 center);
		

		void Create(void** shader_byte_code, size_t* size_shader);
		void setColor(vec3 color);
		void setColor(vec3 color1, vec3 color2, vec3 color3, vec3 color4);
		void Draw();
		void Release();


	private:
		VertexBuffer* m_vbuffer;
		vertex vertexes[4];
		UINT size_vertexes;



};

