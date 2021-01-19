/* -------------------------------------------------------- */
/* ----------------   PROYECTO FINAL ---------------------- */
/* --------------- COMPUTACION GRAFICA  ------------------- */
/* ------------------- SEMESTRE 2021-1 -------------------- */
/* ----------------------- ALUMNO ------------------------- */
/* ------------- ESCAMILLA JAIMES NEFTALÍ RAFAEL ---------- */
/* -------------------------------------------------------- */

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")
/* ---------------------------- */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow *window);

// settings
// = 16 milisec // 1000 millisec == 1 sec
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;

// Camera Position Keys
/* --------------------------------------- */
float mov_x = -50.0f;
float mov_y = 10.0f;
float mov_z = 100.0f;

// camera
/* ---------------------------- */
Camera camera(glm::vec3(mov_x, mov_y, mov_z));
float MovementSpeed = 10.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
/* ---------------------------- */
const int FPS = 60;
// = 16 milisec // 1000 millisec == 1 sec
const int LOOP_TIME = 10000 / FPS; 
double	deltaTime = 0.0f,
		lastFrame = 0.0f;


// Light
/* ---------------------------- */
glm::vec3 lightPosition(mov_x, mov_y, mov_z);
glm::vec3 lightDirection(10, 10, -10);

// For Animated Variables
/*---------------------------------------------------------*/
float movpuertap = 0.0f;	//principal door
float movpuertab = 0.0f;	//garage doot
float movpuertat = 0.0f;	//backyard door
float movpuertar = 0.0f;	//room door



//Create animation
/* ---------------------------------------- */
void animateChair(void)
{
	/*if (animacion)
	{
		if (avanza)
		{
			movAuto_z += 20.0f;
			if (movAuto_z >= 20.0f)
				avanza = false;
		}
		else
		{
			movAuto_z -= 20.0f;
			if (movAuto_z <= 0.0f)
				avanza = true;
		}
		std::cout << "posicion = " << movAuto_z << " en Z" << std::endl;
	}*/
}

void animateGlobe(void)
{
	//if (animacion)
	//{
	//	if (avanza)
	//	{
	//		movAuto_z += 20.0f;
	//		if (movAuto_z >= 20.0f)
	//			avanza = false;
	//	}
	//	else
	//	{
	//		movAuto_z -= 20.0f;
	//		if (movAuto_z <= 0.0f)
	//			avanza = true;
	//	}
	//	std::cout << "posicion = " << movAuto_z << " en Z" << std::endl;
	//}
}



int main()
{
	// glfw: initialize and configure
	/* ---------------------------- */
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	/* -------------------------------------------------------------------------------------- */
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto_Final", NULL, NULL);
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
	/* ------------------------------------------------------- */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	/* ---------------------------------------------------- */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	/* ------------------- */
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	/* ---------------------------------------------------------------------- */
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");

	vector<std::string> faces
	{
		// Loading Skybox images
		/* -------------------------- */
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	/* ---------------------------- */
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	/* ---------------------.....................------- */
	Model casa_muro("resources/casa/walls.obj");
	Model casa_techo("resources/casa/techo.obj");
	Model casa_comedor("resources/casa/salacomedor.obj");
	Model casa_bano("resources/casa/mueblesbath.obj");
	Model casa_recamara("resources/casa/recamara.obj");
	Model casa_regadera("resources/casa/regadera.obj");
	Model casa_sauna("resources/casa/sauna.obj");
	Model casa_cocina("resources/casa/cocina.obj");


	//load animated objects
	/* ------------------------------------------------------- */
	Model puerta_principal("resources/objetos/puertapt.obj");
	Model puerta_recamara("resources/objetos/puertart.obj");
	Model puerta_bano("resources/objetos/puertabt.obj");
	Model puerta_trasera("resources/objetos/puertatt.obj");
	Model mundo("resources/objetos/mundot.obj");
	Model silla("resources/objetos/sillat.obj");
	Model lampara_s("resources/objetos/lamparat.obj");
	Model lampara_r("resources/objetos/lampt.obj");

	// render loop
	/* ----------------------------------- */
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		/* ---------------------- */
		lastFrame = SDL_GetTicks();

		// input
		/* ------------- */
		my_input(window);
		animateChair();
		animateGlobe();

		// render
		/* --------------------------------------------- */
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		/* -------------- */
		staticShader.use();

		//Setup Advanced Lights
		/*--------------------------------------------------------------------------*/
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f);
		staticShader.setFloat("pointLight[0].linear", 0.1f);
		staticShader.setFloat("pointLight[0].quadratic", 0.1f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		//Set material shininess
		/*-------------------------------------------------*/
		staticShader.setFloat("material_shininess", 32.0f);

		//Set model matrix
		/*--------------------------------*/
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelanim = glm::mat4(1.0f);
		glm::mat4 tmp_pd = glm::mat4(1.0f);  //Principal Door animation
		glm::mat4 tmp_bd = glm::mat4(1.0f);  //Bath Door animation
		glm::mat4 tmp_td = glm::mat4(1.0f);  //Backyard Door animation
		glm::mat4 tmp_rd = glm::mat4(1.0f);  //Room Door animation
		glm::mat4 tmp_mg = glm::mat4(1.0f);  //Globe animation
		glm::mat4 tmp_mc = glm::mat4(1.0f);  //Chair animation

		// view/projection transformations
		/*-------------------------------------------------------------------------------------------------------------------------*/
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		// Light
		/*--------------------------------------------------------*/
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Load Scenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		staticShader.setVec3("material.specular", glm::vec3(0.5f));
		staticShader.setFloat("material.shininess", 32.0f);
		staticShader.setVec3("light.ambient", ambientColor);
		staticShader.setVec3("light.diffuse", diffuseColor);
		staticShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		staticShader.setVec3("light.direction", lightDirection);
		staticShader.setVec3("viewPos", camera.Position);

		//Works for animated Models but not for Simple Objects because they are already loaded with traslation, scale and rotation
		/* ----------------------------------------------------------------------------------------------------------------------- */
		/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));*/

		// Load Models
		/* ---------------------------------- */
		tmp_rd = tmp_bd = tmp_pd = model = glm::mat4(1.0f);	
		staticShader.setMat4("model", model);
		casa_muro.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_techo.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_comedor.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_regadera.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_sauna.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_recamara.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		casa_cocina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movpuertat, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara_r.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movpuertat, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara_s.Draw(staticShader);


		// Load Anmimated Objects
		/* --------------------------------------------------------------------------------------- */

		//Principal Door
		tmp_pd = glm::translate(tmp_pd, glm::vec3(-15.0f,0.0f,0.0f));
		model = glm::rotate(tmp_pd, glm::radians(movpuertap), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		puerta_principal.Draw(staticShader);

		//Bath Door
		model = glm::translate(tmp_bd, glm::vec3(-11.0f, 0.0f, 0.0f));
		model = glm::translate(tmp_bd, glm::vec3(-movpuertab, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		puerta_bano.Draw(staticShader);

		//Room Door
		model = glm::translate(tmp_rd, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movpuertar), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		puerta_recamara.Draw(staticShader);

		//Back Door
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(movpuertat, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		puerta_trasera.Draw(staticShader);

		//Spinning Globe
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(movpuertat, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		mundo.Draw(staticShader);


		//Moving Chair
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(movpuertat, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

	
		// -------------------------------------------------------------------------------------------------------------------------
		// End Scenario
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Draw skybox as last
		// -------------------------------------------------------------------------------------------------------------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limit Framework at 60 fps
		/* ---------------------------------------- */
		deltaTime = SDL_GetTicks() - lastFrame; 
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	skybox.Terminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
/* --------------------------------------------------------------------------------------------------------- */
void my_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Camera Movement
	/* ----------------------------------------------------------- */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.Position.z -= 5.0f;
		lightPosition.z -= 5.0f;
	}	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.Position.z += 5.0f;
		lightPosition.z += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.Position.x += 5.0f;
		lightPosition.x += 10.0f;
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		camera.Position.x -= 5.0f;
		lightPosition.x -= 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.Position.y += 5.0f;
		lightPosition.y += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.Position.y -= 5.0f;
		lightPosition.y -= 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = MovementSpeed * 2.0f;
		

	//Animation Keys
	/* -------------------------------------------------------------------- */
	// Front Door
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && 90 > movpuertap)
		movpuertap += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && 0 < movpuertap)
		movpuertap -= 1.0f;
	// Bath Door
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && 90 > movpuertat)
		movpuertat += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && 0 < movpuertat)
		movpuertat -= 1.0f;
	// Room door
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && 90 > movpuertar)
		movpuertar += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && 0 < movpuertar)
		movpuertar -= 1.0f;
	// Backyard Door
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 25 > movpuertab)
		movpuertab += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && 0 < movpuertab)
		movpuertab -= 1.0f;

	//Animation FrameKeys
	/* ------------------------- */
	// Kitchen Chair
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 90 > movpuertap)
		//animacion = true;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && 0 < movpuertap)
		//animacion = false;
	// Spinnig Globe
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && 90 > movpuertat)
		//animacion = true;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && 0 < movpuertat)
		movpuertab -= 1.0f;
		//animacion = false;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
/* ------------------------------------------------------------------------- */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
/* -------------------------------------------------------------- */
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
/* ---------------------------------------------------------------------- */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}