# asd-endgame - italiano

English version below.

## Obiettivo

1. Recuperare le pietre
  - M pietre
  - peso diverso
  - energia diversa

2. Usare il guanto
  - consuma R energia per unità di tempo

3. Massimizzare l'energia raccolta
   $E(p,t) = G(P) - R\cdot T(p,t)$


## Com'è fatto il grafo

- Ci sono N nodi
  - ogni nodo ha 0/+ pietre
  - ogni nodo è connesso ad ogni altro da un arco
  - ogni arco ha un costo


## Constrains

- bisogna visitare ogni nodo, al massimo una sola volta
- si puo prendere una sola pietra per nodo
- la pietra nella citta di partenza puo essere presa solo alla partenza
- c'è un limite al peso che posso portare 
- mi muovo con velocità V
  - parte da Vmax
  - arriva a Vmin
  - proporzionale al peso trasportato
  - $v = v_{max}-W\cdot \frac{v_{max} - v_{min}}{V}$
   > **C**: capacità del guanto
   > **W**: peso trasportato


## Input

1. N (*numero città*) S (*città di partenza*)
2. M (*numero di pietre*) C (*capacità guanto*)  R (*consumo per unità di tempo*) $V_{max}$ $V_{min}$
 #M: m (*massa*) e (*energia*)
 #2M: disponibilità pietre nelle città: ogni indice è una pietra
    - lunghezza LA della lista di disponibilita
    - #LA interi, ossia gli ID delle citta in cui questa pietra è presente
 #N-1: distanze tra citta
    - ogni riga (i) mi da il peso degli archi collegati a tutti i nodi precedenti


## Output
 
Quattro righe.
1. E (*energia finale*) G (*energia raccolta*) T (*tempo impiegato*) [double]
2. quali pietre son state raccolte in quale città, avendo come indici della lista il numer odella pietra e come valore il numero della citta (o -1)
3. percorso seguito aka lista citta visitate (S ..... S)
4. tre asterischi * * * 

 
## Misc

Compilazione: 
```c
/usr/bin/g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o endgame grader.cpp endgame.cpp
```


# asd-endgame - english

## Objective

1. Collect the stones
 - M stones
 - different weight
 - different energy

2. Use the glove
 - consumes R energy per unit of time

3. Maximize the energy collected
 $E(p,t) = G(P) - R\cdot T(p,t)$


## How the graph is made

- There are N nodes
 - each node has 0/+ stones
 - each node is connected to each other by an arc
 - each bow has a cost


## Constrains

- you need to visit each node, at most only once
- you can only take one stone per node
- the stone in the starting city can only be taken at the start
- there is a limit to the weight I can carry
- I move with speed V
 - starts from Vmax
 - arrives at Vmin
 - proportional to the weight transported
 - $v = v_{max}-W\cdot \frac{v_{max} - v_{min}}{V}$
 > **C**: glove capacity
 > **W**: transported weight


## Input

1. N (*city number*) S (*departure city*)
2. M (*number of stones*) C (*glove capacity*) R (*consumption per unit of time*) $V_{max}$ $V_{min}$
 #M: m (*mass*) and (*energy*)
 #2M: availability of stones in cities: each index is a stone
 - length LA of the availability list
 - #LA integers, i.e. the IDs of the cities in which this stone is present
 #N-1: distances between cities
 - each row (i) gives me the weight of the arcs connected to all the previous nodes


## Outputs

Four lines.
1. E (*final energy*) G (*harvested energy*) T (*time taken*) [double]
2. which stones were collected in which city, having the stone number as indexes of the list and the city number as value (or -1)
3. route followed aka list of cities visited (S ..... S)
4. three asterisks * * *


## Misc

Compilation:
```c
/usr/bin/g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o endgame grader.cpp endgame.cpp
```
