echo Compiling SGE client and tools...
echo --- Compiling Type Maker
cd TypeMaker
make
echo --- Compiling Move Maker
cd ../MoveMaker
make
echo --- Compiling Map Maker
cd ../MapMaker
make
echo --- Compiling Client
cd ../Client
mkdir out
make
echo
echo Done!

