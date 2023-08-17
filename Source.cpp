#include <glad/glad.h> 
#include <glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "model1.hpp"

#include <iostream>
#include <string>

using namespace std;

const unsigned int sW = 1100;
const unsigned int sH = 500;
const string fail = "Failed to create window!!\n";

glm::vec3 cPos = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cUp = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 cDown = glm::vec3(1.0f, 0.0f,  0.0f);
glm::vec3 lightPos(0.0f, 0.0f, 10.0f);

float deltaTime = 0.0f;
float lFrame = 0.0f;
float fov = 25.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(sW, sH, "First Reponse", NULL, NULL);
	if(window == NULL)
	{
		cout << fail;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	 
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << fail;
		return -1;
	}
	
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	
	shaders s1("shader1.xx", "shader2.xy");
	shaders s2("light1.xx", "light1.xy");
	loader m1("models/sqr.obj");
	loader m2("models/sqr1.obj"); //light

	//loop
	while (!glfwWindowShouldClose(window))
	{
        float cFrame = static_cast<float>(glfwGetTime());
        deltaTime = cFrame - lFrame;
        lFrame = cFrame;

		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		s1.use();
		s1.setVec3("objectColor", 1.0f, 0.5f, 0.1f);
		s1.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		s1.setVec3("lightPos", lightPos);
		s1.setVec3("viewPos", cPos);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)sW/(float)sH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cPos, cPos + cFront, cUp);
		s1.setMat4("projection", projection);
		s1.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        s1.setMat4("model", model);
        m1.Draw(s1);

        s2.use();
        s2.setMat4("projection", projection);
        s2.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        s2.setMat4("model", model);
		m2.Draw(s2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cSpeed = static_cast<float>(2.5 * deltaTime);
	if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
	{
		cPos -= glm::normalize(glm::cross(cFront, cDown)) * cSpeed;
	}
	else if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
	{
		cPos += glm::normalize(glm::cross(cFront, cDown)) * cSpeed;
	}
	else if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
	{
		cPos -= glm::normalize(glm::cross(cFront, cUp)) * cSpeed;
	}
	else if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
	{
		cPos += glm::normalize(glm::cross(cFront, cUp)) * cSpeed;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
	{
        fov = 1.0f;
	}
    if (fov > 45.0f)
	{
        fov = 45.0f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0,0,w,h);
}