#include "prototypes/client.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void valid_cpf(const char *cpf, int *is_valid)
{
    if (strlen(cpf) != 11)
    {
        *is_valid = 0;
        return;
    }
    for (int i = 0; i < 11; i++)
    {
        if (cpf[i] < '0' || cpf[i] > '9')
        {
            *is_valid = 0;
            return;
        }
    }
    *is_valid = 1;
}


void cpf_exists(const char *cpf, int *exists)
{
    FILE *file = fopen(CLIENTS_FILE_PATH, "r");
    char line[256];
    *exists = 0;

    if (file == NULL)
    {
        perror("Erro ao abrir arquivo de clientes para verificação de CPF");
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, cpf) != NULL)
        {
            *exists = 1;
            break;
        }
    }

    fclose(file);
}

void register_client(const struct client *new_client)
{
    FILE *file = fopen(CLIENTS_FILE_PATH, "a");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo de clientes para registro");
        return;
    }

    int cpf_valid;
    valid_cpf(new_client->cpf, &cpf_valid);
    if (!cpf_valid)
    {
        fprintf(stderr, "CPF inválido.\n");
        fclose(file);
        return;
    }

    int cpf_exists_flag;
    cpf_exists(new_client->cpf, &cpf_exists_flag);
    if (cpf_exists_flag)
    {
        fprintf(stderr, "CPF já cadastrado.\n");
        fclose(file);
        return;
    }

    fprintf(file, "%s;%s;%s\n", new_client->name, new_client->cpf, new_client->phone);
    fclose(file);
}

void delete_client(const char *cpf)
{
    FILE *file = fopen(CLIENTS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a exclusão de cliente");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, cpf) == NULL)
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(CLIENTS_FILE_PATH);
    rename("temp.txt", CLIENTS_FILE_PATH);
}

void update_client(const struct client *updated_client)
{
    FILE *file = fopen(CLIENTS_FILE_PATH, "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];

    if (file == NULL || temp == NULL)
    {
        perror("Erro ao abrir arquivo durante a atualização de cliente");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, updated_client->cpf) != NULL)
        {
            fprintf(temp, "%s;%s;%s\n", updated_client->name, updated_client->cpf, updated_client->phone);
        }
        else
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(CLIENTS_FILE_PATH);
    rename("temp.txt", CLIENTS_FILE_PATH);
}


void load_clients()
{
    printf("Client Module: Load completed.\n");
}

void save_clients()
{
    printf("Client Module: Save completed.\n");
}

Client *find_client_by_cpf(const char *cpf)
{
    static Client found_client; 
    FILE *file = fopen(CLIENTS_FILE_PATH, "r");
    char line[256];

    if (!file)
    {
        perror("Erro ao abrir arquivo de clientes");
        return NULL;
    }

    while (fgets(line, sizeof(line), file))
    {
        char name[100], cpf_read[20], phone[20];

        sscanf(line, "%99[^;];%19[^;];%19[^\n]", name, cpf_read, phone);

        if (strcmp(cpf_read, cpf) == 0)
        {
            strcpy(found_client.name, name);
            strcpy(found_client.cpf, cpf_read);
            strcpy(found_client.phone, phone);

            fclose(file);
            return &found_client;
        }
    }

    fclose(file);
    return NULL; 
}