#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "Camera.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "SOM.h"
#include "MeshLoader.h"
#include "MeshLibrary.h"

void processInput(GLFWwindow*);
void framebufferSizeCallback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool cameraLock = true;
bool fillSOM = false;
glm::vec3 objectColor = glm::vec3(0, 1, 1);
glm::vec3 somColor = glm::vec3(1, 0, 0);
int somMeshWidth = 2;
int somMeshHeight = 2;
float somLearningRate = 0.5;
int somConfigValue = 0;
bool somTrainingStarted = false;
bool showSOM = false;
bool showMeshSOM = true;
bool SOMinitialized = false;
int maxEpochs = 500;
bool unlimitedEpochs = false;
unsigned int numVertices;

std::pair<unsigned int, unsigned int> coloredMesh;
std::pair<float*, unsigned int> coloredArray;

int meshType = 0;

int main(void)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Self Organized Map", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create the window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		glfwTerminate();
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


	unsigned int simpleProgram = ShaderLoader::Load("simple");
	unsigned int coloredProgram = ShaderLoader::Load("colored");

	MeshLoader::InitializeColorMap();

	

	SOM som(2,2);

	MeshLibrary::meshMap["cube"] = MeshLoader::LoadColoredMeshFromFile("cube");
	MeshLibrary::meshMap["cone"] = MeshLoader::LoadColoredMeshFromFile("cone");
	MeshLibrary::meshMap["cylinder"] = MeshLoader::LoadColoredMeshFromFile("cylinder");
	MeshLibrary::meshMap["uvsphere"] = MeshLoader::LoadColoredMeshFromFile("uvsphere");
	MeshLibrary::meshMap["bunny"] = MeshLoader::LoadColoredMeshFromFile("bunny");

	MeshLibrary::arrayMap["cube"] = MeshLoader::LoadColoredArrayFromFile("cube");
	MeshLibrary::arrayMap["cone"] = MeshLoader::LoadColoredArrayFromFile("cone");
	MeshLibrary::arrayMap["cylinder"] = MeshLoader::LoadColoredArrayFromFile("cylinder");
	MeshLibrary::arrayMap["uvsphere"] = MeshLoader::LoadColoredArrayFromFile("uvsphere");
	MeshLibrary::arrayMap["bunny"] = MeshLoader::LoadColoredArrayFromFile("bunny");

	MeshLibrary::meshList.push_back("cube");
	MeshLibrary::meshList.push_back("cone");
	MeshLibrary::meshList.push_back("cylinder");
	MeshLibrary::meshList.push_back("uvsphere");
	MeshLibrary::meshList.push_back("bunny");
	
	
	coloredMesh = MeshLibrary::meshMap["cube"];
	coloredArray = MeshLibrary::arrayMap["cube"];

    unsigned int cubeVertices = 8;
	unsigned int  coneVertices = 33;
    unsigned int cylinderVertices = 64;
	unsigned int sphereVertices = 31746;
	unsigned int bunnyVertices = 34834;

	numVertices = cubeVertices;

	int numIterations = 0;


	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(coloredProgram);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 10000.0f);
		int loc = glGetUniformLocation(coloredProgram, "projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 view = camera.GetViewMatrix();
		loc = glGetUniformLocation(coloredProgram, "view");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 Transform = glm::mat4(1.0f);
		if (meshType == 4)
		{
			Transform = glm::scale(Transform, glm::vec3(10, 10, 10));
		}
		else
		{
			Transform = glm::scale(Transform, glm::vec3(1, 1, 1));
		}
		
		loc = glGetUniformLocation(coloredProgram, "transform");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Transform));
		

		glBindVertexArray(coloredMesh.first);
		glDrawElements(GL_POINTS, coloredMesh.second, GL_UNSIGNED_INT, 0);


		if (showMeshSOM && SOMinitialized)
		{
			som.DrawMesh();
		}

		
		

		glUseProgram(simpleProgram);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 10000.0f);
		loc = glGetUniformLocation(simpleProgram, "projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
		view = camera.GetViewMatrix();
		loc = glGetUniformLocation(simpleProgram, "view");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

		if (!fillSOM)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		


		glm::mat4 transform = glm::mat4(1.0f);
		
		loc = glGetUniformLocation(simpleProgram, "transform");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transform));
		loc = glGetUniformLocation(simpleProgram, "objectColor");
		glUniform3fv(loc, 1, glm::value_ptr(somColor));

		

		if (somTrainingStarted && SOMinitialized && ((numIterations<maxEpochs) || (unlimitedEpochs)))
		{
			som.UpdateSOM(coloredArray.first, coloredArray.second, Transform);
			
		}


		if (showSOM && SOMinitialized)
		{
			som.Draw();
		}

		
		
		





		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Parameters");
		ImGui::ColorEdit3("SOM Color", glm::value_ptr(somColor));
		ImGui::Checkbox("Show SOM", &showSOM);
		ImGui::SameLine();
		ImGui::Checkbox("Show Grid", &showMeshSOM);
		ImGui::DragInt("Mesh Width", &somMeshWidth, 1, 2);
		ImGui::DragInt("Mesh Height", &somMeshHeight, 1, 2);
		ImGui::DragFloat("LearningRate", &somLearningRate);
		ImGui::DragInt("Max Epochs", &maxEpochs, 1, 1);
		ImGui::SameLine();
		ImGui::Checkbox("Infinite",&unlimitedEpochs);
		

		ImGui::RadioButton("Cube", &meshType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Cone", &meshType, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Cylinder", &meshType, 2);
		ImGui::SameLine();
		ImGui::RadioButton("Sphere", &meshType, 3);
		ImGui::SameLine();
		ImGui::RadioButton("Bunny", &meshType, 4);

		if (ImGui::Button("Update Mesh", ImVec2{ 120,20 }))
		{
			if (meshType == 0)
			{
				coloredMesh = MeshLibrary::meshMap["cube"];
				coloredArray = MeshLibrary::arrayMap["cube"];
				numVertices = cubeVertices;
			}

			if (meshType == 1)
			{
				coloredMesh = MeshLibrary::meshMap["cone"];
				coloredArray = MeshLibrary::arrayMap["cone"];
				numVertices = coneVertices;
			}

			if (meshType == 2)
			{
				coloredMesh = MeshLibrary::meshMap["cylinder"];
				coloredArray = MeshLibrary::arrayMap["cylinder"];
				numVertices = cylinderVertices;
			}

			if (meshType == 3)
			{
				coloredMesh = MeshLibrary::meshMap["uvsphere"];
				coloredArray = MeshLibrary::arrayMap["uvsphere"];
				numVertices = sphereVertices;

			}

			if (meshType == 4)
			{
				coloredMesh = MeshLibrary::meshMap["bunny"];
				coloredArray = MeshLibrary::arrayMap["bunny"];
				numVertices = bunnyVertices;
			}

			somTrainingStarted = false;



		}
		ImGui::SameLine();
		ImGui::Text("No. Vertices");
		ImGui::SameLine();
		ImGui::Text("%u",numVertices);

		ImGui::RadioButton("Cross", &somConfigValue, 0);
		ImGui::SameLine();
		ImGui::RadioButton("X", &somConfigValue, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Star", &somConfigValue, 2);


		if (ImGui::Button("Initialize SOM", ImVec2{ 120,20 }))
		{
			som.SetLearningRate(somLearningRate);
			som.SetMeshWidth(somMeshWidth);
			som.SetMeshHeight(somMeshHeight);

			if (somConfigValue == 0)
			{
				som.InitializeSOMCrossConfiguration();
			}

			if (somConfigValue == 1)
			{
				som.InitializeSOMXConfiguration();
			}

			if (somConfigValue == 2)
			{
				som.InitializeSOMStarConfiguration();
			}

			SOMinitialized = true;
			
			
		}

		if (ImGui::Button("Start Training", ImVec2{ 120,20 }))
		{
			somTrainingStarted = true;
			
		}

		if (ImGui::Button("Stop training", ImVec2{ 120,20 }))
		{
			somTrainingStarted = false;
			numIterations = 0;
		}

		ImGui::DragFloat("Camera Speed", &camera.MovementSpeed, 1, 2);
		
		ImGui::End();



		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




		glfwSwapBuffers(window);
		glfwPollEvents();

		numIterations++;

	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (!cameraLock)
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

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		cameraLock = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		cameraLock = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		camera.Position = glm::vec3(0, 0, 3);
		camera.Yaw = -90.0f;
		camera.Pitch = 0.0f;
		camera.Front = glm::vec3(0, 0, -1);
	}

	


}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!cameraLock)
	{

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
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
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!cameraLock)
	{
		camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}
}