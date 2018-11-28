#!/bin/bash
# Basic while loop
counter=1
while [ $counter -le 100 ]
    do
        ./raceA
        echo "--------------------"
        sleep 2 
        ((counter++))
        

    done
echo All done