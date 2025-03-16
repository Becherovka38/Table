#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <locale>

#ifdef __unix__
void os() {
    setlocale(LC_ALL, "");
}
#elif _WIN32
void os() {
    #include <Windows.h>
    std::setlocale(LC_ALL,"Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}
#endif

using namespace std;

struct student {
    wchar_t surname[1025] = L"";
    wchar_t name[1025] = L"";
    wchar_t patronymic[1025] = L"";
    int born;
    int course;
    wchar_t color[1025] = L"";
    int mark1;
    int mark2;
    int mark3;
    int mark4;
};



void iToC(wchar_t* outstr, int num, int i);
void wchar_tCheck(wchar_t* outstr);
int streamIntCheck(const wchar_t s[] = L"");
int courseCheck();
int markCheck();
int yearCheck();
void printMenu(int a = 0);
student tableInit();
void tableDraw(struct student table[], int tblcount);
void tableDrawMove(struct student p, wostream& out);
void tableEdit(struct student table[], int tblcount);
void tableSort(struct student table[], int tblcount);
void intSort(struct student table[], int tblcount, int sortType, int student::* ptr);
int tableDelete(struct student table[], int tblcount);
student* tableTxtLoad(struct student table[], int& s, int& size);
student* tableBinLoad(struct student table[], int& s, int& size);
void tableDrawToTxt(struct student table[], int tblcount);
void tableDrawToBin(struct student table[], int tblcount);
void tableConvert(struct student table[], int tblcount, int& s);
student* tableResize(struct student table[], int tblcount, int& size);

int main() {
    os();
    printMenu();
    int size{ 100 };
    student* table = new student[size]{};
    int tblcount{ 0 };
    int a{};
    do {
        a = streamIntCheck();
        switch (a) {
        case 1:
        {
            table[tblcount] = tableInit();;
            tblcount += 1;
            break;
        }
        case 2:
            if (tblcount == 0) {
                wcout << L"Таблица пустая" << '\n';
            }
            else {
                tableDraw(table, tblcount);
                tableEdit(table, tblcount);
            }
            break;
        case 3:
            if (tblcount == 0) {
                wcout << L"Таблица пустая" << '\n';
            }
            else {
                tblcount = tableDelete(table, tblcount);
            }
            break;
        case 4:
            if (tblcount == 0) {
                wcout << L"Таблица пустая" << '\n';
            }
            else {
                tableSort(table, tblcount);
            }
            break;
        case 5:
            printMenu(a);
            a = streamIntCheck();
            switch (a) {
            case 1:
                table = tableTxtLoad(table, size, tblcount);
                break;
            case 2:
                table = tableBinLoad(table, size, tblcount);
                break;
            case 3:
                if (tblcount == 0) {
                    wcout << L"Таблица пустая" << '\n';
                }
                else {
                    tableDrawToTxt(table, tblcount);
                }
                break;
            case 4:
                if (tblcount == 0) {
                    wcout << L"Таблица пустая" << '\n';
                }
                else {
                    tableDrawToBin(table, tblcount);
                    tableDraw(table, tblcount);
                }
                break;
            case 5:
                tableConvert(table, tblcount, size);
            case 0:
                a = 1;
                break;
            default:
                wcout << L"Команда введена неверно! Попробуйте ещё раз." << '\n';
                break;
            }
            break;
        case 0:
            wcout << L"Выход из программы" << '\n';
            break;
        default:
            wcout << L"Команда введена неверно! Попробуйте ещё раз." << '\n';
            break;
        }
        table = tableResize(table, tblcount, size);
        if (tblcount != 0 and a != 0 and a != 4) {
            tableDraw(table, tblcount);
        }
        if (a != 0) {
            printMenu(a);
        }
    } while (a);

    delete[] table;
    return 0;
}

void iToC(wchar_t* outstr, int num, int i) {
    wchar_t cnum[256] = L"";

    int j{ i - 1 };
    while (j != -1) {
        cnum[j] = (num % 10) + '0';
        num /= 10;
        j -= 1;
    }
    for (int k{ 0 }; k < i; k++) {
        outstr[k] = cnum[k];
    }
}

int streamIntCheck(const wchar_t s[]) {
    int val{};
    while (true) {
        if (s != L"") {
            wcout << s << '\n';
        }
        if (wcin >> val) {
            break;
        }
        else {
            wcout << L"Неверный ввод. Попробуйте ещё раз." << '\n';
            wcin.clear();
            wcin.ignore(32767, '\n');
        }
    }
    return val;
}

void wchar_tCheck(wchar_t* outstr) {
    wchar_t temp[1025] = L"";
    std::wstring temp1 = L"";
    int c{ 0 };

    while (true) {
        getline(wcin,temp1);
        for (int i{ 0 }; i < temp1.size(); i++) {
            if (strchr(" :;|,.?-_!",temp1[i])) {
                c += 1;
            }
        }
        if (temp1.size() == 0 or temp1.size() == c) {
            wcout << L"Данное поле не может быть пустым" << '\n';
            c = 0;
        }
        else {
            wcscpy(outstr,temp1.c_str());
            break;
        }
    };

}

int courseCheck() {
    while (true) {
        int m{ streamIntCheck() };
        if (m > 6 or m < 1) {
            wcout << L"Неправильно введён курс (1-6)" << '\n';
        }
        else {
            return m;
            break;
        }
    }
}

int markCheck() {
    while (true) {
        int m{ streamIntCheck() };
        if (m > 5 or m < 2) {
            wcout << L"Неправильный формат оценки (2-5)" << '\n';
        }
        else {
            return m;
            break;
        }
    }
}

int yearCheck() {
    while (true) {
        int year{ streamIntCheck() };
        if (year < 1980 or year > 2006) {
            wcout << L"Неправильный формат даты (1980-2006)" << '\n';
        }
        else {
            return year;
        }
    }
}

void printMenu(int a) {
    if (a != 5) {
        wcout << L"Выберите команду:" << '\n'
              << L"1. Добавление записи" << '\n'
              << L"2. Изменение записи" << '\n'
              << L"3. Удаление записи" << '\n'
              << L"4. Сортировка" << '\n'
              << L"5. Работа с файлами" << '\n'
              << L"0. Выход" << '\n';
    }
    else {
        wcout << L"Работа с файлами:" << '\n'
              << L"1. Загрузка базы данных из текстового файла" << '\n'
              << L"2. Загрузка базы данных из бинарного файла" << '\n'
              << L"3. Выгрузка базы данных в текстовый файл" << '\n'
              << L"4. Выгрузка базы данных в бинарный файл" << '\n'
              << L"5. Перевод файла из текстового в бинарный " << '\n'
              << L"0. Выход в главное меню" << '\n';
    }
}

struct student tableInit() {
    struct student p {};

    wcin.ignore();
    wcout << L"Введите название товара:" << '\n';
    wchar_tCheck(p.surname);
    wcout << L"Введите название поставщика:" << '\n';
    wchar_tCheck(p.name);
    wcout << L"Введите дату поступления товара:" << '\n';
    wchar_tCheck(p.patronymic);
    wcout << L"Введите цену товара:" << '\n';
    p.born = streamIntCheck();
    wcout << L"Введите артикул товара:" << '\n';
    p.course = streamIntCheck();
    wcin.ignore();
    wcout << L"Введите цвет товара:" << '\n';
    wchar_tCheck(p.color);
    wcout << L"Введите размер товара:" << '\n';
    p.mark2 = streamIntCheck();
    wcout << L"Запись добавлена" << '\n';
    return p;
}

void tableDraw(struct student table[], int tblcount) {
    // крышка
    wchar_t cover{'='};
    for (int i{ 0 }; i < 82; i++) {
        wcout << cover;
    }
    wcout << '\n';
    wcout
        << L"|" << setw(4) << L"№"
        << L"|" << setw(10) << L"Название" << L"|"
        << setw(10) << L"Поставщик" << L"|"
        << setw(10) << L"Дата" << L"|"
        << setiosflags(ios::left) << setw(10 + 2) << L"Цена" << L"|"
        << setw(9) << L"Артикул" << L"|"
        << setw(9) << L"Цвет" << L"|"
        << setw(9) << L"Размер" << L"|"
        << '\n';

    // крышка
    int c{ 0 };
    wcout << "|";
    for (int i{ 0 }; i < 80; i++) {
        wcout << cover;
    }
    wcout << "|" << '\n';
    c = 0;

    // печать данных таблицы
    for (int i{ 0 }; i < tblcount; i++) {
        int column{ 0 };
        wcout << "|" << setw(4) << setiosflags(ios::right) << i + 1 << resetiosflags(ios::right);
        tableDrawMove(table[i], wcout);
        if (i != tblcount - 1) {
            wcout << "|";
            for (int i{ 0 }; i < 8; i++) {
                if (column == 0) {
                    for (int j{ 0 }; j < 4; j++) {
                        wcout << cover;
                    }
                    wcout << "|";
                }
                else if (column == 4) {
                    for (int j{ 0 }; j < 12; j++) {
                        wcout << cover;
                    }
                    wcout << "|";
                }
                else if (column >= 5) {
                    for (int j{ 0 }; j < 9; j++) {
                        wcout << cover;
                    }
                    wcout << "|";
                }
                else {
                    for (int j{ 0 }; j < 10; j++) {
                        wcout << cover;
                    }
                    wcout << "|";
                }
                column += 1;
            }
            wcout << '\n';
            c = 0;
        }
    }

    // крышка
    for (int i{ 0 }; i < 82; i++) {
        wcout << cover;
    }
    wcout << '\n';
    // очистка флагов вывода
    wcout << resetiosflags(ios::right) << resetiosflags(ios::left);
}

void tableDrawMove(struct student p, wostream& out) {
    int i{ 0 }, c{ 0 }, j{ 0 };
    wchar_t buf[11] = L"";
    size_t len(max(wcslen(p.surname), wcslen(p.name)));
    len = max(len, wcslen(p.patronymic));
    while (i < len) {
        if (i > 0) {
            wcout << setw(5) << "|";
        }
        out << setiosflags(ios::right) << "|";
        for (int f{ c }; f < c + 10; f++) {
            buf[j] = p.surname[f];
            j += 1;
        }
        j = 0;
        out << setw(10) << std::wstring(buf) << "|";
        for (int f{ c }; f < c + 10; f++) {
            buf[j] = p.name[f];
            j += 1;
        }
        out << setw(10) << std::wstring(buf) << "|";
        j = 0;
        for (int f{ c }; f < c + 10; f++) {
            buf[j] = p.patronymic[f];
            j += 1;
        }
        j = 0;
        out << setw(10) << std::wstring(buf) << "|";
        if (i == 0) {
            out
                << resetiosflags(ios::right)
                << setiosflags(ios::left)
                << setw(12) << p.born << "|"
                << setw(9) << p.course << "|"
                << setw(9) << p.color << "|"
                << setw(9) << p.mark2 << "|"
                << '\n';

        }
        if (i != 0 and i % 10 == 0) {
            out
                << resetiosflags(ios::right)
                << setiosflags(ios::left)
                << setw(12) << "" << "|"
                << setw(9) << "" << "|"
                << setw(9) << "" << "|"
                << setw(9) << "" << "|"
                << '\n';
        }
        c += 10;
        i += 10;
    }
}

void tableEdit(struct student table[], int tblcount) {
    int row{}, column{};
    while (true) {
        row = streamIntCheck(L"Введите номер строки для изменения: ");
        column = streamIntCheck(L"Введите номер столбца для изменения (первый столбец - фамилия ученика): ");
        if (row > tblcount or row <= 0 or column > 9 or column <= 0) {
            wcout << L"Выход за границы значения строки или столбца" << '\n';
        }
        else {
            break;
        }
    }

    column -= 1;
    row -= 1;
    wcout << L"Введите новое значение элемента таблицы" << '\n';
    cin.ignore();
    switch (column) {
    case 0:
        wchar_tCheck(table[row].surname);
        break;
    case 1:
        wchar_tCheck(table[row].name);
        break;
    case 2:
        wchar_tCheck(table[row].patronymic);
        break;
    case 3:
        table[row].born = yearCheck();
        break;
    case 4:
        table[row].course = courseCheck();
        break;
    case 5:
        table[row].mark1 = markCheck();
        break;
    case 6:
        table[row].mark2 = markCheck();
        break;
    case 7:
        table[row].mark3 = markCheck();
        break;
    case 8:
        table[row].mark4 = markCheck();
        break;
    }
    wcout << L"Таблица изменена" << '\n';
}

int tableDelete(struct student table[], int tblcount) {
    int n{};
    while (true) {
        n = streamIntCheck(L"Введите номер строки для удаления") - 1;
        if (n < 0 or n >= tblcount) {
            wcout << L"Строки не существует" << '\n';
        }
        else {
            break;
        }
    }
    for (n; n < tblcount; n++) {
        table[n] = table[n + 1];
    }
    if (tblcount > 0) {
        tblcount -= 1;
    }
    else {
        tblcount = 0;
    }
    wcout << L"Таблица изменена" << '\n';
    return tblcount;
}

void tableSort(struct student table[], int tblcount) {
    int call{}, column{}, sortType{}, addit{ 0 };
    student temp{};

    while (true) {
        call = streamIntCheck(L"Укажите сортировку:\n1.Обычная\n2.По запросу");

        if (call == 2) {
            sortType = streamIntCheck(L"Укажите тип сортировки:\n1. По возрастанию\n2. По убыванию");
            student temp{};
            student* tablesort = new student[tblcount];
            int tabletblcount{ 0 };
            for (int i{ 0 }; i < tblcount; i++) {
                if (table[i].course == 2) {
                    tablesort[tabletblcount] = table[i];
                    tabletblcount += 1;
                }
            }
            if (tabletblcount == 0) {
                wcout << L"В таблице нет учеников второго курса" << '\n';
                break;
            }
            else {
                int s1{}, s2{};
                if (sortType == 1) {
                    for (int i{ 0 }; i < tabletblcount; i++) {
                        for (int j{ i + 1 }; j < tabletblcount; j++) {
                            s1 = tablesort[i].mark1 + tablesort[i].mark2 + tablesort[i].mark3 + tablesort[i].mark4;
                            s2 = tablesort[j].mark1 + tablesort[j].mark2 + tablesort[j].mark3 + tablesort[j].mark4;
                            if (s1 > s2) {
                                temp = tablesort[i];
                                tablesort[i] = tablesort[j];
                                tablesort[j] = temp;
                            }
                        }
                    }
                }
                else {
                    for (int i{ 0 }; i < tabletblcount; i++) {
                        for (int j{ i + 1 }; j < tabletblcount; j++) {
                            s1 = tablesort[i].mark1 + tablesort[i].mark2 + tablesort[i].mark3 + tablesort[i].mark4;
                            s2 = tablesort[j].mark1 + tablesort[j].mark2 + tablesort[j].mark3 + tablesort[j].mark4;
                            if (s1 < s2) {
                                temp = tablesort[i];
                                tablesort[i] = tablesort[j];
                                tablesort[j] = temp;
                            }
                        }
                    }
                }
                wcout << L"Запрос: упорядочить список по среднему баллу студентов второго курса" << '\n';
                tableDraw(tablesort, tabletblcount);
                break;
            }
            delete[] tablesort;
        }
        else if (call == 1) {
            column = streamIntCheck(L"Введите номер столбца для сортировки (первый столбец - фамилия ученика): ");
            sortType = streamIntCheck(L"Укажите тип сортировки:\n1. По возрастанию\n2. По убыванию");
            if (column >= 1 and column <= 9 and sortType >= 1 and sortType <= 2) {
                switch (column) {
                case 1:
                    if (sortType == 1) {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].surname[addit] == table[j].surname[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].surname[addit]) > wchar_t(table[j].surname[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    else {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].surname[addit] == table[j].surname[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].surname[addit]) < wchar_t(table[j].surname[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    break;
                case 2:
                    if (sortType == 1) {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].name[addit] == table[j].name[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].name[addit]) > wchar_t(table[j].name[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    else {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].name[addit] == table[j].name[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].name[addit]) < wchar_t(table[j].name[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    break;
                case 3:
                    if (sortType == 1) {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].patronymic[addit] == table[j].patronymic[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].patronymic[addit]) > wchar_t(table[j].patronymic[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    else {
                        for (int i{ 0 }; i < tblcount - 1; i++) {
                            for (int j{ i + 1 }; j < tblcount; j++) {
                                while (table[i].patronymic[addit] == table[j].patronymic[addit]) {
                                    addit += 1;
                                }
                                if (wchar_t(table[i].patronymic[addit]) < wchar_t(table[j].patronymic[addit])) {
                                    temp = table[i];
                                    table[i] = table[j];
                                    table[j] = temp;
                                    addit = 0;
                                }
                            }
                        }
                    }
                    break;
                case 4:
                    intSort(table, tblcount, sortType, &student::born);
                    break;
                case 5:
                    intSort(table, tblcount, sortType, &student::course);
                    break;
                case 6:
                    intSort(table, tblcount, sortType, &student::mark1);
                    break;
                case 7:
                    intSort(table, tblcount, sortType, &student::mark2);
                    break;
                case 8:
                    intSort(table, tblcount, sortType, &student::mark3);
                    break;
                case 9:
                    intSort(table, tblcount, sortType, &student::mark4);
                    break;
                }
                tableDraw(table, tblcount);
                break;
            }
            else {
                wcout << L"Не существует такого столбца или типа сортировки" << '\n';
            }
        }
        else {
            wcout << L"Не существует такого типа сортировки" << '\n';
        }
    }
}

void intSort(struct student table[], int tblcount, int sortType, int student::* ptr) {
    student temp{};
    if (sortType == 1) {
        for (int i{ 0 }; i < tblcount - 1; i++) {
            for (int j{ i + 1 }; j < tblcount; j++) {
                if (wchar_t(table[i].*ptr) > wchar_t(table[j].*ptr)) {
                    temp = table[i];
                    table[i] = table[j];
                    table[j] = temp;
                }
            }
        }
    }
    else {
        for (int i{ 0 }; i < tblcount - 1; i++) {
            for (int j{ i + 1 }; j < tblcount; j++) {
                if (wchar_t(table[i].*ptr) < wchar_t(table[j].*ptr)) {
                    temp = table[i];
                    table[i] = table[j];
                    table[j] = temp;
                }
            }
        }
    }
}

student* tableTxtLoad(struct student table[], int& s, int&size) {
    size = 0;
    wchar_t temp[4096] = L"";
    int mode{ 0 }, i{ 0 }, j{ 0 };
    student* ptr{};

    wifstream fin("database.txt");
#ifdef __unix__
    fin.imbue(locale(""));
#elif _WIN32
    fin.imbue(locale(".1251"));
#endif
    if (fin.is_open()) {
        while (!fin.eof()) {
            ptr = tableResize(table, size, s);
            table = ptr;
            student p{ 0 };
            fin.getline(temp, 4096);
            if(wcslen(temp) != 0){
                while (i < wcslen(temp)) {
                    if (strchr(",;:", temp[i]) == NULL and temp[i]) {
                        switch (mode) {
                        case 0:
                            p.surname[j] = temp[i];
                            break;
                        case 1:
                            p.name[j] = temp[i];
                            break;
                        case 2:
                            p.patronymic[j] = temp[i];
                            break;
                        case 3:
                            p.born = (temp[i] - '0') * pow(10, 3) + (temp[i + 2] - '0') * 10 * 10 + (temp[i + 3] - '0') * 10 + (temp[i + 4] - '0');
                            i += 4;
                            break;
                        case 4:
                            p.course = temp[i] - '0';
                            break;
                        case 5:
                            p.mark1 = temp[i] - '0';
                            break;
                        case 6:
                            p.mark2 = temp[i] - '0';
                            break;
                        case 7:
                            p.mark3 = temp[i] - '0';
                            break;
                        case 8:
                            p.mark4 = temp[i] - '0';
                            break;
                        }
                        j += 1;
                    }
                    else {
                        mode += 1;
                        j = 0;
                    }
                    i++;
                }
                table[size] = p;
                size++;
                mode = 0;
                i = 0;
            }

        }
    }
    else {
        wcout << L"Ошибка чтения файла" << '\n';
    }
    fin.close();
    wcout << L"Данные загружены" << '\n';
    return ptr;
}

student* tableBinLoad(struct student table[], int& s, int& size) {
    size = 0;
    student* ptr{};

    wifstream fin("database.bin", ios::binary);
    if (fin.is_open()) {
        while (fin.eof() == 0) {
            ptr = tableResize(table, size, s);
            table = ptr;
            fin.read((wchar_t*)&table[size], sizeof(table[size]));
            size += 1;
        }
        size -= 1;
    }
    else {
        wcout << L"Ошибка чтения файла" << '\n';
    }
    fin.close();
    wcout << L"Данные загружены" << '\n';
    return ptr;
}

void tableDrawToTxt(struct student table[], int tblcount) {
    wofstream fout("database.txt");
    fout.imbue(locale(""));
    if (fout.is_open()) {
        for (int i{ 0 }; i < tblcount; i++) {
            fout << table[i].surname << ";"
                 << table[i].name << ";"
                 << table[i].patronymic << ";"
                 << table[i].born << ";"
                 << table[i].course << ";"
                 << table[i].mark1 << ";"
                 << table[i].mark2 << ";"
                 << table[i].mark3 << ";"
                 << table[i].mark4 << ";";
            if (i < tblcount - 1) {
                fout << '\n';
            }
        }
    }
    else {
        wcout << L"Ошибка чтения файла" << '\n';
    }
    fout.close();
    wcout << L"Данные загружены" << '\n';
}

void tableDrawToBin(struct student table[], int tblcount) {
    wofstream fout("database.bin", ios::binary);

    if (fout.is_open()) {
        for (int i{ 0 }; i < tblcount; i++) {
            fout.write(reinterpret_cast<const wchar_t*>(&table[i]), sizeof(table[i]));
        }
    }
    else {
        wcout << L"Ошибка чтения файла" << '\n';
    }
    fout.close();

    wcout << L"Данные загружены" << '\n';
}

void tableConvert(struct student table[], int tblcount, int &s) {
    wcout << L"Выберите тип перевода:\n1. Из текстового в бинарный\n2. Из бинарного в текстовый" << '\n';
    int a{}, tempcount{};
    student* temptable = new student[100];
    while (true) {
        a = streamIntCheck();
        if (a == 1 or a == 2) {
            break;
        }
        else {
            cerr << L"Не существует такого типа перевода" << '\n';
        }
    }
    switch (a) {
    case 1:
        tableTxtLoad(temptable, s, tempcount);
        tableDrawToBin(temptable, tempcount);
        break;
    case 2:
        tableBinLoad(temptable, s, tempcount);
        tableDrawToTxt(temptable, tempcount);
        break;
    }
    for (int i{ 0 }; i < tempcount; i++) {
        temptable[i] = student{};
    }
    tempcount = 0;
    delete[] temptable;
    wcout << L"Перевод выполнен" << '\n';
}

student* tableResize(struct student table[], int tblcount, int& size) {
    if (tblcount >= size) {
        student* tabletemp = new student[size + size];
        memcpy(tabletemp, table, size*sizeof(student));
        size += size;
        delete[] table;
        table = tabletemp;
        tabletemp = NULL;
    }
    return table;
}
