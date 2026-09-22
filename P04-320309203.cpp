/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(18, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 cada que haya jerarquia necesitas matriz auxiliar
	glm::mat4 modelaux2(1.0); //Si ha bifurcacion(rama) otra matriz necesaria
	glm::mat4 modelauxRuedaFZ(1.0); //Matriz para la rueda frontal
	glm::mat4 modelauxRuedaFZ2(1.0);
	glm::mat4 modelauxRuedaMZ(1.0);
	glm::mat4 modelauxRuedaTZ(1.0);
	glm::mat4 modelauxRuedaTZ2(1.0);
	glm::mat4 modelauxRosetta(1.0);
	glm::mat4 modelauxPanelIzq(1.0);
	glm::mat4 modelauxCubo(1.0);
	glm::mat4 modelauxPanelDer(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glm::mat4 modelauxRuedaMZ2(1.0);


		

		////articulacion1 hasta articulación5 sólo son puntos de rotación o articulación, en este caso no dibujaremos esferas que los representen
		//model = glm::mat4(1.0);
	
		//model = glm::translate(model, glm::vec3(0.0f, 4.0f, -4.0f)); //NOS POSICIONAMOS EN EL CENTRO DEL OBJETO X AMARILLA 
		//modelaux = model; //guardamos la matriz de modelo para que la base se mueva con el objeto
		//// Creando la cabina del rover ¿CUÁNTAS UNIDADES MEDIRÁ EN Z?
		//model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));//PARA LLEGAR AL CENTRO DE LA CABINA a PARTIR DEL ORIGEN
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		////meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		////sp.render(); //dibuja esfera


		//// BASE
		////model = glm::mat4(1.0); //Si dejamos está linea, la base no estará unida al centro del objeto. se debe comentar para que la base se mueva con el objeto.
		///*En su lugar usamos la matriz auxiliar modelaux para que la base se mueva con el objeto.
		//Lo que debemos de saber es: de las transformaciones geométricas que se aplican a la cabina y 
		//cuales queremos que se apliquen a la base. En este caso, sólo queremos que se aplique la traslación del origen, no la rotación ni el escalado de la cabina. 
		//Por lo tanto, debemos de guardar en modelaux sólo la traslación del origen y luego aplicarla a la base.
		//*/
		//model = modelaux;
		////NOS POSICIONAMOS EN EL CENTRO DE LA BASE
		//model = glm::translate(model, glm::vec3(-1.0f, -2.75f, 0.0f));
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();
		//
		//model = modelaux;
		//modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		///*
		//A PARTIR DE ESTE PUNTO HAY QUE ACOMODAR VARIOS ELEMENTOS:
		//LA BASE PARA LA LLANTA FRONTAL x2

		//LA BASE PARA EL BRAZO

		//LA BASE PARA LA LLANTA DE EN MEDIO x2

		//LA BASE PARA LA LLANTA DE ATRÁS x2

		//.........¿FALTA ALGO MÁS?
		//CÓMO ESTAREMOS REGRESANDO A ESTE PUNTO EN ESPECÍFICO VARIAS VECES, DEBEMOS DE USAR UNA NUEVA MATRIZ AUXILIAR PARA CADA UNO DE LOS ELEMENTOS 
		//QUE SE QUIERAN CONECTAR A LA BASE, YA QUE SI USAMOS LA MISMA MATRIZ AUXILIAR, SE SOBRESCRIBIRÁN LAS TRANSFORMACIONES GEOMÉTRICAS DE CADA ELEMENTO 
		//Y NO SE PODRÁN CONECTAR CORRECTAMENTE.

		//*/

		//

		////BASE PARA EL BRAZO
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-3.5f, 1.25f, 2.5f));
		////no hay scale
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();
		////articulación del  brazo que rota con teclado
		//model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////brazo 1
		//model = modelaux;
		//model = glm::rotate(model, glm::radians(45.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f));//rotar alrededor de z 45°
		//model = glm::translate(model, glm::vec3(0.0f, 2.3f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.5f, 4.0f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////articulación 2
		//model = modelaux;
		////articulación del  brazo que rota con teclado
		//model = glm::translate(model, glm::vec3(0.0f, 2.3f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////brazo 2
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(2.3f, 0.0f, 0.0f));
		//modelaux = model;
		//model = glm::rotate(model, glm::radians(-90.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f));//rotar alrededor de z -90°
		//model = glm::scale(model, glm::vec3(0.5f, 4.0f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////articulación 3
		//model = modelaux;
		////articulación del  brazo que rota con teclado
		//model = glm::translate(model, glm::vec3(2.3f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////brazo 3
		//model = modelaux;
		//model = glm::rotate(model, glm::radians(5.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f));//rotar alrededor de z 45°
		//model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.3f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////Pinza izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-0.3f, 0.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.15f, 0.6f, 0.3f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.85f, 0.23f, 0.7f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		////Pinza derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.3f, 0.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.15f, 0.6f, 0.3f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.85f, 0.23f, 0.7f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		////BASE PARA LA LLANTA FRONTAL +Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-4.0f, -0.1f, 4.4f));
		//modelauxRuedaFZ = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro 
		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.1f, 0.6f, 0.9f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		//modelauxRuedaFZ = model;

		//model = modelauxRuedaFZ;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.85f, 0.35f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
		//modelauxRuedaFZ = model;
		//model = modelauxRuedaFZ;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.6f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(0.0f, -1.4f, 0.0f));
		//modelauxRuedaFZ = model;
		//model = modelauxRuedaFZ;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.8f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.6f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.2f, 0.6f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.6f));
		//modelauxRuedaFZ = model;
		//model = modelauxRuedaFZ;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();


		//

		////BASE PARA LA LLANTA FRONTAL -Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-4.0f, -0.1f, -4.4f));
		//modelauxRuedaFZ2 = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.1f, 0.6f, 0.9f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		//modelauxRuedaFZ2 = model;

		////Cilindro
		//model = modelauxRuedaFZ2;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.85f, 0.35f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ2;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
		//modelauxRuedaFZ2 = model;
		//model = modelauxRuedaFZ2;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.6f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(0.0f, -1.4f, 0.0f));
		//modelauxRuedaFZ2 = model;
		//model = modelauxRuedaFZ2;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.8f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.6f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.2f, 0.6f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaFZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.6f));
		//modelauxRuedaFZ2 = model;
		//model = modelauxRuedaFZ2;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();



		////BASE PARA LA LLANTA DE EN MEDIO +Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(1.0f, -0.1f, 4.4f));
		//modelauxRuedaMZ = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();


		////Cilindro
		//model = modelauxRuedaMZ;
		//model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.3f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaMZ;
		//model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		//modelauxRuedaMZ = model;

		////Cilindro
		//model = modelauxRuedaMZ;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 5.5f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.6f, 0.2f, 0.8f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();


		//model = modelauxRuedaMZ;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -5.5f, 0.0f));
		//modelauxRuedaMZ = model;
		//model = modelauxRuedaMZ;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaMZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.7f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//
		//model = modelauxRuedaMZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.4f));
		//modelauxRuedaMZ = model;
		//model = modelauxRuedaMZ;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////BASE PARA LA LLANTA DE EN MEDIO -Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(1.0f, -0.1f, -4.4f));
		//modelauxRuedaMZ2 = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaMZ2;
		//model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.5f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaMZ2;
		//model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		//modelauxRuedaMZ2 = model;

		////Cilindro
		//model = modelauxRuedaMZ2;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 5.5f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.6f, 0.2f, 0.8f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaMZ2;
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -5.5f, 0.0f));
		//modelauxRuedaMZ2 = model;
		//model = modelauxRuedaMZ2;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaMZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.7f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaMZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.4f));
		//modelauxRuedaMZ2 = model;
		//model = modelauxRuedaMZ2;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////BASE PARA LA LLANTA TRASERA +Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(4.0f, -0.1f, 4.4f));
		//modelauxRuedaTZ = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaTZ;
		//model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.9f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaTZ;
		//model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.0f));
		//modelauxRuedaTZ = model;

		////Cilindro
		//model = modelauxRuedaTZ;
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 5.5f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.2f, 0.9f, 0.4f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaTZ;
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -5.5f, 0.0f));
		//modelauxRuedaTZ = model;
		//model = modelauxRuedaTZ;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.9f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaTZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.7f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaTZ;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.4f));
		//modelauxRuedaTZ = model;
		//model = modelauxRuedaTZ;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		////BASE PARA LA LLANTA TRASERA -Z
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(4.0f, -0.1f, -4.4f));
		//modelauxRuedaTZ2 = model;
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaTZ2;
		//model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.9f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();
		//model = modelauxRuedaTZ2;
		//model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.0f));
		//modelauxRuedaTZ2 = model;

		////Cilindro
		//model = modelauxRuedaTZ2;
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 5.5f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.2f, 0.9f, 0.4f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaTZ2;
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -5.5f, 0.0f));
		//modelauxRuedaTZ2 = model;
		//model = modelauxRuedaTZ2;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.9f, 0.9f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		////Cilindro
		//model = modelauxRuedaTZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.7f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 1.4f, 0.35f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxRuedaTZ2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.4f));
		//modelauxRuedaTZ2 = model;
		//model = modelauxRuedaTZ2;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()),
		//	glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.25f, 0.25f, 0.75f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();


		//SONDA ESPACIAL ROSETTA

		//Cuerpo central
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -4.0f));
		modelauxRosetta = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.55f, 0.45f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		modelauxCubo = modelauxRosetta;
		model = modelauxRosetta;
		model = glm::translate(model, glm::vec3(0.0f, -1.4f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxRosetta = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		//Cilindro
		model = modelauxRosetta;
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.6f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelauxRosetta;
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
		modelauxRosetta = model;
		model = modelauxRosetta;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Esfera
		model = modelauxRosetta;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxRosetta = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		//Cono
		model = modelauxRosetta;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 0.3f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.85f, 0.85f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry();

		//Cilindro
		model = modelauxRosetta;
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.6f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		//Esfera
		model = modelauxRosetta;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		//PANEL SOLAR 

		model = modelauxCubo;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxPanelIzq = model;
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(5.2f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.12f, 7.0f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		//Cilindro de abajo
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(5.2f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.12f, 7.0f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		//Panel 1
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 2
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(3.8f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.85f, 0.45f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel3
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(5.2f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.85f, 0.8f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 4
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(6.6f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.65f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 5
		model = modelauxPanelIzq;
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.2f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//PANEL SOLAR
		model = modelauxCubo;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxPanelDer = model;
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-5.2f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.12f, 7.0f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		//Cilindro de abajo
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-5.2f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.12f, 7.0f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		//Panel 1
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-2.4f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 2
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-3.8f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.85f, 0.45f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 3
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-5.2f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.85f, 0.8f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 4
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-6.6f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.65f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//Panel 5
		model = modelauxPanelDer;
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.2f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		