/// Implementare Automat Moore

#include <bits/stdc++.h>

using namespace std;

ifstream fin("date.in");
ofstream gout("date.out");


typedef pair<int, char> transition;

bool acceptat;
int n, m, x, y, stare_initiala, nr_stari_finale, final, teste;
char litera;
string sir;
int main()
{
    /// n - numar noduri, m - numar muchii
    fin >> n >> m;

    /// vom considera un vector de vectori de pair-uri pentru a tine lista de adiacenta a fiecarui nod de la 0 la n-1;
    vector <transition> dfa[n];

    /// aici vom tine outputul fiecarei stari de la 0 la n-1;
    vector <string> output;

    ///citire output;
    for (int i = 0 ; i < n ; ++i)
    {
        fin >> sir;
        output.push_back(sir);
    }

    ///citire muchii si tranzitii
    for (int i = 0 ; i < m ; ++i)
    {
        fin >> x >> y >> litera;
        dfa[x].push_back({y, litera});
    }
    fin >> stare_initiala;
    fin >> nr_stari_finale;

    /// vector in care tinem starile finale;
    vector <int> stari_finale;
    for (int i = 0 ; i < nr_stari_finale ; ++i)
    {
        fin >> final;
        stari_finale.push_back(final);
    }

    /// numarul de teste
    fin >> teste;
    fin.get();

    for (int t = 0 ; t < teste ; ++t)
    {
        /// in acceptat tinem starea fiecarui cuvant : 1 - acceptat, 0 - respins; initial, pornim cu fiecare cu val. 0;
        acceptat = 0;

        /// coada in care salvam toate perechile : (stare_curenta, sir de litere ramas)
        queue <pair<int, string>> coada;

        /// in acest vector de pai-uri salvam traseul(starile le-am considerat de tip int)
        ///si outputul nostru care este de tip string;
        vector <pair<int, string>> traseu;

        traseu.push_back({stare_initiala, output[stare_initiala]});
        getline(fin, sir);
        coada.push({stare_initiala, sir});
        while (!coada.empty())
        {
            pair <int, string> primul_element = coada.front();

            ///parcurgem lista de vecini a starii curente si verificam daca primul caracter din string corespunde
            ///cu litera din lista de vecini
            for (int i = 0 ; i < dfa[primul_element.first].size(); ++i)
            {
                if (dfa[primul_element.first][i].second == primul_element.second[0])
                {
                    ///daca am gasit litera si starea, eliminam prima litera din string si apoi actualizam coada, traseul
                    ///(care este impreuna cu output-ul deoarece avem vector de pair-uri;
                    sir.erase(sir.begin());
                    coada.push({dfa[primul_element.first][i].first, sir});
                    traseu.push_back({dfa[primul_element.first][i].first, output[dfa[primul_element.first][i].first]});
                    ///gout << coada.back().first << " " << coada.back().second<< '\n';
                }
            }

            ///daca elementul din varful cozii are ca string sirul vid si starea curenta in multimea starilor finale
            ///ex: ( qf, "" ) unde qf este o stare in multimea starilor finale, atunci
            ///afisam "DA" si outputul corespunzator + traseul cuvantului si facem variabila booleana acceptat = 1;
            /// si un break pentru a parisi parcurgerea cozii
            if (coada.front().second == "" and find(stari_finale.begin(), stari_finale.end(), coada.front().first) != stari_finale.end())
                    {
                        gout << "DA" << '\n';
                        for (int i = 0 ; i < traseu.size() ; ++i)
                            gout << traseu[i].second;
                        gout << "\nTraseu: ";
                        for (int i = 0 ; i < traseu.size() ; ++i)
                            gout << traseu[i].first << " ";
                        gout << '\n';
                        acceptat = 1;
                        break;
                    }
            coada.pop();
        }

        /// daca acceptat = 0, asta inseamna ca ori stringul nu este vid ori starea curenta nu face parte din
        /// multimea starilor finale, deci afisam "NU" si continuam cu procesarea cuvintelor ramase
        if (acceptat == 0)
            gout << "NU" << '\n';
        gout << '\n';
    }

    return 0;
}
