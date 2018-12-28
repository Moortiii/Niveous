#include "Game.h"
#include "configuration/Configuration.h"
#include "components/Position.h"
#include "components/Velocity.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>

bool Game::initialize() {
    initializeSDL();
    initializeBox2D();
    initializeOpenGl();
    initializeSystems();
    initializeDelta();
    initializeEntities();

    running = true;

    return true;
}

// Main game loop
int Game::run() {

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while(running) {
        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_r:
                    // Cover with red and update
                    glClearColor(1.0, 0.0, 0.0, 1.0);
                    break;
                case SDLK_g:
                    // Cover with green and update
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    break;
                case SDLK_b:
                    // Cover with blue and update
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    break;
                default:
                    break;
            }
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update all entities
        update();

        // Draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Present the result
        SDL_GL_SwapWindow(window);
    }

    // Clean up and exit the game gracefully
    clean();

    return 0;
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::update() {
    updateDelta();

    for(auto& system : systems) {
        system->update(dt);
    }
}

void Game::updateDelta() {
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    dt = ((currentTick - lastTick) * 1000 / (float)SDL_GetPerformanceFrequency());
}

bool Game::initializeOpenGl() {

    printf ("glGetString (GL_VERSION) returns %s\n", glGetString (GL_VERSION));

    // Enable vertical sync
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
    #endif

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    return true;
}

bool Game::initializeSystems() {
    systems.emplace_back(std::make_shared<MovementSystem>(registry));
    //systems.emplace_back(std::make_shared<RenderingSystem>(registry));

    return true;
}

bool Game::initializeBox2D() {
    b2Vec2 gravity(0.0f, -10.0f);
    world = std::make_shared<b2World>(gravity);

    // Used by the Box2D Iterative Solver
    timestep = 1.0f / 60.0f;
    velocityIterations = 6;
    positionIterations = 2;

    return true;
}

bool Game::initializeSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL_VIDEO: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Niveous", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Configuration::SCREENWIDTH, Configuration::SCREENHEIGHT, SDL_WINDOW_OPENGL);

    if(!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create OpenGL Rendering Context
    mainContext = SDL_GL_CreateContext(window);

    return true;
}

bool Game::initializeDelta() {
    currentTick = SDL_GetPerformanceCounter();
    lastTick    = SDL_GetPerformanceCounter();
    dt = 0;

    return true;
}

bool Game::initializeEntities() {
    // Create an entity with a couple of components to test the systems
    auto entity = registry.create();

    registry.assign<Position>(entity, 0.0f, 0.0f);
    registry.assign<Velocity>(entity, 0.1f, 0.1f);

    return true;
}
