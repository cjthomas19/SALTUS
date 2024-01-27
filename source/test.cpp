#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx/shader_s.h"
#include "gfx/stb_image.h"
#include "gfx/camera.h"
#include "gfx/mesh.h"
#include "util/perlin.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char* path);

// Screen size settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(32.0f,3.0f,32.0f));
bool firstMouse = true;
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
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    if(glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window,GLFW_RAW_MOUSE_MOTION,GLFW_TRUE);
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);

    // Set up vertex shader and buffers
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

    constexpr int W = 64;
    constexpr int subdiv = 2;
    constexpr int N = W * subdiv;

    float heights[N][N] {};
    std::vector<Vertex> vs((N-1) * (N-1) * 6);
    std::vector<unsigned int> is((N-1)*(N-1)*6);
    std::vector<Texture> ts {};

    float w0 = 1.0f/W;
    int octaves = 20;
    float pers = 0.5;

    for(int z = 0; z < N; ++z) {
        for(int x = 0; x < N; ++x) {
            for (int o = 0; o < octaves; ++o) {
                heights[z][x] += 20*pow(pers,o)*perlin(x*w0*pow(2,o)/(float)subdiv,z*w0*pow(2,o)/(float)subdiv);
            }
        }
    }

    for(int iz = 0; iz < N-1; ++iz) {
        for(int ix = 0; ix<N-1; ++ix) {
            float x = (float)ix / subdiv;
            float z = (float)iz / subdiv;
            
            glm::vec3 c00 = glm::vec3(x,heights[iz][ix],z);
            glm::vec3 c01 = glm::vec3((x+1.0f/subdiv),heights[iz][ix+1],z);
            glm::vec3 c10 = glm::vec3(x,heights[iz+1][ix],(z+1.0f/subdiv));
            glm::vec3 c11 = glm::vec3((x+1.0f/subdiv),heights[iz+1][ix+1],(z+1.0f/subdiv));

            glm::vec3 n1 = glm::normalize(-glm::cross(c01-c00,c10-c00));
            glm::vec3 n2 = glm::normalize(glm::cross(c11-c01, c11-c10));
            //glm::vec3 n = glm::vec3(0,0,0);
            int i = (iz*(N-1) + ix)*6;
            vs[i]   = {c00,n1,glm::vec3(0,0,0)}; 
            vs[i+1] = {c01,n1,glm::vec3(0,1,0)}; 
            vs[i+2] = {c10,n1,glm::vec3(1,0,0)}; 

            vs[i+3] = {c01,n2,glm::vec3(0,1,0)}; 
            vs[i+4] = {c11,n2,glm::vec3(1,1,0)};
            vs[i+5] = {c10,n2,glm::vec3(1,0,0)}; 

            int ii = (iz*(N-1)+ix)*6;
            is[ii] = i;
            is[ii+1] = i+1;
            is[ii+2] = i+2;
            is[ii+3] = i+3;
            is[ii+4] = i+4;
            is[ii+5] = i+5;
        }
    }

    glm::vec3 cubePosition = glm::vec3( 0.0f,  0.0f,  0.0f);
    glm::vec3 lightPosition(-10.0f,10.0f,-10.0f);

   // general rendering setup 
    
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
    Mesh landscape = Mesh(vs,is,ts);

    //Cube Shader stuff
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
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int x = floor(camera.Position.x*subdiv);
        int z = floor(camera.Position.z*subdiv);
        float sx = camera.Position.x * subdiv - x;
        float sz = camera.Position.z * subdiv - z;
        float n0 = interpolate(heights[z][x],heights[z][x+1],sx);
        float n1 = interpolate(heights[z+1][x],heights[z+1][x+1],sx);

        //camera.Position.y = interpolate(n0,n1,sz)+0.2f;

        // input
        // -----
        processInput(window);

        // render
        // -----
        glClearColor(0.03f,0.0f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //DRAW CUBE
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

        landscape.Draw(landShader);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // update / physics
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    /*glDeleteVertexArrays(1,&LVAO);
    glDeleteBuffers(1,&LEBO);*/

    cubeMesh.terminate();
    lightMesh.terminate();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset,yoffset);
}

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

