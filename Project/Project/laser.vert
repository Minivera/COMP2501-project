// Source code of vertex shader for particle system
#version 130

// Vertex buffer
in vec2 vertex;

// Uniform (global) buffer
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform float size;
uniform float lifespanLeft;
uniform float lifespan;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

void main()
{
	float steps = size / 6;
	float lifeStep = lifespan / 6;
	float sizeAccumulation = steps * floor(lifespanLeft / lifeStep);

    vec4 ppos = vec4(vertex.x, vertex.y * sizeAccumulation, 0.0, 1.0);
    gl_Position = viewMatrix * transformationMatrix * ppos;
}