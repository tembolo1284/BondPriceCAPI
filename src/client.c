#include "../include/client.h"
#include "../include/bond_pricing.h"
#include <stdio.h>
#include <stdlib.h>

void handle_price_bond(struct mg_connection *nc, struct mg_http_message *hm) {
    double face_value, coupon_rate, yield_to_maturity;
    int periods;

    char face_value_str[100], coupon_rate_str[100], periods_str[100], yield_to_maturity_str[100];

    mg_http_get_var(&hm->body, "face_value", face_value_str, sizeof(face_value_str));
    mg_http_get_var(&hm->body, "coupon_rate", coupon_rate_str, sizeof(coupon_rate_str));
    mg_http_get_var(&hm->body, "periods", periods_str, sizeof(periods_str));
    mg_http_get_var(&hm->body, "yield_to_maturity", yield_to_maturity_str, sizeof(yield_to_maturity_str));

    printf("Received data: face_value=%s, coupon_rate=%s, periods=%s, yield_to_maturity=%s\n",
           face_value_str, coupon_rate_str, periods_str, yield_to_maturity_str);

    face_value = atof(face_value_str);
    coupon_rate = atof(coupon_rate_str);
    periods = atoi(periods_str);
    yield_to_maturity = atof(yield_to_maturity_str);

    double price = price_bond(face_value, coupon_rate, periods, yield_to_maturity);

    char response[256];
    snprintf(response, sizeof(response), "{ \"bond_price\": %.8f }\n", price);

    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",
              (int)strlen(response), response);
}

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_HTTP_MSG) {
        printf("Received request: %.*s %.*s\n", (int)hm->method.len, hm->method.buf,
               (int)hm->uri.len, hm->uri.buf);

        if (mg_strcmp(hm->uri, mg_str("/price_bond")) == 0) {
            printf("Handling /price_bond\n");
            handle_price_bond(nc, hm);
        } else {
            printf("Unknown URI: %.*s\n", (int)hm->uri.len, hm->uri.buf);
            mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "Not Found\n");
        }
    }
}

