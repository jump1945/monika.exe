//this is main
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
void renderAndMove(float speed);
//i know the code is terrible but i am too lazy to fix
//so don't complain
int getWindowWidth();
int getWindowHeight();

void createTwoThread(float speed);
std::vector<int> hslToRGB(float hue, float saturation, float lightness);
std::atomic<int> number;
int main(){
    srand(time(NULL));
    number.store(0);
    createTwoThread(2);
}

void createTwoThread(float speed) {
    std::thread newThread1(renderAndMove,speed);
    std::thread newThread2(renderAndMove,-1*speed);
    newThread1.join();
    newThread2.join();
}

void renderAndMove(float speed) {
    sf::Vector2u size(100, 100);
    sf::VideoMode video(size, 32);
    sf::RenderWindow window(video, std::to_string(number.load()));
    int threadCount = number.load();
    ++threadCount;
    number.store(threadCount);
    sf::Texture texture("monika.png");
    sf::Sprite sprite(texture);;
    int windowWidth = getWindowWidth();
    int windowHeight = getWindowHeight();
    float x = abs((int)(speed * 1234)) % windowWidth, y = abs((int)(speed * 1234)) % getWindowHeight();
    float dx = speed, dy = speed;
    auto windowSize = window.getSize();
    int hue = 0;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        x += dx;
        y += dy;

        if (x < 0 || x+windowSize.x > windowWidth) {
            dx = -dx;
        }
        if (y < 0 || y + (2*windowSize.y) > windowHeight) {
            dy = -dy;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
        sf::Vector2i newPos(x, y);
        std::vector<int> colorRGB = hslToRGB(hue,360,360);
        sf::Color newColor(colorRGB[0],colorRGB[1], colorRGB[2]);

        sprite.setColor(newColor);
        window.setPosition(newPos);
        window.draw(sprite);
        window.display();
        hue += 1;
        if (hue >= 360)hue = 0;
    }
    createTwoThread(speed*2);
}

std::vector<int> hslToRGB(float hue, float saturation, float lightness) {
    hue = fmod(hue, 360.0f);
    if (hue < 0) hue += 360.0f;

    float c = (1 - std::fabs(2 * lightness - 1)) * saturation;
    float x = c * (1 - std::fabs(fmod(hue / 60.0f, 2) - 1));
    float m = lightness - c / 2;

    float r = 0.0f, g = 0.0f, b = 0.0f;
    if (hue < 60) {
        r = c; g = x; b = 0;
    }
    else if (hue < 120) {
        r = x; g = c; b = 0;
    }
    else if (hue < 180) {
        r = 0; g = c; b = x;
    }
    else if (hue < 240) {
        r = 0; g = x; b = c;
    }
    else if (hue < 300) {
        r = x; g = 0; b = c;
    }
    else {
        r = c; g = 0; b = x;
    }

    r += m;
    g += m;
    b += m;

    std::vector<int> newVector;
    newVector.resize(3);
    newVector[0] = (r * 255);
    newVector[1] = (g * 255);
    newVector[2] = (b * 255);
    return newVector;
}

int getWindowWidth() {
    static const int width = GetSystemMetrics(SM_CXSCREEN);
    return width;
}

int getWindowHeight() {
    static const int height = GetSystemMetrics(SM_CYSCREEN);
    return height;
}
