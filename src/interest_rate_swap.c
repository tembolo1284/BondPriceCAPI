#include <stdlib.h>
#include "../include/interest_rate_swap.h"

double calculate_fixed_leg(double notional, double fixed_rate, int periods) {
    return notional * fixed_rate * periods;
}

double calculate_floating_leg(double notional, double *floating_rates, int floating_rate_count) {
    double floating_leg = 0.0;
    for (int i = 0; i < floating_rate_count; i++) {
        floating_leg += notional * floating_rates[i];
    }
    return floating_leg;
}

double price_swap(InterestRateSwap *swap) {
    double fixed_leg_value = calculate_fixed_leg(swap->notional, swap->fixed_rate, swap->periods);
    double floating_leg_value = calculate_floating_leg(swap->notional, swap->floating_rates, swap->floating_rate_count);
    return fixed_leg_value - floating_leg_value;
}

