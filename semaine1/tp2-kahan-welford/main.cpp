// ============================================================
// FILE: main.cpp
// TP2 — Sommation de Kahan & Variance de Welford
// ============================================================
#include <cstdio>
#include <cmath>
#include <numeric>    // std::accumulate
#include <cstdint>

// ---------------------------------------------------------------
// Sommation de Kahan
// ---------------------------------------------------------------
float kahanSum(const float* data, int n) {
    float sum  = 0.0f;
    float comp = 0.0f;  // compensation des erreurs perdues
    for (int i = 0; i < n; i++) {
        float y = data[i] - comp;   // compenser l'erreur precedente
        float t = sum + y;          // t grand, y petit → perte de bits
        comp = (t - sum) - y;       // capture les bits perdus
        sum  = t;
    }
    return sum;
}

// ---------------------------------------------------------------
// Variance naive — DANGEREUSE (cancellation catastrophique)
// ---------------------------------------------------------------
float varianceNaive(const float* data, int n) {
    float sum  = 0.0f;
    float sum2 = 0.0f;
    for (int i = 0; i < n; i++) {
        sum  += data[i];
        sum2 += data[i] * data[i];
    }
    float mean = sum / n;
    // PROBLEME : sum2 - sum*sum peut etre negatif !
    float result = (sum2 - sum * sum / n) / (n - 1);
    return result;
}

// ---------------------------------------------------------------
// Variance Welford — numeriquement stable
// ---------------------------------------------------------------
float varianceWelford(const float* data, int n) {
    float mean = 0.0f;
    float M2   = 0.0f;
    for (int i = 0; i < n; i++) {
        float delta  = data[i] - mean;
        mean        += delta / (i + 1);
        float delta2 = data[i] - mean;
        M2          += delta * delta2;
    }
    return M2 / (n - 1);
}

// ---------------------------------------------------------------
// Mesure de l'epsilon machine par boucle
// ---------------------------------------------------------------
float measureEpsilon() {
    float eps = 1.0f;
    while (1.0f + eps / 2.0f != 1.0f)
        eps /= 2.0f;
    return eps;
}

int main() {
    printf("=== TP2 — Kahan & Welford ===\n\n");

    // -------------------------------------------------------
    // Test 1 : kahanSum vs std::accumulate
    // 1 000 000 copies de 0.1f → theorique = 100000.0
    // -------------------------------------------------------
    const int N = 1000000;
    float* data = new float[N];
    for (int i = 0; i < N; i++) data[i] = 0.1f;

    float valeur_theorique = 100000.0f;

    // std::accumulate
    float accum = std::accumulate(data, data + N, 0.0f);

    // kahanSum
    float kahan = kahanSum(data, N);

    printf("--- Test 1 : Somme de %d x 0.1f ---\n", N);
    printf("Valeur theorique   : %.2f\n", valeur_theorique);
    printf("std::accumulate    : %.4f (erreur = %.4f)\n",
           accum, std::abs(accum - valeur_theorique));
    printf("kahanSum           : %.4f (erreur = %.4f)\n",
           kahan, std::abs(kahan - valeur_theorique));
    printf("Kahan est %s fois plus precis\n\n",
           std::abs(accum - valeur_theorique) >
           std::abs(kahan - valeur_theorique) ? "PLUS" : "moins");

    delete[] data;

    // -------------------------------------------------------
    // Test 2 : varianceNaive vs varianceWelford
    // Cas pathologique : {1e8, 1e8, 1, 2}
    // La variance naive peut donner un resultat negatif !
    // -------------------------------------------------------
    float pathologique[] = {1e8f, 1e8f, 1.0f, 2.0f};
    int n_path = 4;

    float var_naive   = varianceNaive(pathologique, n_path);
    float var_welford = varianceWelford(pathologique, n_path);

    printf("--- Test 2 : Variance de {1e8, 1e8, 1, 2} ---\n");
    printf("varianceNaive   : %.6f %s\n",
           var_naive,
           var_naive < 0 ? "(NEGATIF ! cancellation catastrophique)" : "");
    printf("varianceWelford : %.6f (stable)\n\n", var_welford);

    // -------------------------------------------------------
    // Test 3 : Epsilon machine mesure vs standard
    // -------------------------------------------------------
    float eps_mesure   = measureEpsilon();
    float eps_standard = 1.19209290e-07f; // valeur connue

    printf("--- Test 3 : Epsilon machine ---\n");
    printf("Mesure par boucle  : %.10e\n", eps_mesure);
    printf("Valeur standard    : %.10e\n", eps_standard);
    printf("Identiques         : %s\n\n",
           (eps_mesure == eps_standard) ? "OUI" : "NON");

    // -------------------------------------------------------
    // Test 4 : Demonstration cancellation catastrophique
    // -------------------------------------------------------
    float a = 1234567.89f;
    float b = 1234567.00f;
    float diff = a - b;

    printf("--- Test 4 : Cancellation catastrophique ---\n");
    printf("a          = %.10f\n", a);
    printf("b          = %.10f\n", b);
    printf("a - b      = %.10f\n", diff);
    printf("Attendu    = 0.8900000000\n");
    printf("Erreur     = %.10f\n\n",
           std::abs(diff - 0.89f));

    printf("=== Tous les tests termines ===\n");
    return 0;
}