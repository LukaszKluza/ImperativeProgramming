#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int day;
    int month;
    int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
    const Date *data1 = (const Date *)d1;
    const Date *data2 = (const Date *)d2;
    if(data1->year == data2->year){
        if(data1->month == data2->month){
            if(data1->day == data2->day){
                return 0;
            }
            return data1->day - data2->day;
        }
        return data1->month - data2->month;
    }
    return data1->year - data2->year;
}

// compare foods
int cmp(const void *a, const void *b) {
    const Food *food1 = (const Food*)a;
    const Food *food2 = (const Food*)b;
    int check = strcmp(food1->name,food2->name);
    if(check){
        return check;
    }
    if(food1->price != food2->price){
        if(food1->price - food2->price > 0){
            return 1;
        }
        return -1;
    }
    return cmp_date(&food1->valid_date, &food2->valid_date);
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, int *result) {
    size_t left = 0, right = nmemb;
    int tmp_result;
    while(left < right){
        size_t mid = (left+right)/2;
        tmp_result = compar(key,(const char *)base + (mid*size));
        if (!tmp_result){
            *result = 1;
            return (void *)((const char *)base + (mid*size));
        }
        if (tmp_result > 0){
            left = mid+1;
        }
        else{
            right = mid;
        }
    }
    *result = 0;
    return(void *)((const char *)base +(left*size));
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
    for(size_t i = 0; i<n; ++i){
        if (!strcmp(p->name, art)){
            printf("%.2f %d %02d.%02d.%02d\n", p->price, p->amount,p->valid_date.day,p->valid_date.month,p->valid_date.year);
        }
        p++;
    }
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    int found;
    Food *food = bsearch2(new,tab,*np,sizeof(Food),compar,&found);
    if(found){
        food->amount += new->amount;
    }
    else{
        for(Food *c = tab + *np; c >= food; --c){ //memmove
            memcpy(c,c-1,sizeof (Food));
        }
        (*np)++;
        memcpy(food,new,sizeof(Food));
    }
    return food;
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    Food x;
    int cnt = 0;
    for(int i=0; i<no; ++i){
        fscanf(stream, "%s %f %d %d.%d.%d",x.name,&x.price,&x.amount,&x.valid_date.day,&x.valid_date.month,&x.valid_date.year );
        if (sorted){
            add_record(tab, &cnt, cmp, &x);
        }
        else{
            memcpy(tab+i,&x, sizeof(Food));
        }
    }
    if(sorted) return cnt;
    return no;
}

int cmp_qs(const void *a, const void *b) {
    Food *fa = (Food*)a, *fb = (Food*)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    struct tm date = {0};
    time_t timestamp;
    qsort(food_tab, n, sizeof(Food), cmp_qs);
    date.tm_mday = curr_date.day;
    date.tm_mon = curr_date.month-1;
    date.tm_year = curr_date.year-1900;

    timestamp = mktime(&date);
    timestamp += days*3600*24;

    date = *localtime(&timestamp);
    Date new_date;
    new_date.day = date.tm_mday;
    new_date.month = date.tm_mon+1;
    new_date.year = date.tm_year+1900;

    float res = 0;
    Food *c = food_tab;
    while(c <food_tab+n){ //memmove
        if(cmp_date(&c->valid_date,&new_date) == 0)res += c->amount*c->price;
        c++;
    }
    return res;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL in_line:1;
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
}

int main(void) {
    Person person_tab[] = {
            {"William", {M,yes}, {21, 6, 1982}, "Charles III"},
            {"George", {M,yes}, {22, 7, 2013}, "William"},
            {"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
            {"Louis", {M,yes}, {23, 4, 2018}, "William"},
            {"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
            {"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
            {"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
            {"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
            {"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
            {"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
            {"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
            {"August", {M,yes}, {9, 2, 2021}, "Eugenie"},
            {"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
            {"James", {M,yes}, {17, 12, 2007}, "Edward"},
            {"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
            {"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
            {"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
            {"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
            {"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
            {"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
            {"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
            {"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
            {"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
            {"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
            {"Charles", {M,yes}, {1, 7, 1999}, "David"},
            {"Margarita", {F,yes}, {14, 5, 2002}, "David"},
            {"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
            {"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
            {"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"}
    };

    int to_do, no;
    int n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
            scanf("%s",buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // qsort
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 0);
            Date curr_date;
            int days;
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            scanf("%d", &days);
            printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
            break;
        case 3: // succession
            scanf("%d",&no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            create_list(person_tab,no_persons);
            print_person(person_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}