#include "utility.h"


/**
 * @brief Calculates the power of a number.
 * 
 * This function computes the power of a base number raised 
 * to an exponent, returning the result as an unsigned 32-bit integer.
 *
 * @param m The base number.
 * @param n The exponent to raise the base to.
 * @return The result of m raised to the power of n.
 */
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}
