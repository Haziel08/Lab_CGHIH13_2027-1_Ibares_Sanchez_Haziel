/*
Práctica 7: Iluminación 1 
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexturepng;
Texture holocronTexture;
Texture logosStarWarsTexture;
// Añadir Texturas del dado y del holocron


Model Blackhawk_M;
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

Model Nave_S;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,


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

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
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
		0.5f, -0.5f,  0.5f,	0.1953f, 0.0039f,	0.0f,	0.0f,	0.0f,	//4
		0.5f, -0.5f, -0.5f,	0.3271f, 0.0039f,	0.0f,	0.0f,	0.0f,	//5
		0.5f,  0.5f, -0.5f,	0.3271f, 0.1943f,	0.0f,	0.0f,	0.0f,	//6
		0.5f,  0.5f,  0.5f,	0.1953f, 0.1943f,	0.0f,   0.0f,	0.0f,	//7
		// back
		-0.5f, -0.5f, -0.5f,	0.4687f, 0.0039f,	0.0f,	0.0f,	0.0f,	//8
		 0.5f, -0.5f, -0.5f,	0.3271f, 0.0039f,	0.0f,	0.0f,	0.0f,	//9
		 0.5f,  0.5f, -0.5f,	0.3271f, 0.1943f,	0.0f,	0.0f,	0.0f,	//10
		-0.5f,  0.5f, -0.5f,	0.4687f, 0.1943f,	0.0f,	0.0f,	0.0f,	//11


		// left
		//x		y		z		S		T
		-0.5f, -0.5f, -0.5f,	0.4687f, 0.0039f,	0.0f,	0.0f,	0.0f,	//12
		-0.5f, -0.5f,  0.5f,	0.6005f, 0.0039f,	0.0f,	0.0f,	0.0f,	//13
		-0.5f,  0.5f,  0.5f,	0.6005f, 0.1943f,	0.0f,	0.0f,	0.0f,	//14
		-0.5f,  0.5f, -0.5f,	0.4687f, 0.1943f,	0.0f,	0.0f,	0.0f,	//15

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.7275f, 0.0039f,	0.0f,	0.0f,	0.0f,	//16
		 0.5f, -0.5f,  0.5f,	0.6005f, 0.0039f,	0.0f,	0.0f,	0.0f,	//17
		 0.5f, -0.5f, -0.5f,	0.6005f, 0.1943f,	0.0f,	0.0f,	0.0f,	//18
		-0.5f, -0.5f, -0.5f,	0.7275f, 0.1943f,	0.0f,	0.0f,	0.0f,	//19

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
		// x       y       z        S         T          nx      ny      nz

		// T1: Cara diagonal (+X, +Y, +Z)
		 1.0f,  1.0f,  0.0f,  0.1523f, 0.9785f,  0.0, -1.0, 0.0f,
		 0.0f,  1.0f,  1.0f,  0.0703f, 0.7852f,  0.0, -1.0, 0.0f,
		 1.0f,  0.0f,  1.0f,  0.2344f, 0.7852f,  0.0, -1.0, 0.0f,

		 // T2: Cara diagonal (+X, +Y, -Z)
		  1.0f,  1.0f,  0.0f,  0.3184f, 0.9785f,  0.0, 0.0, 0.0f,
		  1.0f,  0.0f, -1.0f,  0.2461f, 0.7852f,  0.0, 0.0, 0.0f,
		  0.0f,  1.0f, -1.0f,  0.3906f, 0.7852f,  0.0, 0.0, 0.0f,

		  // T3: Cara diagonal (+X, -Y, +Z)
		   1.0f, -1.0f,  0.0f,  0.4844f, 0.7852f,  0.0, 0.0, -0.5f,
		   1.0f,  0.0f,  1.0f,  0.4102f, 0.9785f,  0.0, 0.0, -0.5f,
		   0.0f, -1.0f,  1.0f,  0.5586f, 0.9785f,  0.0, 0.0, -0.5f,

		   // T4: Cara diagonal (+X, -Y, -Z)
			1.0f, -1.0f,  0.0f,  0.6333f, 0.7852f,  0.0, 0.0, 0.0f,
			0.0f, -1.0f, -1.0f,  0.5664f, 0.9785f,  0.0, 0.0, 0.0f,
			1.0f,  0.0f, -1.0f,  0.7002f, 0.9785f,  0.0, 0.0, 0.0f,

			// T5: Cara diagonal (-X, +Y, +Z)
			-1.0f,  1.0f,  0.0f,  0.7749f, 0.9785f, 0.0, -1.0, 0.0f,
			-1.0f,  0.0f,  1.0f,  0.7129f, 0.7852f, 0.0, -1.0, 0.0f,
			 0.0f,  1.0f,  1.0f,  0.8369f, 0.7852f, 0.0, -1.0, 0.0f,

			 // T6: Cara diagonal (-X, +Y, -Z)
			 -1.0f,  1.0f,  0.0f,  0.9038f, 0.9785f, 0.0, 0.0, 0.0f,
			  0.0f,  1.0f, -1.0f,  0.8438f, 0.7852f, 0.0, 0.0, 0.0f,
			 -1.0f,  0.0f, -1.0f,  0.9639f, 0.7852f, 0.0, 0.0, 0.0f,

			 // T7: Cara diagonal (-X, -Y, +Z)
			 -1.0f, -1.0f,  0.0f,  0.1211f, 0.5938f, 0.0, 0.0, -0.5f,
			  0.0f, -1.0f,  1.0f,  0.0449f, 0.7705f, 0.0, 0.0, -0.5f,
			 -1.0f,  0.0f,  1.0f,  0.1973f, 0.7705f, 0.0, 0.0, -0.5f,

			 // T8: Cara diagonal (-X, -Y, -Z)
			 -1.0f, -1.0f,  0.0f,  0.2890f, 0.5938f, 0.0, 0.0, 0.0f,
			 -1.0f,  0.0f, -1.0f,  0.2090f, 0.7705f, 0.0, 0.0, 0.0f,
			  0.0f, -1.0f, -1.0f,  0.3691f, 0.7705f, 0.0, 0.0, 0.0f,

			  // S1: Cara derecha (+X)
			   1.0f,  1.0f,  0.0f,  0.4600f, 0.7705f,  0.0f,  0.0f,  0.0f,
			   1.0f,  0.0f,  1.0f,  0.5371f, 0.6821f,  0.0f,  0.0f,  0.0f,
			   1.0f, -1.0f,  0.0f,  0.4600f, 0.5938f,  0.0f,  0.0f,  0.0f,
			   1.0f,  0.0f, -1.0f,  0.3828f, 0.6821f,  0.0f,  0.0f,  0.0f,

			   // S2: Cara izquierda (-X)
			   -1.0f,  1.0f,  0.0f,  0.6240f, 0.7705f, 0.0f,  0.0f,  0.0f,
			   -1.0f,  0.0f, -1.0f,  0.7080f, 0.6821f, 0.0f,  0.0f,  0.0f,
			   -1.0f, -1.0f,  0.0f,  0.6240f, 0.5938f, 0.0f,  0.0f,  0.0f,
			   -1.0f,  0.0f,  1.0f,  0.5400f, 0.6821f, 0.0f,  0.0f,  0.0f,

			   // S3: Cara superior (+Y)
				1.0f,  1.0f,  0.0f,  0.7710f, 0.7705f,  0.0f,  -1.0f,  0.0f,
				0.0f,  1.0f, -1.0f,  0.8301f, 0.6821f,  0.0f,  -1.0f,  0.0f,
			   -1.0f,  1.0f,  0.0f,  0.7710f, 0.5938f,  0.0f,  -1.0f,  0.0f,
				0.0f,  1.0f,  1.0f,  0.7119f, 0.6821f,  0.0f,  -1.0f,  0.0f,

				// S4: Cara inferior (-Y)
				 1.0f, -1.0f,  0.0f,  0.9033f, 0.7705f,  0.0f, 0.0f,  0.0f,
				 0.0f, -1.0f,  1.0f,  0.9697f, 0.6821f,  0.0f, 0.0f,  0.0f,
				-1.0f, -1.0f,  0.0f,  0.9033f, 0.5938f,  0.0f, 0.0f,  0.0f,
				 0.0f, -1.0f, -1.0f,  0.8369f, 0.6821f,  0.0f, 0.0f,  0.0f,

				 // S5: Cara frontal (+Z)
				  1.0f,  0.0f,  1.0f,  0.0981f, 0.5869f,  0.0f,  0.0f,  -1.0f,
				  0.0f,  1.0f,  1.0f,  0.1758f, 0.4956f,  0.0f,  0.0f,  -1.0f,
				 -1.0f,  0.0f,  1.0f,  0.0981f, 0.4043f,  0.0f,  0.0f,  -1.0f,
				  0.0f, -1.0f,  1.0f,  0.0205f, 0.4956f,  0.0f,  0.0f,  -1.0f,

				  // S6: Cara trasera (-Z)
				   1.0f,  0.0f, -1.0f,  0.2509f, 0.5869f,  0.0f,  0.0f, 0.0f,
				   0.0f, -1.0f, -1.0f,  0.3164f, 0.4956f,  0.0f,  0.0f, 0.0f,
				  -1.0f,  0.0f, -1.0f,  0.2509f, 0.4043f,  0.0f,  0.0f, 0.0f,
				   0.0f,  1.0f, -1.0f,  0.1855f, 0.4956f,  0.0f,  0.0f, 0.0f
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

		24, 25, 26,  24, 26, 27, // S1
		28, 29, 30,  28, 30, 31, // S2
		32, 33, 34,  32, 34, 35, // S3
		36, 37, 38,  36, 38, 39, // S4
		40, 41, 42,  40, 42, 43, // S5
		44, 45, 46,  44, 46, 47  // S6
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);
}



GLfloat limitarRotacion(GLfloat angulo, GLfloat limite)
{
	if (angulo > limite)
		return limite;
	if (angulo < -limite)
		return -limite;
	return angulo;
}

void ActualizarFaro(SpotLight& luz, glm::mat4 padre, glm::vec3 posLocal, glm::vec3 dirLocal)
{
	glm::vec3 pos = glm::vec3(padre * glm::vec4(posLocal, 1.0f));
	glm::vec3 dir = glm::normalize(glm::vec3(padre * glm::vec4(dirLocal, 0.0f)));
	luz.SetFlash(pos, dir);
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

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	logosStarWarsTexture = Texture("Textures/star wars logos.png");
	logosStarWarsTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
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

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.05f, 0.3f,
		0.0f, -0.5f, -0.7f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.4f, 0.1f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	

	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,      
		1.0f, -0.3f, 0.0f,     
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,      
		0.0f, -1.0f, 0.0f,     
		1.0f, 0.05f, 0.02f,
		20.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,      
		0.0f, -0.4f, 1.0f,     
		1.0f, 0.05f, 0.02f,
		20.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
   //Loop mientras no se cierra la ventana
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		glm::mat4 modelRover = glm::mat4(1.0);
		modelRover = glm::translate(modelRover, glm::vec3(mainWindow.getmuevex(), 1.0f, -1.5f));
		ActualizarFaro(spotLights[0], modelRover, glm::vec3(2.5f, 1.0f, 0.0f), glm::vec3(1.0f, -0.3f, 0.0f));

		glm::mat4 modelAvion = glm::mat4(1.0);
		modelAvion = glm::translate(modelAvion, glm::vec3(0.0f, 5.0f + mainWindow.getavionY(), 6.0f + mainWindow.getavionZ()));
		ActualizarFaro(spotLights[1], modelAvion, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		glm::mat4 modelNave = glm::mat4(1.0);
		modelNave = glm::translate(modelNave, glm::vec3(-40.0f, 3.5f, -2.0f + mainWindow.getnaveZ()));
		ActualizarFaro(spotLights[2], modelNave, glm::vec3(0.0f, 0.5f, 2.0f), glm::vec3(0.0f, -0.4f, 1.0f));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		//BLACKHAWK
		model = modelAvion;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Rover: Sustituir por su propio Rover con jerarquía
		
		//Base del Rover
		model = modelRover;
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();//modificar por el modelo de solo cuerpo del Rover, para que se pueda separar el brazo y las llantas
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		//En sesi?n se separara una parte del modelo y se unir? por jeraqu?a al cuerpo

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
		2.- Hacer que al presionar una tecla cada pata de rueda pueda rotar un m?ximo de 45? "hacia adelante y hacia atr?s"
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

		//Dado
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 3.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWarsTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 3.5f, -2.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWarsTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


		//Nave
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelNave;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_S.RenderModel();
		


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
