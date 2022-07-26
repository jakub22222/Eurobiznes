#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <filesystem>
#include "button.h"
#include "GUI.h"

int main()
{
    GUI gui{1920.f, 1080.f};
    gui.start();
    return 0;
}