#ifndef CLIENT_H
#define CLIENT_H

#include "../mongoose.h"

void handle_price_bond(struct mg_connection *nc, struct mg_http_message *hm);
void handle_fetch_sofr_rates(struct mg_connection *nc);
void handle_price_swap(struct mg_connection *nc, struct mg_http_message *hm);
void ev_handler(struct mg_connection *nc, int ev, void *ev_data);

#endif // CLIENT_H

