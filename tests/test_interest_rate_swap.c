#include <criterion/criterion.h>
#include "../include/interest_rate_swap.h"

Test(interest_rate_swap, calculate_fixed_leg_pv) {
    double notional = 1000.0;
    double fixed_rate = 0.05;
    int periods = 10;
    double discount_rate = 0.04;

    double expected_pv = 81.12914;
    double actual_pv = calculate_fixed_leg_pv(notional, fixed_rate, periods, discount_rate);

    cr_assert_float_eq(actual_pv, expected_pv, 1e-5, "Expected PV: %.5f, but got: %.5f", expected_pv, actual_pv);
}

Test(interest_rate_swap, calculate_floating_leg_pv) {
    double notional = 1000.0;
    double sofr_rates[] = {0.03, 0.035, 0.034, 0.032, 0.031, 0.033, 0.035, 0.036, 0.037, 0.038};
    int periods = 10;
    double discount_rate = 0.04;

    double expected_pv = 89.24195; // Example value, needs to be calculated based on the rates
    double actual_pv = calculate_floating_leg_pv(notional, sofr_rates, periods, discount_rate);

    cr_assert_float_eq(actual_pv, expected_pv, 1e-5, "Expected PV: %.5f, but got: %.5f", expected_pv, actual_pv);
}

Test(interest_rate_swap, price_interest_rate_swap) {
    double notional = 1000.0;
    double fixed_rate = 0.05;
    double sofr_rates[] = {0.03, 0.035, 0.034, 0.032, 0.031, 0.033, 0.035, 0.036, 0.037, 0.038};
    int periods = 10;
    double discount_rate = 0.04;

    double expected_price = 918.87086; // Example value, needs to be calculated based on the rates
    double actual_price = price_interest_rate_swap(notional, fixed_rate, sofr_rates, periods, discount_rate);

    cr_assert_float_eq(actual_price, expected_price, 1e-5, "Expected price: %.5f, but got: %.5f", expected_price, actual_price);
}

