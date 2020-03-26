#define STB_IMAGE_IMPLEMENTATION
#include<glad/glad.h>
#include<stb/stb_image.h>
#include<GLFW/glfw3.h>
#include<myShader/Camera.h>
#include<myShader/Shader.h>
#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 760;

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);	//�����Ӧ�ص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);	//������Ӧ�ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); //�����ص�����

//����ʼλ��
float lastX = 640, lastY = 380;	//��һ֡������λ��

//��Ⱦʱ���
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float currentFrame = 0.0f;  //��ǰʱ��
float lastFrame = 0.0f; // ��һ֡��ʱ��

//�����������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct Light
{
	float round_radius;	//��Դ�����뾶
	float round_speed;	//��Դ�����ٶ�
	float ambient, diffuse, specular;		//�������������ֹ�Ӱ��ı���
	glm::vec3 color; //��Դ����Ĺ�
	glm::vec3 pos;	//��Դλ��
	Light(float a = 1.0f, float d = 1.0f, float s = 1.0f, float radius = 3.0f, float speed = 0.1f, glm::vec3 c = glm::vec3(1.0f),glm::vec3 p = glm::vec3(0.0f)) :
		ambient(a), diffuse(d), specular(s), round_radius(radius), round_speed(speed), color(c), pos(p) {}
}light;

struct Cube
{
	float shininess;		//����ȣ����ƾ��淴��Ĺ�Ȧ�뾶
	glm::vec3	ambient, diffuse, specular;		//�������䡢�����䡢���淴�����ɫ��ͨ��������������������ɫӦ��������ͬ�������ܹ�
	Cube(glm::vec3 a = glm::vec3(1.0f), glm::vec3 d = glm::vec3(1.0f), glm::vec3 s = glm::vec3(1.0f), float shininess = 32) :
		ambient(a), diffuse(d), specular(s), shininess(shininess) {}
};
Cube cube;

//���״̬
bool cursorActive = true;

//ImGUI����
bool show_demo_window = false;
bool show_custom_window = false;


int main()
{
	glfwInit();
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3.1", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastX, lastY);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	/*����ImGui������*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_DEPTH_TEST);

	/*ÿ���������λ�������뷨����*/
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader cubeShader("vershader.glsl", "fragshader.glsl");
	Shader lightShader("lightvershader.glsl", "lightFragshader.glsl");

	//������������
	Cube cube;
	cube.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	cube.diffuse =	glm::vec3(1.0f, 0.5f, 0.31f);
	cube.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	cube.shininess = 32;

	//���ù�������
	Light light;
	light.pos = glm::vec3(1.2f, 0.0f, 2.0f);
	light.ambient = 0.2f;
	light.diffuse = 0.5f;
	light.specular = 1.0f;
	

	while (!glfwWindowShouldClose(window))
	{	
		process_input(window);
		glfwPollEvents();
		//glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//������Ⱦʱ���
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//������ͼ������ͶӰ����
		glm::mat4 view(1.0f), projection(1.0f);
		view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		projection = glm::perspective(glm::radians(camera.Zoom), 1.0f * SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);


		//���ƹ�Դ����������
		glm::mat4 lightModel(1.0f);
		float lightX = sin(light.round_speed*glfwGetTime()) * light.round_radius;
		float lightZ = cos(light.round_speed*glfwGetTime()) * light.round_radius;
		light.pos = glm::vec3(lightX, 0.0f, lightZ);
		lightModel = glm::translate(lightModel, light.pos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));


		//���ù�Դ��ɫ����uniform����
		lightShader.use();
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		lightShader.setVec3("lightColor", light.color);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//����������ɫ����uniform����
		glm::mat4  cubeModel(1.0f); 
		cubeShader.use();
		cubeShader.setMat4("model", cubeModel);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cubeShader.setVec3("viewPos", camera.Position);
		
		cubeShader.setVec3("light.color", light.color);
		cubeShader.setVec3("light.pos", light.pos);
		cubeShader.setFloat("light.ambient", light.ambient);
		cubeShader.setFloat("light.diffuse", light.diffuse);
		cubeShader.setFloat("light.specular", light.specular);

		cubeShader.setVec3("material.ambient", cube.ambient);
		cubeShader.setVec3("material.diffuse", cube.diffuse);
		cubeShader.setVec3("material.specular", cube.specular);
		cubeShader.setFloat("material.shininess", cube.shininess);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		/*ImGUI����*/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
		{
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		if(show_custom_window)
		{
			ImGui::Begin("Learn OpenGL");                          // Create a window called "Hello, world!" and append into it.
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::DragFloat("Camera speed", &camera.MovementSpeed, 1.0f);	//������������ƶ��ٶ�
				ImGui::SliderFloat("FOV", &camera.Zoom, 5.0f, 160.0f);	 //������������ƶ��ٶ�
				ImGui::SliderFloat("Mouse sensitivity", &camera.MouseSensitivity, 0.01f, 0.5f);	//���������
				if (ImGui::Button("Reset"))
				{
					camera.MovementSpeed = 5.0f;
					camera.Zoom = 45.0f;
					camera.MouseSensitivity = 0.1f;
				}
			}
			
			if (ImGui::CollapsingHeader("Light"))
			{
				ImGui::SliderFloat("Surround radius", &light.round_radius, 1.0f, 10.0f);	 //��Դ���ư뾶
				ImGui::SliderFloat("Surround speed", &light.round_speed, 0.00001f, 10.0f);	 //��Դ�����ٶ�
				ImGui::SliderFloat("Light ambient", &light.ambient, 0.0f, 1.0f);
				ImGui::SliderFloat("Light diffuse", &light.diffuse, 0.0f, 1.0f);
				ImGui::SliderFloat("Light specular", &light.specular, 0.0f, 1.0f);
				ImGui::ColorEdit3("Light color", &light.color.x);	//������ɫ
				
			}

			if (ImGui::CollapsingHeader("Object"))
			{
				ImGui::ColorEdit3("Cube ambient", &cube.ambient.x);
				ImGui::ColorEdit3("Cube diffuse", &cube.diffuse.x);
				ImGui::ColorEdit3("Cube specular", &cube.specular.x);
				ImGui::SliderFloat("shininess", &cube.shininess, 1.0f, 256.0f);		//������ǿ��	
			}

			ImGui::Separator();
			ImGui::Text("Never knows best.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/*ImGUI���ƽ���*/

		glfwSwapBuffers(window);

		std::cout << glGetError() << std::endl;
		assert(glGetError() == GL_NO_ERROR);
	}

	//ImGui��Դ�ͷ�
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//OpenGL��Դ�ͷ�
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;
}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void process_input(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	void mouse_callback(GLFWwindow* window, double xPos, double yPos)
	{
		if (cursorActive == false)		return;
		float xoffset = xPos - lastX;
		float yoffset = lastY - yPos; //ע�⣬������ƣ�yPos��ֵ���С
		lastX = xPos;
		lastY = yPos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			if (cursorActive == true)
			{
				cursorActive = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				show_custom_window = true;
			}
			else
			{
				cursorActive = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(window, lastX, lastY);
				show_custom_window = false;
				show_demo_window = false;
			}
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

	}


	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (!cursorActive)	return;
		camera.ProcessMouseScroll(yoffset);
	}


