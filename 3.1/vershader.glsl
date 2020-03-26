#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aNormal;	//传入顶点法向量

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0f);
	FragPos = vec3(model * vec4(aPos,1.0f));
	/*为了防止不等比缩放对法向量造成的影响，需引入法线矩阵*/
	/*法线矩阵为模型矩阵的逆的转置，为消除位移影响，只取左上方3*3*/
	/*为了高效，这个操作实际应当在CPU上完成，通过uniform传给着色器*/
	Normal = mat3(transpose(inverse(model))) * aNormal;
	//Normal = mat3(norm) * aNormal;
}