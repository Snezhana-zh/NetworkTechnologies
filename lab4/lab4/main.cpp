#include <iostream>
#include <string>
#include "controller.h"

int main() {
    try {
        Application* application = new Application();
        View* view = new View();
        Controller* controller = new Controller(application, view);

        controller->start();

        delete view;
        delete application;
        delete controller;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}