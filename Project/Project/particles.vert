// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in float displacement;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform float time;
uniform float count;
uniform float distance;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

void main()
{
	float gravity = 0.02;
	float acttime = mod(time, count);

    vec4 ppos = vec4(vertex.x + displacement, vertex.y + distance * gravity * acttime, 0.0, 1.0);

    gl_Position = viewMatrix * transformationMatrix * ppos;
	
    color_interp = vec4(uv, 0.5, 1.0);

	uv_interp = uv;
	uv_interp.x /= count;
	uv_interp.x += floor(acttime)/count;
}