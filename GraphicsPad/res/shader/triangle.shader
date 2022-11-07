#shader vertex

#version 430

in layout(location=0) vec2 position;
in layout(location = 1) vec3 color;
in layout(location = 2) vec3 normal;

//out vec3 o_color;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	//o_color = color;
};

#shader fragment
#version 430

out vec4 daColor;

void main()
{
	daColor = vec4(0.1, 0.1, 0.0 , 1.0);
};