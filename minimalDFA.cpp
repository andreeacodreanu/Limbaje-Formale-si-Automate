#include <iostream>
#include <fstream>
#include <mem.h>
#include <string.h>

using namespace std;

char alfabet[10];
int m[10][10], d[10][10];
int nr_stari, i, nr_simboluri, nr_stari_finale, stare_initiala, finale[10];

int apartine_finale(int a) //verifica daca o stare este finala
{
    for (int i = 0; i < nr_stari_finale; i++)
        if (finale[i] == a)
            return 1;
    return 0;
}

int apartine_excluse(int s, int stari_excluse[], int a) //starile in care nu se poate ajunge
{
    for (int i = 0; i < s; i++)
        if (stari_excluse[i] == a)
            return 1;
    return 0;
}

int main()
{
    ifstream f("automat.in");

    /*Se citesc starile si se retin intr-un vector*/
    f >> nr_stari; //numarul de stari
    int stari[nr_stari];
    for (i = 0; i < nr_stari; i++)
        f >> stari[i];

    /*Se citesc simbolurile alfabetului admis de automat si se retin intr-un
    vector*/
    f >> nr_simboluri; //numarul de simboluri
    for (i = 0; i < nr_simboluri; i++)
        f >> alfabet[i];


    /*Se citesc starile finale si se retin intr-un vector*/
    f >> nr_stari_finale; //numarul stailor finaale
    for (i = 0; i < nr_stari_finale; i++)
        f >> finale[i];

    /*Se citeste starea initiala a automatului*/
    f >> stare_initiala;

    /*Declararea matricei de tranzitie si initializarea cu -1*/
    memset(m, -1, sizeof(m));
    while (f)
    {
        int x, z;
        char y;
        f >> x >> y >> z;
        if (y >= 48 && y <= 57)
            m[x][y - '0'] = z;
        else
            m[x][y - 'a'] = z;
    }

    /*Afisarea matricei de tranzitie a DFA-ului*/
    cout << "   ";
    for (i = 0; i < nr_simboluri; i++)
        cout << alfabet[i] << " ";
    cout << endl;
    for (i = 0; i < nr_simboluri; i++)
        cout << "___";
    cout << endl;
    for (i = 0; i < nr_stari; i++)
    {
        cout << i << " |";
        for (int j = 0; j < nr_simboluri; j++)
            cout << m[i][j] << " ";
        cout << endl;
    }

    /*Se creaza o matrice d in care se vor marca cu 1 starile neechivalente*/
    cout << endl;
    memset (d, 0, sizeof(d));

    for (i = 0; i < nr_stari; i++)
        for (int j = 0; j < nr_stari; j++)
        {
            if (apartine_finale(i)+apartine_finale(j) == 1)
            {
                d[i][j] = 1;
                d[j][i] = 1;
            }
        }

    /*Verificam daca exista o stare in care nu se poate ajunge din starea
    initiala si in caz afirmativ marcam linia si coloana respectiva cu 1*/
    int stari_excluse[10], s = 0;
    for (int k = 0; k < nr_stari; k++)
    {
        int ok = 0;
        for (i = 0; i < nr_stari; i++)
            for (int j = 0; j < nr_simboluri; j++)
                if (k == m[i][j])
                    ok = 1;
        if (ok == 0)
        {
            for (int a = 0; a < nr_stari; a++)
            {
                if (k != a)
                {
                    d[k][a] = 1;
                    d[a][k] = 1;
                }
            }
            stari_excluse[s] = k;
            s++;
        }
    }


    cout << "   ";
    for (i = 0; i < nr_stari; i++) cout << i << " ";
    cout << endl << endl;
    for (i = 0; i < nr_stari; i++)
    {
        cout << i << "  ";
        for (int j = 0; j < nr_stari; j++)
            cout << d[i][j] << " ";
        cout << endl;
    }

    for (i = 0; i < nr_stari-1; i++)
        for (int j = i+1; j < nr_stari; j++)
        {
            if (d[i][j] == 0)
            {
                for (int k = 0; k < nr_simboluri; k++)
                {
                    int a = m[i][k], b = m[j][k];
                    if (apartine_finale(a)+apartine_finale(b) == 1)
                    {
                        d[i][j] = 1;
                        d[j][i] = 1;
                        d[a][b] = 1;
                        d[b][a] = 1;
                    }
                }
            }
        }

    for (i = 0; i < nr_stari-1; i++)
        for (int j = i+1; j < nr_stari; j++)
        {
            if (d[i][j] == 0)
            {
                for (int k = 0; k < nr_simboluri; k++)
                {
                    if (d[m[i][k]][m[j][k]] == 1)
                    {
                        d[i][j] = 1;
                        d[j][i] = 1;
                    }
                }
            }
        }

    cout << endl << "   ";
    for (i = 0; i < nr_stari; i++) cout << i << " ";
    cout << endl << endl;
    for (i = 0; i < nr_stari; i++)
    {
        cout << i << "  ";
        for (int j = 0; j < nr_stari; j++)
            cout << d[i][j] << " ";
        cout << endl;
    }

    /*Cautarea starilor echivalente in matrice(unde apare valoarea 0)*/
    cout << endl;
    for (i = 0; i < nr_stari-1; i++)
        for (int j = i+1; j < nr_stari; j++)
            if (d[i][j] == 0)
                cout << "Sunt echivalente: " << i << " " << j << endl;

    cout << endl;
    for (i = 0; i < nr_stari-1; i++)
        for (int j = i+1; j < nr_stari; j++)
            if (d[i][j] == 0)
            {
                int x = min(i, j);
                stari[i] = min(x,stari[i]);
                stari[j] = min(x,stari[j]);
            }

    /*Identificaree si afisarea starilor ce sunt echivalente in minimal DFA*/
    int k = -1;
    cout << "Automatul minimal are starile: " << endl;
    for (i = 0; i < nr_stari; i++)
    {
        int afis  = 0;
        if (stari[i] >= 0 && apartine_excluse(s, stari_excluse, i) == 0)
            {
                cout << i << " ";
                afis = 1;
            }

        for (int j = i+1; j < nr_stari; j++)
        {
            if (stari[i] == stari[j] && apartine_excluse(s, stari_excluse, j) == 0)
            {
                cout << j << " ";
                stari[j] = k;
                k--;
                afis  = 1;
            }
        }
        if (stari[i] >= 0 && afis == 1)
            cout << endl;

        stari[i] = k;
        k--;
    }

    return 0;
}
