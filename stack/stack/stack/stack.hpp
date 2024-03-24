#pragma once 
#include <stdio.h>

//structures 
enum study_field {
    AR,CS,MAT,CPE,CON,TRANS,
    PHY,PE,BIOTECH,GEOINF
};
static const char* study_fields[] = {
    "architecture","computer science","mathematics","computer engineering","construction",
    "transportation","physics","power engineering","biotechnology","geoinformatics"
};
struct MY_STUDENT {
    char* surname;
    int birth_year;
    study_field field;
    MY_STUDENT(const char* s="student", int y=2000, study_field f=CS);
    void read_student();
    void write_student();
    void save(const char* filename);
    void load(char* filename);
    ~MY_STUDENT();
};

struct stack_element {
    void* data;
    stack_element* next;
};

struct stack {
    stack_element* top;
    int size;
    stack();
    ~stack();
    //adding / removing 
    void add(void* obj);
    void pop();
    void clear();

    //searching
    void search();
    void search_by_name(char* name);
    void search_by_date(int* date);
    void search_by_field(int* a);

    //saving / reading 
    void save_to_file();
    void read_from_file();

    //showing
    void show_first();
    void show_stack();
};

//independent functions
void menu(bool& exit, stack& s);
int read_digit();
void WaitForEnterKey();
