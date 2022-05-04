// engine-core.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <filesystem>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Input.h"
#include "File.h"
#include "Shader.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Helpers.h"
#include "EntityManager.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"
#include "Loader.h"
#include "Time.h"

using namespace Engine::Graphics;
using namespace Engine::InputManager;
using namespace Engine::Filesystem;
using namespace Engine::Misc;
using namespace Engine::Helpers;
using namespace Engine::Entity;
using namespace Engine::Components;
using namespace Engine::Systems;
using namespace Engine::Maths;
using namespace Engine::InternalIO;

std::ostream& operator<<(std::ostream& out, glm::mat4 const& toDisplay)
{
    out << "| " << toDisplay[0][0] << " " << toDisplay[0][1] << " " << toDisplay[0][2] << " " << toDisplay[0][3] << " |" << std::endl;
    out << "| " << toDisplay[1][0] << " " << toDisplay[1][1] << " " << toDisplay[1][2] << " " << toDisplay[1][3] << " |" << std::endl;
    out << "| " << toDisplay[2][0] << " " << toDisplay[2][1] << " " << toDisplay[2][2] << " " << toDisplay[2][3] << " |" << std::endl;
    out << "| " << toDisplay[3][0] << " " << toDisplay[3][1] << " " << toDisplay[3][2] << " " << toDisplay[3][3] << " |";
    return out;
}

int main()
{

    glfwInit();

    Window window(800, 600, "My Window");
    Matrix4 mat;
    Vector2 vec2;
    Vector3 vec3;
    EntityManager entityMgr;
    Renderer renderer;

    if (!window.Create())
    {
        return -1;
    }

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(*window, Input::Update);
    glfwSetCursorPosCallback(*window, Input::MousePosition);
    std::cout << glGetString(GL_VERSION) << std::endl;

    Input::Initialize(window.Width() / 2.0f, window.Height() / 2.0f);

    //Model yu = Loader<ModelType::DAE>::LoadModel("./Persona/Yu/bc001.dae");

    Shader shad("./shaders/default.vert", "./shaders/default.frag");

    Model venti = Loader<ModelType::OBJ>::LoadModel("./Genshin/Venti/Venti.obj");
    Model yu = Loader<ModelType::DAE>::LoadModel("./Cube/cube.dae");

    venti.SetShader(shad);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glClearColor(0.5f, 0.5f, 0.87f, 1.0f);
    
    Entity *ventiEntity = entityMgr.Create("Venti");
    Entity* light = entityMgr.Create("Light");
    
    Entity* camera = entityMgr.Create("MainCamera");

    ventiEntity->AddComponent<ModelRenderer>();

    Transform *lightTransform = light->GetComponent<Transform>();
    lightTransform->Translate({ 0.0f, 2.0f, 0.0f });

    ModelRenderer* ventiRenderer = ventiEntity->GetComponent<ModelRenderer>();
    Transform* ventiTransform = ventiEntity->GetComponent<Transform>();
    ventiRenderer->setModel(venti);
    ventiTransform->Translate({ 0.0f, -5.0f, -10.0f });

    camera->AddComponent<Camera>();
    Transform* transformCam = camera->GetComponent<Transform>();
    Camera* cam = camera->GetComponent<Camera>();
    transformCam->Translate({ 0.0f, 0.0f, 3.0f });

    glm::mat4 projection(1.0f);

    while (!glfwWindowShouldClose(*window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Time::Update();

        if (Input::GetKey(Key::ESC))
        {
            glfwSetWindowShouldClose(*window, true);
        }

        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.Width()) / static_cast<float>(window.Height()), 0.1f, 1000.0f);

        GLuint projectionMatLoc = glGetUniformLocation(shad.ID, "projection");
        glUniformMatrix4fv(projectionMatLoc, 1, GL_FALSE, glm::value_ptr(projection));
        renderer.Update();

        glfwSwapBuffers(*window);
        glfwPollEvents();
    }

    window.Destroy();
    return 0;
}