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
	
	theColor = vec3(1.0, 1.0, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = mvpMatrix * vec4(position, 1.0f);
}

#shader fragment
#version 430

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
//uniform vec3 objectColor;

out vec4 color;

in vec3 theColor;
in vec3 Normal;
in vec3 FragPos;

void main()
{
	//daColor = vec4(theColor, 1.0);
	
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;//0.5
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1032);
	vec3 specular = specularStrength * spec * vec3(1.0f, 0.0f, 0.0f);


	vec3 result = (ambient + diffuse + specular) * theColor;
	color = vec4(result, 1.0);
}