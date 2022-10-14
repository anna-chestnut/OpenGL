#shader vertex

#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 theColor;

void main()
{
	/*vec4 v = vec4(position, 1.0);
	vec4 newPosition = modelTransformMatrix * v;
	vec4 projectedPosition = projectionMatrix * newPosition;
	gl_Position = projectedPosition;*/
	gl_Position = projection * view * model * vec4(position, 1.0f);
	theColor = vertexColor;
}

#shader fragment
#version 430

out vec4 daColor;
in vec3 theColor;

void main()
{
	daColor = vec4(theColor, 1.0);
}