#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <random>
#include <queue>
#include <time.h>
#include "../../include/Shader.h"

#define HEIGHT 1000
#define WIDTH 1200

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setupWindow();
GLFWwindow* window;


int parent[HEIGHT*WIDTH];
int rnk[HEIGHT*WIDTH];
glm::vec3 pixels_definiton[HEIGHT*WIDTH];
float pixels[HEIGHT*WIDTH*3];

int Find(int x){
    if(parent[x]==-1) return x;
    parent[x]=Find(parent[x]);
    return parent[x];
}

void Union(int x, int y){
    int xRoot = Find(x);
    int yRoot = Find(y);
    if(rnk[xRoot]>rnk[yRoot]){
        parent[yRoot] = xRoot;
    }
    else if(rnk[xRoot]<rnk[yRoot]){
        parent[xRoot] = yRoot;
    }
    else {
        parent[yRoot] = xRoot;
        rnk[xRoot]++;
    }
}

struct Connection {
    float weight;
    int y1, x1;
    int y2, x2;
};

class CompareConnections {
public:
    bool operator()(Connection c1, Connection c2) {
        return c1.weight > c2.weight;
    }
};

int main() {
    setupWindow();

    std::mt19937 rng(time(NULL));
    std::uniform_real_distribution<float> dist(0, 1);
    std::priority_queue<Connection, std::vector<Connection>, CompareConnections> q;
    float threshold = 0.0;

    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            parent[i * WIDTH + j] = -1;
            if(i > 0) q.push({dist(rng), i-1, j, i, j});
            if(j > 0) q.push({dist(rng), i, j-1, i, j});
        }
    }


    for(int i = 0; i < HEIGHT; ++i)
        for(int j = 0; j < WIDTH; ++j)
            pixels_definiton[i * WIDTH + j] = {dist(rng), dist(rng), dist(rng)};


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

    float vertices[] = {
            -1.0, -1.0,
            -1.0, 1.0,
            1.0, -1.0,
            1.0, 1.0,
            -1.0, 1.0,
            1.0, -1.0
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    auto shader = Shader("src/glsl/shader.vert", "src/glsl/shader.frag");
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    shader.setInt("tex", 0);


    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.20f, 0.19f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        threshold += 0.0005;

        while(!q.empty() && q.top().weight <= sqrt(threshold)) {
            int v1 = q.top().y1 * WIDTH + q.top().x1;
            int v2 = q.top().y2 * WIDTH + q.top().x2;
            q.pop();
            if(Find(v1) != Find(v2)) Union(v1, v2);
        }

        for(int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                pixels[(i * WIDTH + j) * 3] = pixels_definiton[Find(i * WIDTH + j)].r;
                pixels[(i * WIDTH + j) * 3 + 1] = pixels_definiton[Find(i * WIDTH + j)].g;
                pixels[(i * WIDTH + j) * 3 + 2] = pixels_definiton[Find(i * WIDTH + j)].b;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 360, 40);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

