#include "ui/MainWindow.hpp"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create("com.github.szefron.osutagdivider");
    MainWindow window;

    return app->run(window);
}