#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadBMP(const char * imagepath);

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

    GLfloat cube[] =
	{
		// positions    // colors (r, g, b, a)
		-0.5, -0.5, -0.5,  0, 0, 1, 1,
		-0.5,  0.5, -0.5,  0, 0, 1, 1,
		-0.5, -0.5,  0.5,  0, 0, 1, 1,
		-0.5, -0.5,  0.5,  0, 0, 1, 1,
		-0.5,  0.5, -0.5,  0, 0, 1, 1,
		-0.5,  0.5,  0.5,  0, 0, 1, 1,

		 0.5, -0.5, -0.5,  0, 1, 0, 1,
		 0.5,  0.5, -0.5,  0, 1, 0, 1,
		 0.5, -0.5,  0.5,  0, 1, 0, 1,
		 0.5, -0.5,  0.5,  0, 1, 0, 1,
		 0.5,  0.5, -0.5,  0, 1, 0, 1,
		 0.5,  0.5,  0.5,  0, 1, 0, 1,

		-0.5, -0.5, -0.5,  1, 0, 0, 1,
		 0.5, -0.5, -0.5,  1, 0, 0, 1,
		-0.5, -0.5,  0.5,  1, 0, 0, 1,
		-0.5, -0.5,  0.5,  1, 0, 0, 1,
		 0.5, -0.5, -0.5,  1, 0, 0, 1,
		 0.5, -0.5,  0.5,  1, 0, 0, 1,

		-0.5,  0.5, -0.5,  0, 1, 1, 1,
		 0.5,  0.5, -0.5,  0, 1, 1, 1,
		-0.5,  0.5,  0.5,  0, 1, 1, 1,
		-0.5,  0.5,  0.5,  0, 1, 1, 1,
		 0.5,  0.5, -0.5,  0, 1, 1, 1,
		 0.5,  0.5,  0.5,  0, 1, 1, 1,

		-0.5, -0.5, -0.5,  1, 0, 1, 1,
		 0.5, -0.5, -0.5,  1, 0, 1, 1,
		-0.5,  0.5, -0.5,  1, 0, 1, 1,
		-0.5,  0.5, -0.5,  1, 0, 1, 1,
		 0.5, -0.5, -0.5,  1, 0, 1, 1,
		 0.5,  0.5, -0.5,  1, 0, 1, 1,

		-0.5, -0.5,  0.5,  1, 1, 0, 1,
		 0.5, -0.5,  0.5,  1, 1, 0, 1,
		-0.5,  0.5,  0.5,  1, 1, 0, 1,
		-0.5,  0.5,  0.5,  1, 1, 0, 1,
		 0.5, -0.5,  0.5,  1, 1, 0, 1,
		 0.5,  0.5,  0.5,  1, 1, 0, 1,
	};

    GLfloat cubetext[] =
    {
    	// positions    // colors (r, g, b, a) 		// uvs
		-0.5,-0.5,-0.5,		0,0,1,1,    0.000059f, 1.0f-0.000004f,
		-0.5,0.5,-0.5,		0,0,1,1,    0.000103f, 1.0f-0.336048f,
		-0.5,-0.5,0.5,		0,0,1,1,    0.335973f, 1.0f-0.335903f,
		-0.5,-0.5,0.5,		0,0,1,1,    1.000023f, 1.0f-0.000013f,
		-0.5,0.5,-0.5,		0,0,1,1,    0.667979f, 1.0f-0.335851f,
		-0.5,0.5,0.5,		0,0,1,1,    0.999958f, 1.0f-0.336064f,

		 0.5,-0.5,-0.5,		0,1,0,1,    0.667979f, 1.0f-0.335851f,
		 0.5,0.5,-0.5,		0,1,0,1,    0.336024f, 1.0f-0.671877f,
		 0.5,-0.5,0.5,		0,1,0,1,    0.667969f, 1.0f-0.671889f,
		 0.5,-0.5,0.5,		0,1,0,1,    1.000023f, 1.0f-0.000013f,
		 0.5,0.5,-0.5,		0,1,0,1,    0.668104f, 1.0f-0.000013f,
		 0.5,0.5,0.5,		0,1,0,1,    0.667979f, 1.0f-0.335851f,

		-0.5,-0.5,-0.5,		1,0,0,1,    0.000059f, 1.0f-0.000004f,
		0.5,-0.5,-0.5,		1,0,0,1,    0.335973f, 1.0f-0.335903f,
		-0.5,-0.5,0.5,		1,0,0,1,    0.336098f, 1.0f-0.000071f,
		-0.5,-0.5,0.5,		1,0,0,1,    0.667979f, 1.0f-0.335851f,
		0.5,-0.5,-0.5,		1,0,0,1,    0.335973f, 1.0f-0.335903f,
		0.5,-0.5,0.5,		1,0,0,1,    0.336024f, 1.0f-0.671877f,

		-0.5,0.5,-0.5,		0,1,1,1,    1.000004f, 1.0f-0.671847f,
		0.5,0.5,-0.5,		0,1,1,1,    0.999958f, 1.0f-0.336064f,
		-0.5,0.5,0.5,		0,1,1,1,    0.667979f, 1.0f-0.335851f,
		-0.5,0.5,0.5,		0,1,1,1,    0.668104f, 1.0f-0.000013f,
		0.5,0.5,-0.5,		0,1,1,1,    0.335973f, 1.0f-0.335903f,
		0.5,0.5,0.5,		0,1,1,1,    0.667979f, 1.0f-0.335851f,

		-0.5,-0.5,-0.5,		1,0,1,1,    0.335973f, 1.0f-0.335903f,
		0.5,-0.5,-0.5,		1,0,1,1,    0.668104f, 1.0f-0.000013f,
		-0.5,0.5,-0.5,		1,0,1,1,    0.336098f, 1.0f-0.000071f,
		-0.5,0.5,-0.5,		1,0,1,1,    0.000103f, 1.0f-0.336048f,
		0.5,-0.5,-0.5,		1,0,1,1,    0.000004f, 1.0f-0.671870f,
		0.5,0.5,-0.5,		1,0,1,1,    0.336024f, 1.0f-0.671877f,

		-0.5,-0.5,0.5,		1,1,0,1,    0.000103f, 1.0f-0.336048f,
		0.5,-0.5,0.5,		1,1,0,1,    0.336024f, 1.0f-0.671877f,
		-0.5,0.5,0.5,		1,1,0,1,    0.335973f, 1.0f-0.335903f,
		-0.5,0.5,0.5,		1,1,0,1,    0.667969f, 1.0f-0.671889f,
		0.5,-0.5,0.5,		1,1,0,1,    1.000004f, 1.0f-0.671847f,
		0.5,0.5,0.5,		1,1,0,1,    0.667979f, 1.0f-0.335851f

    };

    const GLuint vertexAttribIndex = 0;
    const GLuint colorAttribIndex = 1;

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint programID = LoadShaders("shaders/simple.vert", "shaders/simple.frag");

    // Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint mvpID = glGetUniformLocation(programID, "mvp");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	GLuint textureID = LoadBMP("cube.bmp");

	GLuint textureSampler = glGetUniformLocation(programID, "textureSampler");

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubetext), cubetext, GL_STATIC_DRAW);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Create a clock for measuring the time elapsed
    sf::Clock clock;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader programs define in shaders/ folder
        glUseProgram(programID);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(textureSampler, 0);

        glEnableVertexAttribArray(vertexAttribIndex);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(
				vertexAttribIndex,
				3,
				GL_FLOAT,
				GL_FALSE,
				9*sizeof(GLfloat),
				(void*)0
			);

		glEnableVertexAttribArray(colorAttribIndex);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(
				colorAttribIndex,
				2,
				GL_FLOAT,
				GL_FALSE,
				9*sizeof(GLfloat),
				(void*)(7*sizeof(GLfloat))
			);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(vertexAttribIndex);
		glDisableVertexAttribArray(colorAttribIndex);

        // Finally, display the rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


GLuint LoadBMP(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file can be closed.
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}
