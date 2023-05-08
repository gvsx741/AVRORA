#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <map>

#include "Renderer/Camera.h"
#include "Renderer/Draw.h"





//initiate variables & arrays
bool firstMouse = true;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

unsigned int vertex_VBO;
unsigned int texCoord_VBO;
unsigned int normal_VBO;
unsigned int VAO;
unsigned int transparentVAO, transparentVBO;
unsigned int texture1, texture2, texture3, specularMap, grassTex;
unsigned int counter; 

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float diffTime = 0.0f;
float lastFrame = 0.0f;
float prevFrame = 0.0f;
float currentFrame;

float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

float CubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,         
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,      
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,    

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,       
};

float CubeNormal[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,

   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
};

float CubeTexCoord[] = {
   0.0f, 0.0f,
   1.0f, 1.0f,
   1.0f, 0.0f,
   1.0f, 1.0f,
   0.0f, 0.0f,
   0.0f, 1.0f,

   0.0f, 0.0f,
   1.0f, 0.0f,
   1.0f, 1.0f,
   1.0f, 1.0f,
   0.0f, 1.0f,
   0.0f, 0.0f,

   1.0f, 0.0f,
   1.0f, 1.0f,
   0.0f, 1.0f,
   0.0f, 1.0f,
   0.0f, 0.0f,
   1.0f, 0.0f,

   1.0f, 0.0f,
   0.0f, 1.0f,
   1.0f, 1.0f,
   0.0f, 1.0f,
   1.0f, 0.0f,
   0.0f, 0.0f,

   0.0f, 1.0f,
   1.0f, 1.0f,
   1.0f, 0.0f,
   1.0f, 0.0f,
   0.0f, 0.0f,
   0.0f, 1.0f,

   0.0f, 1.0f,
   1.0f, 0.0f,
   1.0f, 1.0f,
   1.0f, 0.0f,
   0.0f, 1.0f,
   0.0f, 0.0f
};

float transparentVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

float quadVertices[] = { 
     //pos         //coords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

//positions for lamps
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f, 1.0f, 0.0f),
    glm::vec3(9.2f, 3.0f, 4.5f),
    glm::vec3(3.2f, 3.0f, 9.0f),
};

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

//faces for the cubemap
std::vector<std::string> faces {
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//init the perspective
glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

//define the functions
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void FPSLimiter(GLFWwindow* pWindow);
    
int main(int argc, char** argv)
{
    //init a path to resources for get resoureces
    const std::string resPath = GetResourcePath(argv[0]);

    if (!glfwInit()) {
        std::cout << "failed to init glfw" << std::endl;
        return -1;
    }

    #ifdef DEBUG
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif // DEBUG





    //Initiate object of window
    GLFWwindow* pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AVRORA", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWindow);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(pWindow, mouse_callback);

    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);





    //load OpenGL
    if (!gladLoadGL()) {
        std::cout << "Failed to load GLAD!" << std::endl;
        return -1;
    }
    #ifdef DEBUG
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    #endif // DEBUG





    //init shaders
    Renderer::Shader ShCube(resPath, "cube.vs", "cube.fs");
    Renderer::Shader ShSingleColor(resPath, "singleColor.vs", "singleColor.fs");
    Renderer::Shader ShSingleTexture(resPath, "singleTexture.vs", "singleTexture.fs");
    Renderer::Shader ShBackpack(resPath, "backpack.vs", "backpack.fs");
    Renderer::Shader ShBackpackExplode(resPath, "backpackExplode.vs", "backpackExplode.fs", "backpackExplode.gs");
    Renderer::Shader ShScreen(resPath, "screen.vs", "screen.fs");
    Renderer::Shader ShSkybox(resPath, "skybox.vs", "skybox.fs");
    Renderer::Shader ShReflect(resPath, "reflect.vs", "reflect.fs");
    Renderer::Shader ShQuad(resPath, "quad.vs", "quad.fs");





    //texture upload 
    texture1 = TextureLoad(resPath, "textures/container.jpg");
    texture2 = TextureLoad(resPath, "textures/kotya.jpg");  
    texture3 = TextureLoad(resPath, "textures/container2.jpg");
    specularMap = TextureLoad(resPath, "textures/container2_specular.jpg");
    grassTex = TextureLoad(resPath, "textures/grass.png");
    //loading textures for the cubemap
    unsigned int cubemapTexture = loadCubemap(resPath + "textures/skybox/", faces);





    //framebuffer initiation
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a multisampled color attachment texture
    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    // configure second post-processing framebuffer
    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // create a color attachment texture
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
    }

    //bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);





    //vbo, vao & binding
    //create cube's vbo, vao, binding
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormal), CubeNormal, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTexCoord), CubeTexCoord, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //create transparent's vao & bind
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //create quad's VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));





    //setting uniforms for shaders
    ShCube.Use();
    ShCube.SetUniform("projection", projection);
    
    ShCube.SetUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ShCube.SetUniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ShCube.SetUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ShCube.SetUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);
    
    ShCube.SetUniform("pointLights[0].position", pointLightPositions[0]);
    ShCube.SetUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    ShCube.SetUniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    ShCube.SetUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    ShCube.SetUniform("pointLights[0].constant", 1.0f);
    ShCube.SetUniform("pointLights[0].linear", 0.09f);
    ShCube.SetUniform("pointLights[0].quadratic", 0.032f);
    
    ShCube.SetUniform("pointLights[1].position", pointLightPositions[1]);
    ShCube.SetUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    ShCube.SetUniform("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    ShCube.SetUniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    ShCube.SetUniform("pointLights[1].constant", 1.0f);
    ShCube.SetUniform("pointLights[1].linear", 0.09f);
    ShCube.SetUniform("pointLights[1].quadratic", 0.032f);
    
    ShCube.SetUniform("pointLights[2].position", pointLightPositions[2]);
    ShCube.SetUniform("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    ShCube.SetUniform("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    ShCube.SetUniform("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    ShCube.SetUniform("pointLights[2].constant", 1.0f);
    ShCube.SetUniform("pointLights[2].linear", 0.09f);
    ShCube.SetUniform("pointLights[2].quadratic", 0.032f);
    
    ShCube.SetUniform("material.diffuse", 0);
    ShCube.SetUniform("material.specular", 1);
    ShCube.SetUniform("material.shininess", 32.0f);

    ShSingleColor.Use();
    ShSingleColor.SetUniform("projection", projection);

    ShSingleTexture.Use();
    ShSingleTexture.SetUniform("projection", projection);

    ShSkybox.Use();
    ShSkybox.SetUniform("projection", projection);

    ShReflect.Use();
    ShReflect.SetUniform("projection", projection);

    ShBackpack.Use();
    ShBackpack.SetUniform("projection", projection);
    ShBackpack.SetUniform("light.position", pointLightPositions[0]);
    ShBackpack.SetUniform("light.ambient", 0.05f, 0.05f, 0.05f);
    ShBackpack.SetUniform("light.diffuse", 0.4f, 0.4f, 0.4f);
    ShBackpack.SetUniform("light.specular", 0.5f, 0.5f, 0.5f);

    ShBackpackExplode.Use();
    ShBackpackExplode.SetUniform("projection", projection);





    //init the model class and load backpack model 
    Model MBackpack(resPath, "models/backpack/backpack.obj");





    //time complite project
    #ifdef DEBUG
        std::cout << glfwGetTime() << std::endl;
    #endif // DEBUG





    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

    while (!glfwWindowShouldClose(pWindow))
    {   
        //for limit FPS to 60 frames per second
        FPSLimiter(pWindow);

        processInput(pWindow);
        
        //initiate view
        glm::mat4 view = camera.GetViewMatrix();
        
        //binding the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        //clear color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        //clear depth and stencil buffers
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        //enable depth and stencil buffers
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        //setting for stencil buffer
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0x00);





        //drawing objects
        //draw skybox
        DrawSkybox(view, ShSkybox, vertex_VBO, cubemapTexture); 
            
        //draw reflect cube
        DrawReflectCube(view, ShReflect, vertex_VBO, glm::vec3(3.5f, 1.5f, 1.0f), cubemapTexture, camera.Position);
        
        //draw other obj
        DrawModel(view, camera.Position, ShBackpackExplode, MBackpack, glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(1.0f));
        DrawModel(view, camera.Position, ShBackpack, MBackpack, glm::vec3(4.0f, 0.0f, -5.0f), glm::vec3(1.0f));

        DrawCube(true, view, camera.Position, ShCube, vertex_VBO, glm::vec3(1.0f, 1.3f, 6.0f), glm::vec3(1.02f), texture3, specularMap, (float)glfwGetTime() - 98, glm::vec3(0.0f, 1.0f, 1.0f), &ShSingleColor);
        DrawCube(false, view, camera.Position, ShCube, vertex_VBO, glm::vec3(2.0f, 2.3f, 8.0f), glm::vec3(1.02f), texture3, specularMap, (float)glfwGetTime() + 30, glm::vec3(1.0f, 1.0f, 1.0f), &ShSingleColor);
        DrawCube(false, view, camera.Position, ShCube, vertex_VBO, glm::vec3(2.0f, -0.3f, 5.0f), glm::vec3(1.02f), texture3, specularMap, (float)glfwGetTime() + (float)glfwGetTime() / 2, glm::vec3(0.0f, 1.0f, 0.0f), &ShSingleColor);

        DrawLamp(view, ShSingleColor, vertex_VBO, pointLightPositions[0], glm::vec3(0.25f), lightColor);
        DrawLamp(view, ShSingleColor, vertex_VBO, pointLightPositions[1], glm::vec3(0.05f), lightColor);
        DrawLamp(view, ShSingleColor, vertex_VBO, pointLightPositions[2], glm::vec3(0.05f), lightColor);

        DrawGrass(view, ShSingleTexture, transparentVAO, glm::vec3(-5.0f, -0.8f, -2.0f), grassTex);
        DrawGrass(view, ShSingleTexture, transparentVAO, glm::vec3(-5.0f, -0.5f, -5.0f), grassTex);
        DrawGrass(view, ShSingleTexture, transparentVAO, glm::vec3(-4.0f, 0.0f, -5.0f), grassTex);
        DrawGrass(view, ShSingleTexture, transparentVAO, glm::vec3(-6.0f, -1.0f, -5.0f), grassTex);






        //using our framebuffer & MSAA
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        //bind default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Disable depth and stencil buffers to view image from our framebuffer
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        
        ShScreen.Use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);





        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }





    //clearing
    glDeleteTextures(1, &texture1); 
    glDeleteTextures(1, &texture2);
    glDeleteTextures(1, &texture3);
    glDeleteTextures(1, &specularMap);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &vertex_VBO);
    glDeleteVertexArrays(1, &texCoord_VBO);
    glDeleteVertexArrays(1, &normal_VBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &vertex_VBO);
    glDeleteFramebuffers(1, &framebuffer);
    glfwTerminate;

    return -1;
}





void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void FPSLimiter(GLFWwindow* pWindow) {
    //for border FPS to 60 frames per second
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //for see the FPS 
    diffTime = currentFrame - prevFrame;
    ++counter;

    if (diffTime >= 1.0 / 30.0) {
        std::string FPS = std::to_string((1.0 / diffTime) * counter);
        std::string ms = std::to_string((diffTime / counter) * 1000);
        std::string title = FPS + "FPS / " + ms + "ms";
        glfwSetWindowTitle(pWindow, title.c_str());
        prevFrame = currentFrame;
        counter = 0;
    }
}