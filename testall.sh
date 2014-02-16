#!/bin/bash

time for i in $(find ./input/ -type f); do echo $i; time ./out $i; echo; done
