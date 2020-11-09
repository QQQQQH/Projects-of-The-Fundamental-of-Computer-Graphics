#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "Scene.h"
#include "Shader.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int NUM_OBJECT;

//Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
Camera camera(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = nullptr;

Object* objects[3];
Scene scene;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

GLFWwindow* init_GLFW();
bool prepare(int f);
void clear();
int run1(int f);
int run2(int f);

int main() {
	while (true) {
		cout << "Please Select:\n"
			<< "1. Phong\n"
			<< "2. Ray Tracing\n"
			<< "3. Ray Tracing+\n" << endl;
		string strIn;
		cin >> strIn;
		if (strIn == "11") {
			return run1(1);
		}
		if (strIn == "12") {
			return run1(2);
		}
		if (strIn == "21") {
			return run2(1);
		}
		if (strIn == "22") {
			return run2(2);
		}
		cout << endl;
	}
	clear();
	return 0;
}

bool prepare(int f) {
	// initialization
	window = init_GLFW();
	if (!window) {
		glfwTerminate();
		return false;
	}
	if (f == 1) {
		NUM_OBJECT = 1;
		objects[0] = new Cube;
		scene.add_object(objects[0]);
	}
	else {
		NUM_OBJECT = 3;
		objects[0] = new Model("model/dragon_vrip_res4.ply");
		objects[1] = new Model("model/bun_zipper_res4.ply");
		objects[2] = new Model("model/happy_vrip_res4.ply");
		for (int i = 0; i < 3; ++i) {
			scene.add_object(objects[i]);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-2 + i * 2, 0, 0));
			model = glm::scale(model, glm::vec3(10.0f));
			objects[i]->set_model(model);
		}
	}

	return true;
}

void clear() {
	for (int i = 0; i < NUM_OBJECT; ++i) {
		delete objects[i];
	}
}


int run1(int f) {
	if (!prepare(f)) {
		return -1;
	}

	Cube lightCube;
	Shader objectShader("shader/object.vs", "shader/object.fs");
	Shader lightCubeShader("shader/light_cube.vs", "shader/light_cube.fs");



	objectShader.use();

	objectShader.setVec3("light.position", scene.lightPos);
	objectShader.setVec3("light.ambient", scene.ambientColor);
	objectShader.setVec3("light.diffuse", scene.diffuseColor);
	objectShader.setVec3("light.specular", scene.specularStrength);

	glm::mat4 projection, view, model;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		objectShader.use();
		objectShader.setVec3("viewPos", camera.Position);
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);

		for (int i = 0; i < NUM_OBJECT; ++i) {
			objectShader.setVec3("material.ambient", scene.objects[i]->ambient);
			objectShader.setVec3("material.diffuse", scene.objects[i]->diffuse);
			objectShader.setVec3("material.specular", scene.objects[i]->specular);
			objectShader.setFloat("material.shininess", scene.objects[i]->shininess);
			objectShader.setMat4("model", scene.objects[i]->model);
			scene.objects[i]->Draw(objectShader);
		}

		// draw the lamp object
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setVec3("color", scene.lightColor);
		model = glm::mat4(1.0f);
		model = glm::translate(model, scene.lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		lightCube.Draw(lightCubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

int run2(int f) {
	if (!prepare(f)) {
		return -1;
	}

	scene.prepare_for_ray_tracing();



	float point[] = { 0,0,0 };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader shader("shader/ray_tracing.vs", "shader/ray_tracing.fs");
	shader.use();
	glBindVertexArray(VAO);

	int cnt = 0;
	// 枚举屏幕上每一个像素
	for (unsigned int i = 0; i < SCR_WIDTH; ++i) {
		for (unsigned int j = 0; j < SCR_HEIGHT; ++j) {
			// 将像素坐标分量映射到[0, 1]
			glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2 / SCR_HEIGHT - 1.0f, 0.0f);
			shader.setVec2("screenPos", glm::vec2(pos.x, pos.y));

			// 计算像素在世界坐标中的位置
			glm::vec3 globalPos = camera.Position + camera.Front + pos.x * camera.Right * (float(SCR_WIDTH) / SCR_HEIGHT) + pos.y * camera.Up;

			// 计算出光线并进行光线追踪
			Ray ray(camera.Position, globalPos);
			glm::vec3 color = scene.trace_ray(ray);

			// 绘制该处的像素
			shader.setVec3("vertexColor", color);
			if (color != glm::vec3(0, 0, 0)) {
				//cout << color.x << ", " << color.y << ", " << color.z << endl;
				++cnt;
			}
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
	cout << cnt << endl;

	glfwSwapBuffers(window);
	system("pause");
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* init_GLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple Scene", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	return window;
}

