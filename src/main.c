#include "../mongoose.h"
#include "../include/client.h"
#include <stdio.h>

static const char *s_http_port = "http://0.0.0.0:8000";

int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr);
    nc = mg_http_listen(&mgr, s_http_port, ev_handler, &mgr);

    if (nc == NULL) {
        printf("Error starting server on port %s\n", s_http_port);
        return 1;
    }

    printf("Starting RESTful API on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}

