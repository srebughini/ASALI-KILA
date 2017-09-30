#ifndef CANTERAINTERFACE_H
#define CANTERAINTERFACE_H

#include "cantera/thermo.h"
#include "cantera/transport.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <limits>

namespace ASALI
{
    class canteraInterface
    {
        public:
            canteraInterface(Cantera::ThermoPhase* thermo,
                             Cantera::Transport*   transport);

            void setTemperature(const double T);
            void setPressure(const double T);
            void setMoleFraction(const std::vector<double> x,const std::vector<std::string> name);
            void setMassFraction(const std::vector<double> y,const std::vector<std::string> name);
            void thermoCalculate();
            void transportCalculate();
            void equilibriumCalculate(std::string type);
            
            double Temperature();
            
            std::vector<int>  checkNames(const std::vector<std::string> name);
            
            
            inline std::vector<double> h()                      {return h_;};
            inline std::vector<double> s()                      {return s_;};
            inline std::vector<double> cp()                     {return cp_;};
            inline std::vector<double> MW()                     {return MW_;};
            inline std::vector<double> mu()                     {return mu_;};
            inline std::vector<double> cond()                   {return cond_;};
            inline std::vector<double> moleFractions()          {return mole_;};
            inline std::vector<double> massFractions()          {return mass_;};
            inline std::vector<std::vector<double> > diff()     {return diff_;};
            
            inline std::vector<std::string> names()             {return n_;};
            
            inline unsigned int nSpecies()                      {return NS_;};
            
            virtual ~canteraInterface();
            
        private:
        
            Cantera::ThermoPhase* thermo_;
            Cantera::Transport*   transport_;

            double                 T_;
            double                 p_;
            double                *x_;
            double                *y_;

            unsigned int          NS_;
            
            
            std::vector<double>   h_;
            std::vector<double>   s_;
            std::vector<double>   cp_;
            std::vector<double>   MW_;
            std::vector<double>   mu_;
            std::vector<double>   cond_;
            std::vector<double>   mole_;
            std::vector<double>   mass_;
            
            std::vector<std::vector<double> > diff_;
            
            std::vector<std::string> n_;

    };
}

#endif
