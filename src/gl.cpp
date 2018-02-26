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

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	const GLuint width = 640;
	const GLuint height = 480;

    sf::Window window(sf::VideoMode(width, height), "SFML window with OpenGL", sf::Style::Default, settings);

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

    Camera cam(glm::vec3{0,0,3});
    Shader ourShader("shaders/simple.vert", "shaders/simple.frag");
    Model ourModel("./suzanne.obj");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ourShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = cam.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1);
		ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        window.display();
    }

    return EXIT_SUCCESS;
}
