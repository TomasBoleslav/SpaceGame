#include "Game.hpp"

#include "Errors.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Texture2D.hpp"
#include "ResourceManager.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <optional>
#include <stdexcept>
#include <array>
#include <iostream>

const double Game::UPDATES_PER_SEC = 60.0;
const double Game::UPDATE_INTERVAL = 1 / UPDATES_PER_SEC;


Game::Game()
{
}

void Game::init()
{
    createWindow();
    loadResources();

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
    m_resources.getShader("simple")->use();
    m_resources.getShader("simple")->setMat4("u_projection", projection);

    rectangle.texture = m_resources.getTexture("wood");
    rectangle.position = glm::vec2(100.0, 100.0);
    rectangle.color = glm::vec3(0.0, 1.0, 1.0);
    rectangle.rotation = 0.0;
    rectangle.size = glm::vec2(100.0, 200.0);
    renderer.init();
}

void Game::createWindow()
{
    Window::setHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    Window::setHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    Window::setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    Window::setHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_window = std::make_unique<Window>(SCR_WIDTH, SCR_HEIGHT, "SpaceGame");
}

void Game::loadResources()
{
    m_resources.loadShader("simple", "res/shaders/simple.vert", "res/shaders/simple.frag");
    m_resources.loadTexture("wood", "res/images/wood.jpg");
}

void Game::run()
{
    init();
    gameLoop();
}

void Game::gameLoop()
{
    double lastUpdated = glfwGetTime();
    while (!m_window->shouldClose())
    {
        processInput();
        double currentTime = glfwGetTime();
        while (lastUpdated + UPDATE_INTERVAL <= currentTime)
        {
            lastUpdated += UPDATE_INTERVAL;
            update(UPDATE_INTERVAL);
        }
        render();
        glfwPollEvents();
    }
}

void Game::processInput()
{
    if (m_window->getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        m_window->close();
    }
}

void Game::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // TODO: nebude potreba kdyz budu kreslit texturu pres cele pozadi?
    glClear(GL_COLOR_BUFFER_BIT);

    rectangle.draw(renderer, m_resources.getShader("simple"));

    m_window->swapBuffers();
}


void Game::update(double dt)
{
    
}