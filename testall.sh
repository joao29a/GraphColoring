#!/bin/bash

time for i in {1..30}; do echo gcol$i; time ./out input/gcol$i.txt; echo; done
