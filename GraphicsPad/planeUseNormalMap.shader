#shader vertex

#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 aTexCoord;

uniform mat4 model;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

out vec3 theColor;
//out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{

	theColor = vec3(1.0, 1.0, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	//Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = mvpMatrix * vec4(position, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#shader fragment
#version 430

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D tex;
//uniform vec3 objectColor;

out vec4 color;

in vec3 theColor;
//in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

void main()
{
	//daColor = vec4(theColor, 1.0);

	//lightColor = texture(tex, TexCoord).rgb;
	
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	vec3 norm = vec3(texture(tex, TexCoord).r-0.5f, texture(tex, TexCoord).g-0.5f, texture(tex, TexCoord).b-0.5f);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;//0.5
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * vec3(1.0f, 0.0f, 0.0f) * lightColor;


	vec3 result = (ambient + diffuse + specular);
	color = vec4(result, 1.0);
}