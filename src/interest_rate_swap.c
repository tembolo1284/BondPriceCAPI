#include "../include/interest_rate_swap.h"
#include <math.h>

double calculate_fixed_leg_pv(double notional, double fixed_rate, int periods, double discount_rate) {
    double pv = 0.0;
    for (int i = 1; i <= periods; ++i) {
        pv += (notional * fixed_rate) / pow(1 + discount_rate, i);
    }
    return pv;
}

double calculate_floating_leg_pv(double notional, const double *sofr_rates, int periods, double discount_rate) {
    double pv = 0.0;
    for (int i = 1; i <= periods; ++i) {
        pv += (notional * sofr_rates[i - 1]) / pow(1 + discount_rate, i);
    }
    return pv;
}

double price_interest_rate_swap(double notional, double fixed_rate, const double *sofr_rates, int periods, double discount_rate) {
    double fixed_leg_pv = calculate_fixed_leg_pv(notional, fixed_rate, periods, discount_rate);
    double floating_leg_pv = calculate_floating_leg_pv(notional, sofr_rates, periods, discount_rate);
    return floating_leg_pv - fixed_leg_pv;
}

