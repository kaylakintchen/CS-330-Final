#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "cylinder.h"
#include "Sphere.h"

// floss
#include "floss.h"

// notebook 
#include "notebook.h"

// notebook spirals
#include "notebookSpiral.h"

// speaker 
#include "speaker.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 12.0f, 25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// camera
glm::vec3 cameraPos = glm::vec3(-5.0f, 15.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(5.0f, 15.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// VARIABLE FOR PERSPECTIVE
bool perspectiveMode = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kayla Kintchen", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightSphereShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// Floss Vertices
	float flossVerts[5000];
	// Retrieve floss.h vector
	vector<float>* flossVec = Floss::drawFloss();
	// Add vectors into verts, then delete them
	for (unsigned int i = 0; i < flossVec->size(); i++)
	{
		flossVerts[i] = flossVec->at(i);
	}
	delete(flossVec);

	// Notebook Vertices
	float notebookVerts[5000];
	// Retrieve notebook.h vector
	vector<float>* notebookVec = Notebook::drawNotebook();
	// Add vectors into verts, then delete them
	for (unsigned int i = 0; i < notebookVec->size(); i++)
	{
		notebookVerts[i] = notebookVec->at(i);
	}
	delete(notebookVec);

	// Notebook Spiral Vertices
	float notebookSpiralVerts[5000];
	// Retrieve notebookspiral.h vector
	vector<float>* spiralVec = NotebookSpiral::drawNotebookSpirals();
	// Add vectors into verts, then delete them
	for (unsigned int i = 0; i < spiralVec->size(); i++)
	{
		notebookSpiralVerts[i] = spiralVec->at(i);
	}
	delete(spiralVec);

	// Speaker Vertices
	float speakerVerts[5000];
	// Retrieve speaker.h vector
	vector<float>* speakerVec = Speaker::drawSpeaker();
	// Add vectors into verts, then delete them
	for (unsigned int i = 0; i < speakerVec->size(); i++)
	{
		speakerVerts[i] = speakerVec->at(i);
	}
	delete(speakerVec);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	float planeVertices[] = {
		// Positions            // Texture Coords
		-10.0f, 3.5f, -10.0f,   0.0f, 0.0f,
		 10.0f, 3.5f, -10.0f,   10.0f, 0.0f,
		 10.0f, 3.5f,  10.0f,   10.0f, 10.0f,
		-10.0f, 3.5f,  10.0f,   0.0f, 10.0f,
	};

	float cupVertices[] = {
	-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	};





	// position of floss 
	glm::vec3 flossPosition[] = {
		glm::vec3(0.0f, -1.0f, -17.0f),
	};
	
	glm::vec3 notebookPosition[] = {
		glm::vec3(-4.0f, -1.0f, -15.0f),
	};

	glm::vec3 notebookSpiralPosition[] = {
		glm::vec3(-6.5f, -1.0f, -15.0f)
	};
	
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  12.0f,  -20.0f),
		glm::vec3(-6.0f, 12.0f, -15.0f),
		glm::vec3(6.0f,  12.0f, -10.0f),
		glm::vec3(0.0f,  12.0f, -5.0f)
	};
	
	// position plane 
	glm::vec3 planePositions[] = {
		glm::vec3(5.0f, 3.0f, 6.0f)
	};
	
	// position speaker
	glm::vec3 speakerPosition[] = {
		glm::vec3(2.0f, 3.0f, -25.0f),
	};


	////// Configure Floss VAO and VBO //////
	unsigned int flossVBO, flossVAO;
	glGenVertexArrays(1, &flossVAO);
	glGenBuffers(1, &flossVBO);

	glBindBuffer(GL_ARRAY_BUFFER, flossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flossVerts), flossVerts, GL_STATIC_DRAW);

	glBindVertexArray(flossVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	////// Configure Notebook VAO and VBO //////
	unsigned int notebookVBO, notebookVAO;
	glGenVertexArrays(1, &notebookVAO);
	glGenBuffers(1, &notebookVBO);

	glBindBuffer(GL_ARRAY_BUFFER, notebookVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(notebookVerts), notebookVerts, GL_STATIC_DRAW);

	glBindVertexArray(notebookVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	////// Configure Notebook VAO and VBO //////
	unsigned int notebookSpiralVBO, notebookSpiralVAO;
	glGenVertexArrays(1, &notebookSpiralVAO);
	glGenBuffers(1, &notebookSpiralVBO);

	glBindBuffer(GL_ARRAY_BUFFER, notebookSpiralVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(notebookSpiralVerts), notebookSpiralVerts, GL_STATIC_DRAW);

	glBindVertexArray(notebookSpiralVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	////// Configure the Light VAO, VBO, and IBO //////
	Sphere light(1.5f, 20, 30, true);    // Radius, Sectors, Stacks, Smooth Shading bool
	unsigned int lightVBO, lightVAO, lightIBO;
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, light.getInterleavedVertexSize(), light.getInterleavedVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &lightIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightIBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, light.getIndexSize(), light.getIndices(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// Bind the VBO to the buffer and set up the attributes
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	int lightStride = light.getInterleavedStride();
	glVertexAttribPointer(0, 3, GL_FLOAT, false, lightStride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, lightStride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, lightStride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	//Bind the IBO to the VAO for rendering
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightIBO);


	// Configure plane VAO & VBO
	unsigned int planeVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Configure cup VAO & VBO
	unsigned int cupVBO, cupVAO;
	glGenVertexArrays(1, &cupVAO);
	glGenBuffers(1, &cupVBO);
	glBindVertexArray(cupVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO); // POSSIBLE FIXME

	// Configure straw VAO & VBO
	unsigned int strawVBO, strawVAO;
	glGenVertexArrays(1, &strawVAO);
	glGenBuffers(1, &strawVBO);
	glBindVertexArray(strawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, strawVBO);


	////// Configure Speaker VAO and VBO //////
	unsigned int speakerVBO, speakerVAO;
	glGenVertexArrays(1, &speakerVAO);
	glGenBuffers(1, &speakerVBO);

	glBindBuffer(GL_ARRAY_BUFFER, speakerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerVerts), speakerVerts, GL_STATIC_DRAW);

	glBindVertexArray(speakerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	unsigned int planeTexture = loadTexture("images/pinkMarble.jpg");
	unsigned int cupTexture = loadTexture("images/whitebg.jpg");
	unsigned int strawTexture = loadTexture("images/steel.jpg");
	unsigned int notebookTexture = loadTexture("images/yellow.jpg");
	unsigned int flossTexture = loadTexture("images/seaglass3.jpg");
	unsigned int notebookSpiralTexture = loadTexture("images/whiteFence.jpg");
	unsigned int speakerTexture = loadTexture("images/screen.jpg");

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("flossTexture", 0);
	lightingShader.setInt("notebookTexture", 0);
	lightingShader.setInt("notebookSpiralTexture", 0);
	lightingShader.setInt("speakerTexture", 0);
	


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.6f, 0.8f, 0.5f, 1.0f); // Light olive green
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		////// Build the floss  //////

		// Binds floss texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, flossTexture);

		glBindVertexArray(flossVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, flossPosition[i]);
			model = glm::scale(model, glm::vec3(2.0f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 42);
		}

		////// Build the Notebook //////

			// Binds notebook texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, notebookTexture);

		glBindVertexArray(notebookVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, notebookPosition[i]);
			model = glm::scale(model, glm::vec3(5.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 42);
		}

		////// Build the Notebook Spirals //////

		// Binds notebook spiral texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, notebookSpiralTexture);

		glBindVertexArray(notebookSpiralVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, notebookSpiralPosition[i]);
			model = glm::scale(model, glm::vec3(2.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			lightingShader.setMat4("model", model);

			static_meshes_3D::Cylinder C2(0.3, 10, 5, true, true, true); 
			C2.render();
		}

		////// Build the speaker  //////

		// Binds floss texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, speakerTexture);

		glBindVertexArray(speakerVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, speakerPosition[i]);
			model = glm::scale(model, glm::vec3(1.0f));
			lightingShader.setMat4("model", model);

			static_meshes_3D::Cylinder C2(3, 10, 9, true, true, true);
			C2.render();
		}

		// RENDER PLANE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		

		glBindVertexArray(planeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model_plane = glm::mat4(1.0f);
			model_plane = glm::translate(model_plane, glm::vec3(0.0f, -5.0f, 0.0f)); // Position the plane below the cup's base
			model_plane = glm::rotate(model_plane, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model_plane = glm::scale(model_plane, glm::vec3(30.0f, 1.0f, 20.0f)); // Scale the plane to be larger in x and z directions
			lightingShader.setMat4("model", model_plane);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// RENDER CUP
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cupTexture);
		glBindVertexArray(cupVAO);
		glm::mat4 model_cup = glm::mat4(1.0f);
		model_cup = glm::translate(model_cup, glm::vec3(5.0f, 2.0f, -17.0f));
		lightingShader.setMat4("model", model_cup);

		static_meshes_3D::Cylinder C(2, 8, 7, true, true, true); // cup size
		C.render();

		// RENDER STRAW
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, strawTexture);
		glBindVertexArray(strawVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(5.0f, 7.0f, -17.0f));
		model = glm::rotate(model, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Tilt the straw by 45 degrees around the z-axis
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C2(0.15, 10, 3, true, true, true); //best straw size (0.15, 10, 4)
		C2.render();




		/////// Build the Lights ///////

		lightSphereShader.use();
		lightSphereShader.setMat4("projection", projection);
		lightSphereShader.setMat4("view", view);

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller sphere
			lightSphereShader.setMat4("model", model);
			// Draw the lights
			glDrawElements(GL_TRIANGLES, light.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	glDeleteVertexArrays(1, &flossVAO);
	glDeleteBuffers(1, &flossVBO);
	glDeleteVertexArrays(1, &notebookVAO);
	glDeleteBuffers(1, &notebookVBO);
	glDeleteVertexArrays(1, &notebookSpiralVAO);
	glDeleteBuffers(1, &notebookSpiralVBO);
	glDeleteVertexArrays(1, &speakerVAO);
	glDeleteBuffers(1, &speakerVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteVertexArrays(1, &cupVAO);
	glDeleteBuffers(1, &cupVBO);
	glDeleteVertexArrays(1, &strawVAO);
	glDeleteBuffers(1, &strawVBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	// Move camera up
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // go up with Q
		camera.ProcessKeyboard(UP, deltaTime);
	// Move camera down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // go down with E
		camera.ProcessKeyboard(DOWN, deltaTime);

	// perspective 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		perspectiveMode = !perspectiveMode;
	}


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}