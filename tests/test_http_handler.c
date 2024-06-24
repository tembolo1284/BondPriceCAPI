#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <string.h>
#include <pthread.h>
#include "../include/client.h"
#include "../include/sofr_rates.h"
#include "../mongoose.h"

// Declare the test event handler
void test_ev_handler(struct mg_connection *nc, int ev, void *ev_data);

static struct mg_connection *nc;
static struct mg_mgr mgr;
static pthread_t server_thread;

void *server_func(void *param) {
    struct mg_mgr *mgr = (struct mg_mgr *) param;
    while (true) {
        mg_mgr_poll(mgr, 1000);
    }
    return NULL;
}

void setup_http_test(void) {
    mg_mgr_init(&mgr);
    nc = mg_http_listen(&mgr, "http://0.0.0.0:8000", test_ev_handler, &mgr);
    if (nc == NULL) {
        fprintf(stderr, "Failed to create listener\n");
        exit(1);
    }
    pthread_create(&server_thread, NULL, server_func, &mgr);
}

void teardown_http_test(void) {
    pthread_cancel(server_thread);
    pthread_join(server_thread, NULL);
    mg_mgr_free(&mgr);
}

TestSuite(http_handlers, .init = setup_http_test, .fini = teardown_http_test);

Test(http_handlers, test_handle_price_bond) {
    struct mg_connection conn;
    struct mg_http_message hm;
    char data[] = "face_value=1000&coupon_rate=0.05&periods=10&yield_to_maturity=0.04";

    memset(&conn, 0, sizeof(conn));
    memset(&hm, 0, sizeof(hm));

    mg_http_parse(data, strlen(data), &hm);

    handle_price_bond(&conn, &hm);

    // Verify that the response is correct
    const char *expected_response = "{\"bond_price\": 1081.10508334}\n";
    cr_assert(strstr((const char *)conn.send.buf, expected_response) != NULL);
}

Test(http_handlers, test_handle_fetch_sofr_rates) {
    struct mg_connection conn;

    memset(&conn, 0, sizeof(conn));

    handle_fetch_sofr_rates(&conn);

    // Verify that the response contains the SOFR rates
    const char *expected_response = "{\"sofr_rates\": [";
    cr_assert(strncmp((const char *)conn.send.buf, expected_response, strlen(expected_response)) == 0);
}

int test_custom_http_match_uri(struct mg_http_message *hm, const char *pattern) {
    size_t pattern_len = strlen(pattern);
    return hm->uri.len == pattern_len && strncmp(hm->uri.buf, pattern, pattern_len) == 0;
}

void test_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_HTTP_MSG) {
        if (test_custom_http_match_uri(hm, "/price_bond")) {
            handle_price_bond(nc, hm);
        } else if (test_custom_http_match_uri(hm, "/fetch_sofr_rates")) {
            handle_fetch_sofr_rates(nc);
        } else {
            mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "Not Found\n");
        }
    }
}

