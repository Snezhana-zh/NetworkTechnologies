#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>

#include "view.h"
#include "application.h"
#include "config.h"

class Controller {
public:
    Controller(Application* app, View* view);

    void start();

private:
    Application* application;
    View* view;
};