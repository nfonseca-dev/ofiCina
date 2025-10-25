#ifndef CAR_MODULE_H
#define CAR_MODULE_H

#include <stdbool.h>
#include "client.h" 

#define CARS_FILE_PATH "cars.txt"

typedef struct car {
    char plate[10];
    char model[50];
    int year;
    Client *owner; // Usa o typedef Client
} Car; // <--- Adicionado

// Protótipos existentes (ajustados para usar Car)
bool plate_exists(const char *plate);
void register_car(const Car *new_car);
void delete_car(const char *plate);
void update_car(const Car *updated_car);

// Adicionado: Protótipo da função de busca usada em main.c
Car* find_car_by_plate(const char *plate);

// Adicionado: Protótipos das funções de persistência usadas em main.c
void load_cars();
void save_cars();

#endif // CAR_MODULE_H