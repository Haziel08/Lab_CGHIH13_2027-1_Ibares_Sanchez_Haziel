/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture dadoTexturejpg;
Texture dadoTexturepng;
Texture holocronTexture;
Texture logosStarWarsTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model CuboStarWars_M;
Model Nave_S;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};



	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0732f, 0.0039f,	0.0f,	0.0f,	-1.0f,	//0: Inf-Izq
		 0.5f, -0.5f,  0.5f,	0.1953f, 0.0039f,	0.0f,	0.0f,	-1.0f,	//1: Inf-Der
		 0.5f,  0.5f,  0.5f,	0.1953f, 0.1943f,	0.0f,	0.0f,	-1.0f,	//2: Sup-Der
		-0.5f,  0.5f,  0.5f,	0.0732f, 0.1943f,	0.0f,	0.0f,	-1.0f,	//3: Sup-Izq
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	0.1953f, 0.0039f,	-1.0f,	0.0f,	0.0f,	//4
		0.5f, -0.5f, -0.5f,	0.3271f, 0.0039f,	-1.0f,	0.0f,	0.0f,	//5
		0.5f,  0.5f, -0.5f,	0.3271f, 0.1943f,	-1.0f,	0.0f,	0.0f,	//6
		0.5f,  0.5f,  0.5f,	0.1953f, 0.1943f,	-1.0f,	0.0f,	0.0f,	//7
		// back
		-0.5f, -0.5f, -0.5f,	0.4687f, 0.0039f,	0.0f,	0.0f,	1.0f,	//8
		 0.5f, -0.5f, -0.5f,	0.3271f, 0.0039f,	0.0f,	0.0f,	1.0f,	//9
		 0.5f,  0.5f, -0.5f,	0.3271f, 0.1943f,	0.0f,	0.0f,	1.0f,	//10
		-0.5f,  0.5f, -0.5f,	0.4687f, 0.1943f,	0.0f,	0.0f,	1.0f,	//11


		// left
		//x		y		z		S		T
		-0.5f, -0.5f, -0.5f,	0.4687f, 0.0039f,	1.0f,	0.0f,	0.0f,	//12
		-0.5f, -0.5f,  0.5f,	0.6005f, 0.0039f,	1.0f,	0.0f,	0.0f,	//13
		-0.5f,  0.5f,  0.5f,	0.6005f, 0.1943f,	1.0f,	0.0f,	0.0f,	//14
		-0.5f,  0.5f, -0.5f,	0.4687f, 0.1943f,	1.0f,	0.0f,	0.0f,	//15

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.7275f, 0.0039f,	0.0f,	1.0f,	0.0f,	//16
		 0.5f, -0.5f,  0.5f,	0.6005f, 0.0039f,	0.0f,	1.0f,	0.0f,	//17
		 0.5f, -0.5f, -0.5f,	0.6005f, 0.1943f,	0.0f,	1.0f,	0.0f,	//18
		-0.5f, -0.5f, -0.5f,	0.7275f, 0.1943f,	0.0f,	1.0f,	0.0f,	//19

		//UP
		 //x		y		z		S		T
		 -0.5f,  0.5f,  0.5f,	0.7275f, 0.0039f,	0.0f,  -1.0f,	0.0f,	//20
		 0.5f,  0.5f,  0.5f,	0.8398f, 0.0039f,	0.0f,  -1.0f,	0.0f,	//21
		 0.5f,  0.5f, -0.5f,	0.8398f, 0.1943f,	0.0f,  -1.0f,	0.0f,	//22
		-0.5f,  0.5f, -0.5f,	0.7275f, 0.1943f,	0.0f,  -1.0f,	0.0f	//23

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	GLfloat holocron_vertices[] = {
		//   x      y      z          S         T           nx       ny       nz
		// T1 (Logo 1)
		 1.0f,  1.0f,  0.0f,	0.1523f, 0.9785f,	 0.5774f,  0.5774f,  0.5774f, 
		 0.0f,  1.0f,  1.0f,	0.0703f, 0.7852f,	 0.5774f,  0.5774f,  0.5774f, 
		 1.0f,  0.0f,  1.0f,	0.2344f, 0.7852f,	 0.5774f,  0.5774f,  0.5774f, 

		 // T2 (Logo 2)
		 1.0f,  1.0f,  0.0f,	0.3184f, 0.9785f,	 0.5774f,  0.5774f, -0.5774f, 
		 1.0f,  0.0f, -1.0f,	0.2461f, 0.7852f,	 0.5774f,  0.5774f, -0.5774f,
		 0.0f,  1.0f, -1.0f,	0.3906f, 0.7852f,	 0.5774f,  0.5774f, -0.5774f, 
		 
		  // T3 (Logo 3)
		 1.0f, -1.0f,  0.0f,	0.4844f, 0.7852f,	 0.5774f, -0.5774f,  0.5774f, 
		 1.0f,  0.0f,  1.0f,	0.4102f, 0.9785f,	 0.5774f, -0.5774f,  0.5774f, 
		 0.0f, -1.0f,  1.0f,	0.5586f, 0.9785f,	 0.5774f, -0.5774f,  0.5774f, 

		 // T4 (Logo 4)
		 1.0f, -1.0f,  0.0f,	0.6333f, 0.7852f,	 0.5774f, -0.5774f, -0.5774f, 
		 0.0f, -1.0f, -1.0f,	0.5664f, 0.9785f,	 0.5774f, -0.5774f, -0.5774f, 
		 1.0f,  0.0f, -1.0f,	0.7002f, 0.9785f,	 0.5774f, -0.5774f, -0.5774f, 

		// T5 (Logo 5)
		 -1.0f,  1.0f,  0.0f,	0.7749f, 0.9785f,	-0.5774f,  0.5774f,  0.5774f, 
		 -1.0f,  0.0f,  1.0f,	0.7129f, 0.7852f,	-0.5774f,  0.5774f,  0.5774f, 
		 0.0f,  1.0f,  1.0f,	0.8369f, 0.7852f,	-0.5774f,  0.5774f,  0.5774f, 

		// T6 (Logo 6)
		 -1.0f,  1.0f,  0.0f,	0.9038f, 0.9785f,	-0.5774f,  0.5774f, -0.5774f, 
		 0.0f,  1.0f, -1.0f,	0.8438f, 0.7852f,	-0.5774f,  0.5774f, -0.5774f, 
		 -1.0f,  0.0f, -1.0f,	0.9639f, 0.7852f,	-0.5774f,  0.5774f, -0.5774f, 

		// T7 (Logo 7)
		 -1.0f, -1.0f,  0.0f,	0.1211f, 0.5938f,	-0.5774f, -0.5774f,  0.5774f, 
		  0.0f, -1.0f,  1.0f,	0.0449f, 0.7705f,	-0.5774f, -0.5774f,  0.5774f, 
		 -1.0f,  0.0f,  1.0f,	0.1973f, 0.7705f,	-0.5774f, -0.5774f,  0.5774f,

		// T8 (Logo 8)
		 -1.0f, -1.0f,  0.0f,	0.2890f, 0.5938f,	-0.5774f, -0.5774f, -0.5774f, 
		 -1.0f,  0.0f, -1.0f,	0.2090f, 0.7705f,	-0.5774f, -0.5774f, -0.5774f, 
		 0.0f, -1.0f, -1.0f,	0.3691f, 0.7705f,	-0.5774f, -0.5774f, -0.5774f, 

		// S1 (Logo 9 - Cuadrado)
		 1.0f,  1.0f,  0.0f,	0.4600f, 0.7705f,	 1.0000f,  0.0000f,  0.0000f, 
		 1.0f,  0.0f,  1.0f,	0.5371f, 0.6821f,	 1.0000f,  0.0000f,  0.0000f, 
		 1.0f, -1.0f,  0.0f,	0.4600f, 0.5938f,	 1.0000f,  0.0000f,  0.0000f, 
		 1.0f,  0.0f, -1.0f,	0.3828f, 0.6821f,	 1.0000f,  0.0000f,  0.0000f, 

		// S2 (Logo 10 - Cuadrado)
		 -1.0f,  1.0f,  0.0f,	0.6240f, 0.7705f,	-1.0000f,  0.0000f,  0.0000f,
		 -1.0f,  0.0f, -1.0f,	0.7080f, 0.6821f,	-1.0000f,  0.0000f,  0.0000f,
		 -1.0f, -1.0f,  0.0f,	0.6240f, 0.5938f,	-1.0000f,  0.0000f,  0.0000f,
		 -1.0f,  0.0f,  1.0f,	0.5400f, 0.6821f,	-1.0000f,  0.0000f,  0.0000f,

		// S3 (Logo 11 - Cuadrado)
		 1.0f,  1.0f,  0.0f,	0.7710f, 0.7705f,	 0.0000f,  1.0000f,  0.0000f,
		 0.0f,  1.0f, -1.0f,	0.8301f, 0.6821f,	 0.0000f,  1.0000f,  0.0000f,
		 -1.0f,  1.0f,  0.0f,	0.7710f, 0.5938f,	 0.0000f,  1.0000f,  0.0000f,
		 0.0f,  1.0f,  1.0f,	0.7119f, 0.6821f,	 0.0000f,  1.0000f,  0.0000f,

		// S4 (Logo 12 - Cuadrado)
		 1.0f, -1.0f,  0.0f,	0.9033f, 0.7705f,	 0.0000f, -1.0000f,  0.0000f,
		 0.0f, -1.0f,  1.0f,	0.9697f, 0.6821f,	 0.0000f, -1.0000f,  0.0000f,
		 -1.0f, -1.0f,  0.0f,	0.9033f, 0.5938f,	 0.0000f, -1.0000f,  0.0000f,
		 0.0f, -1.0f, -1.0f,	0.8369f, 0.6821f,	 0.0000f, -1.0000f,  0.0000f,

		// S5 (Logo 13 - Cuadrado)
		 1.0f,  0.0f,  1.0f,	0.0981f, 0.5869f,	 0.0000f,  0.0000f,  1.0000f,
		 0.0f,  1.0f,  1.0f,	0.1758f, 0.4956f,	 0.0000f,  0.0000f,  1.0000f,
		-1.0f,  0.0f,  1.0f,	0.0981f, 0.4043f,	 0.0000f,  0.0000f,  1.0000f,
		 0.0f, -1.0f,  1.0f,	0.0205f, 0.4956f,	 0.0000f,  0.0000f,  1.0000f,

		// S6 (Logo 14 - Cuadrado)
		 1.0f,  0.0f, -1.0f,	0.2509f, 0.5869f,	 0.0000f,  0.0000f, -1.0000f,
		 0.0f, -1.0f, -1.0f,	0.3164f, 0.4956f,	 0.0000f,  0.0000f, -1.0000f,
		 -1.0f,  0.0f, -1.0f,	0.2509f, 0.4043f,	 0.0000f,  0.0000f, -1.0000f,
		 0.0f,  1.0f, -1.0f,	0.1855f, 0.4956f,	 0.0000f,  0.0000f, -1.0000f,
	};

	unsigned int holocron_indices[] = {
		0, 1, 2,        // T1
		3, 4, 5,        // T2
		6, 7, 8,        // T3
		9, 10, 11,      // T4
		12, 13, 14,     // T5
		15, 16, 17,     // T6
		18, 19, 20,     // T7
		21, 22, 23,     // T8
		24, 25, 26,  24, 26, 27,    // S1
		28, 29, 30,  28, 30, 31,    // S2
		32, 33, 34,  32, 34, 35,    // S3
		36, 37, 38,  36, 38, 39,    // S4
		40, 41, 42,  40, 42, 43,    // S5
		44, 45, 46,  44, 46, 47,    // S6
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture.LoadTextureA();
	logosStarWarsTexture = Texture("Textures/star wars logos.png");
	logosStarWarsTexture.LoadTextureA();

	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_simple.obj");

	CuboStarWars_M = Model();
	CuboStarWars_M.LoadModel("Models/cubo_star_wars.obj");

	Holocron_M = Model();
	Holocron_M.LoadModel("Models/HolocronTextura.obj");

	Nave_S = Model();
	Nave_S.LoadModel("Models/navesub.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();




		/*Dado de Opengl
		Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		dadoTexturejpg.UseTexture();
		meshListModel[4]->RenderMeshModel();
		glDisable(GL_BLEND); para tga*/


		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes

		/*
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
		*/




		/*Reporte de práctica :

		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje

		*/


		// Holocrón codigo
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 3.5f, -2.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWarsTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


		// Holocrón importado de Blender
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 3.5f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		//Nave
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 3.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_S.RenderModel();


		//Con Blender
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuboStarWars_M.RenderModel();

		//Con codigo
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWarsTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();






		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/