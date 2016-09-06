#!/bin/bash

cd ~/streamGraphSample/testprocess/testSNS

for para in 2 4 8 16
do

./netstat -d:F -p:c -i:../../testdata/rmat-15-$para -o:$para-origin
./netstat -d:F -p:h -i:../../testdata/rmat-15-$para -o:$para-origin
./netstat -d:F -p:C -i:../../testdata/rmat-15-$para -o:$para-origin

for count in 1 2 3 4 5
do

cd ~/streamGraphSample

./streamGraphSample sns testdata/rmat-15-$para 10000 testprocess/testSNS/sample-$para-$count

cd testprocess/testSNS

./netstat -d:F -p:c -i:sample-$para-$count -o:$para-$count
./netstat -d:F -p:h -i:sample-$para-$count -o:$para-$count
./netstat -d:F -p:C -i:sample-$para-$count -o:$para-$count

done 

done

rm *.plt

sed -i 's/$/\r/' *.tab
sed -i '1,4 d' *.tab
