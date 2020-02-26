// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

out vec4 FragColor;

uniform sampler2D onetex;

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
    if(color.a < 0.1)
	{
 		discard;
	}
	
	FragColor = color;
}