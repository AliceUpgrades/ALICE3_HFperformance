rm *.root *.exe
cp ../compile_pythia_aliceml.sh .
source /home/pyadmin/setup-pythia8.sh
./compile_pythia_aliceml.sh examplehadron

rm -rf outputtest
mkdir outputtest

NJOBS=50 #WARNING: BE AWARE THAT THE FILES PRODUCED BY EACH JOB WILL HAVE 
	 #1/NJOBS AS NORMALIZATION, TO PRODUCE MERGED FILES PROPERLY NORMALIZED.
cd outputtest

for i in {1..50} #FIXME has to be manually set to NJOBS for the moment
do
   mkdir file_$i
   cd file_$i
   cp ../../examplehadron.exe .
   ./examplehadron.exe  $RANDOM $NJOBS &
   cd ..
done

hadd outputtest/mergedresult.root outputtest/file_*/*.root
