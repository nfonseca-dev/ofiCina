#include "prototypes/car.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


bool plate_exists(const char *plate)
{
    FILE *file = fopen(CARS_FILE_PATH, "r");
    char line[256];

    if (!file) return false;

    while (fgets(line, sizeof(line), file))
    {
        char stored_plate[12];
        sscanf(line, "%11[^;]", stored_plate);

        if (strcmp(stored_plate, plate) == 0)
        {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

void register_car(const Car *new_car)
{
    if (!new_car || !new_car->owner)
    {
        fprintf(stderr, "Carro ou proprietário inválido.\n");
        return;
    }

    if (plate_exists(new_car->plate))
    {
        fprintf(stderr, "❌ Placa já cadastrada!\n");
        return;
    }

    FILE *file = fopen(CARS_FILE_PATH, "a");
    if (!file)
    {
        perror("Erro ao abrir arquivo para registro");
        return;
    }

    fprintf(file, "%s;%s;%d;%s;%s\n",
        new_car->plate,
        new_car->model,
        new_car->year,
        new_car->owner->name,
        new_car->owner->cpf
    );

    fflush(file);
    fclose(file);

    printf("Salvando carro no arquivo...%s\n", CARS_FILE_PATH );

    printf("✅ Carro salvo com sucesso!\n");
}


void delete_car(const char *plate)
{
    FILE *file = fopen(CARS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a exclusão de carro");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, plate) == NULL)
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(CARS_FILE_PATH);
    rename("temp.txt", CARS_FILE_PATH);
}

void update_car(const struct car *updated_car)
{
    if (!updated_car || !updated_car->owner)
    {
        fprintf(stderr, "Erro de ponteiro NULL ao atualizar carro.\n");
        return;
    }

    FILE *file = fopen(CARS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a atualização de carro");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, updated_car->plate) != NULL)
        {
            fprintf(temp, "%s;%s;%d;%s;%s\n",
                    updated_car->plate,
                    updated_car->model,
                    updated_car->year,
                    updated_car->owner->name,
                    updated_car->owner->cpf);
        }
        else
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(CARS_FILE_PATH);
    rename("temp.txt", CARS_FILE_PATH);
}


void load_cars()
{
    printf("Car Module: Load completed.\n");
}

void save_cars()
{
    printf("Car Module: Save completed.\n");
}

Car *find_car_by_plate(const char *plate)
{
    FILE *file = fopen(CARS_FILE_PATH, "r");
    static Car found_car;
    char line[256];

    if (!file) return NULL;

    while (fgets(line, sizeof(line), file))
    {
        char stored_plate[12], model[50], owner_name[100], owner_cpf[20];
        int year;

        sscanf(line, "%11[^;];%49[^;];%d;%99[^;];%19[^\n]",
               stored_plate, model, &year, owner_name, owner_cpf);

        if (strcmp(stored_plate, plate) == 0)
        {
            strcpy(found_car.plate, stored_plate);
            strcpy(found_car.model, model);
            found_car.year = year;

            found_car.owner = find_client_by_cpf(owner_cpf);

            fclose(file);
            return &found_car;
        }
    }

    fclose(file);
    return NULL;
}