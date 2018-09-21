#!/bin/bash

LOOPS=${1:-10000}

echo "Compiling in release mode..."
make release > /dev/null
make test-release > /dev/null

echo "Running test with $LOOPS iterations"
time for file in build/test/*-release; do 
	./$file --gtest_repeat=$LOOPS > /dev/null; 
done

