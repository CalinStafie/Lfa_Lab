#include <bits/stdc++.h>

using namespace std;

ifstream fin("date.in");
ofstream gout("date.out");

class Automat
{
    int nrStates, nrTransitions;
    vector <map <char, set <int>>> transitions;
    vector <map <char, int>> dfaTransitions;
    set <int> initialState, finalState;
    int initState;
    set <pair <int, int>> visited;
    string sigma;
    vector <bool> deadStates = vector <bool> (transitions.size(), false);

public:
    // constructori
    Automat();
    Automat(int n, int t);

    // lambda NFA (adaugare elemente)
    void addTransition_reading_auto(int source, int destination, char letter);
    void addTransition(int source, int destination, char letter);
    void setInitialState(int start);
    void setInitState(int start);
    void addFinalState(int f);
    void update_states_transitions();
    void set_alfabet_sigma(string sir);

    friend istream& operator>>(istream&, Automat&);

    //setters
    void setNrTransitions(int value);
    void setNrStates(int value);
    void updateNrTransitions();
    void showNewAutomat();
    void readAutomatFromFile();

     /* Lambda NFA to DFA process:
    1. Lambda NFA to NFA
        a. Lambda Completion
        b. Lambda Transition Removal
    2. NFA to DFA
    3. Find dead states and eliminate them(optional)
    */

    void lambdaCompletion();
    void nfaToDfa();
    void findDeadStates();

    //destructor
    ~Automat();
};

Automat::Automat():transitions(100000), dfaTransitions(100000)
{
    //ctor

}
Automat::~Automat()
{
    //dtor
}
Automat::Automat(int n, int t): nrStates(n), transitions(n+1), nrTransitions(t)
{

}

//adding elements to auto
void Automat::addTransition(int source, int destination, char letter)
{
    this->transitions[source][letter].insert(destination);
    this->nrTransitions++;
}
void Automat::addTransition_reading_auto(int source, int destination, char letter)
{
    this->transitions[source][letter].insert(destination);
}
void Automat::setInitialState(int start)
{
    this->initialState.insert(start);
    this->initState = start;
}
void Automat::setInitState(int start)
{
    this->initState = start;
}
void Automat::addFinalState(int final)
{
    this->finalState.insert(final);
}
void Automat::setNrTransitions(int value)
{
    this->nrTransitions = value;
}
void Automat::setNrStates(int value)
{
    this->nrStates = value;
}
void Automat::updateNrTransitions()
{
    this->nrTransitions = 0;
    for (auto &it : transitions)
        for (auto &it2 : it)
            this->nrTransitions += it2.second.size();
    cout << "The number of transitions have been updated. It is now: " << this->nrTransitions << '\n';
}
void Automat::update_states_transitions()
{
    int newStates = 0, newTran = 0;
    for (auto it : dfaTransitions)
    {
        if (it.size() == 0)
            break;
        else
        {
            ++newStates;
            newTran += it.size();
        }
    }
    this->nrStates = newStates;
    this->nrTransitions = newTran;
}
void Automat::set_alfabet_sigma(string sir)
{
    this->sigma = sir;
}
void Automat::readAutomatFromFile()
{
    int n;
    fin >> n;
    this->setNrStates(n);
    int t;
    fin >> t;
    this->setNrTransitions(t);
    string sir;
    fin >> sir;
    this->set_alfabet_sigma(sir);
    transitions.resize(nrTransitions);
    int source, destination;
    char letter;

    //reading transitions
    for(int i = 0 ; i < t ; ++i)
    {
        fin >> source >> destination >> letter;
        this->addTransition_reading_auto(source, destination, letter);
    }
    //reading the initial state
    int start;
    fin >> start;
    this->setInitialState(start);
    //reading the number of final states
    int final_number, fState;
    fin >> final_number;
    for (int i = 0 ; i < final_number ; ++i)
    {
        fin >> fState;
        this->addFinalState(fState);
    }
}

void Automat::showNewAutomat()
{
    gout << this->nrStates << " " << this->nrTransitions << '\n';
    for (int i = 0 ; i < this->nrStates ; ++i)
    {
        /*
         for (auto it_map :  transitions[i])
            for (auto it_set : it_map.second)
                gout << i << " " << it_set << " " << it_map.first << '\n';
         */
           for (auto it_map : dfaTransitions[i])
                gout << i << " " << it_map.second << " " << it_map.first << '\n';


    }
    gout << this->initState << '\n';
    gout << this->finalState.size() << " ";
    for (auto it : finalState)
        gout << it << " ";
}


void Automat::lambdaCompletion()
{
    bool addedTrans = true;
    while(addedTrans)
    {
        addedTrans = false;
        for (int i = 0 ; i < this->nrStates ; ++i)
        {
            vector <int> aux;
            for (auto it_aux : transitions[i]['#'])
                aux.push_back(it_aux);

            for(int j = 0 ; j < aux.size() ; ++j)
            {
                for (auto state : transitions[aux[j]])
                {
                    for (auto it : state.second)
                    {
                        if (find(aux.begin(), aux.end(), it) == aux.end() and state.first == '#')
                            aux.push_back(it);
                        transitions[i][state.first].insert(it);
                        addedTrans = true;
                    }
                }
                // add initial states
                if (initialState.find(i) != initialState.end())
                {
                    initialState.insert(aux[j]);
                    addedTrans = true;
                }
                //add final states
                if (finalState.find(aux[j]) != finalState.end())
                {
                    finalState.insert(i);
                    addedTrans = true;
                }
            }

            transitions[i]['#'].clear();
        }
    }
    updateNrTransitions();
    dfaTransitions.resize(nrTransitions);
}
void Automat::nfaToDfa()
{
    vector <map <char, int> > newTransitions;
    newTransitions.resize(1000000);
    int newInitialState;
    set <int> newFinalState;
    map <set <int>, int> subset;
    set <int> temporary;
    queue <set <int> > coada;
    int i = 0;
    temporary.insert(initState);
    coada.push(temporary);
    if(finalState.count(initState))
        newFinalState.insert(i);
    subset[temporary] = i;
    ++i;
    temporary.clear();

    while(!coada.empty())
    {
        set <int> currentStates = coada.front();
        coada.pop();
        for (char const &c : sigma)
        {

            for (auto it1 : currentStates)
                for (auto it2 : transitions[it1][c])
                    temporary.insert(it2);

            if(temporary.empty())
                continue;

            if (subset.find(temporary) == subset.end())
            {
                subset[temporary] = i;
                ++i;
                for (auto it : temporary)
                    if (finalState.find(it) != finalState.end())
                        newFinalState.insert(subset[temporary]);
                newTransitions.emplace_back(map<char, int>());
                coada.push(temporary);
            }

            newTransitions[subset[currentStates]][c] = subset[temporary];
            temporary.clear();
        }

    }

    finalState = newFinalState;
    dfaTransitions = newTransitions;
    update_states_transitions();

}
void Automat::findDeadStates()
{
    queue <int> q;
    q.push(initState);
    while (!q.empty())
    {
        int aState = q.front();
        q.pop();
        for (auto trans : dfaTransitions[aState])
            if(deadStates[trans.second] == true)
            {
                deadStates[trans.second] = false;
                q.push(trans.second);
            }
    }
}
int main()
{
    Automat ob;
    ob.readAutomatFromFile();
    ob.lambdaCompletion();
    ob.nfaToDfa();
    //ob.findDeadStates();
    ob.showNewAutomat();
    return 0;
}
