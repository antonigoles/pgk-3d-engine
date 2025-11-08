// main.cpp
#include <iostream>
#include <cmath>
#include <OpenGL.hpp>
#include <Engine/Window.hpp>
#include <Game/Init.hpp>
#include <ModelEditor/Init.hpp>

// --- Ustawienia okna ---
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

Engine::Window* gameWindow = nullptr;

// --- Callback: dopasowanie viewportu po zmianie rozmiaru okna ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (gameWindow != nullptr) gameWindow->setViewportDimensions(width, height);
}

// --- Obsługa wejścia ---
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // --- Inicjalizacja GLFW ---
    if (!glfwInit()) {
        std::cerr << "Nie udało się zainicjalizować GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Dla macOS
#endif
    
    // --- Tworzenie okna ---
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Labirynt3D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Nie udało się utworzyć okna GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(0);

    // --- Inicjalizacja GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Nie udało się załadować GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    
    gameWindow = Game::init(SCR_WIDTH, SCR_HEIGHT);
    // gameWindow = ModelEditor::init(SCR_WIDTH, SCR_HEIGHT);

    float deltaTime = 0.0f;
    float frameTimeCount = 0;
    int frameCount = 0;

    while (!glfwWindowShouldClose(window))
    {
        double timeStamp = glfwGetTime();
        
        processInput(window);

        // --- Czyszczenie ekranu ---
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        gameWindow->step(window, deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        deltaTime = glfwGetTime() - timeStamp;

        // std::cout << 1/deltaTime  << " fps " << "\n";

        if (frameTimeCount > 5.0) {
            std::cout << ((float)frameCount)/(frameTimeCount) << "FPS" << "\n";
            frameCount = 0;
            frameTimeCount = 0;
        }

        frameTimeCount += deltaTime;
        frameCount++;
    }

    // --- Sprzątanie ---
    glfwTerminate();
    return 0;
}
