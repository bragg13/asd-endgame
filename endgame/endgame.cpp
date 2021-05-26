#include <iostream>
#include <vector>
#include <iomanip>      // per i decimali
#include <fstream>
using namespace std;

/**
 * Structures
 * (codice di powarts rn)
 */

struct stone {
    int energia, massa;
};


/**
 * Declarations
 */
void getInput();
void writeSolution();
int getVelocita();
void getInfos();

int N;              // numero di citta
int M;              // numero di pietre
int S;              // citta di partenza
int capacita;       // capacità dello zain ehm guanto
int R;              // energia consumata per unita di tempo
double vmax, vmin;

vector<stone> stones;     // lista (temp!) delle pietre raccolte
vector<vector<int>> stonesLocation;         // per ogni pietra mi segno (con un array) quali città ce l'hanno
int **matrix = new int*[N];                               // matrice di adiacenza

double energia;     // energia RACCOLTA
int tempoImpiegato; // tempo totale






/**
 * Implementations
 */
void getInput(){
    ifstream in("input0.txt");
    in >> N >> S;                               // prima riga
    in >> M >> capacita >> R >> vmin >> vmax;   // seconda riga

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
    stonesLocation = vector<vector<int>>(M);                // inizializzo l'array di locations
    int listlen;
    int tmpCity;
    for(int i=0; i<2*M; i++){ 
        in >> listlen;
        for (int j=0; j<listlen; j++){
            // la pietra `i` è presente nella città in[j]
            in >> tmpCity;
            stonesLocation[i].push_back(tmpCity);
        }
    }

    // distanze tra le citta
    

    for(int i=1; i<N; i++){
        matrix[i] = new int[N];
        
        for(int j=0; j<i; j++){
            int tmp;
            in >> tmp;
            // matrix[i][j] = tmp;
            // matrix[j][i] = tmp;
        }

    }
    
    in.close();
}

void writeSolution(){
    ofstream out("output.txt");
    double energiaFinale = energia - R*tempoImpiegato;
    out << "stuff" << endl;
    out.close();
}

int getVelocita(int carriedStones){
    return vmax-(carriedStones*((vmax-vmin)/capacita));
}

void getInfos(){
    int N;              // numero di citta
int M;              // numero di pietre
int S;              // citta di partenza
int capacita;       // capacità dello zain ehm guanto
int R;              // energia consumata per unita di tempo
double vmax, vmin;

vector<stone> stones = vector<stone>();     // lista (temp!) delle pietre raccolte
vector<vector<int>> stonesLocation;         // per ogni pietra mi segno (con un array) quali città ce l'hanno
int **matrix;                               // matrice di adiacenza

double energia;     // energia RACCOLTA
int tempoImpiegato; // tempo totale

    cout << "Numero di città: " << endl;
    cout << "Città di partenza: " << endl;
    
    cout << "Numero di pietre: " << endl;
    cout << "Capacita' del guanto: " << endl;
    
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
        for(int j=i; j<N; j++){
            cout << "  - " << j << ": " << matrix[i][j] << endl;
        }
    }

    cout << endl;
}


int main(){
    getInput();
    // getInfos();
    return 0;
}