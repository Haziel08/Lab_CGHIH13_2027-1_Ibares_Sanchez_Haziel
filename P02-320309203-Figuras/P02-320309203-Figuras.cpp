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
const float toRadians = 3.14159265f / 180.0; //grados a radianes
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
	Mesh* piramidetriangular = new Mesh();
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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
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
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 36);
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



void CreateShaders()
{
	static const char* fShaderShared = "shaders/shader.frag"; 

	Shader* shaderCafe = new Shader();     // 0
	shaderCafe->CreateFromFiles("shaders/shadercafe.vert", fShaderShared);
	shaderList.push_back(*shaderCafe);

	Shader* shaderMagenta = new Shader();  // 1
	shaderMagenta->CreateFromFiles("shaders/shadermagenta.vert", fShaderShared);
	shaderList.push_back(*shaderMagenta);

	Shader* shaderAmarillo = new Shader(); //2
	shaderAmarillo->CreateFromFiles("shaders/shaderamarillo.vert", fShaderShared);
	shaderList.push_back(*shaderAmarillo);

	Shader* shaderRojo = new Shader();  //3
	shaderRojo->CreateFromFiles("shaders/shaderrojo.vert", fShaderShared);
	shaderList.push_back(*shaderRojo);

	Shader* shaderVerde = new Shader();    //4
	shaderVerde->CreateFromFiles("shaders/shaderverde.vert", fShaderShared);
	shaderList.push_back(*shaderVerde);

	Shader* shaderAzul = new Shader();     //5
	shaderAzul->CreateFromFiles("shaders/shaderazul.vert", fShaderShared);
	shaderList.push_back(*shaderAzul);

	Shader* shaderNegro = new Shader();    //6
	shaderNegro->CreateFromFiles("shaders/shadernegro.vert", fShaderShared);
	shaderList.push_back(*shaderNegro);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
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
		angulo += 0.5f;



		// figura 1

		//Cubo cafe 1
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.90f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.7f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Cubo cafe 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.45f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.7f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Cubo negro piso
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, -1.0f));
		model = glm::scale(model, glm::vec3(1.95f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Triangulo amarillo (pirámides triangular)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.675f, 0.15f, -1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo rojo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.675f, -0.25f, -1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.675f, -0.65f, -1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Figura 2

		//Rombo azul es un cubo
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, -2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Rombo café tambien cubo
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, -1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Triangulo amarillo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.159f, -0.491f, -1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo rojo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.159f, -0.491f, -1.0f));
		model = glm::rotate(model, 315 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.159f, -0.809f, -1.0f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo magenta
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.159f, -0.809f, -1.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Figura 3

		//Triangulo magenta
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.6f, -0.55f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, -0.75f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo rojo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f, -0.75f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//Triangulo amarillo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.6f, -0.75f, -1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();


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