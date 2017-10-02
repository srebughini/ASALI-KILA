rm -f Asali-*

export CANTERA=$HOME/my/cantera/installation

export EIGEN=$HOME/my/eigen/libraries

g++ -std=c++11 -w src/main.cpp src/odeSolver.C src/IdealReactors.C src/Kila.C src/canteraInterface.C src/speciesPopup.C -Isrc/ -I$EIGEN -I$CANTERA/include -L$CANTERA/lib `pkg-config gtkmm-3.0 --cflags --libs` -Wl,--start-group -lcantera -lcantera -lblas -llapack -lcantera_fortran -Wl,--end-group -lgfortran -Wl,--no-as-needed -ldl -lpthread -lm -o Asali-Kila
