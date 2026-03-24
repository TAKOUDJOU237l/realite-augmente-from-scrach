// ============================================================
// FILE: main.cpp
// TP1 — inspectFloat IEEE 754 & Arithmétique flottante
// ============================================================
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>

// Lire les bits d'un float — C++17 légal
uint32_t floatBits(float f) {
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(float));
    return bits;
}

// Affiche les 3 champs d'un float
void inspectFloat(float f) {
    uint32_t bits = floatBits(f);
    uint32_t sign     = (bits >> 31) & 0x1;
    uint32_t exponent = (bits >> 23) & 0xFF;
    uint32_t mantissa =  bits        & 0x7FFFFF;

    printf("=================================\n");
    printf("float        : %.10f\n", f);
    printf("Bits hex     : 0x%08X\n", bits);
    printf("Signe        : %u (%s)\n",
           sign, sign ? "negatif" : "positif");
    printf("Exposant     : %u (biais=127 → 2^%d)\n",
           exponent, (int)exponent - 127);
    printf("Mantisse     : 0x%06X\n", mantissa);

    // Identification valeurs spéciales
    if (exponent == 0xFF && mantissa != 0)
        printf("Type         : NaN\n");
    else if (exponent == 0xFF && mantissa == 0)
        printf("Type         : %cInf\n", sign ? '-' : '+');
    else if (exponent == 0x00 && mantissa != 0)
        printf("Type         : Subnormal\n");
    else if (exponent == 0x00 && mantissa == 0)
        printf("Type         : Zero (%c0)\n", sign ? '-' : '+');
    else
        printf("Type         : Normal\n");
    printf("\n");
}

int main() {
    printf("=== TP1 — inspectFloat IEEE 754 ===\n\n");

    // 1. 0.1f — pas exactement représentable en binaire
    printf("Test 1 : 0.1f\n");
    inspectFloat(0.1f);

    // 2. 1.0f — signe=0, exposant=127, mantisse=0
    printf("Test 2 : 1.0f\n");
    inspectFloat(1.0f);

    // 3. +Inf
    printf("Test 3 : 1.0f / 0.0f = +Inf\n");
    inspectFloat(1.0f / 0.0f);

    // 4. NaN
    printf("Test 4 : sqrt(-1.0f) = NaN\n");
    inspectFloat(std::sqrt(-1.0f));

    // 5. -0.0f
    printf("Test 5 : -0.0f\n");
    inspectFloat(-0.0f);
    printf("  NOTE: -0.0f == +0.0f est %s\n\n",
           (-0.0f == 0.0f) ? "VRAI" : "FAUX");

    // 6. Subnormal
    printf("Test 6 : FLT_MIN (plus petit normal)\n");
    inspectFloat(1.175494351e-38f);

    // NaN != NaN — règle fondamentale
    printf("=== Propriete NaN ===\n");
    float nan = std::sqrt(-1.0f);
    printf("NaN == NaN : %s\n",
           (nan == nan) ? "VRAI" : "FAUX");
    printf("NaN != NaN : %s (regle IEEE 754)\n\n",
           (nan != nan) ? "VRAI" : "FAUX");

    return 0;
}