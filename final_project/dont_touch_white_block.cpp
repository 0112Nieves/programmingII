# include <iostream>
# include <cstdlib>
# include <ctime>
# include <SFML/Graphics.hpp>
# define WIDTH 400
# define HEIGHT 700
# define GRID_W 100
# define GRID_H 150
# define INTERVAL 100
using namespace std;
using namespace sf;

int flags[4] = {0};

void init(){
    srand(time(NULL));
    for(int i = 0; i < 4; i++){
        flags[i] = rand() % 4;
    }
}

void gameDraw(RenderWindow& window){
    const float lineThickness = 2.0f; // 設定線條寬度

    // 畫橫線
    for(int i = 0; i < 4; i++){
        float y = i * GRID_H + INTERVAL;
        RectangleShape hLine(Vector2f(400, lineThickness));
        hLine.setPosition(0, y);
        hLine.setFillColor(Color::Black);
        window.draw(hLine);
    }

    // 畫直線
    for(int i = 0; i < 4; i++){
        float x = i * GRID_W;
        RectangleShape vLine(Vector2f(lineThickness, 700 - INTERVAL));
        vLine.setPosition(x, INTERVAL);
        vLine.setFillColor(Color::Black);
        window.draw(vLine);
    }

    // 畫黑框
    for(int i = 0; i < 4; i++){
        int x = flags[i] * GRID_W;
        int y = i * GRID_H + INTERVAL;

        RectangleShape rect(Vector2f(GRID_W, GRID_H));
        rect.setPosition(x, y);
        rect.setFillColor(Color::Black);
        window.draw(rect);
    }
}


int main() {
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML Window");
    init();
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear(Color::White);
        gameDraw(window);
        window.display();
    }

    return 0;
}
