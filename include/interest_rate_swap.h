#ifndef INTEREST_RATE_SWAP_H
#define INTEREST_RATE_SWAP_H

#include <stddef.h>

double calculate_fixed_leg_pv(double notional, double fixed_rate, int periods, double discount_rate);
double calculate_floating_leg_pv(double notional, const double *sofr_rates, int periods, double discount_rate);
double price_interest_rate_swap(double notional, double fixed_rate, const double *sofr_rates, int periods, double discount_rate);

#endif // INTEREST_RATE_SWAP_H

