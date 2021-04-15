#include "Window.h"

using namespace std;

int main(int argc, char** argv)
{
    Window window;

    window.init();
    window.main_loop();

    return 0;
}
