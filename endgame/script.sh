#!/bin/bash

# Compila
/usr/bin/g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o endgame grader.cpp endgame.cpp 

# Cicla
i=0
while [ $i -lt 20 ];
do
	( cp ./input/input$i.txt ./input.txt ) && ( ./endgame ) && echo "$i: " && ( sed -n 1p output.txt ) && $(( i++ )) && echo -n "";
done

