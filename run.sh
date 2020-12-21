#!/bin/bash
# Arg1 is a comma separated list of benchmarks, Arg2 is a comma separated list of classes, Arg3 is the runtimes to build on
algorithms=${1:-ParallelPVS,ParallelAlphaBeta,Jamboree,SerialPVS,AlphaBeta}
depths=${2:-12,13,14}
work=100
runs=50000
for ((i = 0; i < 5; i++))
do
for ((depth = 8; depth <=12; depth += 1))
do
for algorithm in $(echo $algorithms | sed "s/,/ /g")
do
    for ((c = 1; c <= 32; c*=2)) ;
    do
    	echo Running $algorithm with $c cores and depth $depth >&2
    	execution_output=$(taskset -c 0-$((c - 1)) ./othello -b $algorithm -w Random -d $depth)
        v=$(echo "$execution_output" | grep -E 'Black played by [^0-9]* [0-9]+' | grep -Eo '[0-9]+ microseconds' | grep -Eo '[0-9]+' )
		echo Finished running $algorithm with $c cores >&2
    	echo $algorithm , white , $c , $depth , $v
    	echo Running $algorithm with $c cores >&2
    	execution_output=$(taskset -c 0-$((c - 1)) ./othello -w $algorithm -b Random -d $depth)
        v=$(echo "$execution_output" | grep -E 'White played by [^0-9]* [0-9]+' | grep -Eo '[0-9]+ microseconds' | grep -Eo '[0-9]+' )
		echo Finished running $algorithm with $c cores and depth $depth >&2
    	echo $algorithm , black , $c , $depth , $v
    	if [[ "$algorithm" == "SerialPVS" ]]
    	then
    		break
	    fi
	    if [[ "$algorithm" == "AlphaBeta" ]]
    	then
    		break
	    fi
	done
done
done
done