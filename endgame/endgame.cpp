#include <iostream>
#include <vector>
#include <iomanip>      // per i decimali
#include <fstream>
using namespace std;
#define DEBUG 0

struct stone {
    int energia, massa;
};


int N;              // numero di citta
int M;              // numero di pietre
int S;              // citta di partenza
int capacita;       // capacità dello zain ehm guanto
double R;              // energia consumata per unita di tempo
double vmax, vmin;
double vcost;

vector<int> carriedStones;                  // ogni indice è una citta e mi dice qual è il peso locale trasportato
vector<stone> stones;                       // lista (temp!) delle pietre raccolte
vector<int> takenStones;                    // di base -1, se raccolgo una pietra metto in che citta l'ho raccolta
vector<vector<int>> cities;                 // per ogni citta mi segno (con un array) quali pietre ha (gli ID delle pietre)
int **matrix;                               // matrice di adiacenza

double energia;     // energia RACCOLTA


void getInput(){
    ifstream in("input.txt");
    in >> N >> S;                               // prima riga
    in >> M;
    in >> capacita;
    in >> R;
    in >> vmin;
    in >> vmax;   
    
    // massa/energia di ogni pietra
    int m, e;
    for(int i=0; i<M; i++){
        in >> m >> e;
        stone s;
        s.energia = e;
        s.massa = m;
        stones.push_back(s);
    }

    // dove stanno le pietre?
    cities = vector<vector<int>>();                // inizializzo l'array di locations e carriedStones
    carriedStones = vector<int>();
    for(int i=0; i<N; i++){
        vector<int> row;
        cities.push_back(row);
        carriedStones.push_back(0);
    }

    int listlen;
    int tmpCity;
    for(int i=0; i<M; i++){ 
        in >> listlen;
        for (int j=0; j<listlen; j++){
            // la pietra `i` è presente nella città tmpCity
            in >> tmpCity;
            cities[tmpCity].push_back(i);
        }
    }

    // distanze tra le citta
    matrix = new int*[N];
    int tmp;

    for(int i=0; i<N; i++){
        matrix[i] = new int[N];
    }

    for(int i=0; i<N; i++){
        matrix[i][i] = 0;
    }

    for(int i=1; i<N; i++){
        for(int j=0; j<i; j++){
            in >> tmp;
            matrix[i][j] = tmp;
            matrix[j][i] = tmp;
        }
    }

    vcost = (vmax-vmin)/capacita;
    takenStones = vector<int>(M);
    for(int i=0; i<M; i++){
        takenStones[i] = -1;
    }

    in.close();
}


void getInfos(){
    cout << "Numero di città: " << N << endl;
    cout << "Città di partenza: " << S << endl;
    
    cout << "Numero di pietre: " << M << endl;
    cout << "Capacita' del guanto: " << capacita << endl;
    
    // pietre
    for(int i=0; i<M; i++){
        cout << "[Pietra] " << i 
            << ":\n  energia=" << stones[i].energia
            << "\n  massa=" << stones[i].massa << endl;
    }

    cout << endl;

    // grafo
    for(int i=0; i<N; i++){
        cout << "[Citta] " << i << ":\n"; 
        for(int j=0; j<N; j++){
            cout << "  " << i << " to " << j << ": " << matrix[i][j] << endl;
        }
    }

    cout << "==========" << endl;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cout << matrix[i][j];
        }
        cout << endl;
    }
    cout << "==========" << endl;
}


double getVelocita(int _carriedStones){
    return vmax-((_carriedStones*(vmax-vmin))/capacita);
}


void writeSolution(vector<int> &path, vector<int> &distance){
    ofstream out("output.txt");

    // calcolo il tempo ripercorrendo la path
    int pesosofar = 0;
    double tempoImpiegato;
    for(int i=0; i<N; i++){
        int citta = path[i];
        double v = getVelocita(pesosofar+carriedStones[citta]);
        double t = distance[i]/v;
        tempoImpiegato += t;
        pesosofar += carriedStones[citta];

        // cout << "Citta: " << path[i] << " Peso: " << pesosofar << " Velocita:" << v << " Tempo:" << t << endl;

    }

    // calcolo l'energia
    // cout << "energia " << energia << endl;
    // cout << "tempoImpiegato " << tempoImpiegato << endl;
    double energiaFinale = energia - (R*tempoImpiegato);
    out << scientific << setprecision(10) << energiaFinale << " "; 
    out << scientific << setprecision(10) << energia << " "; 
    out << scientific << setprecision(10) << tempoImpiegato << endl; 

    // lista pietre
    for(int i=0; i<M; i++){
        out << takenStones[i] << " ";
    }
    out << endl;

    // lista citta
    for(int i=0; i<path.size(); i++){
        out << path[i] << " ";
    }
    out << endl;

    out << "***";

    out.close();
}


void collectGems(vector<int> &path, vector<int> &distance){
    // ciclo al contrario
    int capacitaLeft = capacita;
    int distsofar = 0;
    int imax;
    double zetamax, maxsofar;

    #if (DEBUG==1)
    cout << path.size() << path[0] << endl;

    for(int i=0; i<path.size(); i++){
        cout << path[i] << " " << "[" << distance[i] << "]" << endl;
    }
    cout << endl;
    #endif

    // NON DEVO ANDARE AL CONTRARIO PERCHE COSI PRENDEREI ALLA FINE QUELLE CON LA DISTANZA MINORE
    // QUANDO INVECE DOVREI PRENDERE QUELLE PIU PESANTI VERSO LA FINE

    for(int i=path.size()-2; i>=0; i--){    // -2 perche non parto dall'ultimo, che è la partenza aka S
        maxsofar = 0;
        distsofar += distance[i];           // i e basta perche len(distance) == len(path)-{Sin, Sfin} 
        zetamax = -100;
        imax = 0;

        #if (DEBUG==1)
        cout << "\n(citta " << path[i] << ") " << "size " << cities[path[i]].size() << endl;
        cout << "  dist=" << distance[i] << endl;
        #endif

        for(int j=0; j<cities[path[i]].size(); j++){
            stone s = stones[cities[path[i]][j]];
            
            #if (DEBUG==1)
            cout << "Pietra " << cities[path[i]][j] << endl;
            #endif 

            // controllo se ho gia preso la pietra
            if(takenStones[cities[path[i]][j]] == -1 && s.energia >= s.massa){
                // prendo il fattore zeta=e/pd
                
                double zeta = ((double) s.energia) / ( s.massa*distsofar );  // th. devo moltiplicare per distsofar perche è la somma delle distanze da qui alla fine

                #if (DEBUG==1)
                cout << "ZETA=" << zeta << ", e=" << s.energia << ", m=" << s.massa << endl << endl;
                cout << "   zetamax=" << zetamax << " smassa" << s.massa << " capleft=" << capacitaLeft << endl;
                #endif

                if(zeta > zetamax && s.massa<=capacitaLeft){
                    zetamax = zeta;
                    imax = j;
                }
            } 
            #if (DEBUG==1)
            else {
                cout << "...gia presa o troppo pesante" << endl;
            }
            #endif
        }

        // ho trovato la pietra che tendenzialmente ha il miglior rapporto
        if(zetamax > 0){
            // la prendo
            int stoneIndex = cities[path[i]][imax];

            capacitaLeft -= stones[stoneIndex].massa;
            
            // prendo la pietra i
            energia += stones[stoneIndex].energia;

            // ho raccolto tale pietra in imax citta
            takenStones[stoneIndex] = path[i];     // path[i] è la citta in cui sono

            carriedStones[path[i]] = stones[stoneIndex].massa;

            

            #if (DEBUG==1)
            cout << "Ho preso la pietra " << stoneIndex << " nella citta " << path[i] << endl;
            #endif
        }
    }

    writeSolution(path, distance);

}


/**
 * Attraverso il grafo e trovo un percorso ottimale.
 * - versione 1: prendo sempre il ramo piu corto [greedy] O(n^2)
 * - versione 1.1: greedy ma senza controllare l'intera riga(?) O(n^2)--
 * - versione 2: [backtracking]
 */
void getBestPath(){
    vector<int> path = vector<int>();
    vector<int> distance = vector<int>();
    bool visited[N];
    int remaining = N;

    // init
    for(int i=0; i<N; i++){
        // path[i] = -1;   // -inf
        visited[i] = false;
    }

    // path[S] = 0;    // partenza
    path.push_back(S);
    visited[S] = true;

    for(int node=S; node<N;){            // cosi parto da 0 poi salgo, ma potrei perdermi qualcosa?
        // passo i nodi a fianco
        int min = 1000000;
        int minIndex = -1;

        for(int j=0; j<N; j++){
            if (remaining == 1){
                // okay ora mi ricongiungo a S
                min = matrix[node][S];
                minIndex = S;
                break;
            }

            // poi posso evitare di controllare l'intera riga perche so gia che meta/+ sono 0 
            #if (DEBUG==1)
            cout << "min=" << min << ", Matrix[node][j] = " << matrix[node][j] << " [NODE="<< node << ", J="<< j << endl;
            #endif
            if (matrix[node][j] != 0 && !visited[j] && min > matrix[node][j]){
                min = matrix[node][j];
                minIndex = j;
            }
        }

        // la prossima citta è quella piu breve
        #if (DEBUG==1)
        cout << min << " at index " << minIndex << endl;
        #endif 

        path.push_back(minIndex);
        distance.push_back(min);
        visited[minIndex] = true;

        node = minIndex;
        if (minIndex == -1){
            break;
        }
        remaining--;
    }

    path.push_back(S);
    path.resize(N+1);
    collectGems(path, distance);
}



int main(){
    getInput();
    getBestPath();
    return 0;
}