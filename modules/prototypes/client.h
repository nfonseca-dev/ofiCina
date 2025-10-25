#ifndef CLIENT_MODULE_H
#define CLIENT_MODULE_H

#define CLIENTS_FILE_PATH "clients.txt"

// Adicionado o typedef Client
typedef struct client
{
    char name[100];
    char cpf[15];
    char phone[15];
} Client; // <--- Adicionado

// Protótipos existentes (ajustados para usar Client)
void valid_cpf(const char *cpf, int *is_valid);
void cpf_exists(const char *cpf, int *exists);
void register_client(const Client *new_client);
void delete_client(const char *cpf);
void update_client(const Client *updated_client);

// Adicionado: Protótipo da função de busca usada em main.c
Client* find_client_by_cpf(const char *cpf);

// Adicionado: Protótipos das funções de persistência usadas em main.c
void load_clients();
void save_clients();

#endif