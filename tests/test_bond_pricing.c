#include <criterion/criterion.h>
#include "../include/bond_pricing.h"

Test(bond_pricing, calculate_price) {
    double face_value = 1000.0;
    double coupon_rate = 0.05;
    int periods = 10;
    double yield_to_maturity = 0.04;

    double expected_price = 1081.109475;
    double actual_price = price_bond(face_value, coupon_rate, periods, yield_to_maturity);

    cr_assert_float_eq(actual_price, expected_price, 1e-6, "Expected price: %.6f, but got: %.6f", expected_price, actual_price);
}

