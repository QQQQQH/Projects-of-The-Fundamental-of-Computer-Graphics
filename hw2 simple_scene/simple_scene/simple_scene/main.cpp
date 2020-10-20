#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "Shader.h"
#include "Object.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0, 0, 8));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

GLFWwindow* init_GLFW();
bool load_texture(unsigned int num, unsigned int* texture, const char* path[]);

int main() {
	// initialization
	GLFWwindow* window = init_GLFW();
	if (!window) {
		glfwTerminate();
		return -1;
	}
	Shader ourShader("shader.vs", "shader.fs");

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	const int radius = 3;
	Cube scene(glm::vec3(0, 0, 0), glm::vec3(20, 10, 20)),
		cubeLeft(glm::vec3(-radius, 0, 0)),
		cubeRight(glm::vec3(radius, 0, 0)),
		cubeMoving(glm::vec3(0, radius, 0));
	scene.set_vertices_data();
	cubeLeft.find_minmax();
	cubeRight.find_minmax();

	// load and create texture 
	const int textureNum = 3;
	unsigned int texture[textureNum];
	const char* path[textureNum] = {
	   "texture/wall.jpg",
	   "texture/container.jpg",
	   "texture/awesomeface.jpg"
	};
	if (!load_texture(textureNum, texture, path)) {
		glfwTerminate();
		return -1;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	ourShader.use();

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// render scene
		ourShader.setInt("texture0", 0);
		glm::mat4 model = scene.get_model();
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// render cube
		ourShader.setInt("texture0", 1);

		model = cubeLeft.get_model();
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = cubeRight.get_model();
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// render moving cube
		float roatateTime = currentFrame * 2;
		float x = radius * cos(roatateTime),
			y = radius * sin(roatateTime);

		cubeMoving.set_pos(glm::vec3(x, y, 0));
		cubeMoving.set_rotate(roatateTime);
		model = cubeMoving.get_model();
		cubeMoving.find_minmax();
		if (cubeMoving.check_hit(cubeLeft) || cubeMoving.check_hit(cubeRight)) {
			ourShader.setInt("texture0", 2);
		}
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
	glEnable(GL_DEPTH_TEST);
	return window;
}

bool load_texture(unsigned int num, unsigned int* texture, const char* path[]) {
	glGenTextures(num, texture);
	for (int i = 0; i < num; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path[i], &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cerr << "Failed to load texture!" << std::endl;
			return false;
		}
		stbi_image_free(data);
	}
	return true;
}
