#shader vertex

#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;

uniform mat4 mvpMatrix;

out vec3 theColor;

void main()
{
	theColor = vertexColor;
	gl_Position = mvpMatrix * vec4(position, 1.0f);
}

#shader fragment
#version 430


out vec4 color;

in vec3 theColor;

void main()
{
	color = vec4(theColor, 1.0);
}