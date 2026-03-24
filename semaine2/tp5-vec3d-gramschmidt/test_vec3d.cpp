// ============================================================
// FILE: test_vec3d.cpp
// TP5 — Tests 20 à 23 Vec3d + Gram-Schmidt
// ============================================================
#include <cstdio>
#include <cmath>
#include <cassert>
#include "Vec3d.h"

using namespace NkMath;

// Générateur pseudo-aléatoire simple — pas de rand() pour reproductibilité
double pseudoRand(int& seed) {
    seed = seed * 1664525 + 1013904223;
    return (double)(seed & 0x7FFFFFFF) / (double)0x7FFFFFFF * 2.0 - 1.0;
}

int main() {
    printf("=== TP5 — Tests Vec3d + Gram-Schmidt ===\n\n");

    // -------------------------------------------------------
    // Test 20 : Cross({1,0,0},{0,1,0}) = {0,0,1}
    // règle main droite
    // -------------------------------------------------------
    printf("--- Test 20 : Cross product - regle main droite ---\n");

    Vec3d x_axis(1.0, 0.0, 0.0);
    Vec3d y_axis(0.0, 1.0, 0.0);
    Vec3d z_axis(0.0, 0.0, 1.0);

    Vec3d xCrossY = Cross(x_axis, y_axis);
    assert(approxEq(xCrossY.x, 0.0) &&
           approxEq(xCrossY.y, 0.0) &&
           approxEq(xCrossY.z, 1.0) &&
           "Cross(X,Y) doit etre Z");
    printf("   Cross({1,0,0},{0,1,0}) = {0,0,1}\n");

    Vec3d yCrossZ = Cross(y_axis, z_axis);
    assert(approxEq(yCrossZ.x, 1.0) &&
           approxEq(yCrossZ.y, 0.0) &&
           approxEq(yCrossZ.z, 0.0) &&
           "Cross(Y,Z) doit etre X");
    printf("   Cross({0,1,0},{0,0,1}) = {1,0,0}\n");

    Vec3d zCrossX = Cross(z_axis, x_axis);
    assert(approxEq(zCrossX.x, 0.0) &&
           approxEq(zCrossX.y, 1.0) &&
           approxEq(zCrossX.z, 0.0) &&
           "Cross(Z,X) doit etre Y");
    printf("   Cross({0,0,1},{1,0,0}) = {0,1,0}\n");

    // -------------------------------------------------------
    // Test 21 : Cross({0,1,0},{1,0,0}) = {0,0,-1}
    // non-commutatif
    // -------------------------------------------------------
    printf("\n--- Test 21 : Cross product - non-commutatif ---\n");

    Vec3d yCrossX = Cross(y_axis, x_axis);
    assert(approxEq(yCrossX.x,  0.0) &&
           approxEq(yCrossX.y,  0.0) &&
           approxEq(yCrossX.z, -1.0) &&
           "Cross(Y,X) doit etre -Z");
    printf("   Cross({0,1,0},{1,0,0}) = {0,0,-1}\n");

    // Cross parallèles = zéro
    Vec3d parallel = Cross(x_axis, x_axis);
    assert(approxEq(parallel.x, 0.0) &&
           approxEq(parallel.y, 0.0) &&
           approxEq(parallel.z, 0.0) &&
           "Cross de vecteurs paralleles doit etre zero");
    printf("   Cross({1,0,0},{1,0,0}) = {0,0,0}\n");

    // Anti-commutativité : Cross(a,b) = -Cross(b,a)
    Vec3d a(1.0, 2.0, 3.0);
    Vec3d b(4.0, 5.0, 6.0);
    Vec3d ab = Cross(a, b);
    Vec3d ba = Cross(b, a);
    assert(approxEq(ab.x, -ba.x) &&
           approxEq(ab.y, -ba.y) &&
           approxEq(ab.z, -ba.z) &&
           "Cross(a,b) = -Cross(b,a)");
    printf("  Cross(a,b) = -Cross(b,a) anti-commutatif\n");

    // -------------------------------------------------------
    // Test 22 : Gram-Schmidt sur 10 triplets aléatoires
    // Vérifier : |u|=1, u·v=0, u·w=0, v·w=0
    // -------------------------------------------------------
    printf("\n--- Test 22 : Gram-Schmidt 10 triplets ---\n");

    int seed = 42;
    int gs_passed = 0;

    for (int i = 0; i < 10; i++) {
        // Génère 3 vecteurs aléatoires non coplanaires
        Vec3d va(pseudoRand(seed)+2.0,
                 pseudoRand(seed),
                 pseudoRand(seed));
        Vec3d vb(pseudoRand(seed),
                 pseudoRand(seed)+2.0,
                 pseudoRand(seed));
        Vec3d vc(pseudoRand(seed),
                 pseudoRand(seed),
                 pseudoRand(seed)+2.0);

        OrthoBasis basis = GramSchmidt(va, vb, vc);

        // |u| = 1
        assert(approxEq(basis.u.Norm(), 1.0, 1e-9) &&
               "|u| doit etre 1");
        // |v| = 1
        assert(approxEq(basis.v.Norm(), 1.0, 1e-9) &&
               "|v| doit etre 1");
        // |w| = 1
        assert(approxEq(basis.w.Norm(), 1.0, 1e-9) &&
               "|w| doit etre 1");
        // u·v = 0
        assert(approxEq(Dot(basis.u, basis.v), 0.0, 1e-9) &&
               "u.v doit etre 0");
        // u·w = 0
        assert(approxEq(Dot(basis.u, basis.w), 0.0, 1e-9) &&
               "u.w doit etre 0");
        // v·w = 0
        assert(approxEq(Dot(basis.v, basis.w), 0.0, 1e-9) &&
               "v.w doit etre 0");

        gs_passed++;
        printf("   Triplet %d : orthonormal verifie\n", i+1);
    }

    printf("  → %d/10 triplets Gram-Schmidt valides\n", gs_passed);

    // -------------------------------------------------------
    // Test 23 : Project et Reject
    // Vérifier que Project(a,b) + Reject(a,b) == a
    // -------------------------------------------------------
    printf("\n--- Test 23 : Project + Reject = a ---\n");

    // Test sur plusieurs vecteurs
    Vec3d paires[5][2] = {
        {Vec3d(3.0, 4.0, 0.0), Vec3d(1.0, 0.0, 0.0)},
        {Vec3d(1.0, 2.0, 3.0), Vec3d(0.0, 1.0, 0.0)},
        {Vec3d(5.0, 5.0, 5.0), Vec3d(1.0, 1.0, 0.0)},
        {Vec3d(2.0, 0.0, 4.0), Vec3d(0.0, 0.0, 1.0)},
        {Vec3d(1.0, 3.0, 2.0), Vec3d(1.0, 1.0, 1.0)}
    };

    for (int i = 0; i < 5; i++) {
        Vec3d va  = paires[i][0];
        Vec3d vb  = paires[i][1];
        Vec3d proj = Project(va, vb);
        Vec3d rej  = Reject(va, vb);
        Vec3d sum  = proj + rej;

        // Project(a,b) + Reject(a,b) == a
        assert(approxEq(sum.x, va.x, 1e-9) &&
               approxEq(sum.y, va.y, 1e-9) &&
               approxEq(sum.z, va.z, 1e-9) &&
               "Project + Reject doit etre egal a a");

        // proj perpendiculaire à rej
        assert(approxEq(Dot(proj, rej), 0.0, 1e-9) &&
               "proj doit etre perpendiculaire a rej");

        printf("   Paire %d : Project + Reject = a\n", i+1);
    }

    // -------------------------------------------------------
    // Tests supplémentaires layout mémoire
    // -------------------------------------------------------
    printf("\n--- Layout memoire ---\n");
    static_assert(sizeof(Vec3d) == 24, "Vec3d doit faire 24 bytes");
    static_assert(offsetof(Vec3d, x) == 0,  "x en premier");
    static_assert(offsetof(Vec3d, y) == 8,  "y a offset 8");
    static_assert(offsetof(Vec3d, z) == 16, "z a offset 16");
    printf("   sizeof(Vec3d) == 24\n");
    printf("   offsetof(x,y,z) corrects\n");

    printf("\n=== Tous les tests 20-23 passes ! ===\n");
    return 0;
}