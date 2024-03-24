#include "stack.hpp"
#include <stdio.h>
#include <cstdlib>
#include "stack.hpp"
#include <string.h>
#include <cstdio>
#define cls system("cls")

void WaitForEnterKey() {
    printf("Press Enter to continue...");
    fflush(stdout);
    while (getchar() != '\n');
}
int read_digit() {
    int result = 0;
    int ch = getchar();
    while (ch != '\n' && ch != EOF) {
        if (ch >= '0' && ch <= '9') {
            result = ch - '0';
            return result;
        }
        else {
            printf("Error: Invalid input. Please enter a number.\n");
            result = -1;
            break;
        }
        ch = getchar();
    }
    // Ignorowanie dodatkowych znaków wejścia
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }
}
int read_int() {
    char buffer[100];
    int result = -1;
    bool check = true;
    while (true) {
        printf("->");
        fgets(buffer, sizeof(buffer), stdin);
        for (int i = 0; buffer[i] != '\n'; i++) {
            if (buffer[i] < '0' || buffer[i]>'9')
                check = false;
        }
        if (check) {
            buffer[strcspn(buffer, "\n")] = '\0'; // Usuń znak nowej linii z końca
            result = atoi(buffer);
            break;
        }
        else {
            printf("Error: Invalid input. Please enter a number.\n");
        }
        check = true;
    }
    return result;
}



//MY_STUDENT funkctions
MY_STUDENT::MY_STUDENT(const char* s, int y, study_field f) {
    surname = new char[strlen(s) + 1];
    strcpy_s(surname, strlen(s) + 1, s);
    birth_year = y;
    field = f;
}
MY_STUDENT::~MY_STUDENT() {
    delete[] surname;
}
void MY_STUDENT::read_student() {
    char buffer[100];
    // Wczytaj nazwisko studenta
    printf("Enter surname:\n->");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Usuń znak nowej linii z końca
    surname = new char[strlen(buffer) + 1];
    strcpy_s(surname,strlen(buffer)+1, buffer);

    // Wczytaj rok urodzenia studenta
    printf("Enter birth year:\n");
    birth_year = read_int();

    // Wczytaj pole studiów studenta
    printf("Enter field of study: \n");
    for (int i = 0; i < sizeof(study_fields) / sizeof(study_fields[0]); i++) {
        printf("%d - %s\n", i, study_fields[i]);
    }
    int f = -1;
    while (true) {
        f = read_int();
        if (f >= 0 && f < sizeof(study_fields) / sizeof(study_fields[0])) {
            break;
        }
        else {
            printf("Enter a number from 1 to %d\n", (int)(sizeof(study_fields) / sizeof(study_fields[0])) - 1);
        }
    }
    field = static_cast<study_field>(f);
}
void MY_STUDENT::write_student(){ 
    printf("Surname: %s, Birth year: %d, Field: %s.\n", surname, birth_year,study_fields[field]);
}
void MY_STUDENT::save(const char* filename) {
    FILE* file = fopen(filename, "ab");
}

//stack funkctions
stack::stack() {
    top = NULL;
    size = 0;
}
stack::~stack() {
}
void stack::add(void* obj) {
 stack_element* new_element = new stack_element;
    new_element->data = obj;
    new_element->next = top;
    top = new_element;
    size++;
    printf("Element was added to the stack.\n");
    printf("Current stack size: %d\n",size);
}
void stack::pop() {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    stack_element* tmp = top;
    top = top->next;
    delete tmp;
    size--;
    printf("Last added element was removed.\n");
    if (top == NULL) {
        printf("Now the stack is empty.\n");
    }
    else {
        printf("Current stack size: %d\n", size);
    }
}
void stack::clear() {
    while (top != NULL) {
        stack_element* tmp = top;
        top = top->next;
        delete tmp;
        size--;
    }
    printf("The stack has been cleared!\n");
}
void stack::show_first() {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Firts element of the stack:\n");
    MY_STUDENT* a = static_cast<MY_STUDENT*>(top->data);
    a->write_student();
}
void stack::show_stack() {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack contents:\n");
    stack_element* curr = top;
    int i = 1;
    while (curr != NULL) {
        printf("%d. ",i);
        MY_STUDENT* a = static_cast<MY_STUDENT*>(curr->data);
        a->write_student();
        curr = curr->next;
        i++;
    }
}
void stack::save_to_file() {
}

void menu(bool& exit, stack &s) {
    cls;
    printf("Choose operation which you want to do.\n");
    printf("1 - Add to the stack.\n");
    printf("2 - Remove last added element.\n");
    printf("3 - Show first the stack element.\n");
    printf("4 - Show all elements from the stack.\n");
    printf("5 - Clear the stack.\n");
    printf("6 - Research the stack.\n");
    printf("7 - Read the stack from the file.\n");
    printf("8 - Save the stack to file.\n");
    printf("9 - Exit program.\n");
    int choose = -1;
    while (true) {
        choose = read_int();
        if (choose > 0 && choose < 10) {
            break;
        }
        else {
            printf("Enter a number from 1 to 9.\n");
        }
    }
    MY_STUDENT* a = new MY_STUDENT();
    void* data;
    switch (choose) {
    case 1:
        a->read_student();
        data = a;
        s.add(data);
        a->write_student();
        WaitForEnterKey();
        break;
    case 2:
        s.pop();
        WaitForEnterKey();
        break;
    case 3:
        s.show_first();
        WaitForEnterKey();
        break;
    case 4:
        s.show_stack();
        WaitForEnterKey();
        break;
    case 5:
        s.clear();
        WaitForEnterKey();
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        printf("exit = 0");
        exit = false;
        break;
    }
}