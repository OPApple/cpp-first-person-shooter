#include <glad/glad.h> //glad ENSIN!!!!
#include <GLFW/glfw3.h>

#include "GUI/window.hpp"
#include "GUI/main_menu.hpp"
#include "GUI/gui_constants.hpp"
#include "GUI/game_scene.hpp"

GLuint window_height = GUIConstants::game_height;
GLuint window_width  = GUIConstants::game_width;

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window main_window = Window(window_width, window_height);
    main_window.SetScene(std::make_shared<MainMenu>());

    main_window.Start();

    return 0;
}