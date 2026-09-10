//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
//DATOS QUE NOSOTROS DECLAREMOS SE MANDA A LOS UNIFORM
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		2,3,0

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	//0
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,36);
	meshColorList.push_back(letras);



	GLfloat vertices_triangulomagenta[] = { //1
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.502f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.502f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.502f,	0.0f,	1.0f,
		
	};

	MeshColor* triangulomagenta = new MeshColor();
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);

	GLfloat vertices_trianguloamarillo[] = {//2
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,

	};

	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	GLfloat vertices_triangulorojo[] = { //3
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_trianguloverde[] = { //4
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoazul[] = { //5
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	GLfloat vertices_cuadradocafe[] = { //6
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_cuadradonegro[] = {//7
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradonegro = new MeshColor();
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro);

}

void CrearIniciales()
{
	//LETRA H
	GLfloat vertices_H[] = {
		//H izquierda
		-0.9f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.9f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.7f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.9f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.7f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.7f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		//H centro
		-0.7f,  0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.4f,  0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.4f, -0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.7f,  0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.7f, -0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.4f, -0.1f, 0.0f,		0.75f, 1.0f, 0.0f,
		//H derecha
		-0.4f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.2f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.4f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.2f,  0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.4f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
		-0.2f, -0.9f, 0.0f,		0.75f, 1.0f, 0.0f,
	};
	MeshColor* letraH = new MeshColor();
	letraH->CreateMeshColor(vertices_H, 108);
	meshColorList.push_back(letraH);

	//LETRA I
	GLfloat vertices_I[] = {
		//I arriba
		-0.1f, 0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		 0.4f, 0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		 0.4f, 0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		-0.1f, 0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		-0.1f, 0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		 0.4f, 0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		 //I abajo
		 -0.1f, -0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		  0.4f, -0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		  0.4f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		 -0.1f, -0.9f, 0.0f,		0.25f, 0.878f, 0.816f,
		 -0.1f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		  0.4f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		  //I centro
		   0.1f,  0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		   0.2f,  0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		   0.2f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		   0.1f,  0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		   0.2f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
		   0.1f, -0.7f, 0.0f,		0.25f, 0.878f, 0.816f,
	};
	MeshColor* letraI = new MeshColor();
	letraI->CreateMeshColor(vertices_I, 108);
	meshColorList.push_back(letraI);

	//LETRA S
	GLfloat vertices_S[] = {
		//S arriba
		0.5f, 0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, 0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, 0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, 0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, 0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, 0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		//S medio 1
		0.5f,  0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.6f,  0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.6f,  0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.6f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		//S medio 2
		0.6f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.6f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.6f, -0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		//S medio 3
		0.8f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.8f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f,  0.1f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.8f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		//S abajo
		0.5f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, -0.9f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.5f, -0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
		0.9f, -0.7f, 0.0f,		0.616f, 0.0f, 1.0f,
	};
	MeshColor* letraS = new MeshColor();
	letraS->CreateMeshColor(vertices_S, 180);
	meshColorList.push_back(letraS);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CrearIniciales();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Letra H
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[8]->RenderMeshColor();

		//Letra I
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[9]->RenderMeshColor();

		//Letra S
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[10]->RenderMeshColor();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
