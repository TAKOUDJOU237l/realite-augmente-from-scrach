// ============================================================
// FILE: main.cpp
// TP3 — 30 tests unitaires pour Float.h
// ============================================================
#include <cstdio>
#include <cmath>
#include "Float.h"

using namespace NkMath;

// Compteur de tests
static int passed = 0;
static int failed = 0;

// Macro de test simple
#define TEST(nom, condition) \
    do { \
        if (condition) { \
            printf("  PASS : %s\n", nom); \
            passed++; \
        } else { \
            printf("   FAIL : %s\n", nom); \
            failed++; \
        } \
    } while(0)

int main() {
    printf("=== TP3 — 30 Tests Float.h ===\n\n");

    // -------------------------------------------------------
    // Groupe 1 : isFiniteValid (5 tests)
    // -------------------------------------------------------
    printf("--- isFiniteValid ---\n");
    TEST("NaN est invalide",
         !isFiniteValid(std::sqrt(-1.0f)));
    TEST("+Inf est invalide",
         !isFiniteValid(1.0f / 0.0f));
    TEST("-Inf est invalide",
         !isFiniteValid(-1.0f / 0.0f));
    TEST("0.0f est valide",
         isFiniteValid(0.0f));
    TEST("1.0f est valide",
         isFiniteValid(1.0f));

    // -------------------------------------------------------
    // Groupe 2 : nearlyZero (6 tests)
    // -------------------------------------------------------
    printf("\n--- nearlyZero ---\n");
    TEST("0.0f est nearlyZero",
         nearlyZero(0.0f));
    TEST("1e-7f est nearlyZero (dans kFEps)",
         nearlyZero(1e-7f));
    TEST("0.1f n'est pas nearlyZero",
         !nearlyZero(0.1f));
    TEST("0.0 double est nearlyZero",
         nearlyZero(0.0));
    TEST("1e-10 double est nearlyZero",
         nearlyZero(1e-10));
    TEST("0.001 double n'est pas nearlyZero",
         !nearlyZero(0.001));

    // -------------------------------------------------------
    // Groupe 3 : approxEq (8 tests)
    // -------------------------------------------------------
    printf("\n--- approxEq ---\n");
    TEST("1.0f == 1.0f",
         approxEq(1.0f, 1.0f));
    TEST("1.0f == 1.0f + 1e-7f (dans epsilon)",
         approxEq(1.0f, 1.0f + 1e-7f));
    TEST("1.0f != 1.1f",
         !approxEq(1.0f, 1.1f));
    TEST("0.0f == 0.0f",
         approxEq(0.0f, 0.0f));
    TEST("-1.0f == -1.0f",
         approxEq(-1.0f, -1.0f));
    TEST("1.0f != -1.0f",
         !approxEq(1.0f, -1.0f));
    TEST("1.0 double == 1.0 + 1e-10",
         approxEq(1.0, 1.0 + 1e-10));
    TEST("1.0 double != 1.001",
         !approxEq(1.0, 1.001));

    // -------------------------------------------------------
    // Groupe 4 : kahanSum (6 tests)
    // -------------------------------------------------------
    printf("\n--- kahanSum ---\n");

    // 1000 x 0.1f = 100.0
    float data1[1000];
    for (int i = 0; i < 1000; i++) data1[i] = 0.1f;
    float res1 = kahanSum(data1, 1000);
    TEST("1000 x 0.1f = 100.0f",
         approxEq(res1, 100.0f, 0.001f));

    // Somme de 1 element
    float single[] = {42.0f};
    TEST("kahanSum([42]) = 42",
         approxEq(kahanSum(single, 1), 42.0f));

    // Somme de zeros
    float zeros[10] = {};
    TEST("kahanSum([0...0]) = 0",
         nearlyZero(kahanSum(zeros, 10)));

    // Double precision
    double data2[100];
    for (int i = 0; i < 100; i++) data2[i] = 0.1;
    double res2 = kahanSum(data2, 100);
    TEST("100 x 0.1 double = 10.0",
         approxEq(res2, 10.0, 1e-10));

    // Valeurs negatives
    float negdata[] = {-1.0f, -2.0f, -3.0f};
    TEST("kahanSum([-1,-2,-3]) = -6",
         approxEq(kahanSum(negdata, 3), -6.0f));

    // Mix positif/negatif
    float mixdata[] = {1.0f, -1.0f, 1.0f, -1.0f};
    TEST("kahanSum([1,-1,1,-1]) = 0",
         nearlyZero(kahanSum(mixdata, 4)));

    // -------------------------------------------------------
    // Groupe 5 : Clamp (5 tests)
    // -------------------------------------------------------
    printf("\n--- Clamp ---\n");
    TEST("Clamp(-5, 0, 1) = 0",
         approxEq(Clamp(-5.0f, 0.0f, 1.0f), 0.0f));
    TEST("Clamp(5, 0, 1) = 1",
         approxEq(Clamp(5.0f, 0.0f, 1.0f), 1.0f));
    TEST("Clamp(0.5, 0, 1) = 0.5",
         approxEq(Clamp(0.5f, 0.0f, 1.0f), 0.5f));
    TEST("Clamp(-1, -1, 1) = -1",
         approxEq(Clamp(-1.0f, -1.0f, 1.0f), -1.0f));
    TEST("Clamp double 2.5 dans [0,2] = 2",
         approxEq(Clamp(2.5, 0.0, 2.0), 2.0));

    // -------------------------------------------------------
    // Résumé
    // -------------------------------------------------------
    printf("\n=== Résumé ===\n");
    printf("Tests passés : %d / %d\n", passed, passed + failed);
    printf("Tests échoués: %d / %d\n", failed, passed + failed);

    return failed == 0 ? 0 : 1;
}