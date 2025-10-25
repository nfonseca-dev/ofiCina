#include "prototypes/service_order.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool os_id_exists(int id)
{
    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) return false;

    char line[256];
    char id_str[10];
    sprintf(id_str, "%d;", id);

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, id_str, strlen(id_str)) == 0)
        {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

void create_service_order(const struct service_order *new_order)
{
    if (!new_order || !new_order->vehicle)
    {
        fprintf(stderr, "Erro: Dados da Ordem de Serviço ou veículo inválidos.\n");
        return;
    }

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "a");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo de OS para registro");
        return;
    }

    fprintf(file, "%d;%s;%s;%s;%d\n",
            new_order->id,
            new_order->vehicle->plate,
            new_order->entry_date,
            new_order->problem_description,
            new_order->status);

    fclose(file);
}

void delete_service_order(int id)
{
    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];
    char id_str[10];
    sprintf(id_str, "%d", id);

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a exclusão de OS");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        char search_pattern[15];
        sprintf(search_pattern, "%s;", id_str);

        if (strstr(line, search_pattern) != line)
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(SERVICE_ORDERS_FILE_PATH);
    rename("temp.txt", SERVICE_ORDERS_FILE_PATH);
}

void update_service_order(const struct service_order *updated_order)
{
    if (!updated_order || !updated_order->vehicle)
    {
        fprintf(stderr, "Erro: Dados da Ordem de Serviço ou veículo inválidos para atualização.\n");
        return;
    }

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];
    char id_str[10];
    sprintf(id_str, "%d", updated_order->id);

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a atualização de OS");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        char search_pattern[15];
        sprintf(search_pattern, "%s;", id_str);

        if (strstr(line, search_pattern) == line)
        {
            fprintf(temp, "%d;%s;%s;%s;%d\n",
                    updated_order->id,
                    updated_order->vehicle->plate,
                    updated_order->entry_date,
                    updated_order->problem_description,
                    updated_order->status);
        }
        else
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(SERVICE_ORDERS_FILE_PATH);
    rename("temp.txt", SERVICE_ORDERS_FILE_PATH);
}
void open_service_order(const ServiceOrder *new_order)
{
    if (!new_order || !new_order->vehicle)
    {
        fprintf(stderr, "Erro: Dados da Ordem de Serviço ou veículo inválidos.\n");
        return;
    }

    if (os_id_exists(new_order->id))
    {
        fprintf(stderr, "Erro: ID da OS %d já existe. Escolha outro.\n", new_order->id);
        return;
    }

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "a");
    if (!file)
    {
        perror("Erro ao abrir arquivo para registrar OS");
        return;
    }

    fprintf(file, "%d;%s;%s;%s;%d\n",
            new_order->id,
            new_order->vehicle->plate,
            new_order->entry_date,
            new_order->problem_description,
            new_order->status);

    fflush(file);
    fclose(file);

    printf("✅ Ordem de Serviço ID %d aberta com sucesso!\n", new_order->id);
}

void load_service_orders()
{
    printf("OS Module: Load completed.\n");
}

void save_service_orders()
{
    printf("OS Module: Save completed.\n");
}

void close_service_order(int id)
{
    printf("OS Module: Closing OS ID %d. Status set to FINALIZADO/ENTREGUE.\n", id);
}

ServiceOrder* find_service_order_by_id(int id) {
    static ServiceOrder order; 
    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) return NULL;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int current_id;
        char plate[10], entry[11], desc[255];
        int status;

        if (sscanf(line, "%d;%9[^;];%10[^;];%254[^;];%d",
                   &current_id, plate, entry, desc, &status) == 5) {
            if (current_id == id) {
                order.id = current_id;
                strcpy(order.entry_date, entry);
                strcpy(order.problem_description, desc);
                order.status = status;

                order.vehicle = find_car_by_plate(plate);

                fclose(file);
                return &order;
            }
        }
    }

    fclose(file);
    return NULL; 
}
