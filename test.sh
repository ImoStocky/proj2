#!/bin/bash

echo $1

HES=('-1' '0' '20' '101')
HE=${#HES[@]}

SWITCHES=(' ' '--help' '--tan' '-m' '-c')
CMDS=${#SWITCHES[@]}

RADS=(' '  '0.1' '1.4' '-1.6' 'x')
RELEMENTS=${#RADS[@]}

ITERS=(' ' '-1' '13' '14' 'x x')
IELEMENTS=${#ITERS[@]}               

for arg in `seq 0 $[$CMDS - 1]`; do
    varx=${SWITCHES[${arg}]}
    case $arg in
        0)
            echo $arg'-------------------'
            echo `$1 $varx`
            echo $arg'-------------------'
        ;;
        
        1)  
            echo $arg'-------------------'
            echo `$1 $varx`
            echo `$1 $varx "stupp"`
            echo $arg'-------------------'
        ;;
        2)
            echo $arg'-------------------'
            for ((x=0;x<$RELEMENTS;x++)) ; do
                for ((y=0;y<$IELEMENTS;y++)) ; do
                    for ((z=0;z<$IELEMENTS;z++)) ; do
                        
                       echo $1 $varx ${RADS[${x}]} ${ITERS[${y}]} ${ITERS[${z}]}
                       echo `$1 $varx ${RADS[${x}]} ${ITERS[${y}]} ${ITERS[${z}]}`
                    done
                done
            done  
            echo $arg'-------------------'
        ;;
        3) 
            echo $arg'-------------------'
            for ((x=0;x<$RELEMENTS;x++)) ; do
                    for ((z=0;z<$IELEMENTS;z++)) ; do
                        
                       echo  $1 $varx ${RADS[${x}]} ${RADS[${z}]}
                       echo `$1 $varx ${RADS[${x}]} ${RADS[${z}]}`
                    done
                done
            echo $arg'-------------------'
        ;;
        4) 
            echo $arg'-------------------'
            for ((y=0;y<$HE;y++)); do 
                for ((x=0;x<$RELEMENTS;x++)) ; do
                    for ((z=0;z<$IELEMENTS;z++)) ; do
                        
                       echo  $1 $varx ${HES[${y}]} "-m"  ${RADS[${x}]} ${RADS[${z}]}
                       echo `$1 $varx ${HES[${y}]} "-m" ${RADS[${x}]} ${RADS[${z}]}`
                    done
                done
            done
            echo $arg'-------------------'
        ;;
    
    esac    
            
done;
    
exit
