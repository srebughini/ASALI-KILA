![logo](https://github.com/srebughini/ASALI-KILA/blob/master/images/Logo.png)

ASALI is an open-source software for solving gas thermodynamic and transport properties, catalytic reactors and chemical equilibrium calculations.

## 1. How to install ASALI-KILA
ASALI-KILA is based on CANTERA open-source suite and on EIGEN libraries. CANTERA estimates the thermodynamic and transport properties based on the kinetic gas theory and NASA database. EIGEN libraries manage the complex numerical part of ASALI-KILA.

### 1.1 How to install CANTERA
CANTERA can be installed with the following commands:  
* <code>sudo apt-get install python-software-properties</code>
* <code>sudo apt-add-repository ppa:speth/cantera</code>
* <code>sudo aptitude update</code>
* <code>sudo aptitude install cantera-python cantera-python3 cantera-dev</code>

More detailed can be found at: http://www.cantera.org/docs/sphinx/html/install.html

### 1.2 How to install EIGEN libraries
EIGEN libraries can be downloaded from: http://eigen.tuxfamily.org

### 1.3 How to install ASALI-KILA
To install ASALI-KILA modify the make file as follow:
* change the environmental variable **CANTERA** with the path of you CANTERA installation.
  * E.g. <code>export CANTERA=$HOME/my/cantera/installation</code>
* change the environmental variable **EIGEN** with the path of your EIGEN libraries.
  * E.g. <code>export EIGEN=$HOME/my/eigen/libraries</code>

Once you have changed the environmental variables, run the following commands:

<code>sudo apt-get install libgtkmm-3.0-dev</code>

<code>./make.sh</code>

<code>sudo ln -s $HOME/your/asali/folder/Asali-Kila /usr/bin/Asali-Kila</code>

To run ASALI-KILA:

<code>Asali-Kila</code>
