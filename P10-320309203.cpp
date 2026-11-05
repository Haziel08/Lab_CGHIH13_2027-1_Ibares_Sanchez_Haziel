/*
Pr�ctica 7: Iluminaci�n 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <stdlib.h>   // rand() para el humo

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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;
SpotLight spotLightsHeli[6];
SpotLight spotLightNave;   // spotlight de la nave de motores

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexturepng;
Texture holocronTexture;
Texture logosStarWarsTexture;
Texture fuegoTexture;
Texture humoTexture;      // PRACTICA 10: humo de los motores

// ---------- PRACTICA 9: texturas animadas ----------
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0f;
// A�adir Texturas del dado y del holocron


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
Model LlantaFrente1_M;
Model LlantaFrente2_M;
Model LlantaMedia1_M;
Model LlantaMedia2_M;
Model LlantaTrasera1_M;
Model LlantaTrasera2_M;
Model Holocron_M;
Model HolocronBase_M;    // PRACTICA 10: holocron animado (base + 1 punta repetida en las 8 esquinas)
Model HolocronPunta_M;
Model Cohete_M;           // PRACTICA 10: cohete que despega

Model Nave_S;
Model NaveMotores_M;


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
// luces puntuales que se mandan al shader
PointLight pointLightsActivas[MAX_POINT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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



// PRACTICA 9: planos para texturas animadas
// meshListModel[6] = flecha, [7] = plano con todos los numeros, [8] = un solo numero
void CrearPlanosTextura()
{
	unsigned int planoIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,	0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,	0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,	0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	MeshModel* flecha = new MeshModel();
	flecha->CreateMeshModel(flechaVertices, planoIndices, 32, 6);
	meshListModel.push_back(flecha);		// indice 6

	MeshModel* score = new MeshModel();
	score->CreateMeshModel(scoreVertices, planoIndices, 32, 6);
	meshListModel.push_back(score);			// indice 7

	MeshModel* numero = new MeshModel();
	numero->CreateMeshModel(numeroVertices, planoIndices, 32, 6);
	meshListModel.push_back(numero);		// indice 8
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

// Reemplaza una luz puntual
void ActualizarLuzPuntual(PointLight& luz, glm::vec3 color, glm::vec3 pos)
{
	luz = PointLight(color.x, color.y, color.z,
		0.1f, 1.0f,
		pos.x, pos.y, pos.z,
		0.3f, 0.2f, 0.1f);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CrearPlanosTextura();
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

	fuegoTexture = Texture("Textures/fuegoAzul.png");
	fuegoTexture.LoadTextureA();
	humoTexture = Texture("Textures/humo.png");
	humoTexture.LoadTextureA();

	// PRACTICA 9
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


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

	// Llantas
	LlantaFrente1_M = Model();
	LlantaFrente1_M.LoadModel("Models/llanta_frente_1.obj");
	LlantaFrente2_M = Model();
	LlantaFrente2_M.LoadModel("Models/llanta_frente_2.obj");
	LlantaMedia1_M = Model();
	LlantaMedia1_M.LoadModel("Models/llanta_media_1.obj");
	LlantaMedia2_M = Model();
	LlantaMedia2_M.LoadModel("Models/llanta_media_2.obj");
	LlantaTrasera1_M = Model();
	LlantaTrasera1_M.LoadModel("Models/llanta_trasera_1.obj");
	LlantaTrasera2_M = Model();
	LlantaTrasera2_M.LoadModel("Models/llanta_trasera_3.obj");

	Nave_S = Model();
	Nave_S.LoadModel("Models/navesub.obj");

	Holocron_M = Model();
	Holocron_M.LoadModel("Models/HolocronTextura.obj");

	NaveMotores_M = Model();
	NaveMotores_M.LoadModel("Models/naveMotores.obj");

	HolocronBase_M = Model();
	HolocronBase_M.LoadModel("Models/HolocronFinal_Base.obj");
	HolocronPunta_M = Model();
	HolocronPunta_M.LoadModel("Models/HolocronFinal_Punta.obj");

	Cohete_M = Model();
	Cohete_M.LoadModel("Models/cohete.obj");


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


	//luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.05f, 0.3f,
		0.0f, -0.5f, -0.7f);

	// ------------------------------------------------------------------
	// LUCES PUNTUALES
	// ------------------------------------------------------------------
	unsigned int pointLightCount = 0;

	// Colores
	glm::vec3 colorLuzHolocron = glm::vec3(0.318f, 0.663f, 0.725f);
	glm::vec3 colorLuzPinza = glm::vec3(0.0f, 1.0f, 0.0f);

	// Posicion del Holocron importado 
	glm::vec3 posHolocron = glm::vec3(-15.0f, 3.5f, -2.0f);

	// Punta de la pinza
	glm::vec3 puntaPinza = glm::vec3(3.0f, 6.0f, 0.0f);
	glm::vec3 posPinza = glm::vec3(0.0f, 0.0f, 0.0f);

	pointLights[0] = PointLight(colorLuzHolocron.x, colorLuzHolocron.y, colorLuzHolocron.z,
		0.1f, 1.0f,
		posHolocron.x, posHolocron.y, posHolocron.z,
		0.3f, 0.2f, 0.1f);

	pointLights[1] = PointLight(colorLuzPinza.x, colorLuzPinza.y, colorLuzPinza.z,
		0.1f, 1.0f,
		posPinza.x, posPinza.y, posPinza.z,
		0.3f, 0.2f, 0.1f);

	// [2] luz puntual de la nave de motores
	// [3] luz azul del motor 1     [4] luz azul del motor 2
	// su posicion se actualiza cada cuadro en el while aqui solo se crean con su color
	glm::vec3 colorLuzNave = glm::vec3(1.0f, 0.9f, 0.7f);
	glm::vec3 colorLuzMotor = glm::vec3(0.2f, 0.4f, 1.0f);

	pointLights[2] = PointLight(colorLuzNave.x, colorLuzNave.y, colorLuzNave.z,
		0.1f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLights[3] = PointLight(colorLuzMotor.x, colorLuzMotor.y, colorLuzMotor.z,
		0.1f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLights[4] = PointLight(colorLuzMotor.x, colorLuzMotor.y, colorLuzMotor.z,
		0.1f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);

	// ------------------------------------------------------------------
	// LUCES SPOTLIGHT
	// spotLights[] es lo que se manda al shader:
	// [0] rover, [1] helicoptero, [2] nave sub,
	// [3] nave de motores
	// ------------------------------------------------------------------
	unsigned int spotLightCount = 0;

	// [0] faro del rover
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -0.3f, 0.0f,
		1.0f, 0.05f, 0.02f,
		20.0f);
	spotLightCount++;

	// [1] faro del helicoptero: un arreglo con una luz por color
	GLfloat coloresFaro[6][3] = {
		{1.0f, 0.0f, 0.0f},   // rojo
		{1.0f, 1.0f, 0.0f},   // amarillo
		{0.0f, 1.0f, 0.0f},   // verde
		{0.0f, 1.0f, 1.0f},   // cian
		{0.0f, 0.0f, 1.0f},   // azul
		{1.0f, 0.0f, 1.0f}    // magenta
	};
	for (int i = 0; i < 6; i++)
	{
		spotLightsHeli[i] = SpotLight(coloresFaro[i][0], coloresFaro[i][1], coloresFaro[i][2],
			0.0f, 2.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			1.0f, 0.05f, 0.02f,
			20.0f);
	}
	spotLights[1] = spotLightsHeli[0];
	spotLightCount++;

	// [2] faro de la nave sub
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -0.4f, 1.0f,
		1.0f, 0.05f, 0.02f,
		20.0f);
	spotLightCount++;

	// spotlight de la nave de motores
	spotLightNave = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.6f,
		1.0f, 0.05f, 0.02f,
		20.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	GLuint uniformTextureOffset = 0;   // PRACTICA 9: desplazamiento de textura
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// ------------------------------------------------------------------
	// Variables
	// ------------------------------------------------------------------
	GLfloat now = 0.0f;
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	// PRACTICA 9
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 posPanelP9 = glm::vec3(10.0f, 0.0f, -35.0f);  // mueve aqui todo el tablero de numeros/flechas
	int i = 0, j = 0;

	glm::mat4 modelRover(1.0);
	glm::mat4 modelAvion(1.0);
	glm::mat4 modelNave(1.0);

	// ---------- ANIMACION DEL ROVER ----------
	GLfloat posRover = 0.0f;          // posicion actual en X
	GLfloat dirRover = 1.0f;          // +1 avanza, -1 retrocede
	GLfloat velRover = 0.5f;          // canto avanza
	GLfloat limiteRover = 285.0f;     // limite del piso
	//Para hacer que el avance del rover sea de acuerdo al avance d elas llantas
	GLfloat radioLlanta = 1.241f;     // radio
	GLfloat giroLlantas = 0.0f;       // grados que han girado las llantas

	// ---------- ANIMACION DE LA NAVE ----------
	GLfloat alturaNave = 25.0f;       // altura
	GLfloat zInicioNave = -285.0f;    // Inicio
	GLfloat recorridoNave = 570.0f;   // de -285 a +285
	GLfloat zonaRollNave = 200.0f;    // coordenada donde girara para ponerse de nuevo boca abajo
	GLfloat velGiroNave = 9.0f;       // velocdiad de giro
	GLfloat factorMinNave = 0.35f;    // velocidad con la que se va deteneindo
	GLfloat derivaNave = 0.0f;        // cuanto se sigue de largo durante el backflip
	GLfloat zonaFrenadoNave = 80.0f;  // metros en los que frena antes del backflip y acelera despues
	GLfloat factorVelNave = 1.0f;     // 0.05 a 1: multiplica la velocidad (inercia)
	GLfloat tEnderezar = 0.0f;
	GLint faseNave = 0;               // 0 = avanza, 1 = backflip en el lugar
	bool enderezandoNave = false;     // true mientras se endereza despues del backflip
	GLfloat avanceNave = 0.0f;        // cuanto lleva recorrido en este tramo (0 a 570)
	GLfloat dirNave = 1.0f;           // +1 va hacia z+, -1 va hacia z-
	GLfloat velNave = 1.0f;           // velocidad d el anave recorrido
	GLfloat zNave = 0.0f;
	GLfloat yawNave = 0.0f;           // 0 mira a z+, 180 mira a z-
	GLfloat pitchNave = 0.0f;         // backflip
	GLfloat rollNave = 0.0f;          // giro sobre su eje para quedar derecha


	lastTime = (GLfloat)glfwGetTime();

	// jerarquia del brazo del rover
	// colocar la luz de la pinza y luego se reutiliza al dibujar
	glm::mat4 modelBaseGarra(1.0);
	glm::mat4 modelBrazo1(1.0);
	glm::mat4 modelBrazo2(1.0);
	glm::mat4 modelBasePinza(1.0);
	glm::mat4 modelGarra(1.0);

	// ------------------------------------------------------------------
	// Nave de motores
	// ------------------------------------------------------------------
	GLfloat escalaNave = 0.2f;
	glm::mat4 modelNaveMotores(1.0);

	// luces de la nave
	glm::vec3 posLuzNaveLocal = glm::vec3(0.0f, -1.3f, 0.5f);    // puntual
	glm::vec3 posSpotNaveLocal = glm::vec3(0.0f, -0.2f, 5.0f);   // spotlight
	glm::vec3 dirSpotNaveLocal = glm::vec3(0.0f, -1.0f, 0.6f);

	// centro de la llama de cada motor
	glm::vec3 llamaMotor1 = glm::vec3(-2.684f, 0.11f, -3.836f);
	glm::vec3 llamaMotor2 = glm::vec3(2.684f, 0.11f, -3.836f);
	GLfloat anchoLlama = 1.6f;
	GLfloat largoLlama = 3.0f;

	// posiciones de luces
	glm::vec3 posLuzNave = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 posLuzMotor1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 posLuzMotor2 = glm::vec3(0.0f, 0.0f, 0.0f);


	// PRACTICA 10: ACROBACIAS
	glm::vec3 posAcro = glm::vec3(-100.0f, 40.0f, -60.0f);   // donde empieza
	GLfloat yawAcro = 90.0f;           // 90 = mira hacia +X
	GLfloat pitchAcro = 0.0f;
	GLfloat rollAcro = 0.0f;
	GLint faseAcro = 0;
	GLfloat contAcro = 0.0f;           // distancia recorrida dentro de la fase
	GLfloat angAcro = 0.0f;            // grados girados en la vuelta en U
	GLfloat velAcro = 0.7f;            // velocidad
	GLfloat largoOndas = 100.0f;       // largo del vuelo ondulado
	GLfloat ampOndas = 25.0f;          // cuantos grados sube y baja la nariz en las ondas
	GLfloat largoRecta = 30.0f;
	GLfloat radioLoop = 25.0f;         // radio del loop
	GLfloat radioVuelta = 40.0f;       // radio de la vuelta en U
	GLfloat velBarril = 5.0f;          // grados por cuadro
	GLfloat kOnda = 4.0f * 3.14159265f / largoOndas;   // 2 ondas completas en el tramo
	GLfloat gradLoop = velAcro / radioLoop / toRadians;      // grados por cuadro del loop
	GLfloat gradVuelta = velAcro / radioVuelta / toRadians;  // grados por cuadro de la vuelta
	glm::vec3 adelanteAcro = glm::vec3(0.0f, 0.0f, 1.0f);

	// PRACTICA 10: HUMO
	const int MAX_HUMO = 320;
	GLfloat humoX[MAX_HUMO], humoY[MAX_HUMO], humoZ[MAX_HUMO];
	GLfloat humoEdad[MAX_HUMO];        // segundos que lleva viva
	bool humoViva[MAX_HUMO];
	GLfloat vidaHumo = 2.0f;           // segundos que dura
	GLfloat intervaloHumo = 0.03f;     // cada cuantos segundos nace una bolita por motor
	GLfloat tEmision = 0.0f;
	GLfloat dtSeg = 0.0f;              // tiempo real de este cuadro en segundos
	GLfloat tHumo = 0.0f, escalaHumo = 1.0f;
	int h = 0, motorH = 0;
	glm::vec3 posHumo = glm::vec3(0.0f);
	glm::mat4 vistaCam(1.0);
	glm::mat4 billboard(1.0);
	for (h = 0; h < MAX_HUMO; h++)
	{
		humoViva[h] = false;
		humoEdad[h] = 0.0f;
		humoX[h] = humoY[h] = humoZ[h] = 0.0f;
	}

	// PRACTICA 10: HOLOCRON ANIMADO

	const GLfloat HOLO_ESCALA = 4.0f;                 
	const GLfloat HOLO_DIST_PUNTA = 0.57f;            // distancia del centro a la que la punta queda pegada
	const GLfloat HOLO_SEPARACION = 0.55f;            // cuanto se separan las puntas
	const GLfloat HOLO_ALTURA = 8.0f;                 // cuanto se eleva
	const GLfloat DUR_ESPERA = 2.0f, DUR_SUBE = 2.5f, DUR_SEPARA = 1.5f, DUR_GIRO_PUNTAS = 4.0f, DUR_JUNTA = 1.5f, DUR_BAJA = 2.5f;
	const GLfloat VUELTAS_PUNTAS = 2.0f;               // vueltas que da cada punta sobre su eje
	GLfloat holoSueloY = -2.0f + 0.45f * HOLO_ESCALA;   // el piso esta en y = -2
	glm::vec3 holoPos = glm::vec3(-15.0f, holoSueloY, -2.0f);   // aqui queda en el suelo
	glm::vec3 holoCentro = holoPos;                   // centro actual
	GLfloat holoAltura = 0.0f, holoSep = 0.0f, holoGiro = 0.0f, holoTwist = 0.0f;
	GLfloat tHolo = 0.0f, pHolo = 0.0f, sHolo = 0.0f;
	GLint faseHolo = 0;
	GLint q = 0;
	// las 8 esquinas del cubo
	GLfloat esquinaS[8][3] = {
		{-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
		{ 1, -1, -1}, { 1, -1, 1}, { 1, 1, -1}, { 1, 1, 1} };
	glm::vec3 dirEsquina = glm::vec3(1.0f);
	glm::mat4 rotEsquina[8];                          // giro que lleva la punta de la esquina (-,-,-) a cada esquina
	glm::mat4 modelHolo(1.0);
	{
		glm::vec3 d0 = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
		glm::vec3 dq, ejeRot;
		GLfloat cosA = 0.0f;
		for (q = 0; q < 8; q++)
		{
			dq = glm::normalize(glm::vec3(esquinaS[q][0], esquinaS[q][1], esquinaS[q][2]));
			cosA = glm::dot(d0, dq);
			if (cosA > 0.999f)                        // la esquina, solo hay que girarla 60 grados
				rotEsquina[q] = glm::rotate(glm::mat4(1.0f), 60.0f * toRadians, dq);
			else if (cosA < -0.999f)                  // la esquina opuesta
				rotEsquina[q] = glm::rotate(glm::mat4(1.0f), 60.0f * toRadians, dq) *
				glm::rotate(glm::mat4(1.0f), 180.0f * toRadians, glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)));
			else
			{
				ejeRot = glm::normalize(glm::cross(d0, dq));
				rotEsquina[q] = glm::rotate(glm::mat4(1.0f), (GLfloat)acos(cosA), ejeRot);
			}
		}
	}

	
	// PRACTICA 10
	
	const GLfloat COHETE_ESCALA = 2.5f;               
	const GLfloat COHETE_ACEL = 12.0f;                // aceleracion
	const GLfloat COHETE_VMAX = 35.0f;                // velocidad maxima
	const GLfloat COHETE_ALTURA_GIRO = 60.0f;         // altura a la que empieza a inclinarse
	const GLfloat COHETE_VEL_INCLINA = 40.0f;         // grados por segundo que se inclina
	const GLfloat COHETE_VEL_ORBITA = 45.0f;          // grados por segundo que gira en la orbita
	const GLfloat COHETE_ALTURA_FIN = 250.0f;         // altura a la que sale de la escena
	
	glm::vec3 cohetePad = glm::vec3(50.0f, -2.0f + 2.2f * COHETE_ESCALA, -30.0f);   // plataforma
	glm::vec3 posCohete = cohetePad;
	glm::vec3 adelanteCohete = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat yawCohete = 90.0f;                        // 90 = hacia +X
	GLfloat pitchCohete = 90.0f;                      // 90 = nariz hacia arriba
	GLfloat rollCohete = 0.0f;
	GLfloat velCohete = 0.0f;
	GLfloat angOrbitaCohete = 0.0f;
	GLfloat tCohete = 0.0f;
	GLfloat largoLlamaCohete = 8.0f;
	GLint faseCohete = 0;
	glm::mat4 modelCohete(1.0);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = (GLfloat)glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (GLfloat)((now - lastTime) / limitFPS);
		dtSeg = now - lastTime;           // PRACTICA 10: segundos reales (para el humo)
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
		uniformTextureOffset = shaderList[0].getOffsetLocation();   // PRACTICA 9

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// ---------------- ANIMACION ROVER ----------------
		posRover += dirRover * velRover * deltaTime;
		if (posRover >= limiteRover)
		{
			posRover = limiteRover;
			dirRover = -1.0f;         // llego a la orilla y va para atras
		}
		if (posRover <= -limiteRover)
		{
			posRover = -limiteRover;
			dirRover = 1.0f;          // llego a la otra orilla y va pa adelante
		}
		// giro = distancia / radio el signo se invierte solo al retroceder
		giroLlantas = -posRover / radioLlanta / toRadians;

		// ---------------- ANIMACION NAVE ----------------
		if (faseNave == 0)
		{
			// avanza+
			// acelera suave al salir y frena suave al llegar
			factorVelNave = 1.0f;
			if (recorridoNave - avanceNave < zonaFrenadoNave)
				factorVelNave = factorMinNave + (1.0f - factorMinNave) * ((recorridoNave - avanceNave) / zonaFrenadoNave);
			if (avanceNave < zonaFrenadoNave && enderezandoNave)
				factorVelNave = 0.15f + 0.85f * (avanceNave / zonaFrenadoNave);
			if (factorVelNave < 0.05f)
				factorVelNave = 0.05f;
			avanceNave += velNave * factorVelNave * deltaTime;

			// despues del backflip viene boca abajo y se endereza mientras avanza un poco
			if (enderezandoNave)
			{
				tEnderezar = avanceNave / zonaRollNave;
				if (tEnderezar > 1.0f)
					tEnderezar = 1.0f;
				if (tEnderezar < 0.0f)
					tEnderezar = 0.0f;
				rollNave = 180.0f * (1.0f - tEnderezar * tEnderezar * (3.0f - 2.0f * tEnderezar));  //Gira para ponerse otra vez bien
				if (avanceNave >= zonaRollNave)
				{
					rollNave = 0.0f;
					enderezandoNave = false;
				}
			}

			// llego al extremo se detiene y hace el backflip en el lugar
			if (avanceNave >= recorridoNave)
			{
				avanceNave = recorridoNave;
				derivaNave = 0.0f;
				pitchNave = 0.0f;
				faseNave = 1;
			}
		}
		else
		{
			// backflip
			derivaNave += velNave * factorMinNave * (1.0f - pitchNave / 180.0f) * deltaTime;
			avanceNave = recorridoNave + derivaNave;
			pitchNave += velGiroNave * deltaTime;
			if (pitchNave >= 180.0f)
			{
				// ya quedo mirando al otro lado ahora avanza hacia alla, va boca abajo
				pitchNave = 0.0f;
				rollNave = 180.0f;                 // boca abajo
				yawNave = (yawNave == 0.0f) ? 180.0f : 0.0f;
				dirNave = -dirNave;
				avanceNave = -derivaNave;
				enderezandoNave = true;
				faseNave = 0;
			}
		}

		if (dirNave > 0.0f)
			zNave = zInicioNave + avanceNave;
		else
			zNave = zInicioNave + recorridoNave - avanceNave;

		// ---------------- ACROBACIAS DE LA NAV ----------------
		// 1) avanzar hacia donde apunta la nariz
		adelanteAcro = glm::vec3(cos(pitchAcro * toRadians) * sin(yawAcro * toRadians),
			sin(pitchAcro * toRadians),
			cos(pitchAcro * toRadians) * cos(yawAcro * toRadians));
		posAcro += adelanteAcro * velAcro * deltaTime;
		contAcro += velAcro * deltaTime;

		// 2) segun la fase, cambiar hacia donde apunta
		if (faseAcro == 0)                       // vuelo ondulado
		{
			pitchAcro = ampOndas * sin(kOnda * contAcro);
			rollAcro = 30.0f * sin(kOnda * contAcro);
			if (contAcro >= largoOndas)
			{
				pitchAcro = 0.0f;
				rollAcro = 0.0f;
				contAcro = 0.0f;
				faseAcro = 1;
			}
		}
		else if (faseAcro == 1)                  // loop
		{
			pitchAcro += gradLoop * deltaTime;
			if (pitchAcro >= 360.0f)
			{
				pitchAcro = 0.0f;
				contAcro = 0.0f;
				faseAcro = 2;
			}
		}
		else if (faseAcro == 2)                  // recta
		{
			if (contAcro >= largoRecta)
			{
				rollAcro = 0.0f;
				faseAcro = 3;
			}
		}
		else if (faseAcro == 3)                  // giro
		{
			rollAcro += velBarril * deltaTime;
			if (rollAcro >= 360.0f)
			{
				rollAcro = 0.0f;
				angAcro = 0.0f;
				faseAcro = 4;
			}
		}
		else                                     // vuelta en U
		{
			yawAcro += gradVuelta * deltaTime;
			angAcro += gradVuelta * deltaTime;
			rollAcro = -55.0f * sin(angAcro * toRadians);   // se inclina y se endereza solita
			if (angAcro >= 180.0f)
			{
				yawAcro -= (angAcro - 180.0f);   // para quedar exacto
				rollAcro = 0.0f;
				contAcro = 0.0f;
				faseAcro = 0;
			}
		}

		// ---------------- ANIMACION DEL HOLOCRON ----------------
		tHolo += dtSeg;
		pHolo = 0.0f;
		sHolo = 0.0f;
		if (faseHolo == 0)                         // armado en el suelo
		{
			if (tHolo >= DUR_ESPERA)
			{
				faseHolo = 1;
				tHolo = 0.0f;
			}
		}
		else if (faseHolo == 1)                    // se eleva
		{
			pHolo = tHolo / DUR_SUBE;
			if (pHolo > 1.0f)
				pHolo = 1.0f;
			sHolo = pHolo * pHolo * (3.0f - 2.0f * pHolo);   // arranca y frena suave
			holoAltura = HOLO_ALTURA * sHolo;
			if (pHolo >= 1.0f)
			{
				faseHolo = 2;
				tHolo = 0.0f;
			}
		}
		else if (faseHolo == 2)                    // las puntas se separan
		{
			pHolo = tHolo / DUR_SEPARA;
			if (pHolo > 1.0f)
				pHolo = 1.0f;
			sHolo = pHolo * pHolo * (3.0f - 2.0f * pHolo);
			holoSep = HOLO_SEPARACION * sHolo;
			if (pHolo >= 1.0f)
			{
				faseHolo = 3;
				tHolo = 0.0f;
			}
		}
		else if (faseHolo == 3)                    // cada punta gira sobre su propio eje
		{
			pHolo = tHolo / DUR_GIRO_PUNTAS;
			if (pHolo > 1.0f)
				pHolo = 1.0f;
			sHolo = pHolo * pHolo * (3.0f - 2.0f * pHolo);
			holoTwist = 360.0f * VUELTAS_PUNTAS * sHolo;   // arranca y frena suave
			if (pHolo >= 1.0f)
			{
				holoTwist = 0.0f;                  // son vueltas completas se ve igual
				faseHolo = 4;
				tHolo = 0.0f;
			}
		}
		else if (faseHolo == 4)                    // las puntas se juntan
		{
			pHolo = tHolo / DUR_JUNTA;
			if (pHolo > 1.0f)
				pHolo = 1.0f;
			sHolo = pHolo * pHolo * (3.0f - 2.0f * pHolo);
			holoSep = HOLO_SEPARACION * (1.0f - sHolo);
			if (pHolo >= 1.0f)
			{
				faseHolo = 5;
				tHolo = 0.0f;
			}
		}
		else                                       // baja al suelo y empieza otra vez
		{
			pHolo = tHolo / DUR_BAJA;
			if (pHolo > 1.0f)
				pHolo = 1.0f;
			sHolo = pHolo * pHolo * (3.0f - 2.0f * pHolo);
			holoAltura = HOLO_ALTURA * (1.0f - sHolo);
			if (pHolo >= 1.0f)
			{
				holoAltura = 0.0f;
				holoSep = 0.0f;
				faseHolo = 0;
				tHolo = 0.0f;
			}
		}
		// mientras esta en el aire gira
		if (faseHolo >= 1)
			holoGiro += 25.0f * dtSeg;
		holoCentro = holoPos + glm::vec3(0.0f, holoAltura, 0.0f);

		// ---------------- ANIMACION DEL COHETE ----------------
		tCohete += dtSeg;
		if (faseCohete == 0)                       //inicio tiembla
		{
			posCohete = cohetePad;
			posCohete.x += 0.03f * tCohete * sin(90.0f * tCohete);   // vibra
			if (tCohete >= 3.0f)
			{
				faseCohete = 1;
				tCohete = 0.0f;
				velCohete = 0.0f;
			}
		}
		else
		{
			// v = v + a*t, sin pasar de la velocidad maxima
			velCohete += COHETE_ACEL * dtSeg;
			if (velCohete > COHETE_VMAX)
				velCohete = COHETE_VMAX;
			// avanza hacia donde apunta
			adelanteCohete = glm::vec3(cos(pitchCohete * toRadians) * sin(yawCohete * toRadians),
				sin(pitchCohete * toRadians),
				cos(pitchCohete * toRadians) * cos(yawCohete * toRadians));
			posCohete += adelanteCohete * velCohete * dtSeg;

			if (faseCohete == 1)                   // despegue vertical
			{
				rollCohete += 90.0f * dtSeg;
				if (posCohete.y - cohetePad.y >= COHETE_ALTURA_GIRO)
					faseCohete = 2;
			}
			else if (faseCohete == 2)              // se inclina: de vertical a horizontal
			{
				rollCohete += 90.0f * dtSeg;
				pitchCohete -= COHETE_VEL_INCLINA * dtSeg;
				if (pitchCohete <= 0.0f)
				{
					pitchCohete = 0.0f;
					angOrbitaCohete = 0.0f;
					faseCohete = 3;
				}
			}
			else if (faseCohete == 3)              // orbita: una vuelta completa
			{
				yawCohete += COHETE_VEL_ORBITA * dtSeg;
				angOrbitaCohete += COHETE_VEL_ORBITA * dtSeg;
				if (angOrbitaCohete >= 360.0f)
					faseCohete = 4;
			}
			else if (faseCohete == 4)              // sube hasta salir de la escena
			{
				pitchCohete += COHETE_VEL_INCLINA * dtSeg;
				if (pitchCohete > 90.0f)
					pitchCohete = 90.0f;
				if (posCohete.y >= COHETE_ALTURA_FIN)
				{
					faseCohete = 5;
					tCohete = 0.0f;
				}
			}
			else                                   // fuera de la escena: espera y se reinicia
			{
				if (tCohete >= 3.0f)
				{
					posCohete = cohetePad;
					yawCohete = 90.0f;
					pitchCohete = 90.0f;
					rollCohete = 0.0f;
					velCohete = 0.0f;
					faseCohete = 0;
					tCohete = 0.0f;
				}
			}
		}
		modelCohete = glm::mat4(1.0);
		modelCohete = glm::translate(modelCohete, posCohete);
		modelCohete = glm::rotate(modelCohete, yawCohete * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelCohete = glm::rotate(modelCohete, -pitchCohete * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));   // negativo = nariz arriba

		// ---------------- MATRICES PADRE ----------------
		modelRover = glm::mat4(1.0);
		modelRover = glm::translate(modelRover, glm::vec3(posRover, 1.0f, -1.5f));

		modelAvion = glm::mat4(1.0);
		modelAvion = glm::translate(modelAvion, glm::vec3(0.0f, 5.0f + mainWindow.getavionY(), 6.0f + mainWindow.getavionZ()));

		modelNave = glm::mat4(1.0);
		modelNave = glm::translate(modelNave, glm::vec3(-40.0f, alturaNave, zNave));
		modelNave = glm::rotate(modelNave, yawNave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelNave = glm::rotate(modelNave, -pitchNave * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));   // negativo va hacia atras (backflip)
		modelNave = glm::rotate(modelNave, rollNave * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		// jerarquia del brazo del rover
		modelBaseGarra = glm::translate(modelRover, glm::vec3(1.8f, 1.85f, -1.1f));
		modelBrazo1 = glm::translate(modelBaseGarra, glm::vec3(0.0f, 0.9f, 0.1f));
		modelBrazo2 = glm::translate(modelBrazo1, glm::vec3(2.8f, 2.9f, 0.2f));
		modelBasePinza = glm::translate(modelBrazo2, glm::vec3(-3.4f, 2.2f, 0.0f));
		modelGarra = glm::translate(modelBasePinza, glm::vec3(-0.4f, 0.3f, 0.1f));

		// ---------------- ACTUALIZAR LUCES ----------------
		modelNaveMotores = glm::mat4(1.0);
		modelNaveMotores = glm::translate(modelNaveMotores, posAcro);
		modelNaveMotores = glm::rotate(modelNaveMotores, yawAcro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelNaveMotores = glm::rotate(modelNaveMotores, -pitchAcro * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));   // negativo = nariz arriba
		modelNaveMotores = glm::rotate(modelNaveMotores, rollAcro * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		// ---------------- HUMO ----------------
		for (h = 0; h < MAX_HUMO; h++)
		{
			if (humoViva[h])
			{
				humoEdad[h] += dtSeg;
				humoY[h] += 3.0f * dtSeg;            // el humo sube despacito
				if (humoEdad[h] >= vidaHumo)
					humoViva[h] = false;
			}
		}
		// cada cierto tiempo sale una bolita de cada motor encendido
		tEmision += dtSeg;
		if (tEmision >= intervaloHumo)
		{
			tEmision = 0.0f;
			for (motorH = 0; motorH < 2; motorH++)
			{
				if ((motorH == 0 && mainWindow.getmotor1()) || (motorH == 1 && mainWindow.getmotor2()))
				{
					if (motorH == 0)
						posHumo = glm::vec3(modelNaveMotores * glm::vec4(llamaMotor1 + glm::vec3(0.0f, 0.0f, -1.5f), 1.0f));
					else
						posHumo = glm::vec3(modelNaveMotores * glm::vec4(llamaMotor2 + glm::vec3(0.0f, 0.0f, -1.5f), 1.0f));

					for (h = 0; h < MAX_HUMO && humoViva[h]; h++) {}      // buscar una particula libre
					if (h < MAX_HUMO)
					{
						humoViva[h] = true;
						humoEdad[h] = 0.0f;
						humoX[h] = posHumo.x + ((rand() % 100) / 100.0f - 0.5f) * 0.6f;   // random
						humoY[h] = posHumo.y + ((rand() % 100) / 100.0f - 0.5f) * 0.6f;
						humoZ[h] = posHumo.z + ((rand() % 100) / 100.0f - 0.5f) * 0.6f;
					}
				}
			}
			// humo del cohete: sale de la cola mientras tiene los motores encendidos
			if (faseCohete >= 1 && faseCohete <= 4)
			{
				posHumo = glm::vec3(modelCohete * glm::vec4(0.0f, 0.0f, -COHETE_ESCALA * 2.4f - 1.0f, 1.0f));
				for (h = 0; h < MAX_HUMO && humoViva[h]; h++) {}
				if (h < MAX_HUMO)
				{
					humoViva[h] = true;
					humoEdad[h] = 0.0f;
					humoX[h] = posHumo.x + ((rand() % 100) / 100.0f - 0.5f) * 1.2f;
					humoY[h] = posHumo.y + ((rand() % 100) / 100.0f - 0.5f) * 1.2f;
					humoZ[h] = posHumo.z + ((rand() % 100) / 100.0f - 0.5f) * 1.2f;
				}
			}
		}

		// faro del helicoptero: solo se actualiza la luz del color activo y esa se manda al shader
		ActualizarFaro(spotLightsHeli[mainWindow.getcolorFaro()], modelAvion, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		spotLights[1] = spotLightsHeli[mainWindow.getcolorFaro()];

		// spotlightsy a sus vehiculos
		ActualizarFaro(spotLights[0], modelRover, glm::vec3(2.5f, 1.0f, 0.0f), glm::vec3(1.0f, -0.3f, 0.0f));
		ActualizarFaro(spotLights[2], modelNave, glm::vec3(0.0f, 0.5f, 2.0f), glm::vec3(0.0f, -0.4f, 1.0f));

		// spotlights
		spotLightCount = 3;
		if (mainWindow.getluzNave())
		{
			ActualizarFaro(spotLightNave, modelNaveMotores, posSpotNaveLocal, dirSpotNaveLocal);
			spotLights[3] = spotLightNave;
			spotLightCount++;
		}

		// luces puntuales
		pointLightCount = 0;
		if (mainWindow.getluzHolocron())
		{
			ActualizarLuzPuntual(pointLights[0], colorLuzHolocron, holoCentro);   // la luz azul sigue al holocron
			pointLightsActivas[pointLightCount] = pointLights[0];
			pointLightCount++;
		}
		if (mainWindow.getluzPinza())
		{
			posPinza = glm::vec3(modelGarra * glm::vec4(puntaPinza, 1.0f));
			ActualizarLuzPuntual(pointLights[1], colorLuzPinza, posPinza);
			pointLightsActivas[pointLightCount] = pointLights[1];
			pointLightCount++;
		}
		// luz puntual de la nave
		if (mainWindow.getluzNave())
		{
			posLuzNave = glm::vec3(modelNaveMotores * glm::vec4(posLuzNaveLocal, 1.0f));
			ActualizarLuzPuntual(pointLights[2], colorLuzNave, posLuzNave);
			pointLightsActivas[pointLightCount] = pointLights[2];
			pointLightCount++;
		}
		// luz azul de cada motor
		if (mainWindow.getluzNave() && mainWindow.getmotor1())
		{
			posLuzMotor1 = glm::vec3(modelNaveMotores * glm::vec4(llamaMotor1, 1.0f));
			ActualizarLuzPuntual(pointLights[3], colorLuzMotor, posLuzMotor1);
			pointLightsActivas[pointLightCount] = pointLights[3];
			pointLightCount++;
		}
		if (mainWindow.getluzNave() && mainWindow.getmotor2())
		{
			posLuzMotor2 = glm::vec3(modelNaveMotores * glm::vec4(llamaMotor2, 1.0f));
			ActualizarLuzPuntual(pointLights[4], colorLuzMotor, posLuzMotor2);
			pointLightsActivas[pointLightCount] = pointLights[4];
			pointLightCount++;
		}

		//informacion de fuentes de iluminacion
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLightsActivas, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// ---------------- DIBUJO ----------------
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);                              // PRACTICA 9: sin desplazamiento para todo lo demas
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

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

		//Base del Rover
		model = modelRover;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		// Base de la garra
		model = modelBaseGarra;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BaseGarra_M.RenderModel();
		color = glm::vec3(0.2f, 0.8f, 0.2f);

		//Brazo 1
		model = modelBrazo1;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo1_M.RenderModel();
		color = glm::vec3(0.8f, 0.2f, 0.8f);

		//Brazo 2
		model = modelBrazo2;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo2_M.RenderModel();
		color = glm::vec3(0.9f, 0.9f, 0.1f);

		//Base de la pinza
		model = modelBasePinza;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BasePinza_M.RenderModel();
		color = glm::vec3(0.9f, 0.1f, 0.1f);

		//Garra
		model = modelGarra;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Garra_M.RenderModel();
		modelaux = modelGarra;

		// Holocron animado
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelHolo = glm::translate(glm::mat4(1.0), holoCentro);
		modelHolo = glm::rotate(modelHolo, holoGiro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelHolo = glm::scale(modelHolo, glm::vec3(HOLO_ESCALA, HOLO_ESCALA, HOLO_ESCALA));

		// base
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHolo));
		HolocronBase_M.RenderModel();

		// las 8 puntas
		for (q = 0; q < 8; q++)
		{
			dirEsquina = glm::normalize(glm::vec3(esquinaS[q][0], esquinaS[q][1], esquinaS[q][2]));
			model = modelHolo;
			model = glm::translate(model, dirEsquina * (HOLO_DIST_PUNTA + holoSep));   // pegada a la base, o separada
			model = glm::rotate(model, holoTwist * toRadians, dirEsquina);             // gira sobre su propio eje hacia donde mira
			model = model * rotEsquina[q];                                             // mirando hacia afuera
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			HolocronPunta_M.RenderModel();
		}

		//Llanta delantera derecha
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.4f, -7.0f, -2.5f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion1(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaFrente1_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(3.622f, -2.746f, -0.044f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaFrente1_M.RenderModel();

		//Llanta delantera izquierda
		color = glm::vec3(0.15f, 0.15f, 0.15f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.4f, -7.0f, 3.6f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion2(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaFrente2_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(3.541f, -2.718f, 0.042f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaFrente2_M.RenderModel();

		//Llanta media derecha
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, -2.2f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion3(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaMedia1_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(2.210f, -1.855f, -0.381f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaMedia1_M.RenderModel();

		//Llanta media izquierda
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, 3.6f));
		model = glm::rotate(model, glm::radians(limitarRotacion(mainWindow.getarticulacion4(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaMedia2_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(2.166f, -1.959f, 0.262f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaMedia2_M.RenderModel();

		//Llanta trasera derecha
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, -2.2f));
		model = glm::rotate(model, glm::radians(-limitarRotacion(mainWindow.getarticulacion5(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaTrasera1_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(-2.281f, -1.848f, -0.238f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTrasera1_M.RenderModel();

		//Llanta trasera izquierda
		color = glm::vec3(0.35f, 0.35f, 0.35f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -8.0f, 3.6f));
		model = glm::rotate(model, glm::radians(-limitarRotacion(mainWindow.getarticulacion6(), 45.0f)),
			glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RuedaTrasera2_M.RenderModel();

		// llanta
		model = glm::translate(model, glm::vec3(-2.251f, -1.863f, 0.355f));
		model = glm::rotate(model, giroLlantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // la llanta rueda sobre su eje
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTrasera2_M.RenderModel();

		//Dado
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 3.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWarsTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron (hecho con vertices)
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 3.5f, -2.0f));
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

		//Nave de motores
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelNaveMotores;
		model = glm::scale(model, glm::vec3(escalaNave, escalaNave, escalaNave));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		NaveMotores_M.RenderModel();

		//Cohete
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = modelCohete;
		model = glm::rotate(model, rollCohete * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // gira sobre su propio eje
		model = glm::scale(model, glm::vec3(COHETE_ESCALA, COHETE_ESCALA, COHETE_ESCALA));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cohete_M.RenderModel();

		//Agave (se renderiza al final por la transparencia)
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

		//Fuego azul de los motores
		fuegoTexture.UseTexture();
		color = glm::vec3(2.0f, 2.0f, 2.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		if (mainWindow.getmotor1())
		{
			model = modelNaveMotores;
			model = glm::translate(model, llamaMotor1);
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(anchoLlama, largoLlama, anchoLlama));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshListModel[3]->RenderMeshModel();
		}
		if (mainWindow.getmotor2())
		{
			model = modelNaveMotores;
			model = glm::translate(model, llamaMotor2);
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(anchoLlama, largoLlama, anchoLlama));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshListModel[3]->RenderMeshModel();
		}

		// Fuego del cohete
		if (faseCohete >= 1 && faseCohete <= 4)
		{
			largoLlamaCohete = 8.0f + 1.5f * sin(40.0f * now);
			fuegoTexture.UseTexture();
			color = glm::vec3(2.0f, 2.0f, 2.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			model = modelCohete;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -COHETE_ESCALA * 2.2f - largoLlamaCohete * 0.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, largoLlamaCohete, 4.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshListModel[3]->RenderMeshModel();
		}

		// =============== PRACTICA 10 ===============
		vistaCam = camera.calculateViewMatrix();
		billboard = glm::transpose(glm::mat4(glm::mat3(vistaCam)));
		humoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		color = glm::vec3(1.6f, 1.6f, 1.6f);                  // brillo del humo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glDepthMask(GL_FALSE);
		for (h = 0; h < MAX_HUMO; h++)
		{
			if (humoViva[h])
			{
				tHumo = humoEdad[h] / vidaHumo;               // 0 = mace el humo, 1 = a punto de desaparecer
				escalaHumo = 1.5f + 5.5f * tHumo;             // va creciendo
				if (tHumo > 0.6f)
					escalaHumo *= (1.0f - tHumo) / 0.4f;      // en el ultimo tramo se encoge hasta desaparecer
				model = glm::translate(glm::mat4(1.0), glm::vec3(humoX[h], humoY[h], humoZ[h]));
				model = model * billboard;
				model = glm::rotate(model, humoEdad[h] * 40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));   // gira lento
				model = glm::scale(model, glm::vec3(escalaHumo, escalaHumo, escalaHumo));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshListModel[3]->RenderMeshModel();
			}
		}
		glDepthMask(GL_TRUE);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// =============== FIN PRACTICA 10 ===============

		// =============== PRACTICA 9: TEXTURAS ANIMADAS ===============
		// (se dibuja con blending activo; toda posicion es relativa a posPanelP9)

		// Flecha con textura en movimiento
		toffsetflechau += 0.001f;
		toffsetflechav = 0.000f;
		if (toffsetflechau > 1.0f)
			toffsetflechau = 0.0f;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, posPanelP9 + glm::vec3(-2.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[6]->RenderMeshModel();

		// Plano con todos los numeros
		toffsetnumerou = 0.0f;
		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, posPanelP9 + glm::vec3(-6.0f, 2.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[7]->RenderMeshModel();

		// Numero 1
		model = glm::mat4(1.0);
		model = glm::translate(model, posPanelP9 + glm::vec3(-10.0f, 2.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[8]->RenderMeshModel();

		// Numeros 2-4 (se corre la textura 0.25 en u)
		for (i = 1; i < 4; i++)
		{
			toffsetnumerou += 0.25f;
			toffsetnumerov = 0.0f;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, posPanelP9 + glm::vec3(-10.0f - (i * 3.0f), 2.0f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshListModel[8]->RenderMeshModel();
		}

		// Numeros 5-8 (segunda fila de la textura, v = -0.33)
		for (j = 1; j < 5; j++)
		{
			toffsetnumerou += 0.25f;
			toffsetnumerov = -0.33f;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, posPanelP9 + glm::vec3(-7.0f - (j * 3.0f), 5.0f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshListModel[8]->RenderMeshModel();
		}

		// Numero cambiante
		// PREGUNTA DE LA PRACTICA: como hacer que sea a una velocidad visible?
		toffsetnumerocambiau += 0.25f;
		if (toffsetnumerocambiau > 1.0f)
			toffsetnumerocambiau = 0.0f;
		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, posPanelP9 + glm::vec3(-10.0f, 10.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[8]->RenderMeshModel();

		// Cambiar automaticamente entre textura numero 1 y numero 2
		toffsetnumerou = 0.0f;
		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, posPanelP9 + glm::vec3(-13.0f, 10.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		// TODO clase: alternar Numero1Texture / Numero2Texture con una bandera o un contador
		//Numero2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[7]->RenderMeshModel();

		// dejar el desplazamiento y el color como estaban
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// =============== FIN PRACTICA 9 ===============

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}