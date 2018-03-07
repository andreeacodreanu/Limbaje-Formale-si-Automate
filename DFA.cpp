//IMPLEMENTAREA UNUI DFA
#include <iostream>
#include <fstream>
#include <mem.h>
#include <string.h>

using namespace std;

int main()
{
    /*Datele eutomatului se citesc dintr-un fisier*/
    ifstream f("DFA.txt");
    int nr_stari, i, nr_simboluri, nr_stari_finale, stare_initiala;

    /*Se citesc starile si se retin intr-un vector*/
    f >> nr_stari; //numarul de stari
    int stari[nr_stari];
    for (i = 1; i <= nr_stari; i++)
        f >> stari[i];

    /*Se citeste simbolurile alfabetului admis de automat si se retin intr-un
    vector*/
    f >> nr_simboluri; //numarul de simboluri
    char alfabet[nr_simboluri];
    for (i = 1; i <= nr_simboluri; i++)
        f >> alfabet[i];

    /*Se citesc starile finale si se retin intr-un vector*/
    f >> nr_stari_finale; //numarul stailor finaale
    int finale[nr_stari_finale];
    for (i = 1; i <= nr_stari_finale; i++)
        f >> finale[i];

    /*Se citeste starea initiala a automatului*/
    f >> stare_initiala;

    /*Declararea matricei de tranzitie si initializarea cu -1*/
    int m[nr_stari][nr_simboluri];
    memset(m, -1, sizeof(m));
    while (f)
    {
        int x, z;
        char y;
        f >> x >> y >> z;
        m[x][y - '0'] = z;
    }
    /*for (i = 0; i < nr_stari; i++)
    {
        for (int j = 0; j < nr_simboluri; j++)
            cout << m[i][j] << " ";
        cout << endl;
    */

    /*Citirea cuvantului ce urmeaza a fi verificat*/
    char c[20];
    cout << "Dati cuvantul: ";
    cin.get(c, 20);

    /*Verificam daca in cuvant apar litere care nu apartin alfabetului.
    In acest caz cuvantul nu este acceptat*/
    int x = strlen(c), ok = 0;
    for (i = 0; i < x; i++)
        if (strchr(alfabet, c[i]) == 0)
        {
            ok = 1;
            break;
        }
    if (ok == 1)
    {
        cout << "Cuvantul nu este acceptat." << endl;
        return 0;
    }

    /*Verificarea cuvantului*/
    int stare_curenta = stare_initiala;
    for (i = 0; i < x; i++)
    {
        stare_curenta = m[stare_curenta][c[i]-'0'];
        if (stare_curenta == -1)
        {
            cout << "Cuvantul nu este acceptat." << endl;
            return 0;
        }
    }

    ok = 0;
    for (i = 1; i <= nr_stari_finale; i++)
        if (finale[i] == stare_curenta)
            ok = 1;
    if (ok == 1)
        cout << "Cuvantul este acceptat." << endl;
    else
        cout << "Cuvantul nu este acceptat." << endl;

    return 0;
}
