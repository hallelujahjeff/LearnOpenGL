#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aNormal;	//���붥�㷨����

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0f);
	FragPos = vec3(model * vec4(aPos,1.0f));
	/*Ϊ�˷�ֹ���ȱ����ŶԷ�������ɵ�Ӱ�죬�����뷨�߾���*/
	/*���߾���Ϊģ�;�������ת�ã�Ϊ����λ��Ӱ�죬ֻȡ���Ϸ�3*3*/
	/*Ϊ�˸�Ч���������ʵ��Ӧ����CPU����ɣ�ͨ��uniform������ɫ��*/
	Normal = mat3(transpose(inverse(model))) * aNormal;
	//Normal = mat3(norm) * aNormal;
}