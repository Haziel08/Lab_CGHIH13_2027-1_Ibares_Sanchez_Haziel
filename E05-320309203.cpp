/*
Práctica 5: Optimización y Carga de Modelos
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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;
Model BaseGarra_M;
Model BasePinza_M;
Model Brazo1_M;
Model Brazo2_M;
Model Garra_M;
Model RuedaFrente1_M;
Model RuedaFrente2_M;
Model RuedaMedia1_M;
Model RuedaMedia2_M;
Model RuedaTrasera1_M;
Model RuedaTrasera2_M;

Model HolocronBase_M;
Model Esquina1_M;
Model Esquina2_M;
Model Esquina3_M;
Model Esquina4_M;
Model Esquina5_M;
Model Esquina6_M;
Model Esquina7_M;
Model Esquina8_M;


Model Satelite_M;
Model SateliteAntena_M;
Model Brazo1Nuevo_M;
Model Brazo2Nuevo_M;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

GLfloat limitarRotacion(GLfloat angulo, GLfloat limite)
{
	if (angulo > limite)
		return limite;
	if (angulo < -limite)
		return -limite;
	return angulo;
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	Rover_M = Model();
	Rover_M.LoadModel("Models/base.obj");

	BaseGarra_M = Model();
	BaseGarra_M.LoadModel("Models/base_garra.obj");

	Brazo1_M = Model();
	Brazo1_M.LoadModel("Models/brazo_1.obj");

	Brazo2_M = Model();
	Brazo2_M.LoadModel("Models/brazo_2.obj");

	BasePinza_M = Model();
	BasePinza_M.LoadModel("Models/base_pinza.obj");

	Garra_M = Model();
	Garra_M.LoadModel("Models/garra.obj");

	RuedaFrente1_M = Model();
	RuedaFrente1_M.LoadModel("Models/rueda_frente_1.obj");

	RuedaFrente2_M = Model();
	RuedaFrente2_M.LoadModel("Models/rueda_frente_2.obj");

	RuedaMedia1_M = Model();
	RuedaMedia1_M.LoadModel("Models/rueda_media_1.obj");

	RuedaMedia2_M = Model();
	RuedaMedia2_M.LoadModel("Models/rueda_media_2.obj");

	RuedaTrasera1_M = Model();
	RuedaTrasera1_M.LoadModel("Models/rueda_trasera_1.obj");

	RuedaTrasera2_M = Model();
	RuedaTrasera2_M.LoadModel("Models/rueda_trasera_2.obj");

	HolocronBase_M = Model();
	HolocronBase_M.LoadModel("Models/Holocron_base.obj");

	Esquina1_M = Model();
	Esquina1_M.LoadModel("Models/Esquina1.obj");

	Esquina2_M = Model();
	Esquina2_M.LoadModel("Models/Esquina2.obj");

	Esquina3_M = Model();
	Esquina3_M.LoadModel("Models/Esquina3.obj");

	Esquina4_M = Model();
	Esquina4_M.LoadModel("Models/Esquina4.obj");

	Esquina5_M = Model();
	Esquina5_M.LoadModel("Models/Esquina5.obj");

	Esquina6_M = Model();
	Esquina6_M.LoadModel("Models/Esquina6.obj");

	Esquina7_M = Model();
	Esquina7_M.LoadModel("Models/Esquina7.obj");

	Esquina8_M = Model();
	Esquina8_M.LoadModel("Models/Esquina8.obj");

	Satelite_M = Model();
	Satelite_M.LoadModel("Models/Satelite.obj");

	SateliteAntena_M = Model();
	SateliteAntena_M.LoadModel("Models/Satelite_antena.obj");

	Brazo1Nuevo_M = Model();
	Brazo1Nuevo_M.LoadModel("Models/Brazo1.obj");

	Brazo2Nuevo_M = Model();
	Brazo2Nuevo_M.LoadModel("Models/Brazo2.obj");


	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxHolocron(1.0);
	glm::mat4 modelauxSatelite(1.0);
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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Modelo Inicial
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de color azul

		//Base del Rover
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();//modificar por el modelo de solo cuerpo del Rover, para que se pueda separar el brazo y las llantas
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		//En sesión se separara una parte del modelo y se unirá por jeraquía al cuerpo

		// Base de la garra
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.8f, 1.85f, -1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BaseGarra_M.RenderModel();
		modelaux = model;
		color = glm::vec3(0.2f, 0.8f, 0.2f);

		//Brazo 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo1_M.RenderModel();
		modelaux = model;
		color = glm::vec3(0.8f, 0.2f, 0.8f);

		//Brazo 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.8f, 2.9f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo2_M.RenderModel();
		modelaux = model;
		color = glm::vec3(0.9f, 0.9f, 0.1f);

		//Base de la pinza
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.4f, 2.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BasePinza_M.RenderModel();
		modelaux = model;
		color = glm::vec3(0.9f, 0.1f, 0.1f);

		//Garra
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, 0.3f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Garra_M.RenderModel();
		modelaux = model;



		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las llantas
		2.- Hacer que al presionar una tecla cada pata de rueda pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/

		//Llanta delantera derecha
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.4f, -7.0f, -2.5f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion1(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaFrente1_M.RenderModel();


		//Llanta delantera izquierda
		color = glm::vec3(0.15f, 0.15f, 0.15f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.4f, -7.0f, 3.6f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion2(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaFrente2_M.RenderModel();


		//Llanta media derecha
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, -2.2f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion3(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaMedia1_M.RenderModel();


		//Llanta media izquierda
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, 3.6f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion4(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaMedia2_M.RenderModel();

		//Llanta trasera derecha
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, -2.2f));
		model = glm::rotate(model, glm::radians(-limitarRotacion(mainWindow.getarticulacion5(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaTrasera1_M.RenderModel();

		//Llanta trasera izquierda
		color = glm::vec3(0.35f, 0.35f, 0.35f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, 3.6f));
		model = glm::rotate(model, glm::radians(-limitarRotacion(mainWindow.getarticulacion6(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaTrasera2_M.RenderModel();


		//===== HOLOCRÓN =====
		color = glm::vec3(0.5f, 0.4f, 0.15f); //color base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 3.0f, -1.5f));
		modelauxHolocron = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		HolocronBase_M.RenderModel();

		//Esquina 1
		color = glm::vec3(0.8f, 0.1f, 0.1f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(3.9f, 3.8f, 3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()),
			glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina1_M.RenderModel();

		//Esquina 2
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(4.5f, 3.8f, -3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina2_M.RenderModel();

		//Esquina 3
		color = glm::vec3(1.0f, 0.43f, 0.21f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(-4.2f, 3.7f, 3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina3_M.RenderModel();

		//Esquina 4
		color = glm::vec3(0.35f, 1.0f, 0.23f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(-4.2f, 3.8f, -3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina4_M.RenderModel();


		//Esquina 5
		color = glm::vec3(0.86f, 0.35f, 0.79f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(3.8f, -3.9f, 4.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina5_M.RenderModel();

		//Esquina 6
		color = glm::vec3(0.1f, 0.15f, 0.79f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(4.5f, -3.9f, -3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina6_M.RenderModel();

		//Esquina 7
		color = glm::vec3(0.4f, 0.56f, 0.349f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(-4.3f, -3.9f, -3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina7_M.RenderModel();

		//Esquina 8
		color = glm::vec3(1.0f, 0.26f, 0.9f);
		model = modelauxHolocron;
		model = glm::translate(model, glm::vec3(-4.1f, -3.9f, 3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Esquina8_M.RenderModel();

		//===== SATÉLITE =====
		color = glm::vec3(0.7f, 0.7f, 0.75f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 10.0f, -1.5f));
		modelauxSatelite = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Satelite_M.RenderModel();

		//Antena del satélite
		color = glm::vec3(0.9f, 0.9f, 0.9f);
		model = modelauxSatelite;
		model = glm::translate(model, glm::vec3(-2.8f, 10.8f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion15()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		SateliteAntena_M.RenderModel();

		//Brazo 1
		color = glm::vec3(0.3f, 0.3f, 0.9f);
		model = modelauxSatelite;
		model = glm::translate(model, glm::vec3(-6.0f, 5.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion16()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo1Nuevo_M.RenderModel();

		//Brazo 2
		color = glm::vec3(0.9f, 0.3f, 0.3f);
		model = modelauxSatelite;
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion17()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo2Nuevo_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
