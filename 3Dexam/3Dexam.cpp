#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "glm/mat4x3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib> 
#include <stb/stb_image.h>
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
#include "memory"
#include <chrono>
// Some of the code for the spotlight is from the following repo
// https://github.com/VictorGordan/opengl-tutorials.git
// 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Draw& cube0);

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


    //Entity Initializing
    Entity entites;
    entites.AddComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
    entites.AddComponent<RenderComponent>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f),"sphere");
    PositionComponent* position = entites.GetComponent<PositionComponent>();
    if (position) {
        std::cout << "Position: (" << position->position.x << ", "
            << position->position.y << ", "
            << position->position.z << ")" << std::endl;
    }

    //Intializing Rendersystem
    RenderingSystem renderSystem;
   

    //Making Grid for better collison handeling  
    int cellSize = 8; 
    int gridSizeX = 1000; 
    int gridSizeZ = 1000; 
    std::unique_ptr<Grid> m_grid = std::make_unique<Grid>(gridSizeX, gridSizeZ, cellSize);
    glm::vec4 treeBounds(0, 0, gridSizeX, gridSizeZ);
   

    //Initializing objects
    Draw Cube0;
    Cube0.DrawSphere(glm::vec3(23, 100, 145), glm::vec3( -15, 0, 0), glm::vec3(0.45, 0.45, 0.45));
    m_grid->AddBaLL(&Cube0); 


    Draw BoundingBox0;
    BoundingBox0.DrawBoundingBox(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(20, 1, 10));
    Draw TableSurface;
    TableSurface.DrawPlane(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(20, 1, 10));

    Collision collision;

    BoundingBox0.SetMass(10000.0f);

    int ballNumber = 1;
    std::vector<Position> ballPositions;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col <= row; ++col) {
            // Calculate x and y positions based on row and column
            double x = col * 1.f - row * 0.5f;  // Center balls by subtracting row * radius
            double y = -row * 1.f;                   // Increase y with each row

            ballPositions.push_back({ x , -y +10});
            //std::cout << "Ball " << ballNumber << ": (" << x << ", " << y << ")\n";
            ++ballNumber;
        }
    }

    std::vector<Texture> textures;
    std::vector<Draw> balls;
    char basePath[] = "Resources/Textures/";
    char filetype[] = ".png";

    for (int i = 1; i <= 15; ++i) {
        std::string tempPath = std::string(basePath) + std::to_string(i) + filetype;  // Use std::string to build the file path
        char filePath[31];  // Adjust size as needed
        strcpy_s(filePath, tempPath.c_str());  // Copy std::string into C-style string (char array)

        Texture tt(filePath, shaderProgram);  // Pass the C-style string to the constructor
        textures.push_back(tt);

        Draw ball;
        ball.DrawSphere(glm::vec3(23, 100, 145), glm::vec3(ballPositions[i-1].y, 0, ballPositions[i-1].x), glm::vec3(0.45, 0.45, 0.45));
       
        balls.push_back(ball);
     
        m_grid->AddBaLL(&ball);
    }

    //camera FOV & starting position
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 40.0f, 0.0f));

    //Initalizing textures
    Texture wood("Resources/Textures/wood.png", shaderProgram);
    Texture green("Resources/Textures/green.jpg", shaderProgram);
    Texture queball("Resources/Textures/queball.png", shaderProgram);

    //scene light
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
    while (!glfwWindowShouldClose(window))
    {

        processInput(window, Cube0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate delta time (in seconds)
        std::chrono::duration<float> deltaTime = currentTime - previousTime;

        // Update previousTime for the next frame
        previousTime = currentTime;

        // Now you can use deltaTime.count() which gives you the delta time in seconds
        float dt = deltaTime.count();
        camera.Inputs(window);

        //Set render distance and FOV
        glm::mat4 viewproj = camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");


        // UNCOMMENT FOR ROTATION
        Cube0.RotateCube(dt);
        //Cube1.RotateCube(dt);
        //Cube2.RotateCube(dt);
        Cube0.Update(dt, m_grid.get());
        for (size_t i = 0; i < balls.size(); ++i) {
            balls[i].Update(dt, m_grid.get());
            balls[i].FollowPlayer(Cube0, 2.0f);
            balls[i].RotateCube(dt);
        }
 

        // balls
        glBindTexture(GL_TEXTURE_2D, queball.texture);
        Cube0.Render(shaderProgram, viewproj);
        glBindTexture(GL_TEXTURE_2D, wood.texture);
        renderSystem.Render(entites, shaderProgram, viewproj);
        for (int i = 0; i < balls.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, textures[i].texture);
            balls[i].Render(shaderProgram, viewproj);
            collision.InvAABBCollision(BoundingBox0, balls[i], dt);
          
            collision.calculateBarycentricCoordinates(balls[i], TableSurface);
        
        }


        // walls
        glBindTexture(GL_TEXTURE_2D, wood.texture);
        BoundingBox0.Render(shaderProgram, viewproj);
        glBindTexture(GL_TEXTURE_2D, green.texture);
        TableSurface.Render(shaderProgram, viewproj);

        //wall collision
        collision.InvAABBCollision(BoundingBox0, Cube0, dt);
        collision.calculateBarycentricCoordinates(Cube0, TableSurface);
        
        //spheres collision
        collision.UpdateCollision(m_grid.get(), dt); 
        //collision.UpdateQTCollision(tree, balls, Cube0,dt);
        // 
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
void processInput(GLFWwindow* window, Draw& cube0)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        glm::vec3 force(0.0f, -9.810f, 20.0f);

        cube0.ApplyForce(force);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 force(0.0f, -9.810f, -20.0f);

        cube0.ApplyForce(force);

    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 force(-20.0f, -9.810f, 0.0f);

        cube0.ApplyForce(force);

    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 force(20.0f, -9.810f, 0.0f);

        cube0.ApplyForce(force);

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
