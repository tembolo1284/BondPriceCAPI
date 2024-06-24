#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../mongoose.h"
#include "../include/client.h"
#include "../include/sofr_rates.h"
#include "../include/bond_pricing.h"
#include "../include/interest_rate_swap.h"

// Custom URI matching function
int custom_http_match_uri(struct mg_http_message *hm, const char *pattern) {
    size_t pattern_len = strlen(pattern);
    return hm->uri.len == pattern_len && strncmp(hm->uri.buf, pattern, pattern_len) == 0;
}

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_HTTP_MSG) {
        if (custom_http_match_uri(hm, "/price_bond")) {
            printf("Received request: /price_bond\n");
            handle_price_bond(nc, hm);
        } else if (custom_http_match_uri(hm, "/fetch_sofr_rates")) {
            printf("Received request: /fetch_sofr_rates\n");
            handle_fetch_sofr_rates(nc);
        } else if (custom_http_match_uri(hm, "/price_swap")) {
            printf("Received request: /price_swap\n");
            handle_price_swap(nc, hm);
        } else {
            printf("Received unknown request\n");
            mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "Not Found\n");
        }
    }
}

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

    printf("Bond price calculated successfully: %.8f\n", bond_price);
}

void handle_fetch_sofr_rates(struct mg_connection *nc) {
    SofrRates sofr_rates;
    char response[32768];  // Increased buffer size
    int i;
    size_t len = 0;

    if (fetch_sofr_rates(&sofr_rates) == 0) {
        len += snprintf(response + len, sizeof(response) - len, "{\"sofr_rates\": [");
        for (i = 0; i < (int)sofr_rates.count; i++) {
            if (len >= sizeof(response)) {
                mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Response too large\n");
                free(sofr_rates.rates);
                printf("Error: Response too large\n");
                return;
            }
            if (i > 0) len += snprintf(response + len, sizeof(response) - len, ",");
            len += snprintf(response + len, sizeof(response) - len, "%.8f", sofr_rates.rates[i]);
        }
        len += snprintf(response + len, sizeof(response) - len, "]}\n");
        mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
        free(sofr_rates.rates);

        printf("SOFR rates fetched successfully. Total rates: %zu\n", sofr_rates.count);
    } else {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Failed to fetch SOFR rates\n");
        printf("Error: Failed to fetch SOFR rates\n");
    }
}

void handle_price_swap(struct mg_connection *nc, struct mg_http_message *hm) {
    char notional_str[100], fixed_rate_str[100], periods_str[100], floating_rates_str[32768];
    double notional, fixed_rate;
    int periods;
    char response[256];

    mg_http_get_var(&hm->body, "notional", notional_str, sizeof(notional_str));
    mg_http_get_var(&hm->body, "fixed_rate", fixed_rate_str, sizeof(fixed_rate_str));
    mg_http_get_var(&hm->body, "periods", periods_str, sizeof(periods_str));
    mg_http_get_var(&hm->body, "floating_rates", floating_rates_str, sizeof(floating_rates_str));

    notional = strtod(notional_str, NULL);
    fixed_rate = strtod(fixed_rate_str, NULL);
    periods = strtol(periods_str, NULL, 10);

    char *token = strtok(floating_rates_str, ",");
    double floating_rates[32768];
    int floating_rate_count = 0;
    while (token != NULL && floating_rate_count < 32768) {
        floating_rates[floating_rate_count++] = strtod(token, NULL);
        token = strtok(NULL, ",");
    }

    InterestRateSwap swap = {
        .notional = notional,
        .fixed_rate = fixed_rate,
        .periods = periods,
        .floating_rates = floating_rates,
        .floating_rate_count = floating_rate_count
    };

    double swap_price = price_swap(&swap);

    snprintf(response, sizeof(response), "{\"swap_price\": %.8f}\n", swap_price);
    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);

    printf("Swap price calculated successfully: %.8f\n", swap_price);
}

