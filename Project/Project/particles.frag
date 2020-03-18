// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

out vec4 FragColor;

uniform sampler2D onetex;
uniform vec4 objectColor;

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
    if(color.a < 0.1)
	{
 		discard;
	}
	
	if(objectColor.a > 0) {
		float factor = objectColor.a;
		float inverseFactor = 1 - objectColor.a;
		FragColor = vec4(
			color.r * inverseFactor + objectColor.r * factor,
			color.g * inverseFactor + objectColor.g * factor,
			color.b * inverseFactor + objectColor.b * factor,
			color.a
		);
	} else {
		FragColor = color;
	}
}