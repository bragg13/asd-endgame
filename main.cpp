#include <iostream>
#include <vector>
#include <iomanip>      // per i decimali
#include <fstream>
#include <cmath>

using namespace std;

#define DEBUG_GETPATH 1
#define DEBUG_COLLECT 0
#define DEBUG_WRSOLUT 0
#define ZETA 1

struct citta {
    int id, mass, energy;
    vector<int> s;
    double max_goodness;
};

struct stone {
    int energia, massa;
    double goodness;
};

int N;              // numero di citta
int M;              // numero di pietre
int S;              // citta di partenza
int capacita;       // capacità dello zain ehm guanto
double R;              // energia consumata per unita di tempo
double vmax, vmin;
double vcost;
double avg_goodness, avg_mass, kz = 0.955, k_edge = 1, k_goodness = 0.95;

vector<int> carriedStones;                  // ogni indice è una citta e mi dice qual è il peso locale trasportato
vector<stone> stones;                       // lista (temp!) delle pietre raccolte
vector<int> takenStones;                    // di base -1, se raccolgo una pietra metto in che citta l'ho raccolta
vector<citta> cities;                 // per ogni citta mi segno (con un array) quali pietre ha (gli ID delle pietre)
int **matrix;                               // matrice di adiacenza

double energia;     // energia RACCOLTA

double getVelocita(int _carriedStones){
    return vmax-((_carriedStones*(vmax-vmin))/capacita);
}

bool goodStone(stone s, int d, int capacita_left){
    //return s.goodness >= kg*avg_goodness;
    return (R*d/getVelocita(s.massa + capacita - capacita_left) - s.energia) < (R*d/getVelocita(capacita - capacita_left));
}

bool similarZeta(double zeta, double zeta_max){
    return (zeta_max * kz) < zeta;
}

double getStima(int d, int m, int e){
    return e - R*(d / getVelocita(m));
}

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
    double sum_goodness = 0;
    double sum_mass = 0;
    for(int i=0; i<M; i++){
        in >> m >> e;
        stone s;
        s.energia = e;
        s.massa = m;
        sum_mass += m;
        s.goodness =  ((double) e)/m;
        sum_goodness += s.goodness;
        stones.push_back(s);
    }
    avg_mass = sum_mass / M;
    avg_goodness = sum_goodness / M;

    // dove stanno le pietre?cities
    cities = vector<citta>();                // inizializzo l'array di locations e carriedStones
    carriedStones = vector<int>();
    for(int i=0; i<N; i++){
        citta c;    c.max_goodness = 0;
        cities.push_back(c);
        carriedStones.push_back(0);
    }

    int listlen;
    int tmpCity;
    for(int i=0; i<M; i++){ 
        in >> listlen;
        for (int j=0; j<listlen; j++){
            // la pietra `i` è presente nella città tmpCity
            in >> tmpCity;
            cities[tmpCity].s.push_back(i);
            if(cities[tmpCity].max_goodness < stones[i].goodness){
                cities[tmpCity].mass = stones[i].massa; cities[tmpCity].energy = stones[i].energia; 
                cities[tmpCity].max_goodness = stones[i].goodness;
            }
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

void writeSolution(vector<int> &path, vector<int> &distance){
    ofstream out("output.txt");

    // calcolo il tempo ripercorrendo la path
    int pesosofar = 0;
    double tempoImpiegato=0.0;
    
    for(int i=0; i<N; i++){
        int citta = path[i];
        double v = capacita==0 ? vmax : getVelocita(pesosofar+carriedStones[citta]);
        double t = distance[i]/v;
        tempoImpiegato += t;
        pesosofar += carriedStones[citta];

        #if (DEBUG_WRSOLUT==1)
        cout << "Citta: " << path[i] << " Peso: " << pesosofar << " Velocita:" << v << " Tempo:" << t << endl;
        #endif
    }

    // calcolo l'energia
    #if (DEBUG_WRSOLUT==1)
    cout << "energia " << energia << endl;
    cout << "tempoImpiegato " << tempoImpiegato << endl;
    #endif

    // caso limite: capacità=0
    // if(capacita == 0){
    //     out << scientific << setprecision(10) << 0.00 << " "; 
    //     out << scientific << setprecision(10) << 0.00 << " "; 
    //     out << scientific << setprecision(10) << 0.00 << endl; 
    // } else {
        double energiaFinale = energia - (R*tempoImpiegato);
        out << scientific << setprecision(10) << energiaFinale << " "; 
        out << scientific << setprecision(10) << energia << " "; 
        out << scientific << setprecision(10) << tempoImpiegato << endl; 
    // }

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

    #if (DEBUG_COLLECT==1)
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

        #if (DEBUG_COLLECT==1)
        cout << "\n(citta " << path[i] << ") " << "size " << cities[path[i]].size() << endl;
        cout << "  dist=" << distance[i] << endl;
        #endif

        for(int j=0; j<cities[path[i]].s.size(); j++){
            stone s = stones[cities[path[i]].s[j]];
            
            #if (DEBUG_COLLECT==1)
            cout << "Pietra " << cities[path[i]][j] << endl;
            #endif 

            // controllo se ho gia preso la pietra
            if(takenStones[cities[path[i]].s[j]] == -1 && goodStone(s, distsofar, capacitaLeft)){
                // prendo il fattore zeta=e/pd
                
                double zeta = ((double) s.energia) / ( s.massa*distsofar );  // th. devo moltiplicare per distsofar perche è la somma delle distanze da qui alla fine

                #if (DEBUG_COLLECT==1)
                cout << "ZETA=" << zeta << ", e=" << s.energia << ", m=" << s.massa << endl << endl;
                cout << "   zetamax=" << zetamax << " smassa" << s.massa << " capleft=" << capacitaLeft << endl;
                #endif

                if(zeta > zetamax && s.massa<=capacitaLeft){
                    zetamax = zeta;
                    imax = j;
                } else if(similarZeta(zeta, zetamax) && s.massa<=capacitaLeft){
                    if(stones[imax].massa < s.massa){
                        imax = j; zetamax = zeta;
                    }
                }
            } 
            #if (DEBUG_COLLECT==1)
            else {
                cout << "...gia presa o troppo pesante" << endl;
            }
            #endif
        }

        // ho trovato la pietra che tendenzialmente ha il miglior rapporto
        if(zetamax > 0){
            // la prendo
            int stoneIndex = cities[path[i]].s[imax];

            capacitaLeft -= stones[stoneIndex].massa;
            
            // prendo la pietra i
            energia += stones[stoneIndex].energia;

            // ho raccolto tale pietra in imax citta
            takenStones[stoneIndex] = path[i];     // path[i] è la citta in cui sono

            carriedStones[path[i]] = stones[stoneIndex].massa;

            

            #if (DEBUG_COLLECT==1)
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

    int capacitaLeft = capacita;
    int distsofar = 0;
    int imax; double zetamax, maxsofar;


    // init
    for(int i=0; i<N; i++){
        // path[i] = -1;   // -inf
        visited[i] = false;
    }

    // path[S] = 0;    // partenza
    path.push_back(S);
    visited[S] = true;
    vector<int> bestCities;

    for(int node=S; node<N;){            // cosi parto da 0 poi salgo, ma potrei perdermi qualcosa?
        // passo i nodi a fianco
        double dmin = 1000000;
        double max = -1000000;
        int min = 1000000;  int curr_mass = -1;
        int minIndex = -1; stone s; int st_index = -1;


        for(int j=0; j<N; j++){

            if (remaining == 1){
                // okay ora mi ricongiungo a S
                min = matrix[node][S];
                minIndex = S;
                break;
            }

            // poi posso evitare di controllare l'intera riga perche so gia che meta/+ sono 0 
            #if (DEBUG_GETPATH==1)
            cout << "min=" << min << ", Matrix[node][j] = " << matrix[node][j] << " [NODE="<< node << ", J="<< j << endl;
            #endif

            cout << "j: " << j << " m: " << matrix[node][j] << " v: " << visited[j] << endl;

            if(matrix[node][j] != 0 && !visited[j]){
                
                if(getStima(distsofar + matrix[node][j], 0, 0) > max){
                    st_index = -1;
                    max = getStima(distsofar + matrix[node][j], 0, 0);
                    minIndex = j; min = matrix[node][j];
                }
                for(int st_i = 0;st_i < cities[j].s.size(); st_i++){
                    cout << "st_i: " << st_i << endl;
                    s = stones[cities[j].s[st_i]];
                    if(takenStones[cities[j].s[st_i]] == -1 && goodStone(s, distsofar + matrix[node][j], capacitaLeft)){
                        if(getStima(distsofar + matrix[node][j], s.massa, s.massa) > max)
                        {
                            st_index = cities[j].s[st_i];
                            max = getStima(distsofar + matrix[node][j],
                                stones[st_index].massa,
                                stones[st_index].energia);
                            minIndex = j;   min = matrix[node][j];
                        }
                        
                    }
                    
                }
            }

        }


        // la prossima citta è quella piu breve
        #if (DEBUG_GETPATH==1)
        cout << min << " at index " << minIndex << endl;
        #endif 

        path.push_back(minIndex);
        distance.push_back(min);
        visited[minIndex] = true;
        if(st_index > -1){ // se ho preso una pietra
            takenStones[st_index] = minIndex;
            capacitaLeft -= stones[st_index].massa;
            energia += stones[st_index].energia;
            carriedStones[minIndex] = stones[st_index].massa;
        }

        node = minIndex;
        if (minIndex == -1){
            break;
        }
        remaining--;
    }

    path.push_back(S);
    path.resize(N+1);
    //collectGems(path, distance);
    writeSolution(path, distance);
}



int main(){
    getInput();
    getBestPath();
    return 0;
}
