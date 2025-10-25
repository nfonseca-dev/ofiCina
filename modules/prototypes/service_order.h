#ifndef SERVICE_ORDER_MODULE_H
#define SERVICE_ORDER_MODULE_H

#include "car.h" // <--- CRÍTICO: Incluir car.h (que define struct car/Car)

#define SERVICE_ORDERS_FILE_PATH "service_orders.txt"

typedef enum
{
    AGUARDANDO_AVALIACAO,
    EM_REPARO,
    FINALIZADO,
    ENTREGUE
} StatusOS;

// Adicionado o typedef ServiceOrder
typedef struct service_order
{
    int id;
    Car *vehicle; // Usa o typedef Car
    char entry_date[11];
    char problem_description[255];
    StatusOS status;
} ServiceOrder; // <--- Adicionado


// Protótipos existentes (ajustados para usar ServiceOrder)
bool os_id_exists(int id);
void create_service_order(const ServiceOrder *new_order);
void delete_service_order(int id);
void update_service_order(const ServiceOrder *updated_order);

// Renomeado e ajustado o parâmetro para consistência com o main.c
void open_service_order(const ServiceOrder *new_order);
void close_service_order(int id);

// Adicionado: Protótipos das funções de persistência usadas em main.c
void load_service_orders();
void save_service_orders();
ServiceOrder* find_service_order_by_id(int id);


#endif