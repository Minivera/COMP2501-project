// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t;
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
	float gravity = 0.2;
	float acttime = mod(time * 0.5 - t, distance);

    vec4 ppos = vec4(vertex.x + dir.x, vertex.y + dir.y + acttime, 0.0, 1.0);
	//vec4 ppos = vec4(vertex, 0.0, 1.0);

    gl_Position = viewMatrix * transformationMatrix * ppos;
	
    color_interp = vec4(uv, 0.5, 1.0);
	
	float uvTime = mod(time * 6 + time * dir.y, count);
	uv_interp = uv;
	uv_interp.x /= count;
	uv_interp.x += floor(uvTime)/count;
}