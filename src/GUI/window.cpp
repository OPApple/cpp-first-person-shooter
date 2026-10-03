#include <glad/glad.h> //glad ENSIN!!!!
#include <GLFW/glfw3.h>
#include <iostream> 
#include "window.hpp"
#include "game_scene.hpp"
#include "ui_button.hpp"
#include "sound_handler.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(int width, int height) : 
    width_(width), height_(height), scene_(nullptr) { }


void Window::SetScene(std::shared_ptr<Scene> new_scene) {
    scene_ = new_scene;
}


void Window::Start() {
    if (scene_ == nullptr) {
        std::cerr << "No Scene defined for window!" << std::endl;
        return;
    }

    GLFWwindow* glfw_window = glfwCreateWindow(width_, height_, "ChungusWindow", NULL, NULL);
    if (!glfw_window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, width_, height_);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(glfw_window, GameScene::StaticMouseCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TextRenderer text_renderer = TextRenderer();

    SoundHandler sound_handler = SoundHandler();

    sound_handler.PlayBGM();
    scene_->Init();

/*========================MAIN LOOP======================================*/
    while(!glfwWindowShouldClose(glfw_window)){
        std::shared_ptr<Scene> new_scene = scene_->ProcessInput(glfw_window, sound_handler);
        if (new_scene) {
            std::string old_bgm = scene_->GetBGM();
            scene_ = new_scene;
            if (new_scene->GetBGM() != old_bgm) {
                sound_handler.ChangeBGM(new_scene->GetBGM());
                sound_handler.PlayBGM();
            }
            scene_->Init();
        }

        glClearColor(scene_->GetRed(), scene_->GetGreen(), scene_->GetBlue(), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        scene_->Render(text_renderer, sound_handler);

        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
    }

    std::cout << ":3" << std::endl;
    glfwDestroyWindow(glfw_window);
    glfwTerminate();

}


void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}
