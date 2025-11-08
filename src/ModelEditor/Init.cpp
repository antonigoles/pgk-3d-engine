#include <Engine/Window.hpp>
#include <ModelEditor/Init.hpp>

namespace ModelEditor
{
    Engine::Window* init(int v_width, int v_height) {
        // we do all the initializer stuff here
        Engine::Window* window = new Engine::Window(v_width, v_height);

        // TODO: zrobić coś ciekawego tutaj

        return window;
    };
};
