#!/bin/bash
cd ..
bash bash_NCI.sh
cd ./bin

qsub ./Scalable_EA.pbs
qstat -u cxj595


