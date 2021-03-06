#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <ctime>
#include <omp.h>

#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "Scene.h"
#include "Shader.h"


const unsigned int MAX_WIDTH = 1920, MAX_HEIGHT = 1080;
unsigned int SCR_WIDTH, SCR_HEIGHT;
glm::vec3 screenColor[MAX_WIDTH][MAX_HEIGHT];

Camera camera(glm::vec3(0.0f, 5.0f, 5.0f));
//Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, -90.f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Material pureColorMaterial, metalMaterial, smoothMaterial, fullReflectMaterial, fullRefractMaterial;

GLFWwindow* window = nullptr;

Scene scene;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

GLFWwindow* init_GLFW();
void prepare(int f);
void clear();
int run1(int f);
int run2(int f, bool speedUp = false);

int main() {
	camera.Zoom = 90;
	while (true) {
		system("cls");
		cout << "Please Select:\n"
			<< "1. 800 x 600\n"
			<< "2. 1920 x 1080\n" << endl;
		string in;
		cin >> in;
		cout << endl;
		if (in == "1") {
			SCR_WIDTH = 800;
			SCR_HEIGHT = 600;
			break;
		}
		else if (in == "2") {
			SCR_WIDTH = 1920;
			SCR_HEIGHT = 1080;
			break;
		}
	}
	while (true) {
		system("cls");
		cout << "Please Select:\n"
			<< "1. Phong\n"
			<< "2. Ray Tracing\n"
			<< "3. Ray Tracing+\n" << endl;
		string in;
		cin >> in;
		cout << endl;
		if (in == "1") {
			system("cls");
			while (true) {
				cout << "Please Select:\n"
					<< "1. Only 1 cube\n"
					<< "2. Models (Low Res)\n"
					<< "3. Models\n"
					<< "4. Models (Hight Res)\n" << endl;
				cin >> in;
				cout << endl;
				if (in == "1") {
					return run1(1);
				}
				else if (in == "2") {
					return run1(2);
				}
				else if (in == "3") {
					return run1(3);
				}
				else if (in == "4") {
					return run1(4);
				}
			}
		}
		else if (in == "2" || in == "3") {
			system("cls");
			bool speedUp = in == "3";
			while (true) {
				cout << "Please Select:\n"
					<< "1. Only 1 cube\n"
					<< "2. Models (Low Res)\n"
					<< "3. Models\n"
					<< "4. Models (Hight Res) (Time Cost)\n"
					<< "5. Balls\n" << endl;
				cin >> in;
				cout << endl;
				if (in == "1") {
					return run2(1, speedUp);
				}
				else if (in == "2") {
					return run2(2, speedUp);
				}
				else if (in == "3") {
					return run2(3, speedUp);
				}
				else if (in == "4") {
					return run2(4, speedUp);
				}
				else if (in == "5") {
					return run2(5, speedUp);
				}
			}
		}
	}
	return 0;
}

void prepare(int f) {
	// set materials
	pureColorMaterial.set_pure_color();
	metalMaterial.set_metal();
	smoothMaterial.set_smooth();
	fullReflectMaterial.set_full_reflect();
	fullRefractMaterial.set_full_refract();

	// add floor
	glm::mat4 model(1.0f);

	Plane* floor = new Plane(smoothMaterial);
	model = glm::scale(model, glm::vec3(200.0f));
	floor->set_model(model);
	scene.add_object(floor);

	// only 1 cube
	if (f == 1) {
		camera.set_position(glm::vec3(3.0f, 3.0f, 3.0f));
		camera.set_yaw(-135.0f);
		camera.set_pitch(-30.0f);

		scene.set_light_pos(glm::vec3(2.0f, 1.5f, 0.0f));

		Object* cube = new Cube(pureColorMaterial);
		cube->material.set_color(glm::vec3(1.0f, 0.5f, 0.31f));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		cube->set_model(model);
		scene.add_object(cube);
	}

	// models
	else if (f >= 2 && f <= 4) {
		camera.set_position(glm::vec3(6.0f, 4.0f, 6.0f));
		camera.set_yaw(-137.0f);
		camera.set_pitch(-30.0f);

		scene.set_light_pos(glm::vec3(4.0f, 10.0f, 4.0f));

		vector <Object*> objects;

		cout << "Loading Models..." << endl;

		Object* bun, * happy, * dragon;
		if (f == 2) {
			bun = new Model("model/bun_zipper_res4.ply", metalMaterial);
			happy = new Model("model/happy_vrip_res4.ply", metalMaterial);
			dragon = new Model("model/dragon_vrip_res4.ply", metalMaterial);
		}
		else if (f == 3) {
			bun = new Model("model/bun_zipper.ply", metalMaterial);
			happy = new Model("model/happy_vrip_res3.ply", metalMaterial);
			dragon = new Model("model/dragon_vrip_res3.ply", metalMaterial);
		}
		else if (f == 4) {
			bun = new Model("model/bun_zipper.ply", metalMaterial);
			happy = new Model("model/happy_vrip.ply", metalMaterial);
			dragon = new Model("model/dragon_vrip.ply", metalMaterial);
		}

		objects.push_back(bun);
		objects.push_back(happy);
		objects.push_back(dragon);

		glm::vec3 pos[] = {
			glm::vec3(1,0,6),
			glm::vec3(6,0,1),
			glm::vec3(2,2,2),
		};

		float rgb[][3] = {
			255,215,187,
			148,251,240,
			255,127,79,
			//230,228,192,
		};

		for (int i = 0, sz = objects.size(); i < sz; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos[i]);
			model = glm::scale(model, glm::vec3(15.0f));
			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
			objects[i]->set_model(model);
			objects[i]->material.set_color(glm::vec3(
				rgb[i][0] / 255,
				rgb[i][1] / 255,
				rgb[i][2] / 255));
			scene.add_object(objects[i]);
		}
		happy->material.set_jade();
		dragon->material.set_whit_plastic();

		Object* ball1 = new Sphere(fullRefractMaterial, glm::vec3(4, 1, 0), 1);
		Object* ball2 = new Sphere(fullReflectMaterial, glm::vec3(0, 1, 4), 1);
		scene.add_object(ball1);
		scene.add_object(ball2);


		int numOfTriangles = 0;
		for (auto object : scene.objects) {
			numOfTriangles += object->numOfTriangles;
		}
		cout << "Number of Triangles: " << numOfTriangles << endl;
	}

	// balls
	else {
		camera.set_position(glm::vec3(0.0f, 10.0f, 0.0f));
		camera.set_pitch(-90.0f);

		scene.set_light_pos(glm::vec3(0.0f, 5.0f, 0.0f));

		floor->set_color(glm::vec3(0.87f, 0.64f, 0.70f), glm::vec3(0.69f, 0.29f, 0.40f));
		floor->material.set_pure_color();

		//balls high
		const int NUM_BALL_HIGH = 6;
		Object* ballsHigh[NUM_BALL_HIGH];
		glm::vec3 ballHighPos[] = {
			glm::vec3(3.0f, 7.0f, 1.2f),
			glm::vec3(3.0f, 7.0f, -1.2f),
			glm::vec3(0.0f, 7.0f, 1.2f),
			glm::vec3(0.0f, 7.0f, -1.2f),
			glm::vec3(-3.0f, 7.0f, 1.2f),
			glm::vec3(-3.0f, 7.0f, -1.2f),
		};
		float ballHighrRefractiveIndex[] = {
			1.52f,
			1.73f,
			1.73f,
			1.52f,
			1.52f,
			1.73f,
		};
		for (int i = 0; i < NUM_BALL_HIGH; ++i) {
			ballsHigh[i] = new Sphere(fullRefractMaterial, ballHighPos[i]);
			ballsHigh[i]->material.refractiveIndex = ballHighrRefractiveIndex[i];
			scene.add_object(ballsHigh[i]);
		}

		// balls low
		const int NUM_BALL_LOW = 20;
		Object* ballsLow[NUM_BALL_LOW];
		glm::vec3 ballLowPos[] = {
			glm::vec3(-12.0f, 0.6f, -5.0f),
			glm::vec3(12.0f, 0.6f, 5.0f),
			glm::vec3(12.0f, 0.6f, -5.0f),
			glm::vec3(-12.0f, 0.6f, 5.0f),
			glm::vec3(0.0f, 0.4f, 0.0f),

			glm::vec3(0.0f, 1.0f, 3.0f),
			glm::vec3(6.0f, 1.0f, 0.0f),
			glm::vec3(3.0f, 0.8f, 4.0f),

			glm::vec3(-9.0f, 0.6f, 3.0f),
			glm::vec3(-6.0f, 0.7f, -5.0f),
			glm::vec3(9.0f, 0.8f, -4.0f),

			glm::vec3(0.0f, 0.4f, -3.0f),
			glm::vec3(-6.0f, 0.6f, 0.0f),
			glm::vec3(-3.0f, 0.9f, -4.0f),

			glm::vec3(0.0f, 0.5f, -3.0f),
			glm::vec3(-7.0f, 0.7f, 2.0f),
			glm::vec3(3.0f, 1.2f, -4.0f),

			glm::vec3(-2.0f, 0.6f, 6.0f),
			glm::vec3(7.0f, 0.7f, 2.0f),
			glm::vec3(-3.0f, 0.8f, 4.0f),
		};

		float rgb[NUM_BALL_LOW][3] = {
			35,  205, 182,
			142, 229, 213,
			148, 251, 240,
			192, 247, 252,
			172, 248, 211,
			230, 228, 192,
			231, 202, 127,
			255, 215, 187,
			255, 179, 168,
			255, 76,  108,
			35,  205, 182,
			142, 229, 213,
			148, 251, 240,
			192, 247, 252,
			172, 248, 211,
			230, 228, 192,
			231, 202, 127,
			255, 215, 187,
			255, 179, 168,
			255, 76,  108,
		};
		for (int i = 0; i < NUM_BALL_LOW; ++i) {
			ballsLow[i] = new Sphere(metalMaterial, ballLowPos[i], ballLowPos[i].y);
			ballsLow[i]->material.set_color(glm::vec3(
				rgb[i][0] / 255,
				rgb[i][1] / 255,
				rgb[i][2] / 255
			));
			scene.add_object(ballsLow[i]);
		}
	}
	scene.prepare_for_ray_tracing();
}

void clear() {
	for (auto object : scene.objects) {
		delete object;
	}
	scene.objects.clear();
}


int run1(int f) {
	prepare(f);

	// initialize window
	window = init_GLFW();
	if (!window) {
		glfwTerminate();
		return -1;
	}
	scene.setup_gl();

	Cube lightCube(pureColorMaterial);
	Shader objectShader("shader/object.vs", "shader/object.fs");
	Shader lightCubeShader("shader/light_cube.vs", "shader/light_cube.fs");
	Shader floorShader("shader/floor.vs", "shader/floor.fs");

	objectShader.use();

	objectShader.setVec3("light.position", scene.lightPos);
	objectShader.setVec3("light.ambient", scene.ambientColor);
	objectShader.setVec3("light.diffuse", scene.diffuseColor);
	objectShader.setVec3("light.specular", scene.specularStrength);
	objectShader.setFloat("light.constant", scene.constant);
	objectShader.setFloat("light.linear", scene.linear);
	objectShader.setFloat("light.quadratic", scene.quadratic);

	floorShader.use();

	floorShader.setVec3("light.position", scene.lightPos);
	floorShader.setVec3("light.ambient", scene.ambientColor);
	floorShader.setVec3("light.diffuse", scene.diffuseColor);
	floorShader.setVec3("light.specular", scene.specularStrength);
	floorShader.setFloat("light.constant", scene.constant);
	floorShader.setFloat("light.linear", scene.linear);
	floorShader.setFloat("light.quadratic", scene.quadratic);


	glm::mat4 projection, view, model;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		if (scene.objects.size()) {

			floorShader.use();
			floorShader.setVec3("viewPos", camera.Position);
			floorShader.setMat4("projection", projection);
			floorShader.setMat4("view", view);
			floorShader.setMat4("model", scene.objects[0]->model);
			scene.objects[0]->Draw(floorShader);


			objectShader.use();
			objectShader.setVec3("viewPos", camera.Position);
			objectShader.setMat4("projection", projection);
			objectShader.setMat4("view", view);

			for (int i = 1, sz = scene.objects.size(); i < sz; ++i) {
				objectShader.setVec3("material.ambient", scene.objects[i]->ambient());
				objectShader.setVec3("material.diffuse", scene.objects[i]->diffuse());
				objectShader.setVec3("material.specular", scene.objects[i]->specular());
				objectShader.setFloat("material.shininess", scene.objects[i]->shininess());
				objectShader.setMat4("model", scene.objects[i]->model);
				scene.objects[i]->Draw(objectShader);
			}
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
	clear();

	return 0;
}

int run2(int f, bool speedUp) {
	prepare(f);
	scene.set_speedUp(speedUp);

	cout << "Calculating Ray Tracing..." << endl;
	clock_t clockStart = clock();

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SCR_WIDTH; ++i) {
		clock_t t1 = clock();
		for (int j = 0; j < SCR_HEIGHT; ++j) {
			glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2 / SCR_HEIGHT - 1.0f, 0.0f);
			glm::vec3 globalPos = camera.Position + camera.Front + pos.x * camera.Right * (float(SCR_WIDTH) / SCR_HEIGHT) + pos.y * camera.Up;

			Ray ray(camera.Position, globalPos);
			glm::vec3 color = scene.trace_ray(ray, 0);

			screenColor[i][j] = color;
		}
	}

	// initialize window
	//window = init_GLFW();
	//if (!window) {
	//	glfwTerminate();
	//	return -1;
	//}

	//float point[] = { 0,0,0 };

	//unsigned int VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	//glEnableVertexAttribArray(0);

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shader shader("shader/ray_tracing.vs", "shader/ray_tracing.fs");
	//shader.use();
	//glBindVertexArray(VAO);

	for (int i = 0; i < SCR_WIDTH; ++i) {
		for (int j = 0; j < SCR_HEIGHT; ++j) {
			glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2 / SCR_HEIGHT - 1.0f, 0.0f);
			//shader.setVec2("screenPos", glm::vec2(pos.x, pos.y));

			const glm::vec3& color = screenColor[i][j];
			//shader.setVec3("vertexColor", color);
			//glDrawArrays(GL_POINTS, 0, 1);
		}
	}

	//glfwSwapBuffers(window);

	FILE* fp = fopen("res.ppm", "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", SCR_WIDTH, SCR_HEIGHT);
	for (int i = SCR_HEIGHT - 1; i >= 0; --i) {
		for (int j = 0; j < SCR_WIDTH; ++j) {
			static unsigned char color[3];
			color[0] = (unsigned char) min((int) (255 * screenColor[j][i].x), 255);
			color[1] = (unsigned char) min((int) (255 * screenColor[j][i].y), 255);
			color[2] = (unsigned char) min((int) (255 * screenColor[j][i].z), 255);
			fwrite(color, 1, 3, fp);
		}
	}
	fclose(fp);

	cout << "Time = " << (double) (clock() - clockStart) / CLOCKS_PER_SEC << " s." << endl
		<< "Result picture saved in \"res.ppm\"." << endl;

	system("pause");
	//while (!glfwWindowShouldClose(window)) {
	//	processInput(window);
	//	glfwPollEvents();
	//}

	//glfwTerminate();
	clear();
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ray Tracing", NULL, NULL);
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
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	return window;
}

