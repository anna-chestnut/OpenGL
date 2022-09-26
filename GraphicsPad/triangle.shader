#shader vertex

#version 430

in layout(location = 0) vec2 position;
in layout(location = 1) vec3 i_color;

out vec3 o_color;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	o_color = i_color;
};

#shader fragment
#version 430

in vec3 o_color;

out vec4 daColor;

void main()
{
	daColor = vec4(o_color, 1.0);
};