#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Shader.hpp"

float cube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
/*
class GLWindow
{
public:

	static constexpr GLuint width = 1024;
	static constexpr GLuint height = 768;

	GLWindow()
	{
		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 4;
		settings.majorVersion = 3;
		settings.minorVersion = 3;
		settings.attributeFlags = sf::ContextSettings::Core;

		sf::Window m_window(sf::VideoMode(width, height), "SFML window with OpenGL", sf::Style::Default, settings);
		m_window.setMouseCursorVisible(false);
		sf::Mouse::setPosition(sf::Vector2i{width/2, height/2}, m_window);
		m_window.setVerticalSyncEnabled(true);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			fprintf(stderr, "Failed to initialize GLEW\n");
		}

		m_window.setActive();

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

private:

	sf::Window m_window;
	Camera m_camera;
    sf::Clock m_clock;
	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;
};
*/
int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    const GLuint width = 1024;
    const GLuint height = 768;

    sf::Window window(sf::VideoMode(width, height), "SFML window with OpenGL", sf::Style::Default, settings);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2i{width/2, height/2}, window);
    window.setVerticalSyncEnabled(true);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    window.setActive();

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glViewport(0, 0, window.getSize().x, window.getSize().y);

    Camera camera(glm::vec3{0,0,6});
    Shader lampShader("shaders/lamp.vert", "shaders/lamp.frag");
    Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");
    Model suzanne("suzanne.obj");
    Model lamp("sphere.obj");

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec3 lightPos(10, 10, 0);

    sf::Clock clock;
    bool isRunning = true;
    while (isRunning)
    {
        float currentFrame = clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                isRunning = false;
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                isRunning = false;
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            camera.processKeyboard(Camera::Movement::Forward, deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.processKeyboard(Camera::Movement::Backward, deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            camera.processKeyboard(Camera::Movement::Left, deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.processKeyboard(Camera::Movement::Right, deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            camera.processKeyboard(Camera::Movement::Up, deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
            camera.processKeyboard(Camera::Movement::Down, deltaTime);


        camera.processMouseMovement(
                        sf::Mouse::getPosition(window).x - width/2.0f,
                        height/2.0f - sf::Mouse::getPosition(window).y);
        sf::Mouse::setPosition(sf::Vector2i{width/2, height/2}, window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //{
            simpleShader.use();
            glm::mat4 model = glm::mat4(1);
            glm::mat4 projection = camera.getPerspectiveMatrix({width, height});
            glm::mat4 view = camera.getViewMatrix();
            simpleShader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
            simpleShader.setVec3("lightPos", lightPos);
            simpleShader.setVec3("viewPos", camera.getPosition());
            simpleShader.setMat4("model", model);
            simpleShader.setMat4("projection", projection);
            simpleShader.setMat4("view", view);
            suzanne.draw(simpleShader);
        //}
        //{
            lampShader.use();
            model = glm::mat4(1);
            model = glm::translate(model, lightPos);
            projection = camera.getPerspectiveMatrix({width, height});
            view = camera.getViewMatrix();
            lampShader.setMat4("model", model);
            lampShader.setMat4("projection", projection);
            lampShader.setMat4("view", view);
            lamp.draw(lampShader);
        //}


        window.display();
    }

    window.close();

    return EXIT_SUCCESS;
}
