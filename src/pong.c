#include <stdio.h>
// #include <ncurses.h>
// настройки для игры
#define WIDTH 80
#define HEIGHT 25
#define RACKET_HEIGHT 3
#define BALL_SYMBOL 'O'
#define RACKET_SYMBOL '|'
#define WALL_SYMBOL '#'
#define MIDDLE_SYMBOL ':'
#define EMPTY_SPACE_SYMBOL ' '
#define SCORE_FOR_WIN 1

int win(int score1, int score2);
void printwinscreen(int player);
void draw(int ballX, int ballY, int racket1Y, int racket2Y, int score1, int score2);

int main() {
    // располагаем мяч посередине поля
    int ballX = WIDTH / 2;
    int ballY = HEIGHT / 2;
    // создаем мяч с таким направлением (можно было любое написать (но только либо -1, либо 1))
    int ballDirX = 1;
    int ballDirY = 1;
    // располагаем ракетки посередине поля
    int racket1Y = HEIGHT / 2 - RACKET_HEIGHT / 2;
    int racket2Y = HEIGHT / 2 - RACKET_HEIGHT / 2;
    int score1 = 0;
    int score2 = 0;
    char ch;  // буква с клавиатуры
    // инициализируем поле
    draw(ballX, ballY, racket1Y, racket2Y, score1, score2);
    while (1) {
        // проверяем, победил ли уже один из игроков
        if (win(score1, score2) == 1) break;
        // управление
        scanf("%c", &ch);
        // поднимаем ракетку 1
        if (ch == 'A' || ch == 'a') {
            // мы отнимаем от координаты у единицу, так как строки нумеруются сверху вниз
            if (racket1Y > 0) racket1Y--;
        }
        // опускаем ракетку 1
        else if (ch == 'Z' || ch == 'z') {
            if (racket1Y < HEIGHT - RACKET_HEIGHT) racket1Y++;
        }
        // поднимаем ракетку 2
        else if (ch == 'K' || ch == 'k') {
            if (racket2Y > 0) racket2Y--;
        }
        // опускаем ракетку 2
        else if (ch == 'M' || ch == 'm') {
            if (racket2Y < HEIGHT - RACKET_HEIGHT) racket2Y++;
        }
        // если пробел, то не изменияем положения ракеток
        else if (ch == ' ')
            ;
        // если любой другой символ, то ничего не произойдет, так как мы покидаем текущую итерацию цикла
        else {
            printf("\033[H\033[J");
            draw(ballX, ballY, racket1Y, racket2Y, score1, score2);
            continue;
        }
        // проверяем столкновения с верхним и нижним краем
        if (ballY == 0 || ballY == HEIGHT - 1) {
            // отбиваем мяч
            ballDirY = -ballDirY;
        }
        // проверяем на столкновение с левой ракеткой
        if (ballX == 1) {
            if (ballY >= racket1Y && ballY < racket1Y + RACKET_HEIGHT) {
                ballDirX = -ballDirX;
            }
            // проверяем на то, ударился ли мяч в угол ракетки под определенным углом
            else if ((ballY == racket1Y - 1 && ballDirY == 1) ||
                     (ballY == racket1Y + RACKET_HEIGHT && ballDirY == -1)) {
                ballDirX = -ballDirX;
                ballDirY = -ballDirY;
                // здесь мяч вышел за левую границу
            } else {
                score2++;
                // сбрасываем положение мяча
                ballX = WIDTH / 2;
                ballY = HEIGHT / 2;
                // направляем мяч в сторону победителя предыдущего раунда
                ballDirX = -ballDirX;
                ballDirY = 1;
            }
        }
        // проверяем на столкновение с правой ракеткой
        if (ballX == WIDTH - 2) {
            if (ballY >= racket2Y && ballY < racket2Y + RACKET_HEIGHT) {
                ballDirX = -ballDirX;
            }
            // проверяем на то, ударился ли мяч в угол ракетки под определенным углом
            else if ((ballY == racket2Y - 1 && ballDirY == 1) ||
                     (ballY == racket2Y + RACKET_HEIGHT && ballDirY == -1)) {
                ballDirX = -ballDirX;
                ballDirY = -ballDirY;
            }
            // здесь мяч вышел за правую границу
            else {
                score1++;
                // сбрасываем положение мяча
                ballX = WIDTH / 2;
                ballY = HEIGHT / 2;
                // направляем мяч в сторону победителя предыдущего раунда
                ballDirX = -ballDirX;
                ballDirY = 1;
            }
        }
        // в конце цикла изменяем положение мяча по оси x и y
        ballX += ballDirX;
        ballY += ballDirY;
        // отрисовываем поле
        draw(ballX, ballY, racket1Y, racket2Y, score1, score2);
    }
    return 0;
}
// функция для проверки, победил ли один из игроков
int win(int score1, int score2) {
    int result = 0;
    if (score1 == SCORE_FOR_WIN) {
        printwinscreen(1);
        result = 1;
    } else if (score2 == SCORE_FOR_WIN) {
        printwinscreen(2);
        result = 1;
    }
    return result;
}
// функция для отрисовки экрана поздравления победителя
void printwinscreen(int player) {
    // очистка терминала
    printf("\033[H\033[J");
    // вложенные циклы с поиском середины экрана для вывода надписи
    for (int i = 0; i < HEIGHT + 2; i++) {
        if (i == HEIGHT / 2) {
            for (int j = 0; j < WIDTH - 11; j++) {
                if (j == (WIDTH + 2) / 2 - 9) {
                    printf(" PLAYER %d WIN ", player);
                } else {
                    printf("@");
                }
            }
        } else {
            for (int j = 0; j < WIDTH + 2; j++) {
                printf("@");
            }
        }
        printf("\n");
    }
}
// функция для отрисовки поля с текущими положениями мяча и ракеток
void draw(int ballX, int ballY, int racket1Y, int racket2Y, int score1, int score2) {
    // очистка терминала
    printf("\033[H\033[J");
    // цикл отрисовки верхней стенки поля (+2, чтобы именно пустое поле было той ширины, которая нам нужна)
    for (int i = 0; i < WIDTH + 2; i++) printf("%c", WALL_SYMBOL);
    printf("\n");
    // вложенные циклы с отрисовкой поля
    for (int y = 0; y < HEIGHT; y++) {
        printf("%c", WALL_SYMBOL);
        // отрисовка содержимого поля
        for (int x = 0; x < WIDTH; x++) {
            // вставляем символ мяча в координатах мяча
            if (x == ballX && y == ballY) {
                printf("%c", BALL_SYMBOL);
            }
            // вставляем символы ракетки
            else if (x == 0 && y >= racket1Y && y < racket1Y + RACKET_HEIGHT) {
                printf("%c", RACKET_SYMBOL);
            }
            // вставляем символы ракетки
            else if (x == WIDTH - 1 && y >= racket2Y && y < racket2Y + RACKET_HEIGHT) {
                printf("%c", RACKET_SYMBOL);
            }
            // отрисовываем середину поля
            else if (x == WIDTH / 2 || x == WIDTH / 2 - 1) {
                printf("%c", MIDDLE_SYMBOL);
            }
            // заполняем пустые участки поля
            else {
                printf("%c", EMPTY_SPACE_SYMBOL);
            }
        }
        printf("%c\n", WALL_SYMBOL);
    }
    // выводим строк с посчетом очков под игровым полем
    for (int i = 0; i < WIDTH + 2; i++) printf("%c", WALL_SYMBOL);
    printf("\nScore: Player 1: %d | Player 2: %d\n", score1, score2);
}