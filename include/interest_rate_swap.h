#ifndef INTEREST_RATE_SWAP_H
#define INTEREST_RATE_SWAP_H

typedef struct {
    double notional;
    double fixed_rate;
    int periods;
    double *floating_rates;
    int floating_rate_count;
} InterestRateSwap;

double calculate_fixed_leg(double notional, double fixed_rate, int periods);
double calculate_floating_leg(double notional, double *floating_rates, int floating_rate_count);
double price_swap(InterestRateSwap *swap);

#endif // INTEREST_RATE_SWAP_H

