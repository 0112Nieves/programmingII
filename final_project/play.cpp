#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;

const int BOARD_SIZE = 5;  // 遊戲板大小
const int TILE_SIZE = 100; // 每個格子的大小

vector<vector<int>> board(BOARD_SIZE, vector<int>(BOARD_SIZE, 0)); // 0 為黑塊，1 為白塊

void generateWhiteBlock() {
    int x = rand() % BOARD_SIZE;
    int y = rand() % BOARD_SIZE;
    board[x][y] = 1; // 隨機生成白塊
}

bool checkGameOver(int x, int y) {
    return board[x][y] == 1;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 設定隨機種子

    // 創建遊戲窗口
    RenderWindow window(VideoMode(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE), "別踩白塊兒");
    
    // 玩家位置
    int playerX = BOARD_SIZE / 2;
    int playerY = BOARD_SIZE / 2;

    // 遊戲循環
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // 控制玩家移動
        if (Keyboard::isKeyPressed(Keyboard::W) && playerX > 0) playerX--;
        if (Keyboard::isKeyPressed(Keyboard::S) && playerX < BOARD_SIZE - 1) playerX++;
        if (Keyboard::isKeyPressed(Keyboard::A) && playerY > 0) playerY--;
        if (Keyboard::isKeyPressed(Keyboard::D) && playerY < BOARD_SIZE - 1) playerY++;

        // 檢查遊戲是否結束
        if (checkGameOver(playerX, playerY)) {
            cout << "遊戲結束！你踩到白塊了。" << endl;
            window.close();
        }

        // 隨機生成一個白塊
        generateWhiteBlock();

        // 清空畫面
        window.clear();

        // 繪製遊戲板
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);

                if (board[i][j] == 1) {
                    tile.setFillColor(Color::White); // 白塊
                } else {
                    tile.setFillColor(Color::Black); // 黑塊
                }

                window.draw(tile);
            }
        }

        // 繪製玩家（假設玩家為紅色方塊）
        RectangleShape player(Vector2f(TILE_SIZE, TILE_SIZE));
        player.setPosition(playerY * TILE_SIZE, playerX * TILE_SIZE);
        player.setFillColor(Color::Red);
        window.draw(player);

        // 顯示畫面
        window.display();
    }

    return 0;
}
