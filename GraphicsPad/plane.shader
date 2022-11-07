#shader vertex

#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;

uniform mat4 model;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

out vec3 theColor;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	
	gl_Position = mvpMatrix * vec4(position, 1.0f);
	theColor = vertexColor;
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(mvMatrix))) * normal;
}

#shader fragment
#version 430

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;

out vec4 color;

in vec3 theColor;
in vec3 Normal;
in vec3 FragPos;

void main()
{
	//daColor = vec4(theColor, 1.0);
	
	// ambient
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * vec3(0.5,0.2,0.8);
	color = vec4(result, 1.0);
}