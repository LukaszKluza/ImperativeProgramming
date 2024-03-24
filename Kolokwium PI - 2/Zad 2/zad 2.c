#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Vehicle_type { truck, car, v_ndef };

typedef struct{
    float load;
    short axes;
    int trailer_mass
}Truck_data;

typedef struct {
    char vehicle_ID[15];
    unsigned int places: 3;
    unsigned int driving_wheels: 3;
    unsigned int towbar :1;
}Car_data;

typedef struct {
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union {
        Car_data cs;
        Truck_data ts;
    } veh_union;
} Vehicle;


void print_owners(const Vehicle *tv, const int *t_ind, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        const Vehicle *vehicle = &tv[t_ind[i]];
        printf("%s\n", vehicle->owner);
    }
}


int delayed(Vehicle *tv, int size, const Date *base_date, char ***ppdelayed_owners) {
    char **pdelayed_owners = (char **)malloc(sizeof(char *) * size);
    int counter = 0;
    int c_day = base_date->d, c_mon = base_date->m, c_year = base_date->y;

    for (int i = 0; i < size; ++i) {
        int day = tv[i].valid_date->d, mon = tv[i].valid_date->m, year = tv[i].valid_date->y;
        if (c_year > year) {
            pdelayed_owners[counter] = strdup(tv[i].owner);
            counter++;
        } else if (c_mon > mon && c_year == year) {
            pdelayed_owners[counter] = strdup(tv[i].owner);
            counter++;
        } else if (c_day > day && c_mon == mon && c_year == year) {
            pdelayed_owners[counter] = strdup(tv[i].owner);
            counter++;
        }
    }
    *ppdelayed_owners = pdelayed_owners;
    return counter;
}

Vehicle* create_vehicle() {
    Vehicle *new_vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    new_vehicle->owner[0] = '\0';

    new_vehicle->valid_date->d = 1;
    new_vehicle->valid_date->m = 1;
    new_vehicle->valid_date->y = 1970;
    new_vehicle->v_type = v_ndef;

    return new_vehicle;
}

void move_date(Vehicle *v, int month) {
    int month_old = v->valid_date->m;
    int year = v->valid_date->y;

    int new_month = month_old + month;
    int new_year = year + new_month / 12;
    new_month %= 12;

    v->valid_date->m = new_month;
    v->valid_date->y = new_year;
}

void print_v_data(const Vehicle *v) {
    if (v->v_type == car) {
        printf("Car: ID: %s, ", v->veh_union.cs.vehicle_ID);
        printf("MIejsc: %u, ", v->veh_union.cs.places);
        printf("naped na: %u kola, ", v->veh_union.cs.driving_wheels);
        printf("hak: %s\n", v->veh_union.cs.towbar ? "TAK." : "NIE.");
    } else if (v->v_type == truck) {
        printf("Truck ");
        printf("Load %f, ", v->veh_union.ts.load);
        printf("Nacisk na os, %hd", v->veh_union.ts.axes);
        printf("Masa: %d\n", v->veh_union.ts.trailer_mass);
    }
}

int main() {

    return 0;
}