#ifndef SOFR_RATES_H
#define SOFR_RATES_H

#include <stddef.h>

#define SOFR_API_URL "https://api.stlouisfed.org/fred/series/observations"
#define SOFR_SERIES_ID "SOFR"
#define SOFR_API_KEY "1f7bd511cc23f335dd1c8856df53b183"
typedef struct {
    double *rates;
    size_t count;
} SofrRates;

int fetch_sofr_rates(SofrRates *sofr_rates);

#endif // SOFR_RATES_H

