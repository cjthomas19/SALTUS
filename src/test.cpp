#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/shader_s.h"
#include "renderer/stb_image.h"
#include "renderer/camera.h"
#include "renderer/mesh.h"
#include "game/terrain.h"

//TODO remove global variables
// Forward declarations of callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
// Should put this in a utility file
unsigned int loadTexture(const char* path);

// Screen size settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(32.0f,3.0f,60.0f));

// Initialize global variables to store previous mouse coordinates (bad practice)
float lastX = SCR_WIDTH/2.0;
float lastY = SCR_HEIGHT/2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    if(glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window,GLFW_RAW_MOUSE_MOTION,GLFW_TRUE);
    }

    // Bring window to forefront and disable cursor
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Cube vertices
    // TODO : move this to another file
    float v_raw[] = {    // texcoords           // normals
        // front
        0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,0.0f,1.0f,// top right
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,0.0f,1.0f,// bottom right
       -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,0.0f,1.0f,// bottom left
       -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,0.0f,1.0f,// top left
       // back
        0.5f,  0.5f,-0.5f,   1.0f, 1.0f, 0.0f,0.0f,-1.0f,// top right
        0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,0.0f,-1.0f,// bottom right
       -0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 0.0f,0.0f,-1.0f,// bottom left
       -0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,0.0f,-1.0f,// top left
       // left
       -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, -1.0f,0.0f,0.0f,// top right
       -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, -1.0f,0.0f,0.0f,// bottom right
       -0.5f, -0.5f,-0.5f,   0.0f, 0.0f, -1.0f,0.0f,0.0f,// bottom left
       -0.5f,  0.5f,-0.5f,   0.0f, 1.0f, -1.0f,0.0f,0.0f,// top left
       // right
        0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 1.0f,0.0f,0.0f, // top right
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 1.0f,0.0f,0.0f,// bottom right
        0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 1.0f,0.0f,0.0f,// bottom left
        0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 1.0f,0.0f,0.0f,// top left
        // top
        0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,1.0f,0.0f,// top right
        0.5f,  0.5f, 0.5f,   1.0f, 0.0f,  0.0f,1.0f,0.0f,// bottom right
       -0.5f,  0.5f, 0.5f,   0.0f, 0.0f,  0.0f,1.0f,0.0f,// bottom left
       -0.5f,  0.5f,-0.5f,   0.0f, 1.0f,  0.0f,1.0f,0.0f,// top left
       // bottom
        0.5f, -0.5f, -0.5f,   1.0f, 1.0f,0.0f,-1.0f,0.0f, // top right
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,-1.0f,0.0f,// bottom right
       -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,-1.0f,0.0f,// bottom left
       -0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 0.0f,-1.0f,0.0f // top left
    };

    // Cube indices
    // TODO : move this to another file
    unsigned int i_raw[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9,11,
        9,10,11,
        12,13,15,
        13,14,15,
        16,17,19,
        17,18,19,
        20,21,23,
        21,22,23
    };

    // Generate Terrain
    TerrainGenerator tg{};

    
    // set positions
    glm::vec3 cubePosition = glm::vec3( 0.0f,  0.0f,  0.0f);
    glm::vec3 lightPosition(-10.0f,10.0f,-10.0f);

   // General rendering setup 
    
    // LIGHT CUBE
    std::vector<Vertex> vertices {};

    for(unsigned int i = 0; i < sizeof(v_raw)/sizeof(float); i+=8) {
        vertices.push_back(Vertex {glm::vec3 {v_raw[i],v_raw[i+1],v_raw[i+2]}, glm::vec3 {v_raw[i+5],v_raw[i+6],v_raw[i+7]}, glm::vec3 {v_raw[i+3],v_raw[i+4],0}});
    }

    std::vector<unsigned int> indices {};

    for(unsigned int i = 0; i < sizeof(i_raw)/sizeof(unsigned int); i++) {
        indices.push_back(i_raw[i]);
    }

    // texture setup
    std::vector<Texture> textures {};

    unsigned int diffuseMap = loadTexture("resources/container2.png");
    unsigned int specularMap = loadTexture("resources/container2_specular.png");
    textures.push_back(Texture {diffuseMap,"texture_diffuse"});
    textures.push_back(Texture {specularMap,"texture_specular"});

    Mesh cubeMesh = Mesh(vertices,indices,textures);
    Mesh lightMesh = Mesh(vertices,indices,textures);

    //Shader setup 
    Shader cubeShader("shaders/cube.vs","shaders/cube.fs");
    Shader lightCubeShader("shaders/lightcube.vs","shaders/lightcube.fs");
    Shader landShader("shaders/land.vs","shaders/land.fs");
    
    cubeShader.use();
    cubeShader.setInt("material.texture_diffuse1",0);
    cubeShader.setInt("material.texture_specular1",1);

    landShader.use();
    landShader.setVec3("material.color",1.0f,1.0f,1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
	// Calcuate elapsed time 
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

	// Calculate y position from heightmap
        //int x = floor(camera.Position.x*subdiv);
        //int z = floor(camera.Position.z*subdiv);
        //float sx = camera.Position.x * subdiv - x;
        //float sz = camera.Position.z * subdiv - z;
        //float n0 = interpolate(heights[z][x],heights[z][x+1],sx);
        //float n1 = interpolate(heights[z+1][x],heights[z+1][x+1],sx);

        //camera.Position.y = interpolate(n0,n1,sz)+0.2f;

        // input
        // -----
        processInput(window);

        // render
        // -----
        glClearColor(0.03f,0.0f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //DRAW CUBE
	// TODO : move all this into a model-handler class
        cubeShader.use();
        cubeShader.setVec3("light.position",camera.Position);
        cubeShader.setVec3("light.direction",camera.Front);
        cubeShader.setFloat("light.inCutOff",glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("light.outCutOff",glm::cos(glm::radians(17.5f)));

        cubeShader.setVec3("plight.position",lightPosition);
        cubeShader.setFloat("plight.constant",  1.0f);
        cubeShader.setFloat("plight.linear",    0.09f);
        cubeShader.setFloat("plight.quadratic", 0.032f);

        cubeShader.setVec3("viewPos",camera.Position);

        cubeShader.setVec3("light.ambient",0.2f,0.2f,0.2f);
        cubeShader.setVec3("light.diffuse",0.5f,0.5f,0.5f);
        cubeShader.setVec3("light.specular",1.0f,1.0f,1.0f);
        
        cubeShader.setVec3("plight.ambient",0.2f,0.2f,0.2f);
        cubeShader.setVec3("plight.diffuse",0.5f,0.5f,0.5f);
        cubeShader.setVec3("plight.specular",1.0f,1.0f,1.0f);
        
        cubeShader.setFloat("material.shininess",64);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = camera.getViewMatrix();
       
        cubeShader.setMatrix4fv("projection",projection);
        cubeShader.setMatrix4fv("view",view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition);
        cubeShader.setMatrix4fv("model",model);

        cubeMesh.Draw(cubeShader);

        //DRAW LIGHT
        lightCubeShader.use();
        lightCubeShader.setMatrix4fv("projection",projection);
        lightCubeShader.setMatrix4fv("view",view);

        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPosition);
        model = glm::scale(model,glm::vec3(3.0f));
        float angle = glfwGetTime();
        model = glm::rotate(model,angle,glm::vec3(0.0,1.0,0.0));
        lightCubeShader.setMatrix4fv("model",model);

        lightMesh.Draw(lightCubeShader);

        landShader.use();
        landShader.setVec3("dlight.direction",1.0f,-1.0f,1.0f);
        landShader.setVec3("dlight.ambient",0.2f,0.1f,0.3f);
        landShader.setVec3("dlight.diffuse",0.1f,0.0f,0.3f);
        landShader.setVec3("dlight.specular",1.0f,1.0f,1.0f);

        landShader.setVec3("slight.position",camera.Position);
        landShader.setVec3("slight.direction",camera.Front);
        landShader.setFloat("slight.inCutOff",glm::cos(glm::radians(12.5f)));
        landShader.setFloat("slight.outCutOff",glm::cos(glm::radians(45.0f)));

        landShader.setVec3("slight.ambient",0.0f,0.0f,0.0f);
        landShader.setVec3("slight.diffuse",0.3f,0.3f,0.1f);
        landShader.setVec3("slight.specular",0.5f,0.5f,0.0f);

        landShader.setFloat("slight.constant",  1.0f);
        landShader.setFloat("slight.linear",    0.09f);
        landShader.setFloat("slight.quadratic", 0.032f);

        landShader.setMatrix4fv("projection",projection);
        landShader.setMatrix4fv("view",view);
        landShader.setVec3("viewPos",camera.Position);
        landShader.setFloat("material.shininess",64);
        model = glm::mat4(1.0f);
        landShader.setMatrix4fv("model",model);

        tg.draw(landShader);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // update / physics
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.

    cubeMesh.terminate();
    lightMesh.terminate();
    tg.terminate();
    glfwTerminate();
    return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S )==GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT,deltaTime);
    }
}

// Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// Whenever the mouse moves this callback function executes
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset); 
}

// TODO : move into utility file
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1,&textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if(nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        } else {
            std::cout << "unrecognized format" << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        stbi_image_free(data);
    } else {
        std::cout << "texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

