#include <iostream>
#include <vector>
#include <iomanip>          // per i decimali
#include <fstream>
#include <algorithm>

using namespace std;

#define DEBUG_GETPATH 0     // debug - funzione getPath
#define DEBUG_COLLECT 0     // debug - funzione collectGems
#define DEBUG_WRSOLUT 0     // debug - funzione writeSolution

struct citta {
    int id;                 // city ID
    vector<int> s;          // ?
    double max_goodness;    // max goodness in this city
};

struct stone {
    int energia, massa;
    double goodness;        // mi dice quanto è buona una pietra
};

int N;                      // numero di citta
int M;                      // numero di pietre
int S;                      // citta di partenza
int capacita;               // capacità dello zain ehm guanto
double energia;             // energia raccolta (G)
double R;                   // energia consumata per unita di tempo
double vmax, vmin;

double kz = 0.95;           // coefficiente di somiglianza per Zeta
double k_edge = 1;          // ?
double avg_goodness, avg_mass;

int useZaino = 1500;        // ?
int n_stones;               // ?
int maxStones = 6000000;    // ?
int sub = 100;              // ?

vector<int> carriedStones;  // ogni indice è una citta e mi dice qual è il peso locale trasportato
vector<stone> stones;       // lista delle pietre raccolte
vector<int> takenStones;    // di base -1, se raccolgo una pietra metto in che citta l'ho raccolta
vector<citta> cities;       // per ogni citta mi segno (con un array) quali pietre ha (gli ID delle pietre)
int **matrix;               // matrice di adiacenza
int *zaino;                 // ?


/**
 * 
 */
double getVelocita(int _carriedStones){
    return vmax-((_carriedStones*(vmax-vmin))/capacita);
}


/**
 * 
 */
bool goodStone(stone s, int d, int capacita_left){
    //return s.goodness >= kg*avg_goodness;
    return (R*d/getVelocita(s.massa + capacita - capacita_left) - s.energia) < (R*d/getVelocita(capacita - capacita_left));
}


/**
 * 
 */
bool betterStone(const tuple<stone, int> &a, const tuple<stone, int> &b){
    return get<0>(a).goodness > get<0>(b).goodness;
}


/**
 * 
 */
bool similarZeta(double zeta, double zeta_max){
    return (zeta_max * kz) < zeta;
}


/**
 * Legge il file di input
 */
void getInput(){
    ifstream in("input.txt");
    in >> N >> S;
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
        stone s;                        // creo la pietra
        in >> m >> e;
        s.energia = e;
        s.massa = m;
        sum_mass += m;
        s.goodness =  ((double) e)/m;   // calcolo la goodness
        sum_goodness += s.goodness;
        stones.push_back(s);            // aggiungo la pietra
    }
    avg_mass = sum_mass / M;            // calcolo la massa media
    avg_goodness = sum_goodness / M;    // calcolo la goodness media

    // inizializzo gli array
    cities = vector<citta>();
    carriedStones = vector<int>();
    for(int i=0; i<N; i++){
        citta c;
        c.max_goodness = 0;
        cities.push_back(c);
        carriedStones.push_back(0);
    }

    // popolo gli array - "dove sono le pietre"
    int listlen;
    int tmpCity;
    n_stones = 0;
    for(int i=0; i<M; i++){ 
        in >> listlen;
        n_stones += listlen;
        for (int j=0; j<listlen; j++){                              // la pietra `i` è presente nella città tmpCity
            in >> tmpCity;
            cities[tmpCity].s.push_back(i);
            if(cities[tmpCity].max_goodness < stones[i].goodness)   // ?
                cities[tmpCity].max_goodness = stones[i].goodness;
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

    // inizializzo l'array che contiene la citta in cui prendo ogni pietra
    takenStones = vector<int>(M);
    for(int i=0; i<M; i++){
        takenStones[i] = -1;
    }

    in.close();
}


/**
 * Funzione di debug per stampare le informazioni del dataset
 */
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

    // distanze tra citta
    for(int i=0; i<N; i++){
        cout << "[Citta] " << i << ":\n"; 
        for(int j=0; j<N; j++){
            cout << "  " << i << " to " << j << ": " << matrix[i][j] << endl;
        }
    }

    // la matrice di adiacenza effettiva
    cout << "==========" << endl;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cout << matrix[i][j];
        }
        cout << endl;
    }
    cout << "==========" << endl;
}


/**
 * Scrive in output.txt la soluzione, calcolando
 * prima tempo ed energia
 */
void writeSolution(vector<int> &path, vector<int> &distance){
    ofstream out("output.txt");

    // calcolo il tempo ripercorrendo la path
    int pesosofar = 0;
    double tempoImpiegato=0.0;
    
    for(int i=0; i<N; i++){
        int citta = path[i];
        double v = capacita==0 ? vmax : getVelocita(pesosofar+carriedStones[citta]);    // caso limite: C=0
        double t = distance[i]/v;
        tempoImpiegato += t;
        pesosofar += carriedStones[citta];

        #if (DEBUG_WRSOLUT==1)
        cout << "Citta: " << path[i] << " Peso: " << pesosofar << " Velocita:" << v << " Tempo:" << t << endl;
        #endif
    }

    // calcolo l'energia finale
    #if (DEBUG_WRSOLUT==1)
    cout << "energia " << energia << endl;
    cout << "tempoImpiegato " << tempoImpiegato << endl;
    #endif

    double energiaFinale = energia - (R*tempoImpiegato);
    out << scientific << setprecision(10) << energiaFinale << " "; 
    out << scientific << setprecision(10) << energia << " "; 
    out << scientific << setprecision(10) << tempoImpiegato << endl; 

    // scrivo la lista pietre
    for(int i=0; i<M; i++){
        out << takenStones[i] << " ";
    }
    out << endl;

    // scrivo la lista citta
    for(int i=0; i<path.size(); i++){
        out << path[i] << " ";
    }
    out << endl;

    out << "***";

    out.close();
}



/**
 * ?
 */
void greedyZaino(vector<int> &path, vector<int> & distance){
    int *zaino = new int[M];
    vector< tuple<stone, int> > s;
    for(int i = 0;i < stones.size();i++){
        s.push_back(make_tuple(stones[i], i));
        zaino[i] = 0;
    }

    sort(s.begin(), s.end(), betterStone);
    int cLeft = capacita;
    for(int i = 0;i < s.size();++i){
        if(cLeft >= get<0>(s[i]).massa){
            zaino[get<1>(s[i])] = 1;
            cLeft -=  get<0>(s[i]).massa;
        } else if(cLeft <= 0 - (cLeft/10)){
            break;
        }
    }

    
    int capacitaLeft = capacita;
    int distsofar = 0;
    int imax;
    double zetamax, maxsofar;

    for(int i = path.size() - 2;i >= 0; i--){
        maxsofar = 0;
        distsofar += distance[i];
        zetamax = -100;
        imax = 0;

        for(int j = 0;j < cities[path[i]].s.size();++j){
            int st_i = cities[path[i]].s[j];
            stone s = stones[cities[path[i]].s[j]];
            
            #if (DEBUG_COLLECT==1)
            cout << "Pietra " << cities[path[i]][j] << endl;
            #endif 

            // controllo se ho gia preso la pietra
            if(takenStones[cities[path[i]].s[j]] == -1 && zaino[st_i] == 1){
                // prendo il fattore zeta=e/pd
                
                double zeta = s.energia;  // th. devo moltiplicare per distsofar perche è la somma delle distanze da qui alla fine

                #if (DEBUG_COLLECT==1)
                cout << "ZETA=" << zeta << ", e=" << s.energia << ", m=" << s.massa << endl << endl;
                cout << "   zetamax=" << zetamax << " smassa" << s.massa << " capleft=" << capacitaLeft << endl;
                #endif

                if(zeta > zetamax && s.massa<=capacitaLeft){
                    zetamax = zeta;
                    imax = j;
                }
            } 
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
 * ?
 */
void getZaino(vector<int> &path, vector<int> & distance){
    //cout << "ok" << endl;
    int **DP;
    DP = new int*[M];
    zaino = new int[M];
    for(int i = 0;i < M;++i){
        DP[i] = new int[capacita];
        DP[i][0] = 0;
    }
    for(int c = 0;c < capacita;c++){
        DP[0][c] = 0;
    }
    //cout << "initialized..." << endl;
    for(int i = 1;i < M;++i){
        for(int c = 1;c < capacita;++c){
            //cout << "i: " << i << " c: " << c <<endl;
            if(stones[i].massa < c){
                int max = DP[i-1][c-stones[i].massa] + stones[i].energia;
                if(DP[i-1][c] > max){
                    max = DP[i-1][c];
                }

                DP[i][c] = max;
            } else {
                DP[i][c] = DP[i - 1][c];
            }
        }
    }
    //cout << "DP built" << endl;
    int c = capacita - 1;
    int i = M - 1;
    while(i >= 0 && c >= 0){
        //cout << "i: " << i << " c: " << c <<endl;
        if(i > 0){
            if(DP[i][c] == DP[i-1][c]){
                zaino[i] = 0;
            } else{
                zaino[i] = 1;
                c -= stones[i].massa;
            }
        } else if(stones[i].massa < c){
            zaino[i] = 1;
            c -= stones[i].massa;
        }
        --i;
    }
    //cout << "zaino built" << endl;
    int capacitaLeft = capacita;
    int distsofar = 0;
    int imax;
    double zetamax, maxsofar;

    for(int i = path.size() - 2;i >= 0; i--){
        maxsofar = 0;
        distsofar += distance[i];
        zetamax = -100;
        imax = 0;

        for(int j = 0;j < cities[path[i]].s.size();++j){
            int st_i = cities[path[i]].s[j];
            stone s = stones[cities[path[i]].s[j]];
            
            #if (DEBUG_COLLECT==1)
            cout << "Pietra " << cities[path[i]][j] << endl;
            #endif 

            // controllo se ho gia preso la pietra
            if(takenStones[cities[path[i]].s[j]] == -1 && zaino[st_i] == 1){
                
                double zeta = s.massa;

                #if (DEBUG_COLLECT==1)
                cout << "ZETA=" << zeta << ", e=" << s.energia << ", m=" << s.massa << endl << endl;
                cout << "   zetamax=" << zetamax << " smassa" << s.massa << " capleft=" << capacitaLeft << endl;
                #endif

                // vedo se aggiornare zetamax
                if(zeta > zetamax && s.massa<=capacitaLeft){
                    zetamax = zeta;
                    imax = j;
                }
            } 
        }


        // ho trovato la pietra che tendenzialmente ha il miglior rapporto
        if(zetamax > 0){
            // la prendo
            int stoneIndex = cities[path[i]].s[imax];

            capacitaLeft -= stones[stoneIndex].massa;
            
            // prendo la pietra i
            energia += stones[stoneIndex].energia;

            // ho raccolto tale pietra in imax citta'
            takenStones[stoneIndex] = path[i];     // NB. path[i] è la citta in cui sono

            // mi segno che peso porto in questa citta'
            carriedStones[path[i]] = stones[stoneIndex].massa;

            #if (DEBUG_COLLECT==1)
            cout << "Ho preso la pietra " << stoneIndex << " nella citta " << path[i] << endl;
            #endif
        }
    }

    writeSolution(path, distance);
}


/**
 * Ciclo all'indietro il path trovato e prendo le gemme a seconda della loro "ottimalita'"
 */
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

    for(int i=path.size()-2; i>=0; i--){    // -2 perche non parto dall'ultimo, che è la partenza aka S
        maxsofar = 0;
        distsofar += distance[i];           // i e basta perche len(distance) == len(path)-{Sin, Sfin} 
        zetamax = -100;
        imax = 0;

        #if (DEBUG_COLLECT==1)
        cout << "\n(citta " << path[i] << ") " << "size " << cities[path[i]].size() << endl;
        cout << "  dist=" << distance[i] << endl;
        #endif

        // ciclo su tutte le pietre disponibili in questa citta'
        for(int j=0; j<cities[path[i]].s.size(); j++){
            stone s = stones[cities[path[i]].s[j]];
            
            #if (DEBUG_COLLECT==1)
            cout << "Pietra " << cities[path[i]][j] << endl;
            #endif 

            // controllo se ho gia preso la pietra
            if(takenStones[cities[path[i]].s[j]] == -1 && goodStone(s, distsofar, capacitaLeft)){
                
                // calcolo il fattore zeta=e/pd
                double zeta = ((double) s.energia) / ( s.massa*distsofar );  // th. devo moltiplicare per distsofar perche è la somma delle distanze da qui alla fine

                #if (DEBUG_COLLECT==1)
                cout << "ZETA=" << zeta << ", e=" << s.energia << ", m=" << s.massa << endl << endl;
                cout << "   zetamax=" << zetamax << " smassa" << s.massa << " capleft=" << capacitaLeft << endl;
                #endif

                // vedo se aggiornare zetamax
                if(zeta > zetamax && s.massa<=capacitaLeft){
                    zetamax = zeta;
                    imax = j;
                } else if(similarZeta(zeta, zetamax) && s.massa<=capacitaLeft){     // in caso di pietre con zeta simile
                    if(stones[imax].massa < s.massa){                               // prendo la massa piu grande, perche tendenzialmente
                        imax = j;                                                   // sono alla fine
                        zetamax = zeta;
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
            takenStones[stoneIndex] = path[i];     // NB. path[i] è la citta in cui sono

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
 */
void getBestPath(){
    vector<int> path = vector<int>();
    vector<int> distance = vector<int>();
    bool visited[N];
    int remaining = N;
    double avg_d = 0;

    // init
    for(int i=0; i<N; i++){
        visited[i] = false;
    }

    path.push_back(S);
    visited[S] = true;
    vector<int> bestCities;

    for(int node=S; node<N;){            // cosi parto da 0 poi salgo, ma potrei perdermi qualcosa?
        // passo i nodi a fianco
        double dmin = 1000000;
        int min = 1000000; double e_ratio;
        int minIndex = -1;

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


            if (matrix[node][j] != 0 && !visited[j]){

                if(min >= matrix[node][j]){
                    e_ratio = ((double)min )/ matrix[node][j];
                    if(min * k_edge < matrix[node][j]){
                        if(cities[minIndex].max_goodness * e_ratio < cities[j].max_goodness ){
                            min = matrix[node][j];
                            minIndex = j;
                        }
                    } else {
                        min = matrix[node][j]; 
                        minIndex = j;
                    }
                } else if(min >= k_edge * matrix[node][j]){
                    e_ratio = ((double)matrix[node][j]) / min;
                    if(min > matrix[node][j]){
                        min = matrix[node][j];
                        minIndex = j;
                    } else {
                        if(cities[minIndex].max_goodness * e_ratio < cities[j].max_goodness){
                            min = matrix[node][j];
                            minIndex = j;
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
        avg_d += min;
        visited[minIndex] = true;

        node = minIndex;
        if (minIndex == -1){
            break;
        }
        remaining--;
    }
    avg_d /= N;

    path.push_back(S);
    path.resize(N+1);

    // ?
    if((R * avg_d) / vmin < useZaino && N*capacita > 0){
        getZaino(path, distance);
    } else if((R*avg_d)/vmin < useZaino ){
        greedyZaino(path, distance);
    } else
        collectGems(path, distance);
}



int main(){
    getInput();
    getBestPath();
    return 0;
}