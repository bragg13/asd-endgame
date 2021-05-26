using namespace std;

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <tuple>

struct pietra{
    int m;  //massa
    int e;  //energia
};

struct citta{
    vector<pietra> p;   //pietre presenti
    vector<int> d;      //distanze
};

//valori di input
int N, S, M, C;
double R, Vmin, Vmax;
vector<pietra> pietre;  //tipologia delle pietre
vector<citta> graph;    //lista delle città

//importa l'input da un file "input.txt"
void getInput(){
    fstream f;
    f.open("input.txt");
    
    //prime 2 righe
    f >> N; f >> S;
    f >> M; f >> C; f >> R; f >> Vmin; f >> Vmax;

    for(int i = 0;i < N;++i){
        citta c;
        graph.push_back(c);
    }
    
    //memorizza le tipologie di pietre
    int m, e;

    for(int i = 0;i < M;++i){
        f >> m; f >> e;
        pietra p; p.m = m; p.e = e;
        pietre.push_back(p);
    }

    //memorizza le pietre presenti in ogni specifica città
    int n, c;

    for(int i = 0;i < M;++i){
        f >> n;
        for(int j = 0;j < n;++j){
            f >> c;
            graph[c].p.push_back(pietre[i]);
        }
    }

    //memorizza le distanze delle città
    int d;
    graph[0].d.push_back(0);
    for(int i = 1;i < N;i++){
        for(int j = 0;j < i;j++){
            f >> d;
            graph[i].d.push_back(d);
            graph[j].d.push_back(d);
        }
        graph[i].d.push_back(0)
    }

}

int main(){
    getInput();
}
