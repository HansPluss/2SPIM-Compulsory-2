#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "glm/mat4x3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib> 
#include <stb/stb_image.h>
#include "memory"
#include <chrono>
#include "Tick.h"

#include "Resources/Shaders/shaderClass.h"
#include "Texture.h"
#include "Draw.h"
#include "Camera.h"
#include "Collision.h"
#include "Grid.h"
#include "Entity.h"
#include "Component.h"

#include "RenderingSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "InputSystem.h"

#include "InventoryComponent.h"
#include "ItemData.h"
#include "BaseItem.h"
#include "HealthPotion.h"
#include "SpeedPotion.h"
// Some of the code for the spotlight is from the following repo
// https://github.com/VictorGordan/opengl-tutorials.git
// 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1960;
const unsigned int SCR_HEIGHT = 1080;

struct Position {
    double x;
    double y;
};

int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test Win", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGL();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader shaderProgram("default.vert", "default.frag");
    shaderProgram.Activate(); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Shader lightShader("light.vert", "light.frag");
    lightShader.Activate();


    // Initializing player entity
    Entity player;
    player.AddComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
    player.AddComponent<VelocityComponent>();
    player.AddComponent<AccelerationComponent>();
    player.AddComponent<InputComponent>();
    player.AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "cube");

    InputSystem inputSystem;

    //Entity Initializing
    Entity entites;
    entites.AddComponent<PositionComponent>(0.0f,0.0f,0.0f);
    entites.AddComponent<RenderComponent>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f),"sphere");
    entites.AddComponent<VelocityComponent>();
    entites.AddComponent<AccelerationComponent>();

    Entity boundingbox;
    boundingbox.AddComponent<PositionComponent>(0.0f,0.0f,0.0f);
    boundingbox.AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 1.0f, 10.0f), "plane");
   
    PositionComponent* position = entites.GetComponent<PositionComponent>();
    if (position) {
        std::cout << "Position: (" << position->position.x << ", "
            << position->position.y << ", "
            << position->position.z << ")" << std::endl;
    }

    //Intializing System
    RenderingSystem renderSystem;
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;

	renderSystem.initalize(entites);
	renderSystem.initalize(boundingbox);
	renderSystem.initalize(player);
   //tick

    std::vector<Tick*> Ticks;

    //Making Grid for better collison handeling  
    int cellSize = 8; 
    int gridSizeX = 1000; 
    int gridSizeZ = 1000; 
    std::unique_ptr<Grid> m_grid = std::make_unique<Grid>(gridSizeX, gridSizeZ, cellSize);
    glm::vec4 treeBounds(0, 0, gridSizeX, gridSizeZ);
   

    //Initializing queue ball
  


	//inventory TESTING
	InventoryComponent inventory;
	HealthPotion healthPotion;
	SpeedPotion speedPotion;
	ItemData healthPotionData(&healthPotion, healthPotion.GetItemID(), true, 5, 1);
	ItemData SpeedPotionData(&speedPotion, speedPotion.GetItemID(), true, 5, 1);
	inventory.AddItem(healthPotionData);
	inventory.AddItem(SpeedPotionData);
	inventory.UseItem(0);
	inventory.UseItem(1);
    inventory.UseItem(1);


    

    

    std::vector<Texture> textures;
    
    char basePath[] = "Resources/Textures/";
    char filetype[] = ".png";

    for (int i = 1; i <= 15; ++i) {
        std::string tempPath = std::string(basePath) + std::to_string(i) + filetype;  // Use std::string to build the file path
        char filePath[31];  // Adjust size as needed
        strcpy_s(filePath, tempPath.c_str());  // Copy std::string into C-style string (char array)

        Texture tt(filePath, shaderProgram);  // Pass the C-style string to the constructor
        textures.push_back(tt);

       
    }

    //camera FOV & starting position
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 40.0f, 0.0f));

    //Initalizing textures
    Texture wood("Resources/Textures/wood.png", shaderProgram);
    Texture green("Resources/Textures/green.jpg", shaderProgram);
    Texture queball("Resources/Textures/queball.png", shaderProgram);

    //scene light
    glm::vec4 lightColor = glm::vec4(1.0f, 0.9f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    int timeSinceStart = 0;

    auto previousTime = std::chrono::high_resolution_clock::now();

    // ---------------------------------------------------------------------------------------------------------------------------
    //                                                        Main Loop
    // ---------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate delta time (in seconds)
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        // Update previousTime for the next frame
        previousTime = currentTime;
        // Now you can use deltaTime.count() which gives you the delta time in seconds
        float dt = deltaTime.count();
        //updates Tick
        
        for (Tick* obj : Ticks)
        {
            obj->UpdateTick(dt);
        }

        camera.Inputs(window);

        //Set render distance and FOV
        glm::mat4 viewproj = camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");

        


      
 

        // BALLS
        glBindTexture(GL_TEXTURE_2D, queball.texture);
        //Cube0.Render(shaderProgram, viewproj);

        
        glBindTexture(GL_TEXTURE_2D, wood.texture);
        renderSystem.Render(entites, shaderProgram, viewproj);

        glBindTexture(GL_TEXTURE_2D, textures[0].texture);
        renderSystem.Render(boundingbox, shaderProgram, viewproj);
        physicsSystem.Update(entites, dt);
        physicsSystem.ApplyForce(entites, glm::vec3(-1.0f, 0.0f, 1.0f));
        collisionSystem.BarycentricCoordinates(entites, boundingbox, physicsSystem);
        collisionSystem.InvAABBCollision(boundingbox, entites, dt);
       
        
        
        // Player
        inputSystem.processInput(player, window);
        physicsSystem.Update(player, dt);
        glBindTexture(GL_TEXTURE_2D, green.texture);
        renderSystem.Render(player, shaderProgram, viewproj);
        collisionSystem.BarycentricCoordinates(player, boundingbox, physicsSystem);




       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
  
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
