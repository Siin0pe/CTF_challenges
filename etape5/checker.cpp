#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <random>
#include <signal.h>

using namespace std;

static string trim_newline(const string &s) {
    if (!s.empty() && (s.back() == '\n' || s.back() == '\r'))
        return s.substr(0, s.size() - 1);
    return s;
}

int main() {
    int to_child[2];
    int from_child[2];

    if (pipe(to_child) == -1 || pipe(from_child) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Enfant : exécute python3 badge.py
        dup2(to_child[0], STDIN_FILENO);
        dup2(from_child[1], STDOUT_FILENO);

        close(to_child[0]);
        close(to_child[1]);
        close(from_child[0]);
        close(from_child[1]);

        execlp("python3", "python3", "badge.py", (char*)nullptr);

        perror("execlp");
        _exit(1);
    }

    // Parent : checker
    close(to_child[0]);
    close(from_child[1]);

    FILE *child_in  = fdopen(to_child[1], "w");
    FILE *child_out = fdopen(from_child[0], "r");

    if (!child_in || !child_out) {
        cerr << "Erreur interne.\n";
        kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);
        return 1;
    }

    auto read_line_or_crash = [&](int etape) -> string {
        char buf[1024];
        if (!fgets(buf, sizeof(buf), child_out)) {
            cerr << "Le script s'est arrêté avant la fin (étape " 
                 << etape << ").\n";
            cerr << "Il y a sûrement une erreur Python.\n";
            cerr << "Lance ton script directement avec : python3 badge.py\n";
            waitpid(pid, nullptr, 0);
            exit(1);
        }
        return trim_newline(buf);
    };

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> d10(1, 10);
    uniform_int_distribution<int> dage(10, 30);
    uniform_int_distribution<int> dniv(1, 5);

    // ------------------------
    // ÉTAPE 1 : version
    // ------------------------
    const int version_attendue = 1234;
    string out1 = read_line_or_crash(1);

    if (out1 != to_string(version_attendue)) {
        cerr << "Étape 1 : La version du badge n'est pas correcte, version attendue est : " << version_attendue << "\n";
        cerr << "Ton script a renvoyé : " << out1 << "\n";
        kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);
        return 0;
    }
    cout << "[Étape 1 OK]\n";

    // ------------------------
    // ÉTAPE 2 : etape_2(x) = 2 * x
    // ------------------------
    {
        int x = d10(gen);
        fprintf(child_in, "%d\n", x);
        fflush(child_in);

        string out2 = read_line_or_crash(2);
        int attendu = x * 2;

        if (out2 != to_string(attendu)) {
            cerr << "Étape 2 : Mauvaise réponse, le calcul à faire est : 2 fois a.\n";
            cerr << "Résultat attendu : " << attendu << "\n";
            cerr << "Ton script a renvoyé : " << out2 << "\n";
            kill(pid, SIGKILL);
            waitpid(pid, nullptr, 0);
            return 0;
        }

        cout << "[Étape 2 OK]\n";
    }

    // ------------------------
    // ÉTAPE 3 : etape_3(a, b) = a * b + 3
    // ------------------------
    {
        int a = d10(gen);
        int b = d10(gen);
        fprintf(child_in, "%d\n%d\n", a, b);
        fflush(child_in);

        string out3 = read_line_or_crash(3);
        int attendu = a * b + 3;

        if (out3 != to_string(attendu)) {
            cerr << "Étape 3 : Mauvaise réponse, le calcul à faire est : a fois b plus 3.\n";
            cerr << "Résultat attendu : " << attendu << "\n";
            cerr << "Ton script a renvoyé : " << out3 << "\n";
            kill(pid, SIGKILL);
            waitpid(pid, nullptr, 0);
            return 0;
        }

        cout << "[Étape 3 OK]\n";
    }

    // ------------------------
    // ÉTAPE 4 : etape_4(age, niveau)
    //          "OUI" si age >= 18 et niveau >= 3, sinon "NON"
    // ------------------------
    {
        int niveau = dniv(gen);
        fprintf(child_in, "%d\n", niveau);
        fflush(child_in);

        string out4 = read_line_or_crash(4);
        string attendu = (niveau > 3) ? "OUI" : "NON";

        if (out4 != attendu) {
            cerr << "Étape 4 : Mauvaise réponse, le badge doit renvoyé si \"OUI\" si le niveau fourni est supprieur à 3, sinon \"NON\".\n";
            cerr << "Résultat attendu : " << attendu << "\n";
            cerr << "Ton script a renvoyé : " << out4 << "\n";
            kill(pid, SIGKILL);
            waitpid(pid, nullptr, 0);
            return 0;
        }

        cout << "[Étape 4 OK]\n";
    }

    cout << "----------------------------------------\n";
    cout << "Toutes les étapes sont validées.\n\n";

    cout << "FLAG_BADGE_VALIDE\n";

    waitpid(pid, nullptr, 0);
    return 0;
}

