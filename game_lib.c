#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_lib.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// 1. Инициализация (используем указатель на структуру и malloc)
void initPlayer(Player *p, const char *targetName) {
    p->heart = 2;
    p->remembers_name = "####";
    // Выделяем 32 байта под имя системы
    p->knows_system = (char*)malloc(32 * sizeof(char)); 
    // Записываем начальное значение
    strcpy(p->knows_system, "???"); 
    
    // Выделяем память под имя динамически
    p->name = (char*)malloc(strlen(targetName) + 1);
    if (p->name != NULL) {
        strcpy(p->name, targetName);
    }
}

#ifdef _WIN32
#include <windows.h>
// Функция для включения поддержки ANSI-цветов в консоли Windows
void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // Включаем флаг ENABLE_VIRTUAL_TERMINAL_PROCESSING
    dwMode |= 0x0004; 
    SetConsoleMode(hOut, dwMode);
}
#endif

// 2. Очистка (обязательно для динамической памяти)
void freePlayer(Player *p) {
    if (p->name != NULL) {
        free(p->name); // Освобождаем память из-под имени
        p->name = NULL;
        free(p->knows_system);
    }
}

// 3. Динамическое чтение файла
char* readArtFromFile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Выделяем память ровно под размер файла
    char *string = (char*)malloc(fsize + 1);
    if (string) {
        fread(string, 1, fsize, f);
        string[fsize] = '\0';
    }
    fclose(f);
    return string; // Возвращаем указатель наружу
}

// 4. Плавный вывод текста
void printTypewriter(const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
#ifdef _WIN32
        Sleep(20);
#else
        usleep(20000);
#endif
    }
}

// 5. Валидация ввода
int getValidatedInput(int min, int max) {
    int val;
    printf("\x1b[36mВыберите (%d-%d): \x1b[0m", min, max);
    while (scanf("%d", &val) != 1 || val < min || val > max) {
        printf("Кто ты, чтобы идти против Системы? Выбрать можно только из предложенных вариантов (%d-%d): ", min, max);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return val;
}