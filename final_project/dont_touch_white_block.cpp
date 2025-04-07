#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;

const int WIDTH = 400;
const int HEIGHT = 700;
const int GRID_W = 100;
const int GRID_H = 150;
const float SPEED = 250.0f;
const float SPAWN_INTERVAL = 0.8f;
const float TARGET_Y = HEIGHT - GRID_H;
const float TOLERANCE = 60.0f;

struct Tile {
    int lane;
    float y;
    bool active;
};

vector<Tile> tiles;
bool isDone = false;
int score = 0; // 分數

void spawnTile() {
    Tile t;
    t.lane = rand() % 4;
    t.y = 0;
    t.active = true;
    tiles.push_back(t);
}

bool handleKeyPress(int lanePressed) {
    for (auto& tile : tiles) {
        if (tile.active && tile.lane == lanePressed) {
            if (abs(tile.y - TARGET_Y) < TOLERANCE) {
                tile.active = false;
                score += 1; // 成功點擊加分
                return true;
            }
        }
    }
    // isDone = true;
    return false;
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "別踩白塊兒 - 加分版");
    Clock clock, spawnClock;

    // 設定 FPS 上限
    window.setFramerateLimit(60);

    // 載入字型
    Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        cerr << "❌ 無法載入字型檔 arial.ttf" << endl;
        return -1;
    }

    // 分數文字
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(WIDTH / 2 - 50, 10); // 上方中央

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // 處理事件
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // 鍵盤處理
        if (Keyboard::isKeyPressed(Keyboard::A)) handleKeyPress(0);
        if (Keyboard::isKeyPressed(Keyboard::F)) handleKeyPress(1);
        if (Keyboard::isKeyPressed(Keyboard::H)) handleKeyPress(2);
        if (Keyboard::isKeyPressed(Keyboard::L)) handleKeyPress(3);

        // 更新位置
        for (auto& tile : tiles) {
            tile.y += SPEED * dt;
        }

        // 檢查是否掉出還沒按
        for (auto& tile : tiles) {
            if (tile.y > HEIGHT && tile.active) {
                isDone = true;
            }
        }

        // 生成新方塊
        if (spawnClock.getElapsedTime().asSeconds() > SPAWN_INTERVAL) {
            spawnTile();
            spawnClock.restart();
        }

        // 更新分數文字內容
        scoreText.setString("Score: " + to_string(score));

        // 畫面更新
        window.clear(Color::White);

        // 畫分數
        window.draw(scoreText);

        // 畫觸發區
        RectangleShape line(Vector2f(WIDTH, 2));
        line.setPosition(0, TARGET_Y + GRID_H);
        line.setFillColor(Color(200, 0, 0));
        window.draw(line);

        // 畫黑塊
        for (auto& tile : tiles) {
            RectangleShape rect(Vector2f(GRID_W, GRID_H));
            rect.setPosition(tile.lane * GRID_W, tile.y);
            rect.setFillColor(tile.active ? Color::Black : Color(156, 156, 156));
            window.draw(rect);
        }

        window.display();

        if (isDone) {
            cout << "Game Over! Final Score: " << score << endl;
            window.close();
        }
    }

    return 0;
}
