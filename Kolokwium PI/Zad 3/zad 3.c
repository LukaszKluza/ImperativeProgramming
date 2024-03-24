#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum VehicleType { car, bus, truck };
enum DriveType { electric, hybrid, combustion };

struct Vehicle {
    char owner[20];
    int date[3];
    enum VehicleType vehicle_type;
    enum DriveType drive_type;
    union {
        struct {
            int max_passengers;
            float engine_power;
        } car_data;
        struct {
            int number_of_seats;
            float number_of_standing_places;
        } bus_data;
        struct {
            float engine_capacity;
            float max_axle_load;
        } truck_data;
    } specific;
};

void new_owner(struct Vehicle* pvehicle) {
    scanf("%s", pvehicle->owner);
    scanf("%d %d %d", &pvehicle->date[0], &pvehicle->date[1], &pvehicle->date[2]);
    scanf("%d", (int*)&pvehicle->vehicle_type);
    scanf("%d", (int*)&pvehicle->drive_type);


    switch (pvehicle->vehicle_type) {
        case car:
            scanf("%d", &pvehicle->specific.car_data.max_passengers);
            scanf("%f", &pvehicle->specific.car_data.engine_power);
            break;

        case bus:
            scanf("%d", &pvehicle->specific.bus_data.number_of_seats);
            scanf("%f", &pvehicle->specific.bus_data.number_of_standing_places);
            break;

        case truck:
            scanf("%f", &pvehicle->specific.truck_data.engine_capacity);
            scanf("%f", &pvehicle->specific.truck_data.max_axle_load);
            break;
    }
}

int delayed(struct Vehicle *pvehicle, int size, int *base_date, char ***ppdelayed_owners){
    char **pdelayed_owners = (char **)malloc(sizeof(char *) * size);
    int counter = 0;
    int c_day = base_date[0], c_mon = base_date[1], c_year = base_date[2];

    for(int i = 0; i < size; ++i){
        int day = pvehicle[i].date[0], mon = pvehicle[i].date[1], year = pvehicle[i].date[2];
        if(c_year > year){
            pdelayed_owners[counter] = strdup(pvehicle[i].owner);
            counter++;
        } else if(c_mon > mon && c_year == year){
            pdelayed_owners[counter] = strdup(pvehicle[i].owner);
            counter++;
        } else if(c_day > day && c_mon == mon && c_year == year){
            pdelayed_owners[counter] = strdup(pvehicle[i].owner);
            counter++;
        }
    }
    *ppdelayed_owners = pdelayed_owners;
    return counter;
}

void print_vehicle(const struct Vehicle *pvehicle){
    printf("Owner: %s\n", pvehicle->owner);
    printf("Date: %d-%d-%d\n", pvehicle->date[0], pvehicle->date[1], pvehicle->date[2]);
    printf("Vehicle Type: %d\n", pvehicle->vehicle_type);
    printf("Drive Type: %d\n", pvehicle->vehicle_type);
    switch(pvehicle->vehicle_type){
        case car:
            printf("Max Passengers: %d\n", pvehicle->specific.car_data.max_passengers);
            printf("Engine Power: %.2f\n", pvehicle->specific.car_data.engine_power);
            break;
        case bus:
            printf("Number of Seats: %d\n", pvehicle->specific.bus_data.number_of_seats);
            printf("Number of Standing Places: %.2f\n", pvehicle->specific.bus_data.number_of_standing_places);
            break;
        case truck:
            printf("Engine Capacity: %.2f\n", pvehicle->specific.truck_data.engine_capacity);
            printf("Max Axle Load: %.2f\n", pvehicle->specific.truck_data.max_axle_load);
            break;
    }
}

int main() {
    struct Vehicle myVehicle;
    new_owner(&myVehicle);
    printf("Właściciel: %s\n", myVehicle.owner);
    printf("Data: %d-%d-%d\n", myVehicle.date[0], myVehicle.date[1], myVehicle.date[2]);

}