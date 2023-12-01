// engine-core.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <filesystem>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "IconsFontAwesome6.h"

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
#include "Entity.h"

using namespace Engine::Managers;
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

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    //fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

void SetupMenuBar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(36.0f, 0));
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(ICON_FA_FILE " File"))
        {
            if (ImGui::MenuItem(ICON_FA_FILE_ARROW_UP " Open..."))
            {

            }
            if (ImGui::MenuItem(ICON_FA_FILE_ARROW_DOWN " Save..."))
            {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_CUBE " Scene"))
        {
            if (ImGui::BeginMenu("Entities"))
            {
                if (ImGui::MenuItem("New"))
                {

                }
                if (ImGui::BeginMenu("Entities List"))
                {
                    for (Engine::Entity::Entity* entity : Engine::Managers::EntityManager::GetAll())
                    {
                        if (ImGui::BeginMenu(entity->ID().c_str()))
                        {
                            std::string label = ((entity->IsEnabled) ? "Disable" : "Enable");
                            if (ImGui::MenuItem(label.c_str()))
                            {
                                entity->IsEnabled = !entity->IsEnabled;
                            }
                            if (ImGui::MenuItem("Delete"))
                            {
                                Engine::Managers::EntityManager::Delete(entity);
                            }
                            ImGui::EndMenu();
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

void SetupEntityList()
{
    ImGui::Begin("Entities");

    for (Engine::Entity::Entity* entity : Engine::Managers::EntityManager::GetAll())
    {
        if (ImGui::TreeNode(entity->ID().c_str()))
        {
            entity->SetupDebugWindow();
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

int main()
{

    glfwInit();

    Window window(1600, 900, "OpenGL");
    Matrix4 mat;
    Vector2 vec2;
    Vector3 vec3;
    Renderer renderer;

    bool freeMouseMovement = false;

    if (!window.Create())
    {
        return -1;
    }

    glfwSetKeyCallback(*window, Input::Update);
    glfwSetCursorPosCallback(*window, Input::MousePosition);
    std::cout << glGetString(GL_VERSION) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(*window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/Ubuntu.ttf", 13.0f);
    float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);
    // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

    Input::Initialize(window.Width() / 2.0f, window.Height() / 2.0f);

    Model kindra = Loader::LoadModel("./models/LoL/Kindred/scene.gltf");
    Model venti = Loader::LoadModel("./models/Genshin/Venti/Venti.obj");
    Model daeModel = Loader::LoadModel("./models/MetalGear/Peace_walker.dae");
    Model yu = Loader::LoadModel("./models/Persona/Yu/bc001.dae");
    Model dvalin = Loader::LoadModel("./models/Genshin/Dvalin/Dvalin.obj");
    Model callie = Loader::LoadModel("./models/Splatoon3/CallieMarie/Npc_IdolA/Npc_IdolA.dae");
    Model marie = Loader::LoadModel("./models/Splatoon3/CallieMarie/Npc_IdolB/Npc_IdolB.dae");
    Model naoto = Loader::LoadModel("./models//Persona/Naoto/pc007_13.dae");

    Shader shad("./shaders/default.vert", "./shaders/default.frag");
    //Shader shad("./shaders/TextureDebug.vert", "./shaders/TextureDebug.frag");

    kindra.SetShader(shad);
    venti.SetShader(shad);
    yu.SetShader(shad);
    dvalin.SetShader(shad);
    daeModel.SetShader(shad);
    callie.SetShader(shad);
    marie.SetShader(shad);
    naoto.SetShader(shad);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glDebugMessageCallback(MessageCallback, 0);
    glClearColor(0.5f, 0.5f, 0.87f, 1.0f);

    Entity* kindraEntity = Engine::Managers::EntityManager::Create("Kindra");
    Entity* ventiEntity = Engine::Managers::EntityManager::Create("Venti");
    Entity* yuEntity = Engine::Managers::EntityManager::Create("Yu");
    Entity *dvalinEntity = Engine::Managers::EntityManager::Create("Dvalin");
    Entity* daeTest = Engine::Managers::EntityManager::Create("Test");
    Entity* callieEntity = Engine::Managers::EntityManager::Create("Callie");
    Entity* marieEntity = Engine::Managers::EntityManager::Create("Marie");
    Entity* naotoEntity = Engine::Managers::EntityManager::Create("Naoto");

    Entity* camera = Engine::Managers::EntityManager::Create("MainCamera");

    kindraEntity->AddComponent<ModelRenderer>();
    ventiEntity->AddComponent<ModelRenderer>();
    yuEntity->AddComponent<ModelRenderer>();
    dvalinEntity->AddComponent<ModelRenderer>();
    daeTest->AddComponent<ModelRenderer>();
    callieEntity->AddComponent<ModelRenderer>();
    marieEntity->AddComponent<ModelRenderer>();
    naotoEntity->AddComponent<ModelRenderer>();

    ModelRenderer* ventiRenderer = ventiEntity->GetComponent<ModelRenderer>();
    Transform* ventiTransform = ventiEntity->GetComponent<Transform>();
    ventiRenderer->setModel(venti);
    ventiTransform->Translate({ 0.0f, -5.0f, -10.0f });

    ModelRenderer* yuRenderer = yuEntity->GetComponent<ModelRenderer>();
    Transform* yuTransform = yuEntity->GetComponent<Transform>();
    yuRenderer->setModel(yu);
    yuTransform->Translate({ 15.0f, -5.0f, -10.0f });
    yuTransform->Scale({ 0.1f, 0.1f, 0.1f });

    ModelRenderer* dvalinRenderer = dvalinEntity->GetComponent<ModelRenderer>();
    Transform* dvalinTransform = dvalinEntity->GetComponent<Transform>();
    dvalinRenderer->setModel(dvalin);
    dvalinTransform->Translate({ -30.0f, -5.0f, -10.0f });

    ModelRenderer* daeRenderer = daeTest->GetComponent<ModelRenderer>();
    Transform* daeTransform = daeTest->GetComponent<Transform>();
    daeRenderer->setModel(daeModel);
    daeTransform->Translate({ 30.0f, 0.0f, -10.0f });
    daeTransform->Rotate(80, { 0.0f, 1.0f, 0.0f });

    ModelRenderer* callieRenderer = callieEntity->GetComponent<ModelRenderer>();
    Transform* callieTransform = callieEntity->GetComponent<Transform>();
    callieRenderer->setModel(callie);
    callieTransform->Translate({ 60.0f, -5.0f, -10.0f });
    callieTransform->Scale({ 10.0f, 10.0f, 10.0f });
    callieTransform->Rotate(80, { 1.0f, 0.0f, 0.0f });

    ModelRenderer* marieRenderer = marieEntity->GetComponent<ModelRenderer>();
    Transform* marieTransform = marieEntity->GetComponent<Transform>();
    marieRenderer->setModel(marie);
    marieTransform->Translate({ 90.0f, -5.0f, -10.0f });
    marieTransform->Scale({ 10.0f, 10.0f, 10.0f });
    marieTransform->Rotate(80, { 1.0f, 0.0f, 0.0f });

    ModelRenderer* kindraRenderer = kindraEntity->GetComponent<ModelRenderer>();
    Transform* kindraTransform = kindraEntity->GetComponent<Transform>();
    kindraRenderer->setModel(kindra);
    kindraTransform->Translate({ 90.0f, -5.0f, 30.0f });
    kindraTransform->Scale({ 10.0f, 10.0f, 10.0f });

    ModelRenderer* naotoRenderer = naotoEntity->GetComponent<ModelRenderer>();
    Transform* naotoTransform = naotoEntity->GetComponent<Transform>();
    naotoRenderer->setModel(naoto);
    naotoTransform->Translate({ 120.0f, -5.0f, -10.0f });
    naotoTransform->Scale({ 0.1f, 0.1f, 0.1f });

    camera->AddComponent<Camera>();
    Transform* transformCam = camera->GetComponent<Transform>();
    Camera* cam = camera->GetComponent<Camera>();
    transformCam->Translate({ 0.0f, 0.0f, 3.0f });

    bool show_metrics_window = true;

    while (!glfwWindowShouldClose(*window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Time::Update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        SetupMenuBar();
        SetupEntityList();
        ImGui::ShowMetricsWindow(&show_metrics_window);

        if (Input::GetKey(Key::ESC))
        {
            glfwSetWindowShouldClose(*window, true);
        }

        if (Input::GetKeyDown(Key::ALT))
        {
            window.ShowCursor(true);
            cam->DisableCameraMovements();
        }
        else
        {
            window.ShowCursor(false);
            cam->EnableCameraMovements();
        }

        renderer.Update();

        glfwSwapBuffers(*window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.Destroy();
    return 0;
}