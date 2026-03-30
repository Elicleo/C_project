#ifndef GAME_LIB_H
#define GAME_LIB_H

// Структура игрока
typedef struct {
    int heart;
    char *knows_system;
    char *remembers_name;
    char *name; // Указатель на строку (выделим память динамически)
} Player;

// Прототипы функций библиотеки (уже 6 функций)
void initPlayer(Player *p, const char *targetName); // Передача по указателю
void freePlayer(Player *p);
void changeHealth(Player *p, int amount);
void enableColors();
char* readArtFromFile(const char *filename);       // Возвращает указатель на выделенную память
void printTypewriter(const char *text);
int getValidatedInput(int min, int max);

#endif