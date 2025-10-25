
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/prototypes/client.h"
#include "modules/prototypes/car.h"
#include "modules/prototypes/service_order.h"
#include "modules/prototypes/reports.h"

void display_main_menu();
void handle_client_menu();
void handle_car_menu();
void handle_os_menu();
void handle_report_menu();

void initialize_system()
{
    printf("Carregando dados dos arquivos...\n");
    load_clients();
    load_cars();
    load_service_orders();
    printf("Dados carregados com sucesso.\n");
}

void finalize_system()
{
    printf("Salvando dados nos arquivos e liberando memória...\n");
    save_clients();
    save_cars();
    save_service_orders();
    printf("Sistema finalizado.\n");
}

int main()
{
    int option;

    initialize_system();

    do
    {
        display_main_menu();
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            option = 0;
        }

        switch (option)
        {
        case 1:
            handle_client_menu();
            break;
        case 2:
            handle_car_menu();
            break;
        case 3:
            handle_os_menu();
            break;
        case 4:
            handle_report_menu();
            break;
        case 0:
            printf("Saindo do sistema...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            break;
        }

    } while (option != 0);

    finalize_system();

    return 0;
}

void display_main_menu()
{
    printf("\n--- Menu Principal da Oficina ---\n");
    printf("1. Cadastro de Clientes\n");
    printf("2. Cadastro de Veículos\n");
    printf("3. Ordens de Serviço\n");
    printf("4. Relatórios\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

void handle_client_menu()
{
    int option;
    do
    {
        printf("\n--- Menu de Clientes ---\n");
        printf("1. Inserir Novo Cliente\n");
        printf("2. Remover Cliente (por CPF)\n");
        printf("3. Atualizar Cliente (por CPF)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            option = 0;
        }

        switch (option)
        {
        case 1:
        {
            Client new_client;
            printf("  > Inserção de Cliente\n");
            printf("  Nome: ");
            scanf("%99s", new_client.name);
            printf("  CPF (Chave Única): ");
            scanf("%14s", new_client.cpf);
            printf("  Telefone: ");
            scanf("%14s", new_client.phone);

            register_client(&new_client);
            break;
        }
        case 2:
        {
            char cpf_to_delete[15];
            printf("  > Remoção de Cliente\n");
            printf("  Digite o CPF a ser removido: ");
            scanf("%14s", cpf_to_delete);

            delete_client(cpf_to_delete);
            break;
        }
        case 3:
        {
            Client updated_client;
            printf("  > Atualização de Cliente\n");
            printf("  Digite o CPF do cliente a atualizar: ");
            scanf("%14s", updated_client.cpf);
            printf("  Novo Nome: ");
            scanf("%99s", updated_client.name);
            printf("  Novo Telefone: ");
            scanf("%14s", updated_client.phone);

            update_client(&updated_client);
            break;
        }
        case 0:
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (option != 0);
}

void handle_car_menu()
{
    int option;
    do
    {
        printf("\n--- Menu de Veículos ---\n");
        printf("1. Inserir Novo Veículo\n");
        printf("2. Remover Veículo (por Placa)\n");
        printf("3. Atualizar Veículo (por Placa)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            option = 0;
        }

        switch (option)
        {
        case 1:
        {
            Car new_car;
            char client_cpf[15];

            printf("  > Inserção de Veículo\n");
            printf("  Placa (Chave Única): ");
            scanf("%9s", new_car.plate);
            printf("  Modelo: ");
            scanf("%49s", new_car.model);
            printf("  Ano: ");
            scanf("%d", &new_car.year);

            printf("  CPF do Cliente Dono: ");
            scanf("%14s", client_cpf);

            new_car.owner = find_client_by_cpf(client_cpf);

            if (new_car.owner == NULL)
            {
                printf("  ERRO: Cliente com CPF %s não encontrado. Cadastro de veículo cancelado.\n", client_cpf);
                break;
            }

            register_car(&new_car);
            break;
        }
        case 2:
        {
            char plate_to_delete[10];
            printf("  > Remoção de Veículo\n");
            printf("  Digite a Placa a ser removida: ");
            scanf("%9s", plate_to_delete);

            delete_car(plate_to_delete);
            break;
        }
        case 3:
        {
            Car updated_car;
            char client_cpf[15];
            printf("  > Atualização de Veículo\n");
            printf("  Digite a Placa do veículo a atualizar: ");
            scanf("%9s", updated_car.plate);

            printf("  Novo Modelo: ");
            scanf("%49s", updated_car.model);
            printf("  Novo Ano: ");
            scanf("%d", &updated_car.year);

            printf("  Novo CPF do Dono: ");
            scanf("%14s", client_cpf);
            updated_car.owner = find_client_by_cpf(client_cpf);

            if (updated_car.owner == NULL)
            {
                printf("  ERRO: Novo Cliente com CPF %s não encontrado. Atualização cancelada.\n", client_cpf);
                break;
            }

            update_car(&updated_car);
            break;
        }
        case 0:
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (option != 0);
}

void handle_os_menu()
{
    int option;
    do
    {
        printf("\n--- Menu de Ordens de Serviço ---\n");
        printf("1. Abrir Nova OS\n");
        printf("2. Atualizar Detalhes da OS\n");
        printf("3. Encerrar OS (Mudar Status para FINALIZADO/ENTREGUE)\n");
        printf("4. Deletar OS (Opcional - se permitido)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            option = 0;
        }

        switch (option)
        {
        case 1:
        {
            ServiceOrder new_order;
            char plate[10];

            printf("  > Abertura de OS\n");
            printf("  ID da Ordem (Deve ser único): ");
            scanf("%d", &new_order.id);
            printf("  Data de Entrada (DD/MM/AAAA): ");
            scanf("%10s", new_order.entry_date);
            printf("  Descrição do Problema (sem espaços): ");
            scanf("%254s", new_order.problem_description);

            printf("  Placa do Veículo: ");
            scanf("%9s", plate);
            new_order.vehicle = find_car_by_plate(plate);

            if (new_order.vehicle == NULL)
            {
                printf("  ERRO: Veículo com placa %s não encontrado. Abertura de OS cancelada.\n", plate);
                break;
            }

            if (os_id_exists(new_order.id))
            {
                printf("  ERRO: ID %d já existe. Escolha outro.\n", new_order.id);
                break;
            }

            new_order.status = AGUARDANDO_AVALIACAO;
            open_service_order(&new_order);
            break;
        }
        case 2:
        {
            {
                int id_to_update;
                char new_desc[255];

                printf("  > Atualização de OS\n");
                printf("  Digite o ID da Ordem a atualizar: ");
                scanf("%d", &id_to_update);

                ServiceOrder *existing_order = find_service_order_by_id(id_to_update);
                if (!existing_order)
                {
                    printf("  ERRO: Ordem de Serviço ID %d não encontrada.\n", id_to_update);
                    break;
                }

                printf("  Nova Descrição do Problema: ");
                scanf("%254s", new_desc);
                strcpy(existing_order->problem_description, new_desc);

                update_service_order(existing_order);
                printf("✅ Ordem de Serviço ID %d atualizada com sucesso!\n", id_to_update);

                break;
            }
        }
        case 3:
        {
            int id_to_close;
            printf("  > Encerrar OS\n");
            printf("  Digite o ID da Ordem a encerrar: ");
            scanf("%d", &id_to_close);

            close_service_order(id_to_close);
            break;
        }
        case 4:
        {
            int id_to_delete;
            printf("  > Deletar OS\n");
            printf("  Digite o ID da Ordem a deletar: ");
            scanf("%d", &id_to_delete);

            delete_service_order(id_to_delete);
            break;
        }
        case 0:
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (option != 0);
}

void handle_report_menu()
{
    int option;
    do
    {
        printf("\n--- Menu de Relatórios ---\n");
        printf("1. Histórico de serviços de um veículo\n");
        printf("2. Ordens de serviço abertas em um dia específico\n");
        printf("3. Listar todos os veículos de um cliente\n");
        printf("4. Relatório de ordens de serviço por status\n");
        printf("5. Relatório de clientes mais recorrentes\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            option = 0;
        }

        switch (option)
        {
        case 1:
            generate_vehicle_history_report();
            break;
        case 2:
            generate_daily_orders_report();
            break;
        case 3:
            generate_client_cars_report();
            break;
        case 4:
            generate_orders_by_status_report();
            break;
        case 5:
            generate_top_clients_report();
            break;
        case 0:
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (option != 0);
}