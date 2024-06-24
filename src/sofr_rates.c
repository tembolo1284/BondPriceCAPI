#include "../include/sofr_rates.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

typedef struct {
    char *data;
    size_t size;
} MemoryStruct;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if(ptr == NULL) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    return realsize;
}

int fetch_sofr_rates(SofrRates *sofr_rates) {
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk = {0};

    char url[512];
    snprintf(url, sizeof(url), "%s?series_id=%s&api_key=%s&file_type=json", SOFR_API_URL, SOFR_SERIES_ID, SOFR_API_KEY);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }

    curl_easy_cleanup(curl);

    // Parse JSON response
    struct json_object *parsed_json;
    struct json_object *observations;
    struct json_object *observation;
    struct json_object *value;

    printf("Raw JSON data: %s\n", chunk.data);  // Debugging print statement

    parsed_json = json_tokener_parse(chunk.data);
    if (!parsed_json) {
        fprintf(stderr, "json_tokener_parse() failed\n");
        curl_global_cleanup();
        free(chunk.data);
        return -1;
    }

    if (!json_object_object_get_ex(parsed_json, "observations", &observations)) {
        fprintf(stderr, "json_object_object_get_ex() failed to find 'observations'\n");
        json_object_put(parsed_json);
        curl_global_cleanup();
        free(chunk.data);
        return -1;
    }

    size_t count = json_object_array_length(observations);
    if (count == 0 || count > 10000) { // Add a sanity check for the count
        fprintf(stderr, "Invalid count of observations: %zu\n", count);
        json_object_put(parsed_json);
        curl_global_cleanup();
        free(chunk.data);
        return -1;
    }

    sofr_rates->rates = malloc(count * sizeof(double));
    if (!sofr_rates->rates) {
        fprintf(stderr, "Failed to allocate memory for SOFR rates\n");
        json_object_put(parsed_json);
        curl_global_cleanup();
        free(chunk.data);
        return -1;
    }

    sofr_rates->count = count;

    for(size_t i = 0; i < count; i++) {
        observation = json_object_array_get_idx(observations, i);
        if (!observation || !json_object_object_get_ex(observation, "value", &value)) {
            fprintf(stderr, "json_object_object_get_ex() failed to find 'value' in observation at index %zu\n", i);
            free(sofr_rates->rates);
            json_object_put(parsed_json);
            curl_global_cleanup();
            free(chunk.data);
            return -1;
        }
        printf("Observation %zu: %s\n", i, json_object_get_string(value));  // Debugging print statement
        sofr_rates->rates[i] = atof(json_object_get_string(value));
    }

    json_object_put(parsed_json);
    curl_global_cleanup();
    free(chunk.data);

    printf("SOFR rates fetched successfully. Total rates: %zu\n", sofr_rates->count);  // Debugging print statement

    return 0;
}


