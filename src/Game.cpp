#include "Game.hpp"

#include "Errors.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <stdexcept>

const double Game::UPDATES_PER_SEC = 60.0;
const double Game::UPDATE_INTERVAL = 1 / UPDATES_PER_SEC;

Game::Game()
{
}

Game::~Game()
{
    m_resources.clear();
}

void Game::init()
{
    createWindow();
    loadResources();

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
    m_resources.getShader("simple")->use();
    m_resources.getShader("simple")->setMat4("u_projection", projection);

    m_renderer.init(m_resources);

    m_player.texture = m_resources.getTexture("ship");
    m_player.size = glm::vec2(64.0f, 64.0f);
    m_player.position = glm::vec2(200.0f, 200.0f);
    m_player.bounds.push_back(glm::vec2(1.0));

    m_asteroid.texture = m_resources.getTexture("wood");
    m_asteroid.size = glm::vec2(64.0f, 64.0f);
    m_asteroid.position = glm::vec2(0.0f);
    m_asteroid.velocity = 30.0f * glm::vec2(1.0, 1.0);
    m_asteroid.angularVelocity = 30.0f;
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::loadResources()
{
    m_resources.loadShader("simple", "res/shaders/simple.vert", "res/shaders/simple.frag");
    m_resources.loadTexture("wood", "res/images/wood.jpg", false);
    m_resources.loadTexture("ship", "res/images/ship.png", true);
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
    if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
    {
        m_window->setToClose();
        return;
    }
    m_player.processInput();
    
}

void Game::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // TODO: nebude potreba kdyz budu kreslit texturu pres cele pozadi?
    glClear(GL_COLOR_BUFFER_BIT);

    m_player.draw(m_renderer, m_resources.getShader("simple"));
    m_asteroid.draw(m_renderer, m_resources.getShader("simple"));

    m_window->swapBuffers();
}


void Game::update(double deltaTime)
{
    m_player.update(deltaTime);
    m_asteroid.update(deltaTime);
}