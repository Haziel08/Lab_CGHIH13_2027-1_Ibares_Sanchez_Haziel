#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <string>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"

//para iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
const float PI_F = 3.14159265f;

Window mainWindow;
std::vector<Shader> shaderList;
Camera camera;

//---------------- Texturas ----------------
Texture plainTexture;

//---------------- Modelos ----------------
Model pista_carreras;
Model nave_seamoth;
Model satelite_cuerpo;
Model satelite_plato;
Model satelite_brazoA;
Model satelite_brazoB;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional (solo 1 y siempre debe existir)
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>> Pista y trayectoria de la nave >>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Puntos (x, y, z) de la linea central de la pista, tomados de trayectoria_centro.csv (uno cada 4 unidades).
//El circuito es cerrado: despues del ultimo punto vuelve al primero.
const int PISTA_NUM_PUNTOS = 1060;
const float PISTA_PASO = 4.0f;
const float PISTA_PUNTOS[] = {
	0.00f,0.00f,0.00f, -4.00f,0.00f,0.00f, -8.00f,0.00f,0.00f, -12.00f,0.00f,0.00f,
	-16.00f,0.00f,0.00f, -20.00f,0.00f,0.00f, -24.00f,0.00f,0.00f, -28.00f,0.00f,0.00f,
	-32.00f,0.00f,0.00f, -36.00f,0.00f,0.00f, -40.00f,0.00f,0.00f, -44.00f,0.00f,0.00f,
	-48.00f,0.00f,0.00f, -52.00f,0.00f,0.00f, -56.00f,0.00f,0.00f, -60.00f,0.00f,0.00f,
	-64.00f,0.00f,0.00f, -68.00f,0.00f,0.00f, -72.00f,0.00f,0.00f, -76.00f,0.00f,0.00f,
	-80.00f,0.00f,0.00f, -82.83f,2.83f,0.00f, -85.66f,5.66f,0.00f, -88.49f,8.49f,0.00f,
	-91.31f,11.31f,0.00f, -94.14f,14.14f,0.00f, -96.97f,16.97f,0.00f, -99.80f,19.80f,0.00f,
	-102.63f,22.63f,0.00f, -105.46f,25.46f,0.00f, -108.28f,28.28f,0.00f, -112.28f,28.28f,0.00f,
	-116.28f,28.28f,0.00f, -120.28f,28.28f,0.00f, -124.28f,28.28f,0.00f, -128.28f,28.28f,0.00f,
	-132.28f,28.28f,0.00f, -136.28f,28.28f,0.00f, -140.28f,28.28f,0.00f, -144.28f,28.28f,0.00f,
	-148.28f,28.28f,0.00f, -151.11f,31.11f,0.00f, -153.94f,33.94f,0.00f, -156.77f,36.77f,0.00f,
	-159.60f,39.60f,0.00f, -162.43f,42.43f,0.00f, -165.25f,45.25f,0.00f, -168.08f,48.08f,0.00f,
	-170.91f,50.91f,0.00f, -173.74f,53.74f,0.00f, -176.57f,56.57f,0.00f, -180.57f,56.57f,0.00f,
	-184.57f,56.57f,0.00f, -188.57f,56.57f,0.00f, -192.57f,56.57f,0.00f, -196.57f,56.57f,0.00f,
	-200.57f,56.57f,0.00f, -204.57f,56.57f,0.00f, -208.57f,56.57f,0.00f, -212.57f,56.57f,0.00f,
	-216.57f,56.57f,0.00f, -220.57f,56.57f,0.00f, -224.57f,56.57f,0.00f, -228.57f,56.57f,0.00f,
	-232.57f,56.57f,0.00f, -236.57f,56.57f,0.00f, -240.57f,56.57f,0.00f, -244.57f,56.57f,0.00f,
	-248.57f,56.57f,0.00f, -252.57f,56.57f,0.00f, -256.57f,56.57f,0.00f, -259.40f,53.74f,0.00f,
	-262.23f,50.91f,0.00f, -265.05f,48.08f,0.00f, -267.88f,45.25f,0.00f, -270.71f,42.43f,0.00f,
	-273.54f,39.60f,0.00f, -276.37f,36.77f,0.00f, -279.20f,33.94f,0.00f, -282.02f,31.11f,0.00f,
	-284.85f,28.28f,0.00f, -288.85f,28.28f,0.00f, -292.85f,28.28f,0.00f, -296.85f,28.28f,0.00f,
	-300.85f,28.28f,0.00f, -304.85f,28.28f,0.00f, -308.85f,28.28f,0.00f, -312.85f,28.28f,0.00f,
	-316.85f,28.28f,0.00f, -320.85f,28.28f,0.00f, -324.85f,28.28f,0.00f, -327.68f,25.46f,0.00f,
	-330.51f,22.63f,0.00f, -333.34f,19.80f,0.00f, -336.17f,16.97f,0.00f, -339.00f,14.14f,0.00f,
	-341.82f,11.31f,0.00f, -344.65f,8.49f,0.00f, -347.48f,5.66f,0.00f, -350.31f,2.83f,0.00f,
	-353.14f,0.00f,0.00f, -357.14f,0.00f,0.00f, -361.14f,0.00f,0.00f, -365.14f,0.00f,0.00f,
	-369.14f,0.00f,0.00f, -373.14f,0.00f,0.00f, -377.14f,0.00f,0.00f, -381.14f,0.00f,0.00f,
	-385.14f,0.00f,0.00f, -389.14f,0.00f,0.00f, -393.14f,0.00f,0.00f, -397.14f,0.00f,0.00f,
	-401.14f,0.00f,0.00f, -405.14f,0.00f,0.00f, -409.14f,0.00f,0.00f, -413.14f,0.00f,0.00f,
	-417.14f,0.00f,0.00f, -421.14f,0.00f,0.00f, -425.14f,0.00f,0.00f, -429.14f,0.00f,0.00f,
	-433.14f,0.00f,0.00f, -437.14f,0.00f,0.00f, -441.14f,0.00f,0.00f, -445.14f,0.00f,0.00f,
	-449.14f,0.00f,0.00f, -453.14f,0.00f,0.00f, -457.14f,0.00f,0.00f, -461.14f,0.00f,0.00f,
	-465.14f,0.00f,0.00f, -469.14f,0.00f,0.00f, -473.14f,0.00f,0.00f, -477.14f,0.00f,0.00f,
	-481.14f,0.00f,0.00f, -485.14f,0.00f,0.00f, -489.14f,0.00f,0.00f, -493.14f,0.00f,0.00f,
	-497.14f,0.00f,0.00f, -501.14f,0.00f,0.00f, -505.14f,0.00f,0.00f, -509.14f,0.00f,0.00f,
	-513.14f,0.00f,0.00f, -517.14f,0.00f,0.00f, -521.14f,0.00f,0.00f, -525.13f,0.00f,0.13f,
	-529.10f,0.00f,0.67f, -532.98f,0.00f,1.62f, -536.74f,0.00f,2.98f, -540.34f,0.00f,4.72f,
	-543.73f,0.00f,6.83f, -546.89f,0.00f,9.28f, -549.77f,0.00f,12.06f, -552.34f,0.00f,15.12f,
	-554.58f,0.00f,18.43f, -556.45f,0.00f,21.97f, -557.95f,0.00f,25.67f, -559.04f,0.00f,29.52f,
	-559.73f,0.00f,33.46f, -560.00f,0.00f,37.45f, -560.00f,0.00f,41.45f, -560.00f,0.00f,45.45f,
	-560.00f,0.00f,49.45f, -560.00f,0.00f,53.45f, -560.00f,0.00f,57.45f, -560.00f,0.00f,61.45f,
	-560.00f,0.00f,65.45f, -560.00f,0.00f,69.45f, -560.00f,0.00f,73.45f, -560.00f,0.00f,77.45f,
	-560.00f,0.00f,81.45f, -560.00f,0.00f,85.45f, -560.00f,0.00f,89.45f, -560.00f,0.00f,93.45f,
	-560.00f,0.00f,97.45f, -560.00f,0.00f,101.45f, -560.00f,0.00f,105.45f, -560.00f,0.00f,109.45f,
	-560.00f,0.00f,113.45f, -560.00f,0.00f,117.45f, -560.00f,0.00f,121.45f, -559.84f,0.00f,125.44f,
	-559.27f,0.00f,129.40f, -558.29f,0.00f,133.27f, -556.90f,0.00f,137.02f, -555.13f,0.00f,140.61f,
	-552.99f,0.00f,143.99f, -550.51f,0.00f,147.12f, -547.71f,0.00f,149.98f, -544.63f,0.00f,152.53f,
	-541.30f,0.00f,154.73f, -537.75f,0.00f,156.58f, -534.03f,0.00f,158.04f, -530.18f,0.00f,159.11f,
	-526.23f,0.00f,159.76f, -522.24f,0.00f,160.00f, -518.24f,0.00f,160.00f, -514.24f,0.00f,160.00f,
	-510.24f,0.00f,160.00f, -506.24f,0.00f,160.00f, -502.24f,0.00f,160.00f, -498.24f,0.00f,160.00f,
	-494.24f,0.00f,160.00f, -490.24f,0.00f,160.00f, -486.24f,0.00f,160.00f, -482.24f,0.00f,160.00f,
	-478.24f,0.00f,160.00f, -474.25f,0.00f,160.19f, -470.30f,0.00f,160.79f, -466.43f,0.00f,161.80f,
	-462.69f,0.00f,163.22f, -459.12f,0.00f,165.02f, -455.76f,0.00f,167.19f, -452.65f,0.00f,169.69f,
	-449.81f,0.00f,172.51f, -447.29f,0.00f,175.62f, -445.11f,0.00f,178.97f, -443.29f,0.00f,182.53f,
	-441.86f,0.00f,186.26f, -440.83f,0.00f,190.12f, -440.20f,0.00f,194.07f, -440.00f,0.00f,198.07f,
	-440.00f,0.00f,202.07f, -440.00f,0.00f,206.07f, -440.00f,0.00f,210.07f, -440.00f,0.00f,214.07f,
	-440.00f,0.00f,218.07f, -440.00f,0.00f,222.07f, -440.00f,0.00f,226.07f, -440.00f,0.00f,230.07f,
	-440.00f,0.00f,234.07f, -440.00f,0.00f,238.07f, -440.00f,0.00f,242.07f, -440.00f,0.00f,246.07f,
	-440.00f,0.00f,250.07f, -440.00f,0.00f,254.07f, -440.00f,0.00f,258.07f, -440.00f,0.00f,262.07f,
	-440.00f,0.00f,266.07f, -440.00f,0.00f,270.07f, -440.00f,0.00f,274.07f, -440.00f,0.00f,278.07f,
	-440.00f,0.00f,282.07f, -440.00f,0.00f,286.07f, -440.00f,0.00f,290.07f, -440.00f,0.00f,294.07f,
	-440.00f,0.00f,298.07f, -440.00f,0.00f,302.07f, -440.22f,0.00f,306.06f, -440.85f,0.00f,310.01f,
	-441.90f,0.00f,313.86f, -443.35f,0.00f,317.59f, -445.18f,0.00f,321.15f, -447.37f,0.00f,324.49f,
	-449.90f,0.00f,327.58f, -452.75f,0.00f,330.39f, -455.87f,0.00f,332.89f, -459.24f,0.00f,335.04f,
	-462.81f,0.00f,336.83f, -466.56f,0.00f,338.24f, -470.43f,0.00f,339.24f, -474.38f,0.00f,339.83f,
	-478.38f,0.00f,340.00f, -482.38f,0.00f,340.00f, -486.38f,0.00f,340.00f, -490.38f,0.00f,340.00f,
	-494.38f,0.00f,340.00f, -498.38f,0.00f,340.00f, -502.38f,0.00f,340.00f, -506.38f,0.00f,340.00f,
	-510.38f,0.00f,340.00f, -514.38f,0.00f,340.00f, -518.38f,0.00f,340.00f, -522.38f,0.00f,340.00f,
	-526.37f,0.00f,340.25f, -530.31f,0.00f,340.92f, -534.16f,0.00f,342.00f, -537.87f,0.00f,343.47f,
	-541.41f,0.00f,345.33f, -544.74f,0.00f,347.55f, -547.81f,0.00f,350.11f, -550.60f,0.00f,352.98f,
	-553.07f,0.00f,356.12f, -555.20f,0.00f,359.51f, -556.96f,0.00f,363.10f, -558.33f,0.00f,366.85f,
	-559.30f,0.00f,370.73f, -559.86f,0.00f,374.69f, -560.00f,0.00f,378.69f, -560.00f,0.00f,382.69f,
	-560.00f,0.00f,386.69f, -560.00f,0.00f,390.69f, -560.00f,0.00f,394.69f, -560.00f,0.00f,398.69f,
	-560.00f,0.00f,402.69f, -560.00f,0.00f,406.69f, -560.00f,0.00f,410.69f, -560.00f,0.00f,414.69f,
	-560.00f,0.00f,418.69f, -560.00f,0.00f,422.69f, -560.00f,0.00f,426.69f, -560.00f,0.00f,430.69f,
	-560.00f,0.00f,434.69f, -560.00f,0.00f,438.69f, -560.00f,0.00f,442.69f, -560.00f,0.00f,446.69f,
	-560.00f,0.00f,450.69f, -560.00f,0.00f,454.69f, -560.00f,0.00f,458.69f, -560.00f,0.00f,462.69f,
	-560.00f,0.00f,466.69f, -560.00f,0.00f,470.69f, -560.00f,0.00f,474.69f, -560.00f,0.00f,478.69f,
	-560.00f,0.00f,482.69f, -560.00f,0.00f,486.69f, -560.00f,0.00f,490.69f, -560.00f,0.00f,494.69f,
	-560.00f,0.00f,498.69f, -560.00f,0.00f,502.69f, -560.00f,0.00f,506.69f, -560.00f,0.00f,510.69f,
	-560.00f,0.00f,514.69f, -560.00f,0.00f,518.69f, -559.99f,0.00f,522.69f, -559.71f,0.00f,526.67f,
	-559.01f,0.00f,530.61f, -557.90f,0.00f,534.45f, -556.40f,0.00f,538.15f, -554.51f,0.00f,541.68f,
	-552.26f,0.00f,544.99f, -549.68f,0.00f,548.04f, -546.79f,0.00f,550.80f, -543.62f,0.00f,553.25f,
	-540.22f,0.00f,555.35f, -536.62f,0.00f,557.08f, -532.85f,0.00f,558.42f, -528.96f,0.00f,559.36f,
	-525.00f,0.00f,559.88f, -521.00f,0.00f,560.00f, -517.00f,0.00f,560.00f, -513.00f,0.00f,560.00f,
	-509.00f,0.00f,560.00f, -505.00f,0.00f,560.00f, -501.00f,0.00f,560.00f, -497.00f,0.00f,560.00f,
	-493.00f,0.00f,560.00f, -489.00f,0.00f,560.00f, -485.00f,0.00f,560.00f, -481.00f,0.00f,560.00f,
	-477.00f,0.00f,560.00f, -473.00f,0.00f,560.00f, -469.00f,0.00f,560.00f, -465.00f,0.00f,560.00f,
	-461.00f,0.00f,560.00f, -457.00f,0.00f,560.00f, -453.00f,0.00f,560.00f, -449.00f,0.00f,560.00f,
	-445.00f,0.00f,560.00f, -441.00f,0.00f,560.00f, -437.00f,0.00f,559.99f, -433.02f,0.00f,559.67f,
	-429.09f,0.00f,558.94f, -425.26f,0.00f,557.80f, -421.57f,0.00f,556.26f, -418.06f,0.00f,554.35f,
	-414.77f,0.00f,552.07f, -411.74f,0.00f,549.46f, -409.00f,0.00f,546.55f, -406.58f,0.00f,543.37f,
	-404.51f,0.00f,539.95f, -402.81f,0.00f,536.33f, -401.50f,0.00f,532.55f, -400.59f,0.00f,528.66f,
	-400.10f,0.00f,524.69f, -400.00f,0.00f,520.69f, -400.00f,0.00f,516.69f, -400.00f,0.00f,512.69f,
	-400.00f,0.00f,508.69f, -400.00f,0.00f,504.69f, -400.00f,0.00f,500.69f, -399.98f,0.00f,496.69f,
	-399.63f,0.00f,492.71f, -398.87f,0.00f,488.79f, -397.69f,0.00f,484.96f, -396.13f,0.00f,481.29f,
	-394.18f,0.00f,477.79f, -391.88f,0.00f,474.52f, -389.25f,0.00f,471.51f, -386.32f,0.00f,468.80f,
	-383.11f,0.00f,466.40f, -379.68f,0.00f,464.36f, -376.04f,0.00f,462.69f, -372.26f,0.00f,461.41f,
	-368.36f,0.00f,460.54f, -364.38f,0.00f,460.07f, -360.38f,0.00f,460.00f, -356.38f,0.00f,460.00f,
	-352.38f,0.00f,460.00f, -348.38f,0.00f,460.00f, -344.38f,0.00f,460.00f, -340.38f,0.00f,460.00f,
	-336.38f,0.00f,460.00f, -332.38f,0.00f,460.00f, -328.38f,0.00f,460.00f, -324.38f,0.00f,460.00f,
	-320.38f,0.00f,460.00f, -316.38f,0.00f,460.00f, -312.38f,0.00f,460.00f, -308.38f,0.00f,460.00f,
	-304.38f,0.00f,460.00f, -300.38f,0.00f,460.00f, -296.38f,0.00f,460.00f, -292.38f,0.00f,460.00f,
	-288.38f,0.00f,460.00f, -284.38f,0.00f,460.00f, -280.38f,0.00f,460.00f, -276.38f,0.00f,460.00f,
	-272.38f,0.00f,460.00f, -268.38f,0.00f,460.00f, -264.38f,0.00f,460.00f, -260.38f,0.00f,460.00f,
	-256.38f,0.00f,460.00f, -252.39f,0.00f,459.94f, -248.41f,0.00f,459.50f, -244.51f,0.00f,458.64f,
	-240.71f,0.00f,457.38f, -237.07f,0.00f,455.73f, -233.62f,0.00f,453.71f, -230.41f,0.00f,451.33f,
	-227.46f,0.00f,448.63f, -224.81f,0.00f,445.63f, -222.49f,0.00f,442.38f, -220.53f,0.00f,438.89f,
	-218.94f,0.00f,435.22f, -217.75f,0.00f,431.41f, -216.97f,0.00f,427.49f, -216.60f,0.00f,423.51f,
	-216.57f,0.00f,419.51f, -216.57f,0.00f,415.51f, -216.57f,0.00f,411.51f, -216.57f,0.00f,407.51f,
	-216.57f,0.00f,403.51f, -216.57f,0.00f,399.51f, -216.57f,0.00f,395.51f, -216.57f,0.00f,391.51f,
	-216.57f,0.00f,387.51f, -216.57f,0.00f,383.51f, -216.57f,0.00f,379.51f, -216.57f,0.00f,375.51f,
	-216.57f,0.00f,371.51f, -216.57f,0.00f,367.51f, -216.30f,0.00f,363.52f, -215.62f,0.00f,359.58f,
	-214.53f,0.00f,355.73f, -213.05f,0.00f,352.02f, -211.18f,0.00f,348.49f, -208.94f,0.00f,345.17f,
	-206.38f,0.00f,342.10f, -203.50f,0.00f,339.32f, -200.35f,0.00f,336.86f, -196.96f,0.00f,334.75f,
	-193.36f,0.00f,333.00f, -189.60f,0.00f,331.64f, -185.72f,0.00f,330.68f, -181.76f,0.00f,330.13f,
	-177.77f,0.00f,330.00f, -173.77f,0.00f,330.00f, -169.77f,0.00f,330.00f, -165.77f,0.00f,330.00f,
	-161.77f,0.00f,330.00f, -157.77f,0.00f,330.00f, -153.77f,0.00f,329.99f, -149.78f,0.00f,329.70f,
	-145.84f,0.00f,328.98f, -142.01f,0.00f,327.86f, -138.31f,0.00f,326.35f, -134.79f,0.00f,324.45f,
	-131.49f,0.00f,322.19f, -128.45f,0.00f,319.60f, -125.69f,0.00f,316.70f, -123.26f,0.00f,313.53f,
	-121.17f,0.00f,310.12f, -119.45f,0.00f,306.51f, -118.12f,0.00f,302.74f, -117.19f,0.00f,298.85f,
	-116.68f,0.00f,294.88f, -116.57f,0.00f,290.89f, -116.57f,0.00f,286.89f, -116.57f,0.00f,282.89f,
	-116.57f,0.00f,278.89f, -116.57f,0.00f,274.89f, -116.57f,0.00f,270.89f, -116.58f,0.00f,266.89f,
	-116.91f,0.00f,262.90f, -117.66f,0.00f,258.97f, -118.81f,0.00f,255.15f, -120.36f,0.00f,251.46f,
	-122.28f,0.00f,247.96f, -124.57f,0.00f,244.68f, -127.19f,0.00f,241.65f, -130.11f,0.00f,238.92f,
	-133.30f,0.00f,236.51f, -136.72f,0.00f,234.45f, -140.35f,0.00f,232.76f, -144.13f,0.00f,231.46f,
	-148.02f,0.00f,230.57f, -151.99f,0.00f,230.09f, -155.99f,0.00f,230.00f, -159.99f,0.00f,230.00f,
	-163.99f,0.00f,230.00f, -167.99f,0.00f,230.00f, -171.99f,0.00f,230.00f, -175.99f,0.00f,230.00f,
	-179.99f,0.00f,229.97f, -183.97f,0.00f,229.61f, -187.89f,0.00f,228.84f, -191.71f,0.00f,227.65f,
	-195.39f,0.00f,226.08f, -198.87f,0.00f,224.12f, -202.14f,0.00f,221.81f, -205.14f,0.00f,219.17f,
	-207.85f,0.00f,216.23f, -210.23f,0.00f,213.02f, -212.26f,0.00f,209.57f, -213.92f,0.00f,205.93f,
	-215.19f,0.00f,202.14f, -216.05f,0.00f,198.24f, -216.50f,0.00f,194.27f, -216.57f,0.00f,190.27f,
	-216.57f,0.00f,186.27f, -216.57f,0.00f,182.27f, -216.57f,0.00f,178.27f, -216.57f,0.00f,174.27f,
	-216.57f,0.00f,170.27f, -216.57f,0.00f,166.27f, -216.57f,0.00f,162.27f, -216.57f,0.00f,158.27f,
	-216.57f,0.00f,154.27f, -216.57f,0.00f,150.27f, -216.57f,0.00f,146.27f, -216.57f,0.00f,142.27f,
	-216.57f,0.00f,138.27f, -216.57f,0.00f,134.27f, -216.57f,0.00f,130.27f, -216.57f,0.00f,126.27f,
	-216.57f,0.00f,122.27f, -216.57f,0.00f,118.27f, -216.57f,0.00f,114.27f, -216.57f,0.00f,110.27f,
	-216.57f,0.00f,106.27f, -216.57f,0.00f,102.27f, -216.57f,0.00f,98.27f, -216.57f,0.00f,94.27f,
	-216.57f,0.00f,90.27f, -216.57f,0.00f,86.27f, -216.57f,0.00f,82.27f, -216.57f,0.00f,78.27f,
	-216.57f,0.00f,74.27f, -216.57f,0.00f,70.27f, -216.57f,0.00f,66.27f, -216.57f,0.00f,62.27f,
	-216.57f,0.00f,58.27f, -216.57f,0.00f,54.27f, -216.57f,0.00f,50.27f, -216.57f,0.00f,46.27f,
	-216.57f,0.00f,42.27f, -216.57f,0.00f,38.27f, -216.57f,0.00f,34.27f, -216.57f,0.00f,30.27f,
	-216.57f,0.00f,26.27f, -216.57f,0.00f,22.27f, -216.57f,0.00f,18.27f, -216.57f,0.00f,14.27f,
	-216.57f,0.00f,10.27f, -216.57f,0.00f,6.27f, -216.57f,0.00f,2.27f, -216.57f,0.00f,-1.73f,
	-216.57f,0.00f,-5.73f, -216.57f,0.00f,-9.73f, -216.57f,0.00f,-13.73f, -216.57f,0.00f,-17.73f,
	-216.57f,0.00f,-21.73f, -216.57f,0.00f,-25.73f, -216.57f,0.00f,-29.73f, -216.57f,0.00f,-33.73f,
	-216.57f,0.00f,-37.73f, -216.57f,0.00f,-41.73f, -216.57f,0.00f,-45.73f, -216.57f,0.00f,-49.73f,
	-216.57f,0.00f,-53.73f, -216.57f,0.00f,-57.73f, -216.57f,0.00f,-61.73f, -216.57f,0.00f,-65.73f,
	-216.57f,0.00f,-69.73f, -216.57f,0.00f,-73.73f, -216.57f,0.00f,-77.73f, -216.57f,0.00f,-81.73f,
	-216.57f,0.00f,-85.73f, -216.57f,0.00f,-89.73f, -216.57f,0.00f,-93.73f, -216.57f,0.00f,-97.73f,
	-216.57f,0.00f,-101.73f, -216.57f,0.00f,-105.73f, -216.57f,0.00f,-109.73f, -216.57f,0.00f,-113.73f,
	-216.57f,0.00f,-117.73f, -216.57f,0.00f,-121.73f, -216.57f,0.00f,-125.73f, -216.57f,0.00f,-129.73f,
	-216.57f,0.00f,-133.73f, -216.57f,0.00f,-137.73f, -216.57f,0.00f,-141.73f, -216.57f,0.00f,-145.73f,
	-216.57f,0.00f,-149.73f, -216.57f,0.00f,-153.73f, -216.57f,0.00f,-157.73f, -216.57f,0.00f,-161.73f,
	-216.57f,0.00f,-165.73f, -216.57f,0.00f,-169.73f, -216.57f,0.00f,-173.73f, -216.57f,0.00f,-177.73f,
	-216.57f,0.00f,-181.73f, -216.57f,0.00f,-185.73f, -216.57f,0.00f,-189.73f, -216.61f,0.00f,-193.73f,
	-217.00f,0.00f,-197.71f, -217.81f,0.00f,-201.63f, -219.02f,0.00f,-205.44f, -220.63f,0.00f,-209.10f,
	-222.62f,0.00f,-212.57f, -224.95f,0.00f,-215.81f, -227.62f,0.00f,-218.79f, -230.58f,0.00f,-221.47f,
	-233.81f,0.00f,-223.83f, -237.27f,0.00f,-225.84f, -240.92f,0.00f,-227.47f, -244.72f,0.00f,-228.70f,
	-248.63f,0.00f,-229.53f, -252.61f,0.00f,-229.95f, -256.61f,0.00f,-230.00f, -260.61f,0.00f,-230.00f,
	-264.61f,0.00f,-230.00f, -268.61f,0.00f,-230.00f, -272.61f,0.00f,-230.00f, -276.61f,0.00f,-230.00f,
	-280.61f,0.00f,-230.05f, -284.59f,0.00f,-230.48f, -288.49f,0.00f,-231.32f, -292.29f,0.00f,-232.57f,
	-295.94f,0.00f,-234.20f, -299.40f,0.00f,-236.22f, -302.62f,0.00f,-238.58f, -305.58f,0.00f,-241.27f,
	-308.24f,0.00f,-244.26f, -310.57f,0.00f,-247.50f, -312.54f,0.00f,-250.98f, -314.14f,0.00f,-254.64f,
	-315.35f,0.00f,-258.46f, -316.15f,0.00f,-262.37f, -316.53f,0.00f,-266.35f, -316.57f,0.00f,-270.35f,
	-316.57f,0.00f,-274.35f, -316.57f,0.00f,-278.35f, -316.57f,0.00f,-282.35f, -316.57f,0.00f,-286.35f,
	-316.57f,0.00f,-290.35f, -316.50f,0.00f,-294.35f, -316.04f,0.00f,-298.32f, -315.17f,0.00f,-302.22f,
	-313.89f,0.00f,-306.01f, -312.22f,0.00f,-309.65f, -310.18f,0.00f,-313.08f, -307.79f,0.00f,-316.29f,
	-305.08f,0.00f,-319.23f, -302.07f,0.00f,-321.86f, -298.80f,0.00f,-324.16f, -295.31f,0.00f,-326.11f,
	-291.63f,0.00f,-327.68f, -287.81f,0.00f,-328.86f, -283.89f,0.00f,-329.63f, -279.91f,0.00f,-329.98f,
	-275.91f,0.00f,-330.00f, -271.91f,0.00f,-330.00f, -267.91f,0.00f,-330.00f, -263.91f,0.00f,-330.00f,
	-259.91f,0.00f,-330.00f, -255.91f,0.00f,-330.00f, -251.91f,0.00f,-330.09f, -247.94f,0.00f,-330.58f,
	-244.05f,0.00f,-331.49f, -240.27f,0.00f,-332.79f, -236.65f,0.00f,-334.49f, -233.23f,0.00f,-336.56f,
	-230.04f,0.00f,-338.98f, -227.13f,0.00f,-341.71f, -224.52f,0.00f,-344.74f, -222.24f,0.00f,-348.03f,
	-220.32f,0.00f,-351.54f, -218.78f,0.00f,-355.23f, -217.64f,0.00f,-359.06f, -216.90f,0.00f,-362.99f,
	-216.58f,0.00f,-366.97f, -216.57f,0.00f,-370.97f, -216.57f,0.00f,-374.97f, -216.57f,0.00f,-378.97f,
	-216.57f,0.00f,-382.97f, -216.57f,0.00f,-386.97f, -216.57f,0.00f,-390.97f, -216.57f,0.00f,-394.97f,
	-216.57f,0.00f,-398.97f, -216.57f,0.00f,-402.97f, -216.57f,0.00f,-406.97f, -216.57f,0.00f,-410.97f,
	-216.57f,0.00f,-414.97f, -216.57f,0.00f,-418.97f, -216.57f,0.00f,-422.97f, -216.57f,0.00f,-426.97f,
	-216.57f,0.00f,-430.97f, -216.57f,0.00f,-434.97f, -216.57f,0.00f,-438.97f, -216.57f,0.00f,-442.97f,
	-216.57f,0.00f,-446.97f, -216.57f,0.00f,-450.97f, -216.57f,0.00f,-454.97f, -216.57f,0.00f,-458.97f,
	-216.56f,0.00f,-462.97f, -216.24f,0.00f,-466.96f, -215.51f,0.00f,-470.89f, -214.38f,0.00f,-474.72f,
	-212.84f,0.00f,-478.41f, -210.93f,0.00f,-481.92f, -208.65f,0.00f,-485.21f, -206.05f,0.00f,-488.25f,
	-203.14f,0.00f,-490.99f, -199.96f,0.00f,-493.41f, -196.54f,0.00f,-495.48f, -192.92f,0.00f,-497.18f,
	-189.15f,0.00f,-498.50f, -185.25f,0.00f,-499.41f, -181.29f,0.00f,-499.90f, -177.29f,0.00f,-500.00f,
	-173.29f,0.00f,-500.00f, -169.29f,0.00f,-500.00f, -165.29f,0.00f,-500.00f, -161.29f,0.00f,-500.00f,
	-157.29f,0.00f,-500.00f, -153.29f,0.00f,-500.00f, -149.29f,0.00f,-500.00f, -145.29f,0.00f,-500.00f,
	-141.29f,0.00f,-500.00f, -137.29f,0.00f,-500.00f, -133.29f,0.00f,-500.00f, -129.29f,0.00f,-500.00f,
	-125.29f,0.00f,-500.00f, -121.29f,0.00f,-500.00f, -117.29f,0.00f,-500.00f, -113.29f,0.00f,-500.00f,
	-109.29f,0.00f,-500.00f, -105.29f,0.00f,-500.00f, -101.29f,0.00f,-500.00f, -97.29f,0.00f,-500.00f,
	-93.29f,0.00f,-500.00f, -89.29f,0.00f,-500.00f, -85.29f,0.00f,-500.00f, -81.29f,0.00f,-500.00f,
	-77.29f,0.00f,-499.99f, -73.30f,0.00f,-499.71f, -69.36f,0.00f,-499.01f, -65.52f,0.00f,-497.89f,
	-61.82f,0.00f,-496.38f, -58.30f,0.00f,-494.49f, -54.99f,0.00f,-492.24f, -51.94f,0.00f,-489.66f,
	-49.18f,0.00f,-486.77f, -46.74f,0.00f,-483.60f, -44.64f,0.00f,-480.20f, -42.91f,0.00f,-476.59f,
	-41.57f,0.00f,-472.83f, -40.64f,0.00f,-468.94f, -40.12f,0.00f,-464.97f, -40.00f,0.00f,-460.98f,
	-40.00f,0.00f,-456.98f, -40.00f,0.00f,-452.98f, -40.00f,0.00f,-448.98f, -40.00f,0.00f,-444.98f,
	-40.00f,0.00f,-440.98f, -39.99f,0.00f,-436.98f, -39.67f,0.00f,-432.99f, -38.93f,0.00f,-429.06f,
	-37.79f,0.00f,-425.23f, -36.25f,0.00f,-421.54f, -34.33f,0.00f,-418.03f, -32.06f,0.00f,-414.75f,
	-29.45f,0.00f,-411.72f, -26.53f,0.00f,-408.98f, -23.35f,0.00f,-406.56f, -19.93f,0.00f,-404.49f,
	-16.31f,0.00f,-402.80f, -12.53f,0.00f,-401.49f, -8.63f,0.00f,-400.58f, -4.67f,0.00f,-400.09f,
	-0.67f,0.00f,-400.00f, 3.33f,0.00f,-400.00f, 7.33f,0.00f,-400.00f, 11.33f,0.00f,-400.00f,
	15.33f,0.00f,-400.00f, 19.33f,0.00f,-400.00f, 23.33f,0.00f,-400.00f, 27.33f,0.00f,-400.00f,
	31.33f,0.00f,-400.00f, 35.33f,0.00f,-400.00f, 38.94f,0.94f,-400.00f, 41.77f,3.77f,-400.00f,
	44.60f,6.60f,-400.00f, 47.43f,9.43f,-400.00f, 50.26f,12.26f,-400.00f, 53.08f,15.08f,-400.00f,
	55.91f,17.91f,-400.00f, 58.74f,20.74f,-400.00f, 61.57f,23.57f,-400.00f, 64.40f,26.40f,-400.00f,
	67.62f,28.28f,-400.00f, 71.62f,28.28f,-400.00f, 75.62f,28.28f,-400.00f, 79.62f,28.28f,-400.00f,
	83.62f,28.28f,-400.00f, 87.62f,28.28f,-400.00f, 91.62f,28.28f,-400.00f, 95.62f,28.28f,-400.00f,
	99.62f,28.28f,-400.00f, 103.62f,28.28f,-400.00f, 107.23f,29.23f,-400.00f, 110.05f,32.05f,-400.00f,
	112.88f,34.88f,-400.00f, 115.71f,37.71f,-400.00f, 118.54f,40.54f,-400.00f, 121.37f,43.37f,-400.00f,
	124.20f,46.20f,-400.00f, 127.03f,49.02f,-400.00f, 129.85f,51.85f,-400.00f, 132.68f,54.68f,-400.00f,
	135.90f,56.57f,-400.00f, 139.90f,56.57f,-400.00f, 143.90f,56.57f,-400.00f, 147.90f,56.57f,-400.00f,
	151.90f,56.57f,-400.00f, 155.90f,56.57f,-400.00f, 159.90f,56.57f,-400.00f, 163.90f,56.57f,-400.00f,
	167.90f,56.57f,-400.00f, 171.90f,56.57f,-400.00f, 175.51f,57.51f,-400.00f, 178.34f,60.34f,-400.00f,
	181.17f,63.17f,-400.00f, 184.00f,66.00f,-400.00f, 186.82f,68.82f,-400.00f, 189.65f,71.65f,-400.00f,
	192.48f,74.48f,-400.00f, 195.31f,77.31f,-400.00f, 198.14f,80.14f,-400.00f, 200.97f,82.97f,-400.00f,
	204.18f,84.85f,-400.00f, 208.18f,84.85f,-400.00f, 212.18f,84.85f,-400.00f, 216.18f,84.85f,-400.00f,
	220.18f,84.85f,-400.00f, 224.18f,84.85f,-400.00f, 228.18f,84.85f,-400.00f, 232.18f,84.85f,-400.00f,
	236.18f,84.85f,-400.00f, 240.18f,84.85f,-400.00f, 244.18f,84.85f,-400.00f, 248.18f,84.85f,-400.00f,
	252.18f,84.85f,-400.00f, 256.18f,84.85f,-400.00f, 260.18f,84.85f,-400.00f, 264.18f,84.85f,-399.94f,
	268.16f,84.85f,-399.50f, 272.06f,84.85f,-398.64f, 275.86f,84.85f,-397.38f, 279.50f,84.85f,-395.73f,
	282.95f,84.85f,-393.71f, 286.16f,84.85f,-391.33f, 289.11f,84.85f,-388.63f, 291.76f,84.85f,-385.63f,
	294.08f,84.85f,-382.38f, 296.04f,84.85f,-378.89f, 297.63f,84.85f,-375.22f, 298.82f,84.85f,-371.41f,
	299.60f,84.85f,-367.49f, 299.97f,84.85f,-363.51f, 300.00f,84.85f,-359.51f, 300.00f,84.85f,-355.51f,
	300.00f,84.85f,-351.51f, 300.00f,84.85f,-347.51f, 300.00f,84.85f,-343.51f, 300.00f,84.85f,-339.51f,
	300.00f,84.85f,-335.51f, 300.00f,84.85f,-331.51f, 300.00f,84.85f,-327.51f, 300.00f,84.85f,-323.51f,
	300.00f,83.09f,-320.24f, 300.00f,80.26f,-317.41f, 300.00f,77.43f,-314.58f, 300.00f,74.60f,-311.75f,
	300.00f,71.78f,-308.92f, 300.00f,68.95f,-306.09f, 300.00f,66.12f,-303.27f, 300.00f,63.29f,-300.44f,
	300.00f,60.46f,-297.61f, 300.00f,57.63f,-294.78f, 300.00f,56.57f,-291.22f, 300.00f,56.57f,-287.22f,
	300.00f,56.57f,-283.22f, 300.00f,56.57f,-279.22f, 300.00f,56.57f,-275.22f, 300.00f,56.57f,-271.22f,
	300.00f,56.57f,-267.22f, 300.00f,56.57f,-263.22f, 300.00f,56.57f,-259.22f, 300.00f,56.57f,-255.22f,
	300.00f,54.80f,-251.95f, 300.00f,51.98f,-249.12f, 300.00f,49.15f,-246.30f, 300.00f,46.32f,-243.47f,
	300.00f,43.49f,-240.64f, 300.00f,40.66f,-237.81f, 300.00f,37.83f,-234.98f, 300.00f,35.01f,-232.15f,
	300.00f,32.18f,-229.32f, 300.00f,29.35f,-226.50f, 300.00f,28.28f,-222.94f, 300.00f,28.28f,-218.94f,
	300.00f,28.28f,-214.94f, 300.00f,28.28f,-210.94f, 300.00f,28.28f,-206.94f, 300.00f,28.28f,-202.94f,
	300.00f,28.28f,-198.94f, 300.00f,28.28f,-194.94f, 300.00f,28.28f,-190.94f, 300.00f,28.28f,-186.94f,
	300.00f,28.28f,-182.94f, 300.00f,28.28f,-178.94f, 300.00f,28.28f,-174.94f, 300.00f,28.28f,-170.94f,
	300.00f,28.28f,-166.94f, 300.00f,28.28f,-162.94f, 300.00f,28.28f,-158.94f, 300.00f,28.28f,-154.94f,
	300.00f,28.28f,-150.94f, 300.00f,28.28f,-146.94f, 300.00f,26.52f,-143.67f, 300.00f,23.69f,-140.84f,
	300.00f,20.86f,-138.01f, 300.00f,18.04f,-135.18f, 300.00f,15.21f,-132.35f, 300.00f,12.38f,-129.53f,
	300.00f,9.55f,-126.70f, 300.00f,6.72f,-123.87f, 300.00f,3.89f,-121.04f, 300.00f,1.06f,-118.21f,
	300.00f,0.00f,-114.65f, 300.00f,0.00f,-110.65f, 300.00f,0.00f,-106.65f, 300.00f,0.00f,-102.65f,
	300.00f,0.00f,-98.65f, 300.00f,0.00f,-94.65f, 300.00f,0.00f,-90.65f, 300.00f,0.00f,-86.65f,
	300.00f,0.00f,-82.65f, 300.00f,0.00f,-78.65f, 300.00f,0.00f,-74.65f, 300.00f,0.00f,-70.65f,
	300.00f,0.00f,-66.65f, 300.00f,0.00f,-62.65f, 300.00f,0.00f,-58.65f, 300.00f,0.00f,-54.65f,
	300.00f,0.00f,-50.65f, 300.00f,0.00f,-46.65f, 300.00f,0.00f,-42.65f, 300.00f,0.00f,-38.65f,
	299.85f,0.00f,-34.66f, 299.29f,0.00f,-30.70f, 298.32f,0.00f,-26.82f, 296.94f,0.00f,-23.07f,
	295.18f,0.00f,-19.48f, 293.05f,0.00f,-16.09f, 290.58f,0.00f,-12.95f, 287.79f,0.00f,-10.09f,
	284.71f,0.00f,-7.53f, 281.39f,0.00f,-5.32f, 277.84f,0.00f,-3.46f, 274.13f,0.00f,-1.99f,
	270.28f,0.00f,-0.91f, 266.33f,0.00f,-0.25f, 262.34f,0.00f,-0.00f, 258.34f,0.00f,0.00f,
	254.34f,0.00f,0.00f, 250.34f,0.00f,0.00f, 246.34f,0.00f,0.00f, 242.34f,0.00f,0.00f,
	238.34f,0.00f,0.00f, 234.34f,0.00f,0.00f, 230.34f,0.00f,0.00f, 226.34f,0.00f,0.00f,
	222.34f,0.00f,0.00f, 218.34f,0.00f,0.00f, 214.34f,0.00f,0.00f, 210.34f,0.00f,0.00f,
	206.34f,0.00f,0.00f, 202.34f,0.00f,0.00f, 198.34f,0.00f,0.00f, 194.34f,0.00f,0.00f,
	190.34f,0.00f,0.00f, 186.34f,0.00f,0.00f, 182.34f,0.00f,0.00f, 178.34f,0.00f,0.00f,
	174.34f,0.00f,0.00f, 170.34f,0.00f,0.00f, 166.34f,0.00f,0.00f, 162.34f,0.00f,0.00f,
	158.34f,0.00f,0.00f, 154.34f,0.00f,0.00f, 150.34f,0.00f,0.00f, 146.34f,0.00f,0.00f,
	142.34f,0.00f,0.00f, 138.34f,0.00f,0.00f, 134.34f,0.00f,0.00f, 130.34f,0.00f,0.00f,
	126.34f,0.00f,0.00f, 122.34f,0.00f,0.00f, 118.34f,0.00f,0.00f, 114.34f,0.00f,0.00f,
	110.34f,0.00f,0.00f, 106.34f,0.00f,0.00f, 102.34f,0.00f,0.00f, 98.34f,0.00f,0.00f,
	94.34f,0.00f,0.00f, 90.34f,0.00f,0.00f, 86.34f,0.00f,0.00f, 82.34f,0.00f,0.00f,
	78.34f,0.00f,0.00f, 74.34f,0.00f,0.00f, 70.34f,0.00f,0.00f, 66.34f,0.00f,0.00f,
	62.34f,0.00f,0.00f, 58.34f,0.00f,0.00f, 54.34f,0.00f,0.00f, 50.34f,0.00f,0.00f,
	46.34f,0.00f,0.00f, 42.34f,0.00f,0.00f, 38.34f,0.00f,0.00f, 34.34f,0.00f,0.00f,
	30.34f,0.00f,0.00f, 26.34f,0.00f,0.00f, 22.34f,0.00f,0.00f, 18.34f,0.00f,0.00f,
	14.34f,0.00f,0.00f, 10.34f,0.00f,0.00f, 6.34f,0.00f,0.00f, 2.34f,0.00f,0.00f,
};

//Nave
const float NAVE_ESCALA = 7.0f;            
const float NAVE_NARIZ_SIGNO = 1.0f;
const float NAVE_VELOCIDAD = 2.0f;           
const float NAVE_ALTURA = 5.0f;              
const float NAVE_SUAVIZADO = 10.0f;           //distancia hacia adelante/atras
const glm::vec3 NAVE_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

float naveS = 0.0f;                           //distancia recorrida sobre la pista
glm::vec3 navePos = glm::vec3(0.0f);
glm::vec3 naveAdelante = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 naveArriba = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 naveDerecha = glm::vec3(0.0f, 0.0f, -1.0f);
glm::mat4 modelNave = glm::mat4(1.0f);

//Satelite Variables
const float SAT_ESCALA = 1.5f;               //el modelo original mide ~50 de ancho
const float SAT_CENTRO_X = -130.0f;           //centro de la pista
const float SAT_CENTRO_Z = 30.0f;
const float SAT_LADO = 330.0f;                //distancia del centro a cada lado del cuadrado
const float SAT_ALTURA = 140.0f;
const float SAT_VELOCIDAD = 1.2f;             //unidades por deltaTime
int satTramo = 0;                             //0,1,2,3 = el lado del cuadrado en el que va
float satX = SAT_CENTRO_X + SAT_LADO, satY = SAT_ALTURA, satZ = SAT_CENTRO_Z - SAT_LADO;
float satYaw = 0.0f;

const float SAT_ANTENA_VEL = 2.0f;            
const float SAT_BRAZO_MIN = -10.0f, SAT_BRAZO_MAX = 35.0f, SAT_BRAZO_VEL = 0.6f;
float satAngAntena = 0.0f;
float satAngBrazo = 0.0f;
float satSentido = 1.0f;                      //+1 sube, -1 baja

//---------------- Variables de dibujo ----------------
std::vector<std::string> skyboxFaces;
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
GLuint uniformColor = 0;
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 modelSat = glm::mat4(1.0f);
glm::mat4 modelCuerpo = glm::mat4(1.0f);
glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
GLfloat now = 0.0f;

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//---------------- Punto de la linea central de la pista a la distancia s ----------------
//Interpola entre dos puntos consecutivos; si s se pasa del final, da la vuelta al inicio
glm::vec3 puntoPista(float s)
{
	float largo = PISTA_NUM_PUNTOS * PISTA_PASO;
	s = std::fmod(s, largo);
	if (s < 0.0f)
		s += largo;

	float k = s / PISTA_PASO;
	int i0 = (int)k;
	if (i0 >= PISTA_NUM_PUNTOS)
		i0 = 0;
	int i1 = i0 + 1;
	if (i1 >= PISTA_NUM_PUNTOS)
		i1 = 0;                                   //cierra el circuito
	float f = k - (float)i0;

	glm::vec3 a(PISTA_PUNTOS[3 * i0], PISTA_PUNTOS[3 * i0 + 1], PISTA_PUNTOS[3 * i0 + 2]);
	glm::vec3 b(PISTA_PUNTOS[3 * i1], PISTA_PUNTOS[3 * i1 + 1], PISTA_PUNTOS[3 * i1 + 2]);
	return a + (b - a) * f;
}

//Animacion de la nave
void actualizaNave(float dt)
{
	float largo = PISTA_NUM_PUNTOS * PISTA_PASO;

	//Velocidad segun el terreno el terreno
	float velocidad = NAVE_VELOCIDAD;
	if (naveAdelante.y > 0.3f)
		velocidad = NAVE_VELOCIDAD * 0.6f;
	else if (naveAdelante.y < -0.3f)
		velocidad = NAVE_VELOCIDAD * 1.4f;

	naveS += velocidad * dt;
	if (naveS >= largo)
		naveS -= largo;                            //termino la vuelta: empieza otra

	//Posicion y hacia donde mira
	glm::vec3 centro = puntoPista(naveS);
	naveAdelante = glm::normalize(puntoPista(naveS + NAVE_SUAVIZADO) - puntoPista(naveS - NAVE_SUAVIZADO));
	naveDerecha = glm::normalize(glm::cross(naveAdelante, glm::vec3(0.0f, 1.0f, 0.0f)));
	naveArriba = glm::normalize(glm::cross(naveDerecha, naveAdelante));
	navePos = centro + naveArriba * NAVE_ALTURA;

	//Matriz de la nave
	modelNave = glm::mat4(
		glm::vec4(NAVE_NARIZ_SIGNO * naveAdelante, 0.0f),
		glm::vec4(naveArriba, 0.0f),
		glm::vec4(NAVE_NARIZ_SIGNO * naveDerecha, 0.0f),
		glm::vec4(navePos, 1.0f));
	modelNave = glm::scale(modelNave, glm::vec3(NAVE_ESCALA));
}

void actualizaSatelite(float dt)
{
	satY = SAT_ALTURA;

	
	if (satTramo == 0)            //va hacia +Z
	{
		satZ += SAT_VELOCIDAD * dt;
		satYaw = 270.0f * toRadians;
		if (satZ >= SAT_CENTRO_Z + SAT_LADO)
			satTramo = 1;
	}
	else if (satTramo == 1)       //va hacia -X
	{
		satX -= SAT_VELOCIDAD * dt;
		satYaw = 180.0f * toRadians;
		if (satX <= SAT_CENTRO_X - SAT_LADO)
			satTramo = 2;
	}
	else if (satTramo == 2)       //va hacia -Z
	{
		satZ -= SAT_VELOCIDAD * dt;
		satYaw = 90.0f * toRadians;
		if (satZ <= SAT_CENTRO_Z - SAT_LADO)
			satTramo = 3;
	}
	else                          //va hacia +X y al llegar vuelve a empezar
	{
		satX += SAT_VELOCIDAD * dt;
		satYaw = 0.0f;
		if (satX >= SAT_CENTRO_X + SAT_LADO)
			satTramo = 0;
	}

	//Antena: gira continuo sobre Y
	satAngAntena += SAT_ANTENA_VEL * dt;
	if (satAngAntena > 360.0f)
		satAngAntena -= 360.0f;

	//Brazos: suben y bajan entre su minimo y su maximo
	satAngBrazo += satSentido * SAT_BRAZO_VEL * dt;
	if (satAngBrazo >= SAT_BRAZO_MAX)
	{
		satAngBrazo = SAT_BRAZO_MAX;
		satSentido = -1.0f;
	}
	else if (satAngBrazo <= SAT_BRAZO_MIN)
	{
		satAngBrazo = SAT_BRAZO_MIN;
		satSentido = 1.0f;
	}
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();

	//La camara 
	camera = Camera(glm::vec3(60.0f, 25.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -10.0f, 4.0f, 0.5f);

	//Modelos y texturas
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	//Pista
	pista_carreras = Model();
	pista_carreras.LoadModel("Models/pista_carreras_larga_v2.obj");

	//Nave
	nave_seamoth = Model();
	nave_seamoth.LoadModel("Models/seamouth_tex.obj");

	//Satelite
	satelite_cuerpo = Model();
	satelite_cuerpo.LoadModel("Models/Satelite_cuerpo.obj");

	satelite_plato = Model();
	satelite_plato.LoadModel("Models/Satelite_plato.obj");

	satelite_brazoA = Model();
	satelite_brazoA.LoadModel("Models/Satelite_brazoA.obj");

	satelite_brazoB = Model();
	satelite_brazoB.LoadModel("Models/Satelite_brazoB.obj");

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
		0.45f, 0.6f,
		1.0f, -1.0f, -1.0f);

	//Donde aparecemos
	projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.5f, 3000.0f);

	
	lastTime = (GLfloat)glfwGetTime();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = (GLfloat)glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (GLfloat)((now - lastTime) / limitFPS);
		lastTime = now;

		//Animaciones
		actualizaNave(deltaTime);
		actualizaSatelite(deltaTime);

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//informacion al shader de fuentes de iluminacion (solo la direccional, 0 puntuales y 0 spot)
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, 0);
		shaderList[0].SetSpotLights(spotLights, 0);

		//Valores base para todo lo que se dibuja
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		//Pista
		plainTexture.UseTexture(); //Por si no hay textura
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pista_carreras.RenderModel();

		//Seamoth nave
		plainTexture.UseTexture();
		color = NAVE_COLOR;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		nave_seamoth.RenderModel();

		//Satelite Jerarquia
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//Inicio
		modelSat = glm::translate(glm::mat4(1.0f), glm::vec3(satX, satY, satZ));
		modelSat = glm::rotate(modelSat, satYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		modelSat = glm::scale(modelSat, glm::vec3(SAT_ESCALA));

		//Cuerpo
		modelCuerpo = modelSat;
		plainTexture.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpo));
		satelite_cuerpo.RenderModel();

		//Plato
		model = glm::rotate(modelCuerpo, satAngAntena * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		plainTexture.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_plato.RenderModel();

		//Brazo A
		model = glm::translate(modelCuerpo, glm::vec3(0.0f, 5.781f, 3.32f));
		model = glm::rotate(model, -satAngBrazo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		plainTexture.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_brazoA.RenderModel();

		//Brazo B
		model = glm::translate(modelCuerpo, glm::vec3(0.0f, 5.851f, -5.298f));
		model = glm::rotate(model, satAngBrazo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		plainTexture.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_brazoB.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}