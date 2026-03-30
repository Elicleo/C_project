#include <stdio.h>
#include <stdlib.h>
#include "game_lib.h"
#include <string.h>
#include <locale.h>

#ifdef _WIN32
    #include <windows.h>
#endif
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_BOLD    "\x1b[1m"

// Прототипы сцен
void scene_School(Player *p);
void scene_Lunch(Player *p);
void scene_Park(Player *p);
void scene_End(Player *p);
void final(Player *p, int answer);
void renderFrame(const char *artFile, const char *artColor, Player *p);

void renderFrame(const char *artFile, const char *artColor, Player *p) {
    // #ifdef _WIN32
    //     system("cls");
    // #else
    //     system("clear");
    // #endif

    char *art = readArtFromFile(artFile);
    if (art) {
        printf("\n\n%s%s%s\n", artColor, art, COLOR_RESET);
        free(art); // ОСВОБОЖДАЕМ память сразу после отрисовки
    }

    printf("--- Игрок: %s | Hearts: %d ---\n\n", p->name, p->heart);
}

void changeHealth(Player *p, int amount) {
    p->heart += amount; // Прибавляем (если урон, то amount будет отрицательным)
    if (amount < 0) {renderFrame("broken_heart.txt", COLOR_RED, p);} else {renderFrame("living_heart.txt", COLOR_RED, p);}
    if (p->heart <= 0) {
        p->heart = -1; // Устанавливаем в -1 для триггера плохой концовки в scene_End
        printTypewriter("\n*В глазах резко темнеет...*\n");
        scene_End(p); // Перебрасываем в финал
        
        // После финала мы должны завершить программу, 
        // иначе мы вернемся в ту функцию, откуда вызвали урон.
        getchar();
        exit(0); // Полный выход из программы
    }
}

void one_question(Player *p) {
    printTypewriter("???: Прежде чем начнём, ты можешь задать вопрос. Только один. Что тебя интересует?");
    printf("\n1. Где я?\n2. Кто ты?\n3. Почему только один?\n4. У меня нет вопросов, давай сразу к веселью.\n");
    int answer = getValidatedInput(1, 4);
    if (answer == 1) {
        printTypewriter("???: Это мемориальное подземелье, созданное на основе твоих утраченных воспоминаний. Тебе нужно восстановить их, чтобы вернуться."); getchar();
    } else if (answer == 2) {
        strcpy(p->knows_system, "Система");
        printTypewriter("Система: Тебе правда нужна сейчас подобная бесполезная информация? Раз уж спрашиваешь, отвечу. Я - Система."); getchar();
    }  else if (answer == 3) {
        printTypewriter("???: Потому что я так сказал. Твой вопрос потрачен весьма бесполезно. Попытка была одна."); getchar();
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s: !?", p->name);
        printTypewriter(buffer); getchar();
    } 
    else {
        printTypewriter("???: Ты... Интересно, мне такое нравится."); getchar();
    }
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s: Раз с вопросами всё, идём веселиться. Но перед этим пара важных моментов.", p->knows_system);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Многие из твоих выборов будут влиять на вероятность успеха. Запомни, сделанный выбор нельзя изменить.", p->knows_system);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Изначально у тебя будет только 2 шанса на ошибку, так что будь внимательнее, чтобы не потратить их, не успев толком ничего начать.", p->knows_system);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Условно назовём эти шансы -Hearts-. Количество -Hearts- будет меняться. Хотя по-настоящему ты конечно не умрёшь. Не бойся.", p->knows_system);
    printTypewriter(buffer); getchar();
    scene_School(p);
}

void scene_School(Player *p) {
    renderFrame("school.txt", COLOR_CYAN, p);
    char buffer[256];

    printf("*Ты инстинктивно моргаешь и, снова открыв глаза, видишь...*"); getchar();
    printf("*Это школа? Да, определённо она. Мой старый класс. Всё выглядит таким знакомым и... далёким.*"); getchar();
    printTypewriter("Учитель: Недавно к нам перевелась новая ученица. С сегодняшнего дня она учится в вашем классе."); getchar();
    printTypewriter("Учитель: Её зовут ####. Ребята, поздоровайтесь!"); getchar();

    printf("*Как зовут? Кажется, учитель сказал ...*");
    printf("\n1. Lena\n2. Leo\n3. Cleo\n4. Celo\n");
    int answer = getValidatedInput(1, 4);
    if (answer == 3) {
        p->remembers_name = "Cleo";
        changeHealth(p, 1);
    } else {
        snprintf(buffer, sizeof(buffer), "%s: Промазать с первого раза... Тоже своего рода талант.", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    }
    
    printf("*%s. Да, наверное, это звучало так.*", p->remembers_name); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Привет! Буду рада со всеми познакомиться!", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "Учитель: У окна сидит %s. Место рядом как раз свободно. %s, будешь сидеть там.", p->name, p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("*Она подходит и с неловкой улыбкой садится на соседнее место.*", p->remembers_name); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Эм, привет... %s.", p->name, p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Приятно познакомитсья, %s!", p->remembers_name, p->name);
    printTypewriter(buffer); getchar();
    printf("*Её волосы...*");
    printf("\n1. окрашены в ярко-красный\n2. были кудрявыми\n3. собраны в высокий пучок\n");
    answer = getValidatedInput(1, 3);
    if (answer != 2) {
        snprintf(buffer, sizeof(buffer), "%s: Ты ведь стараешься, правда?", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}
    printf("*Она милая.*\n"); getchar();
    scene_Lunch(p);
}

void scene_Lunch(Player *p) {
    renderFrame("coffee.txt", COLOR_BLUE, p);
    char buffer[256];
    
    printf("*Очередная смена локации уже не вызывает удивления.*"); getchar();
    printf("*Школьный коридор почти пустой. Неудивительно, все ушли на обед и оккупировали столовую.*"); getchar();
    printf("*Пока никто из учителей не видит, %s сидит на подоконнике. В руках у неё два бумажных стаканчика.*", p->remembers_name); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Едва получилось успеть до толкучки. Вот, держи, твой ...", p->name);
    printTypewriter(buffer);
    printf("\n1. Milkyway\n2. Mars\n3. Bounty\n4. Twix\n");
    int answer = getValidatedInput(1, 4);
    if (answer != 4) {
        snprintf(buffer, sizeof(buffer), "%s: Вообще-то, она всегда делилась с тобой второй палочкой. Кажется, правой?", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}
    
    snprintf(buffer, sizeof(buffer), "%s: Здорово сработано! А у меня для тебя латте!", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "*Я беру из рук %s почти обжигающий стаканчик и вдыхаю горьковатый аромат.*", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Спасибо. Ты, смотрю, совсем себе не изменяешь. Снова ...", p->name);
    printTypewriter(buffer);
    printf("\n1. Апельсиновый фреш?\n2. Какао с маршмэллоу?\n3. Айс латтэ?\n4. Чёрный чай?\n");
    answer = getValidatedInput(1, 4);
    if (answer != 3) {
        snprintf(buffer, sizeof(buffer), "%s: Только ты пьёшь такое.", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}
    
    snprintf(buffer, sizeof(buffer), "%s: Ага. Грустно было бы забыть вкус любимого напитка.", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Ты что? Да разве любимое забывается?", p->name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Почему нет? Тебе просто ещё слишком мало лет, чтобы понять такое.", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: ...", p->name);
    printTypewriter(buffer);
    printf("\n1. Наверное, ты права.\n2. Возраст тут ни при чём.\n3. Сама-то!\n");
    answer = getValidatedInput(1, 3);

    printf("*От громкого смеха ещё не остывший кофе грозится расплескаться на белоснежную рубашку.*"); getchar();
    printf("*И всё-таки, думаю, у меня бы не получилось забыть.*"); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Кстати. А следующий урок у нас какой?", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: ...", p->name);
    printTypewriter(buffer);
    printf("\n1. Английский\n2. Ин. яз\n3. English\n4. КАЯиПК\n");
    answer = getValidatedInput(1, 4);

    snprintf(buffer, sizeof(buffer), "%s: Ну наконец! Уже соскучилась по нему!", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: ...", p->name);
    printTypewriter(buffer);
    printf("\n1. Кого ты обманываешь? Опять весь урок в окно смотреть будешь.\n2. Точно, ты ведь только его и знаешь из всех предметов.\n3. Не по английскому ты соскучилась, а по нашей учительнице.\n");
    answer = getValidatedInput(1, 3);
    if (answer != 1) {
        snprintf(buffer, sizeof(buffer), "%s: Не груби ей.", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}
    snprintf(buffer, sizeof(buffer), "*%s смотрит на меня с выражением |можно было сделать вид, что не знаешь|.*", p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("*Стаканчик с мягким стуком ударяется о край мусорки. Звонок.*"); getchar();

    scene_Park(p);
}

void scene_Park(Player *p) {
    renderFrame("forest.txt", COLOR_GREEN, p);
    char buffer[256];

    printf("*Ты осознаёшь, что идёшь по давно привычной каменной дорожке. В этот раз даже не опаздываешь.*"); getchar();
    snprintf(buffer, sizeof(buffer), "*Вот и %s. Она уже сидит на парковой скамейке.*", p->remembers_name);
    printTypewriter(buffer); getchar();
    printTypewriter("*Ты без удивления замечаешь в её руках ...*");
    printf("\n1. книгу\n2. музыкальный плеер с наушниками\n3. кубик рубика\n4. скетчбук и карандаш\n");
    int answer = getValidatedInput(1, 4);
    if (answer != 1) {
        snprintf(buffer, sizeof(buffer), "%s: Хоть немного полагайся на свою интуицию.", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}

    snprintf(buffer, sizeof(buffer), "%s: %s! Как всегда, ты уже на месте.", p->name, p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("*Она моментально отвлекается и переводит внимание на меня.*"); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Да, как всегда!", p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("*Улыбается.*"); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Ну как...  Как у тебя результы экзамена?", p->name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Вечно ты найдёшь что вспомнить... Ничего. Нормально.", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Нормально это сколько?", p->name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: ...", p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("1. 100\n2. 70\n3. 50\n");
    answer = getValidatedInput(1, 3);
    if (answer != 1) {
        snprintf(buffer, sizeof(buffer), "%s: Я о тебе даже худшего мнения, чем ты о ней.", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}
    printf("*Хм... Действительно. Нормально.*"); getchar();

    snprintf(buffer, sizeof(buffer), "%s: Но я ведь не про это поговорить хотела.", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Знаешь, на самом деле... Мы скоро переезжаем.", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Я пыталась спорить с родителями, но они сказали, что это не обсуждается. Всё уже решено.", p->remembers_name);
    printTypewriter(buffer); getchar();
    printf("*...*"); getchar();
    printf("*Учитывая её обстоятельства... Это должно было бы быть ожидаемо.*"); getchar();
    printf("*Но как же всё равно обидно.*"); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Далеко? Надолго? Нет, я имею в виду... Мы ведь... ещё увидимся?", p->name);
    printTypewriter(buffer); getchar();
    printf("*После моего вопроса...*"); getchar();
    printf("1. Её улыбка становится шире. Будто она мать ребёнка, задающего глупые вопросы.\n2. Улыбка на мгновение застывает на её лице, но она тут же подавляет свои эмоции. По привычке.\n3. Я вижу, как её глаза наполняются слезами. Она всегда была эмоциональной.\n");
    answer = getValidatedInput(1, 3);
    if (answer != 2) {
        snprintf(buffer, sizeof(buffer), "%s: Эй, ты. Да разве это могло быть про неё?", p->knows_system);
        printTypewriter(buffer); getchar();
        changeHealth(p, -1);
    } else {changeHealth(p, 1);}

    snprintf(buffer, sizeof(buffer), "%s: Да... Мне кажется, что да. Ты ведь тоже так думаешь?", p->remembers_name);
    printTypewriter(buffer); getchar();
    snprintf(buffer, sizeof(buffer), "%s: Я верю. Только обещай, что не забудешь меня, ладно?", p->name);
    printTypewriter(buffer); getchar();
    printf("*Мы засмеялись почти с облегчением.*"); getchar();
    printf("*Время всегда течёт так быстро.*"); getchar();
    printf("*Почему-то внезапно... Так хочется спать...*"); getchar();
    scene_End(p);
}

void scene_End(Player *p) {
    renderFrame("window.txt", COLOR_YELLOW, p);
    char buffer[512];
    
    if (p->heart < 0) {
        printf("*Тебе кажется, что ты начинаешь падать в глубокую яму. И тут ты просыпаешься.*"); getchar();
        printf("*Ты дома, в своей кровати. Тело ломит, а веки такие тяжёлые, что разлепить их удаётся с большим трудом*"); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Ах, что за... Давно у меня не было кошмаров. Только... почему я ничего не помню?..", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Без раницы. Мой телефон?..", p->name);
        printTypewriter(buffer); getchar();
        printf("Болезненно яркий свет дисплея слепит глаза. Цифры на нём подсказывают: 3:02 am."); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Ну дела. Надо попробовать уснуть снова.", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Самочувствие такое отстойное, неужели сезонная простуда наконец настигла? Похоже, на этих выходных я из дома ни ногой...", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Поздравляю, прохождение подземелья окончено! Хотя... Поздравлять не с чем. Тебе так и не удалось восстановить воспоминания.", p->knows_system);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Настолько всё заруинить удалось бы, наверное, только тебе, %s.", p->knows_system, p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: ?", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Этот странный голос в голове... Видимо, всё ещё отголоски кошмара. Я ведь не схожу с ума, в самом деле? Хах.", p->name);
        printTypewriter(buffer); getchar();
    } else if (p->heart < 2 || p->remembers_name != "Cleo") {
        printf("*Ты открываешь глаза и понимаешь, что находишься в своей кровати. Яркое субботнее солнце уже начинает пробиваться сквозь задёрнутые шторы.*"); getchar();
        printf("*Даже после длительного сна всё ещё чувствуется усталось и лёгкий озноб.*"); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Надеюсь, я не заболеваю... Надо скорее принять таблетки, пока не стало хуже.", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Кажется, уже довольно поздно. Время...", p->name);
        printTypewriter(buffer); getchar();
        printf("*Ты протягиваешь руку к прикроватной тумбочке, пытаясь наощупь достать телефон. Вот же он. Кружка с водой, задетая твоей рукой, лишь чудом остаётся стоять на краю тубмы.*"); getchar();
        printf("*Включив телефон, ты первом делом обращаешь внимание не на время, а на новые сообщения, отправленные около часа назад.*"); getchar();
        snprintf(buffer, sizeof(buffer), "+79#??&#*?$*: Привет, %s! Это я, Cleo, мы вместе учились в средней школе. Надеюсь, ты ещё помнишь меня? Хахахаха", p->name);
        printTypewriter(buffer); getchar();
        printTypewriter("+79#??&#*?$*: Всё-таки много воды утекло... В любом, случае, я снова в городе. Не хочешь встретиться? Например, завтра?.."); getchar();
        printTypewriter("+79#??&#*?$*: Если неудобно, ничего страшного, можно в другой день! Я задержусь здесь на какое-то время. Так что... Жду твоего ответа."); getchar();
        printf("*Незнакомый номер. Незнакомое имя. Ты не помнишь ни одного человека, которого звали бы Cleo*"); getchar();
        final(p, 1);
    } else {
        printf("*Ты открываешь глаза и понимаешь, что находишься в своей кровати. Яркое субботнее солнце уже начинает пробиваться сквозь задёрнутые шторы.*"); getchar();
        printf("*Тело такое отдохнувшее. Всегда бы так высыпаться.*"); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Мне было показалось вчера, что могу заразиться от коллег. Видимо, пронесло...", p->name);
        printTypewriter(buffer); getchar();
        printf("*Ты довольно потягиваешься и берёшь телефон с прикроватной тумбочки, чтобы проверить последние новости.*"); getchar();
        printf("*Включив телефон, ты первом делом обращаешь внимание не на время, а на новые сообщения, отправленные около часа назад.*\n"); getchar();
        snprintf(buffer, sizeof(buffer), "+79#??&#*?$*: Привет, %s! Это я, Cleo, мы вместе учились в средней школе. Надеюсь, ты ещё помнишь меня? Хахахаха", p->name);
        printTypewriter(buffer); getchar();
        printTypewriter("+79#??&#*?$*: Всё-таки много воды утекло... В любом, случае, я снова в городе. Не хочешь встретиться? Например, завтра?.."); getchar();
        printTypewriter("+79#??&#*?$*: Если неудобно, ничего страшного, можно в другой день! Я задержусь здесь на какое-то время. Так что... Жду твоего ответа."); getchar();
        printf("*Незнакомый номер. %s.*", (p->remembers_name == "Cleo") ? "Но знакомое имя" : "Незнакомое имя"); getchar();
        printf("*Ты задумываешься на пару секунд и после уверенно набираешь сообщение...*\n");
        if (p->remembers_name != "Cleo") {
            printf("\n1. ?\n2. ?\n3. ?\n");
        } else {
            printf("1. Извините, кажется, вы ошиблись номером. До свидания.\n2. Сколько лет сколько зим. Извини, сейчас период очень загруженный. Ближайший месяц точно не смогу.\n3. Cleo! Конечно я помню тебя. Давай встретимся, у меня как раз нет планов на эти выходные.\n");
        }
        int answer = getValidatedInput(1, 3);
        final(p, answer);
    }
}

void final(Player *p, int answer) {
    if (answer == 1) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "@%s: Извините, кажется, вы ошиблись номером. До свидания.", p->name);
        printTypewriter(buffer); getchar();
        printf("*Ты тут же забыв о произошедшем, ты начинаешь свою субботу как обычно. Столько дел накопилось за неделю, и со всем нужно разобраться... Ещё эта слабость и ломота в теле так не ко времени.*"); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Cleo? А что Cleo? Прочитав подобные сообщения... Конечно она больше ничего не писала.", p->knows_system);
        printTypewriter(buffer); getchar();        
        snprintf(buffer, sizeof(buffer), "%s: Если бы только у тебя получилось бережнее относиться к своим воспоминаниям... и к людям. Такого бы не произошло.", p->knows_system);
        printTypewriter(buffer); getchar();
        printf("*Пытаясь избавиться от назойливого звона в ушах, ты встряхиваешь пару раз головой.*"); getchar();
        printf("*Кажется, помогло. Больше никаких лишних звуков. Ничего лишнего в твоей жизни.*"); getchar();
    } else if (answer == 2) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "@%s: Сколько лет сколько зим. Извини, сейчас период очень загруженный. Ближайший месяц точно не смогу.", p->name);
        printTypewriter(buffer); getchar();
        printf("*Ты тут же забыв о произошедшем, ты начинаешь свою субботу как обычно. Столько дел накопилось за неделю, и со всем нужно разобраться... Ещё эта слабость и ломота в теле так не ко времени.*"); getchar();
        printf("*Лишь спустя долгое время приходит ответ.*"); getchar();
        printTypewriter("+79#??&#*?$*: Ох, понимаю... Надеюсь, и у тебя всё в порядке."); getchar();
        snprintf(buffer, sizeof(buffer), "%s: ...", p->knows_system);
        printTypewriter(buffer); getchar();
    } else if (answer == 3) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "@%s: Cleo! Конечно я помню тебя. Давай встретимся, у меня как раз нет планов на эти выходные.", p->name);
        printTypewriter(buffer); getchar();
        printf("*Ты немного лукавишь насчёт отсутствия планов, но... Это же Cleo.*"); getchar();
        printTypewriter("@Cleo: Здорово! Тогда в 12, буду ждать."); getchar();
        printTypewriter("@Cleo: На нашем месте, конечно)"); getchar();
        printf("*Лишь прочитав последнее сообщение, ты понимаешь...*"); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Я... оказывается, скучаю...", p->name);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: У тебя правда получилось. Честно, это... неожиданно.", p->knows_system);
        printTypewriter(buffer); getchar();
        snprintf(buffer, sizeof(buffer), "%s: Мои искренние поздравления, %s!", p->knows_system, p->name);
        printTypewriter(buffer); getchar();
    }
}

int main() {
    setlocale(LC_ALL, ".65001"); 
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
        enableColors();
    #endif
    setvbuf(stdout, NULL, _IONBF, 0);

    char tempName[50];
    printf("*Какое-то странное место. Где я? Что только что произошло? Не получается вспомнить ничего полезного.*");
    printf("\n(Здесь и далее нажимайте Enter, чтобы продолжить)"); getchar();
    printTypewriter("???: Привет. Как тебя зовут? Назови себя: ");
    fgets(tempName, 50, stdin);
    tempName[strcspn(tempName, "\n")] = 0; // Удаляем символ переноса строки

    Player hero;
    initPlayer(&hero, tempName); // Здесь выделяется динамическая память под имя
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "???: Правда %s? Хах, какое странное имя. Ну ладно, %s, здесь будет весело.", hero.name, hero.name);
    printTypewriter(buffer); getchar();
    one_question(&hero);
    // scene_Lunch(&hero);
    getchar();

    // if (hero.heart <= 0) {
    //     scene_End(&hero);
    // }

    snprintf(buffer, sizeof(buffer), "%s: Мемориальное подземелье закрыто.", hero.knows_system);
    printTypewriter(buffer); getchar();
    if (hero.knows_system == "Система") {
        snprintf(buffer, sizeof(buffer), "%s: Система... Что же это было такое?", hero.name);
        printTypewriter(buffer); getchar();
    }
    printf("THE END.");
    freePlayer(&hero); 
    return 0;
}