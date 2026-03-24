// ============================================================
// FILE: main.cpp
// TP6 — Tests 24 à 27 : Vec4d + Projection perspective
// ============================================================
#include <cstdio>
#include <cmath>
#include <cassert>
#include "Vec4d.h"
#include "NkImage.h"

using namespace NkMath;

// ---------------------------------------------------------------
// Projection perspective simple
// u = fx * (X/Z) + cx
// v = fy * (Y/Z) + cy
// ---------------------------------------------------------------
struct ProjectedPoint {
    double u, v;    // coordonnées image
    bool   valid;   // false si derrière la caméra
};

ProjectedPoint project(const Vec3d& p,
                        double fx, double fy,
                        double cx, double cy) {
    // Point derrière la caméra
    if (p.z <= 0.0) return {0, 0, false};
    return {
        fx * (p.x / p.z) + cx,
        fy * (p.y / p.z) + cy,
        true
    };
}

int main() {
    printf("=== TP6 — Vec4d + Projection perspective ===\n\n");

    // -------------------------------------------------------
    // Test 24 : 8 coins du cube [-0.5, 0.5]³
    // Distance caméra-cube : z_cam = 2.0
    // -------------------------------------------------------
    printf("--- Test 24 : Coins du cube + distance camera ---\n");

    // Les 8 coins du cube unitaire
    // w=1 → points dans l'espace
    Vec4d coins[8] = {
        {-0.5, -0.5, -0.5, 1.0},
        { 0.5, -0.5, -0.5, 1.0},
        { 0.5,  0.5, -0.5, 1.0},
        {-0.5,  0.5, -0.5, 1.0},
        {-0.5, -0.5,  0.5, 1.0},
        { 0.5, -0.5,  0.5, 1.0},
        { 0.5,  0.5,  0.5, 1.0},
        {-0.5,  0.5,  0.5, 1.0}
    };

    // Caméra à z=0, cube à z=2 → décaler z de +2
    double z_cam = 2.0;
    Vec3d coins_cam[8];
    for (int i = 0; i < 8; i++) {
        // Déhomogénéisation + translation caméra
        Vec3d p = coins[i].ToVec3();
        coins_cam[i] = Vec3d(p.x, p.y, p.z + z_cam);
        printf("  Coin %d : (%.2f, %.2f, %.2f)\n",
               i, coins_cam[i].x,
               coins_cam[i].y,
               coins_cam[i].z);
    }

    // Vérification : tous les z > 0 (devant la caméra)
    for (int i = 0; i < 8; i++) {
        assert(coins_cam[i].z > 0.0 &&
               "Tous les coins doivent etre devant la camera");
    }
    printf("   Tous les coins sont devant la camera (z>0)\n");

    // -------------------------------------------------------
    // Test 25 : Projection simple
    // u = fx*(X/Z)+cx, v = fy*(Y/Z)+cy
    // fx=fy=500, cx=cy=256
    // -------------------------------------------------------
    printf("\n--- Test 25 : Projection perspective ---\n");

    double fx = 500.0, fy = 500.0;
    double cx = 256.0, cy = 256.0;

    ProjectedPoint proj[8];
    for (int i = 0; i < 8; i++) {
        proj[i] = project(coins_cam[i], fx, fy, cx, cy);
        assert(proj[i].valid && "Point doit etre valide");
        printf("  Coin %d → u=%.1f, v=%.1f\n",
               i, proj[i].u, proj[i].v);
    }

    // Vérification : coins projetés dans l'image 512x512
    for (int i = 0; i < 8; i++) {
        assert(proj[i].u >= 0 && proj[i].u < 512.0 &&
               proj[i].v >= 0 && proj[i].v < 512.0 &&
               "Coins doivent etre dans l image");
    }
    printf("  Tous les coins projetes dans [0,512]\n");

    // -------------------------------------------------------
    // Test 26 : NkImage 512x512 — afficher les 8 coins
    // -------------------------------------------------------
    printf("\n--- Test 26 : NkImage 512x512 - 8 coins ---\n");

    NkImage img(512, 512);
    img.Fill(0, 0, 0);  // fond noir

    // Afficher les 8 coins en ROUGE
    for (int i = 0; i < 8; i++) {
        int px = (int)proj[i].u;
        int py = (int)proj[i].v;
        img.DrawPoint(px, py, 255, 0, 0);
        printf("  Coin %d dessiné en (%d, %d)\n", i, px, py);
    }

    printf("   8 coins affiches en rouge\n");

    // -------------------------------------------------------
    // Test 27 : Tracer les 12 arêtes du cube
    // -------------------------------------------------------
    printf("\n--- Test 27 : 12 aretes du cube ---\n");

    // Les 12 arêtes définies par paires d'indices
    // Face avant (0-3), face arrière (4-7)
    // + 4 arêtes de connexion
    int aretes[12][2] = {
        // Face avant (z=-0.5 + z_cam = 1.5)
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        // Face arrière (z=0.5 + z_cam = 2.5)
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        // Arêtes de connexion avant-arrière
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    // Dessiner les 12 arêtes en VERT
    for (int i = 0; i < 12; i++) {
        int a = aretes[i][0];
        int b = aretes[i][1];

        int x0 = (int)proj[a].u;
        int y0 = (int)proj[a].v;
        int x1 = (int)proj[b].u;
        int y1 = (int)proj[b].v;

        img.DrawLine(x0, y0, x1, y1, 0, 255, 0);
        printf("  Arete %2d : (%d,%d) → (%d,%d)\n",
               i, x0, y0, x1, y1);
    }

    // Redessiner les coins en rouge PAR DESSUS les arêtes
    for (int i = 0; i < 8; i++) {
        int px = (int)proj[i].u;
        int py = (int)proj[i].v;
        img.DrawPoint(px, py, 255, 0, 0);
    }

    // Sauvegarde PPM
    bool saved = img.SavePPM("cube_projection.ppm");
    assert(saved && "Sauvegarde PPM doit reussir");
    printf("   Image sauvegardee : cube_projection.ppm\n");
    printf("   12 aretes tracees en vert\n");
    printf("   8 coins en rouge par dessus\n");

    // -------------------------------------------------------
    // Tests supplémentaires Vec4d
    // -------------------------------------------------------
    printf("\n--- Tests Vec4d ---\n");

    // w=1 → point, ToVec3 divise par w
    Vec4d point(2.0, 4.0, 6.0, 2.0);
    Vec3d p3 = point.ToVec3();
    assert(approxEq(p3.x, 1.0) &&
           approxEq(p3.y, 2.0) &&
           approxEq(p3.z, 3.0));
    printf("   ToVec3() : (2,4,6,2) → (1,2,3)\n");

    // w=0 → direction
    Vec4d dir(1.0, 0.0, 0.0, 0.0);
    assert(approxEq(dir.w, 0.0));
    printf("   Direction w=0 correcte\n");

    // Layout mémoire
    static_assert(sizeof(Vec4d) == 32,
        "Vec4d doit faire 32 bytes");
    printf("   sizeof(Vec4d) == 32\n");

    printf("\n=== Tous les tests 24-27 passes ! ===\n");
    printf("Ouvrir cube_projection.ppm pour visualiser\n");
    return 0;
}