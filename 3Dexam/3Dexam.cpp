#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "glm/mat4x3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib> 
#include <stb/stb_image.h>
#include <chrono>

#include "Resources/Shaders/shaderClass.h"
#include "Texture.h"
#include "Camera.h"
#include "Grid.h"
#include "Entity.h"
#include "Player.h"
#include "Component.h"
#include "Enemy.h"
#include "Projectile.h"

//can be removed if unused 
#include "Tick.h"
#include "memory" // for smart pointers

//systems
#include "EntityManager.h"
#include "RenderingSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "Resources/Systems/CombatSystem.h"

// Some of the code for the spotlight is from the following repo
// https://github.com/VictorGordan/opengl-tutorials.git
// 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool del = false;
bool spawnObj = false;
bool isEKeyPressed = false;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

double scrollY = 0.0;  // Store scroll amount

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Get the InputSystem from user pointer
    InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));

    scrollY += yoffset;
   
    if (scrollY > 8.0) {
        scrollY = 1.0;
    }
    else if (scrollY < 0.0) {
        scrollY = 8.0;
    }
    inputSystem->SetMouseInput(scrollY);
}

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basic RPG Game", NULL, NULL);
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Shader setup
    Shader shaderProgram("default.vert", "default.frag");
    shaderProgram.Activate();

    Shader lightShader("light.vert", "light.frag");
    lightShader.Activate();

    // Entities setup
    EntityManager manager;
   

    Enemy& enemy = manager.CreateEntityDerivedFromClass<Enemy>();
    enemy.GetComponent<AIComponent>()->speed = 5.0f;


    // Player Entity
    Player player;
    InputSystem inputSystem;
    glfwSetWindowUserPointer(window, &inputSystem);
    glfwSetScrollCallback(window, scroll_callback);

    // woodenBall Entity


    // planeObject Entity
    Entity planeObject;
    planeObject.AddComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
    planeObject.AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), "terrain");



    // Intializing Systems
    RenderingSystem renderSystem;
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;
    CombatSystem combatSystem;

    renderSystem.initalize(enemy);
    renderSystem.initalize(planeObject);
    renderSystem.initalize(player);

    std::vector<Tick*> Ticks;

    // FOR TESTING PURPOSE
    std::vector<Entity*> myEntities;
   

    for (auto& entity : myEntities) {
        renderSystem.initalize(*entity);

    }

    // Setting up grid for collison optimization  
    int cellSize = 8;
    int gridSizeX = 1000;
    int gridSizeZ = 1000;
    std::unique_ptr<Grid> m_grid = std::make_unique<Grid>(gridSizeX, gridSizeZ, cellSize);
    glm::vec4 treeBounds(0, 0, gridSizeX, gridSizeZ);
   
    m_grid->AddBaLL(&player);
    m_grid->AddBaLL(&enemy);


    std::vector<Texture> textures;

    char basePath[] = "Resources/Textures/";
    char filetype[] = ".png";

    for (int i = 1; i <= 15; ++i) {
        std::string tempPath = std::string(basePath) + std::to_string(i) + filetype;    // Use std::string to build the file path
        char filePath[31];
        strcpy_s(filePath, tempPath.c_str());                                           // Copy std::string into C-style string (char array)

        Texture tt(filePath, shaderProgram);                                            // Pass the C-style string to the constructor
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

    // myEntities.push_back(&newEntity);
    auto previousTime = std::chrono::high_resolution_clock::now();
    Collision collision;

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
        previousTime = currentTime;
        float dt = deltaTime.count();

        //updates Tick
        for (Tick* obj : Ticks)
        {
            obj->UpdateTick(dt);
        }

        //Setup camera settings and inputs
        camera.Inputs(window);
        glm::mat4 viewproj = camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");
        camera.Position = glm::vec3(player.GetComponent<PositionComponent>()->position.x, camera.Position.y, player.GetComponent<PositionComponent>()->position.z + 25);

        //collision detection
        collision.UpdateCollision(m_grid.get(), dt);

        // BALLS
      
        glBindTexture(GL_TEXTURE_2D, green.texture);
        renderSystem.Render(planeObject, shaderProgram, viewproj);
  
        // Player
        inputSystem.processInput(player, window);
        physicsSystem.Update(player, dt);
        glBindTexture(GL_TEXTURE_2D, textures[1].texture);
        renderSystem.Render(player, shaderProgram, viewproj);
        collisionSystem.BarycentricCoordinates(player, planeObject, physicsSystem);

        physicsSystem.Update(enemy, dt);
        collisionSystem.BarycentricCoordinates(enemy, planeObject, physicsSystem);
        enemy.FollowEntity(enemy, player, physicsSystem);
        renderSystem.Render(enemy, shaderProgram, viewproj);

        if (spawnObj) {
            Projectile& bullet = manager.CreateEntityDerivedFromClass<Projectile>();
            renderSystem.initalize(bullet);
            bullet.MoveProjectile(player, physicsSystem);
            myEntities.push_back(&bullet);
            spawnObj = false;
        }
        combatSystem.Update(dt);
        for (auto& entity : myEntities) {
            glBindTexture(GL_TEXTURE_2D, textures[4].texture);
            renderSystem.Render(*entity, shaderProgram, viewproj);
            collisionSystem.BarycentricCoordinates(*entity, planeObject, physicsSystem);


            if (collisionSystem.SphereCollision(*entity, enemy, dt)) {

                combatSystem.DealDamage(*entity, enemy);
                entity->isMarkedForDeletion = true;

            }
            if (Projectile* projectile = dynamic_cast<Projectile*>(entity)) {
                // It's a Projectile
                //std::cout << "This is a projectile!" << std::endl;
                if (!projectile->isMarkedForDeletion) {
                    projectile->DespawnTimer(dt);

                }
            }

        }
        for (auto& entity : myEntities) {
            glBindTexture(GL_TEXTURE_2D, wood.texture);
            physicsSystem.Update(*entity, dt);

        }
        if (del) {
            manager.CleanupEntities(myEntities);
            del = false;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clearing GLFW resources
    glfwTerminate();
    return 0;
}

// Process all input
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        del = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        // Only trigger if the key wasn't pressed in the previous frame
        if (!isEKeyPressed) {
            std::cout << "E key pressed!" << std::endl;
            // Handle the action you want on key press
            spawnObj = true;
        }
        // Mark that the key is now pressed
        isEKeyPressed = true;
    }
    // Check if E key is released
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        // Mark that the key has been released
        isEKeyPressed = false;
    }

}

// Dynamic window size
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
