#ifndef REPORT_MODULE_H
#define REPORT_MODULE_H

#include "client.h"
#include "car.h"
#include "service_order.h"
#include <stdio.h>

void generate_vehicle_history_report();

void generate_daily_orders_report();

void generate_client_cars_report();

void generate_orders_by_status_report();

void generate_top_clients_report();

#endif // REPORT_MODULE_H