# asd-endgame

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
