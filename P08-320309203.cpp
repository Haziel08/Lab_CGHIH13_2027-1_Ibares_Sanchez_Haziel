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
Model Holocron_M;

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

	Holocron_M = Model();
	Holocron_M.LoadModel("Models/HolocronTextura.obj");

	NaveMotores_M = Model();
	NaveMotores_M.LoadModel("Models/naveMotores.obj");


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
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// ------------------------------------------------------------------
	// Variables
	// ------------------------------------------------------------------
	GLfloat now = 0.0f;
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 modelRover(1.0);
	glm::mat4 modelAvion(1.0);
	glm::mat4 modelNave(1.0);

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
	glm::vec3 posNaveMotores = glm::vec3(14.0f, 4.5f, -14.0f);
	GLfloat anguloNave = 0.0f;       
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

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = (GLfloat)glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (GLfloat)((now - lastTime) / limitFPS);
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

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// ---------------- MATRICES PADRE ----------------
		modelRover = glm::mat4(1.0);
		modelRover = glm::translate(modelRover, glm::vec3(mainWindow.getmuevex(), 1.0f, -1.5f));

		modelAvion = glm::mat4(1.0);
		modelAvion = glm::translate(modelAvion, glm::vec3(0.0f, 5.0f + mainWindow.getavionY(), 6.0f + mainWindow.getavionZ()));

		modelNave = glm::mat4(1.0);
		modelNave = glm::translate(modelNave, glm::vec3(-40.0f, 3.5f, -2.0f + mainWindow.getnaveZ()));

		// jerarquia del brazo del rover
		modelBaseGarra = glm::translate(modelRover, glm::vec3(1.8f, 1.85f, -1.1f));
		modelBrazo1 = glm::translate(modelBaseGarra, glm::vec3(0.0f, 0.9f, 0.1f));
		modelBrazo2 = glm::translate(modelBrazo1, glm::vec3(2.8f, 2.9f, 0.2f));
		modelBasePinza = glm::translate(modelBrazo2, glm::vec3(-3.4f, 2.2f, 0.0f));
		modelGarra = glm::translate(modelBasePinza, glm::vec3(-0.4f, 0.3f, 0.1f));

		// ---------------- ACTUALIZAR LUCES ----------------
		modelNaveMotores = glm::mat4(1.0);
		modelNaveMotores = glm::translate(modelNaveMotores, posNaveMotores);
		modelNaveMotores = glm::rotate(modelNaveMotores, anguloNave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

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

		// Holocron importado de Blender
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, posHolocron);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

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

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}