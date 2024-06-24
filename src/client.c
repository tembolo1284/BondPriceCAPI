#include "../mongoose.h"
#include "../include/client.h"
#include "../include/sofr_rates.h"
#include "../include/bond_pricing.h"  // Include the header for price_bond function

// Custom URI matching function
int custom_http_match_uri(struct mg_http_message *hm, const char *pattern) {
    size_t pattern_len = strlen(pattern);
    return hm->uri.len == pattern_len && strncmp(hm->uri.buf, pattern, pattern_len) == 0;
}

#ifndef EV_HANDLER_DEFINED
#define EV_HANDLER_DEFINED
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_HTTP_MSG) {
        if (custom_http_match_uri(hm, "/price_bond")) {
            handle_price_bond(nc, hm);
        } else if (custom_http_match_uri(hm, "/fetch_sofr_rates")) {
            handle_fetch_sofr_rates(nc);
        } else {
            mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "Not Found\n");
        }
    }
}
#endif

void handle_price_bond(struct mg_connection *nc, struct mg_http_message *hm) {
    char face_value_str[100], coupon_rate_str[100], periods_str[100], yield_to_maturity_str[100];
    double face_value, coupon_rate, yield_to_maturity;
    int periods;
    double bond_price;
    char response[256];

    mg_http_get_var(&hm->body, "face_value", face_value_str, sizeof(face_value_str));
    mg_http_get_var(&hm->body, "coupon_rate", coupon_rate_str, sizeof(coupon_rate_str));
    mg_http_get_var(&hm->body, "periods", periods_str, sizeof(periods_str));
    mg_http_get_var(&hm->body, "yield_to_maturity", yield_to_maturity_str, sizeof(yield_to_maturity_str));

    face_value = strtod(face_value_str, NULL);
    coupon_rate = strtod(coupon_rate_str, NULL);
    periods = strtol(periods_str, NULL, 10);
    yield_to_maturity = strtod(yield_to_maturity_str, NULL);

    bond_price = price_bond(face_value, coupon_rate, periods, yield_to_maturity);

    snprintf(response, sizeof(response), "{\"bond_price\": %.8f}\n", bond_price);
    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
}

void handle_fetch_sofr_rates(struct mg_connection *nc) {
    SofrRates sofr_rates;
    char response[32768];
    int i;
    size_t len = 0;

    if (fetch_sofr_rates(&sofr_rates) == 0) {
        len += snprintf(response + len, sizeof(response) - len, "{\"sofr_rates\": [");
        for (i = 0; i < (int)sofr_rates.count; i++) {
            if (i > 0) len += snprintf(response + len, sizeof(response) - len, ",");
            len += snprintf(response + len, sizeof(response) - len, "%.8f", sofr_rates.rates[i]);
        }
        len += snprintf(response + len, sizeof(response) - len, "]}\n");
        mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
        free(sofr_rates.rates);
    } else {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Failed to fetch SOFR rates\n");
    }
}

