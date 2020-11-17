#!/bin/bash

#input ./ws.sh 08 06 2000 Sun 13 12 2001

Function_1() {
    birth_date=$(date -d "$3$2$1" +"%a")
    if [[ $birth_date == $4 ]]
    then
        echo "True"
    else
        echo "False"
    fi
}

Function_2() {
    year=$(date -d "$(date)" +"%Y")
    birth_date=$(date -d "$year$2$1" +"%j")
    now=$(date -d "$(date)" +"%j")

    diff=$(( now - birth_date ))
    if [[ $diff -lt 0 ]]
    then
        qDiff=$(( 0 - diff ))
    else
        qDiff=$diff
    fi

    if [[ $qDiff -lt 182 ]]
    then
        echo $qDiff days
    else
        echo $(( 365 - qDiff )) days
    fi
}

Function_3() {
    birth_date=$(date -d "$3$2$1" +"%s")
    inputDate=$(date -d "$6$5$4" +"%s")

    diff=$(( (inputDate - birth_date) / 86400 ))
    if [[ $diff -lt 0 ]]
    then
        qDiff=$(( 0 - diff ))
    else
        qDiff=$diff
    fi

    echo $qDiff
}

Function_1 "$1" "$2" "$3" "$4"
Function_2 "$1" "$2"
Function_3 "$1" "$2" "$3" "$5" "$6" "$7"
