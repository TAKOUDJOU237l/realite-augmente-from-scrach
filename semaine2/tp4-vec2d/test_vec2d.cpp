// ============================================================
// FILE: test_vec2d.cpp
// TP4 — 20 tests unitaires Vec2d avec assert
// ============================================================
#include <cstdio>
#include <cmath>
#include <cassert>
#include "Vec2d.h"

using namespace NkMath;

int main() {
    printf("=== TP4 — 20 Tests Vec2d ===\n\n");

    // -------------------------------------------------------
    // Test 15 : Dot product
    // -------------------------------------------------------
    printf("--- Test 15 : Dot product ---\n");

    // (1,0)·(0,1) = 0
    assert(approxEq(Dot(Vec2d(1,0), Vec2d(0,1)), 0.0)
        && "(1,0).(0,1) doit etre 0");
    printf("   (1,0)·(0,1) = 0\n");

    // (1,0)·(1,0) = 1
    assert(approxEq(Dot(Vec2d(1,0), Vec2d(1,0)), 1.0)
        && "(1,0).(1,0) doit etre 1");
    printf("   (1,0)·(1,0) = 1\n");

    // (3,4)·(3,4) = 25
    assert(approxEq(Dot(Vec2d(3,4), Vec2d(3,4)), 25.0)
        && "(3,4).(3,4) doit etre 25");
    printf("   (3,4)·(3,4) = 25\n");

    // -------------------------------------------------------
    // Test 16 : Cross2D
    // -------------------------------------------------------
    printf("\n--- Test 16 : Cross2D ---\n");

    // (1,0)×(0,1) = 1
    assert(approxEq(Cross2D(Vec2d(1,0), Vec2d(0,1)), 1.0)
        && "(1,0)x(0,1) doit etre 1");
    printf("   (1,0)×(0,1) = 1\n");

    // (0,1)×(1,0) = -1
    assert(approxEq(Cross2D(Vec2d(0,1), Vec2d(1,0)), -1.0)
        && "(0,1)x(1,0) doit etre -1");
    printf("   (0,1)×(1,0) = -1\n");

    // -------------------------------------------------------
    // Test 17 : Normalisation
    // -------------------------------------------------------
    printf("\n--- Test 17 : Normalisation ---\n");

    // ||(3,4).Normalized()|| = 1.0
    Vec2d v34 = Vec2d(3.0, 4.0).Normalized();
    assert(approxEq(v34.Norm(), 1.0, kEps)
        && "Norme apres normalisation doit etre 1.0");
    printf("   ||(3,4).Normalized()|| = 1.0\n");

    // Vecteur nul → retourne (0,0)
    Vec2d vzero = Vec2d(0.0, 0.0).Normalized();
    assert(approxEq(vzero.x, 0.0) && approxEq(vzero.y, 0.0)
        && "Vecteur nul normalise doit etre (0,0)");
    printf("   (0,0).Normalized() = (0,0)\n");

    // IsNormalized apres Normalized()
    assert(v34.IsNormalized()
        && "IsNormalized doit etre true apres Normalized()");
    printf("   IsNormalized() = true apres Normalized()\n");

    // -------------------------------------------------------
    // Test 18 : Opérateur []
    // -------------------------------------------------------
    printf("\n--- Test 18 : Operateur [] ---\n");

    Vec2d v(5.0, 7.0);

    // Accès lecture
    assert(approxEq(v[0], 5.0) && "v[0] doit etre 5.0");
    printf("   v[0] = 5.0\n");

    assert(approxEq(v[1], 7.0) && "v[1] doit etre 7.0");
    printf("   v[1] = 7.0\n");

    // Modification
    v[0] = 99.0;
    assert(approxEq(v[0], 99.0) && "v[0] doit etre 99.0");
    assert(approxEq(v.x,  99.0) && "v.x doit aussi etre 99.0");
    printf("  v[0] = 99.0 apres modification\n");
    printf("  v.x  = 99.0 synchronise\n");

    v[1] = 42.0;
    assert(approxEq(v[1], 42.0) && "v[1] doit etre 42.0");
    assert(approxEq(v.y,  42.0) && "v.y doit aussi etre 42.0");
    printf("   v[1] = 42.0 apres modification\n");
    printf("   v.y  = 42.0 synchronise\n");

    // -------------------------------------------------------
    // Test 19 : static_assert layout mémoire
    // -------------------------------------------------------
    printf("\n--- Test 19 : Layout memoire ---\n");

    // Ces static_assert sont dans Vec2d.h
    // On les vérifie ici aussi
    static_assert(sizeof(Vec2d) == 16,
        "Vec2d doit faire 16 bytes");
    static_assert(offsetof(Vec2d, x) == 0,
        "x doit etre en premier");
    static_assert(offsetof(Vec2d, y) == 8,
        "y doit etre a l offset 8");
    printf("   sizeof(Vec2d) == 16\n");
    printf("   offsetof(x) == 0\n");
    printf("   offsetof(y) == 8\n");

    // -------------------------------------------------------
    // Tests supplémentaires — opérateurs (5 tests)
    // -------------------------------------------------------
    printf("\n--- Tests supplementaires : Operateurs ---\n");

    Vec2d a(1.0, 2.0);
    Vec2d b(3.0, 4.0);

    // Addition
    Vec2d sum = a + b;
    assert(approxEq(sum.x, 4.0) && approxEq(sum.y, 6.0));
    printf("   (1,2)+(3,4) = (4,6)\n");

    // Soustraction
    Vec2d diff = b - a;
    assert(approxEq(diff.x, 2.0) && approxEq(diff.y, 2.0));
    printf("   (3,4)-(1,2) = (2,2)\n");

    // Multiplication scalaire
    Vec2d mul = a * 2.0;
    assert(approxEq(mul.x, 2.0) && approxEq(mul.y, 4.0));
    printf("  (1,2)*2 = (2,4)\n");

    // Commutativité scalaire
    Vec2d mul2 = 2.0 * a;
    assert(approxEq(mul2.x, 2.0) && approxEq(mul2.y, 4.0));
    printf("   2*(1,2) = (2,4) commutativite\n");

    // Negation
    Vec2d neg = -a;
    assert(approxEq(neg.x, -1.0) && approxEq(neg.y, -2.0));
    printf("   -(1,2) = (-1,-2)\n");

    // -------------------------------------------------------
    // Résumé
    // -------------------------------------------------------
    printf("\n=== Tous les 20 tests passes ! ===\n");
    return 0;
}