#!/bin/bash
qsub -lncpus=1 -v ISLAND_SIZE=50,FUNC1=1,FUNC2=30 Scalable_EA.pbs

