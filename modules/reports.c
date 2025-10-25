
#include "prototypes/reports.h"
#include "prototypes/car.h"
#include "prototypes/service_order.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *get_status_description(StatusOS status)
{
    switch (status)
    {
    case AGUARDANDO_AVALIACAO: return "AGUARDANDO AVALIACAO";
    case EM_REPARO: return "EM REPARO";
    case FINALIZADO: return "FINALIZADO";
    case ENTREGUE: return "ENTREGUE";
    default: return "DESCONHECIDO";
    }
}

void generate_vehicle_history_report()
{
    char plate[10];
    printf("Digite a placa do veículo para o histórico: ");
    scanf("%9s", plate);

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir arquivo de ordens de serviço.\n");
        return;
    }

    char filename[50];
    sprintf(filename, "historico_%s.txt", plate);
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Erro ao criar arquivo de relatório.\n");
        fclose(file);
        return;
    }

    fprintf(out, "--- Histórico de Serviços para Placa %s ---\n", plate);
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        int id, status;
        char plat[10], date[11], desc[255];

        if (sscanf(line, "%d;%9[^;];%10[^;];%254[^;];%d",
                   &id, plat, date, desc, &status) == 5)
        {
            if (strcmp(plat, plate) == 0)
            {
                fprintf(out, "ID: %d | Data: %s | Problema: %s | Status: %s\n",
                        id, date, desc, get_status_description((StatusOS)status));
            }
        }
    }

    fclose(file);
    fclose(out);

    printf("Relatório gerado em '%s'.\n", filename);
}

void generate_daily_orders_report()
{
    char date[11];
    printf("Digite a data (DD/MM/AAAA) para listar as OS: ");
    scanf("%10s", date);

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir arquivo de ordens de serviço.\n");
        return;
    }

    char filename[50];
    sprintf(filename, "os_diarias_%s.txt", date);
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Erro ao criar arquivo de relatório.\n");
        fclose(file);
        return;
    }

    fprintf(out, "--- Ordens de Serviço em %s ---\n", date);
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        int id, status;
        char plat[10], entry[11], desc[255];

        if (sscanf(line, "%d;%9[^;];%10[^;];%254[^;];%d",
                   &id, plat, entry, desc, &status) == 5)
        {
            if (strcmp(entry, date) == 0)
            {
                fprintf(out, "ID: %d | Placa: %s | Problema: %s | Status: %s\n",
                        id, plat, desc, get_status_description((StatusOS)status));
            }
        }
    }

    fclose(file);
    fclose(out);
    printf("Relatório gerado em '%s'.\n", filename);
}

void generate_client_cars_report()
{
    char cpf[15];
    printf("Digite o CPF do cliente para listar os veículos: ");
    scanf("%14s", cpf);

    FILE *file = fopen(CARS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir arquivo de veículos.\n");
        return;
    }

    char filename[50];
    sprintf(filename, "veiculos_cliente_%s.txt", cpf);
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Erro ao criar arquivo de relatório.\n");
        fclose(file);
        return;
    }

    fprintf(out, "--- Veículos do Cliente CPF %s ---\n", cpf);
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char plate[10], model[50], owner_name[50], owner_cpf[15];
        int year;

        if (sscanf(line, "%9[^;];%49[^;];%d;%49[^;];%14s",
                   plate, model, &year, owner_name, owner_cpf) == 5)
        {
            if (strcmp(owner_cpf, cpf) == 0)
            {
                fprintf(out, "Placa: %s | Modelo: %s | Ano: %d\n", plate, model, year);
            }
        }
    }

    fclose(file);
    fclose(out);
    printf("Relatório gerado em '%s'.\n", filename);
}

void generate_orders_by_status_report()
{
    int status_code;
    printf("Digite o status para filtrar (0=AGUARDANDO, 1=REPARO, 2=FINALIZADO, 3=ENTREGUE): ");
    scanf("%d", &status_code);

    StatusOS target_status = (StatusOS)status_code;

    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir arquivo de ordens de serviço.\n");
        return;
    }

    char filename[50];
    sprintf(filename, "os_status_%d.txt", status_code);
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Erro ao criar arquivo de relatório.\n");
        fclose(file);
        return;
    }

    fprintf(out, "--- Ordens de Serviço com Status: %s ---\n", get_status_description(target_status));
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        int id, status;
        char plat[10], entry[11], desc[255];

        if (sscanf(line, "%d;%9[^;];%10[^;];%254[^;];%d",
                   &id, plat, entry, desc, &status) == 5)
        {
            if (status == status_code)
            {
                fprintf(out, "ID: %d | Placa: %s | Data: %s | Problema: %s\n",
                        id, plat, entry, desc);
            }
        }
    }

    fclose(file);
    fclose(out);
    printf("Relatório gerado em '%s'.\n", filename);
}

void generate_top_clients_report()
{
    FILE *file = fopen(SERVICE_ORDERS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir arquivo de ordens de serviço.\n");
        return;
    }

    typedef struct {
        char cpf[15];
        int count;
    } ClientCount;

    ClientCount counts[1000]; 
    int total_clients = 0;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char plate[10], entry[11], desc[255], owner_cpf[15];
        int id, status;

        if (sscanf(line, "%d;%9[^;];%10[^;];%254[^;];%d",
                   &id, plate, entry, desc, &status) == 5)
        {
            Car *car = find_car_by_plate(plate);
            if (!car) continue;

            int i, found = 0;
            for (i = 0; i < total_clients; i++)
            {
                if (strcmp(counts[i].cpf, car->owner->cpf) == 0)
                {
                    counts[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && total_clients < 1000)
            {
                strcpy(counts[total_clients].cpf, car->owner->cpf);
                counts[total_clients].count = 1;
                total_clients++;
            }
        }
    }

    fclose(file);

    char filename[50];
    sprintf(filename, "clientes_recorrentes.txt");
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Erro ao criar arquivo de relatório.\n");
        return;
    }

    fprintf(out, "--- Clientes Mais Recorrentes ---\n");
    for (int i = 0; i < total_clients; i++)
    {
        fprintf(out, "CPF: %s | Número de OS: %d\n", counts[i].cpf, counts[i].count);
    }

    fclose(out);
    printf("Relatório gerado em '%s'.\n", filename);
}
