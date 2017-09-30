#include "canteraInterface.h"

namespace ASALI
{
    canteraInterface::canteraInterface(Cantera::ThermoPhase* thermo,
                                       Cantera::Transport*   transport):
    thermo_(thermo),
    transport_(transport)
    {
        T_       = 0.;
        p_       = 0.;
        
        NS_      = thermo->nSpecies();
        
        y_       = new double[NS_];
        x_       = new double[NS_];
        
        mole_.resize(NS_);
        mass_.resize(NS_);
        h_.resize(NS_ + 1);
        s_.resize(NS_ + 1);
        cp_.resize(NS_ + 1);
        MW_.resize(NS_ + 1);
        n_.resize(NS_ + 1);
        mu_.resize(NS_ + 1);
        cond_.resize(NS_ + 1);
        
        diff_.resize(NS_ + 1);
        for (unsigned int i=0;i<(NS_+1);i++)
        {
            diff_[i].resize(NS_ + 1);
        }
    }

    std::vector<int>  canteraInterface::checkNames(const std::vector<std::string> name)
    {
        std::vector<int> check(name.size());
        
        for (unsigned int i=0;i<name.size();i++)
        {
            check[i] = 1;
            for (unsigned int j=0;j<NS_;j++)
            {
                if ( name[i] == thermo_->speciesName(j) )
                {
                    check[i] = 0;
                    break;
                }
            }
        }

        return check;
    }
    
    
    void canteraInterface::setTemperature(const double T)
    {
        T_ = T;
    }
    
    void canteraInterface::setPressure(const double p)
    {
        p_ = p;
    }
    
    void canteraInterface::setMoleFraction(const std::vector<double> x,const std::vector<std::string> name)
    {
        for (unsigned int i=0;i<NS_;i++)
        {
            x_[i] = 0.;
            for (unsigned int j=0;j<name.size();j++)
            {
                if ( name[j] == thermo_->speciesName(i) )
                {
                    x_[i] = x[j];
                    break;
                }
            }
        }
        thermo_->setState_TPX(T_,p_,x_);
        thermo_->getMassFractions(y_);
        
        for(unsigned int i=0;i<NS_;i++)
        {
            mass_[i] = y_[i];
            mole_[i] = x_[i];
            n_[i]    = thermo_->speciesName(i);
            MW_[i]   = thermo_->molecularWeight(i);
        }

    }

    void canteraInterface::setMassFraction(const std::vector<double> y,const std::vector<std::string> name)
    {
        for (unsigned int i=0;i<NS_;i++)
        {
            y_[i] = 0.;
            for (unsigned int j=0;j<name.size();j++)
            {
                if ( name[j] == thermo_->speciesName(i) )
                {
                    y_[i] = y[j];
                    break;
                }
            }
        }

        thermo_->setState_TPY(T_,p_,y_);
        thermo_->getMoleFractions(x_);

        for(unsigned int i=0;i<NS_;i++)
        {
            mass_[i] = y_[i];
            mole_[i] = x_[i];
            n_[i]    = thermo_->speciesName(i);
            MW_[i]   = thermo_->molecularWeight(i);
        }
    }
    
    void canteraInterface::equilibriumCalculate(std::string type)
    {
        thermo_->equilibrate(type);
        thermo_->getMassFractions(y_);
        thermo_->getMoleFractions(x_);
        
        for(unsigned int i=0;i<NS_;i++)
        {
            mass_[i] = y_[i];
            mole_[i] = x_[i];
            n_[i]    = thermo_->speciesName(i);
        }
    }
    
    double canteraInterface::Temperature()
    {
        return thermo_->temperature();
    }
    
    
    void canteraInterface::thermoCalculate()
    {
        double h[NS_];
        double s[NS_];
        double cp[NS_];
        
        thermo_->getPartialMolarEnthalpies(h);
        thermo_->getPartialMolarEntropies(s);
        thermo_->getPartialMolarCp(cp);


        for (unsigned int i=0;i<NS_;i++)
        {
            n_[i]  = thermo_->speciesName(i);
            h_[i]  = h[i];
            s_[i]  = s[i];
            cp_[i] = cp[i];
            MW_[i] = thermo_->molecularWeight(i);
        }

        n_[NS_]  = "mix";
        h_[NS_]  = thermo_->enthalpy_mole();        //J/Kmol
        s_[NS_]  = thermo_->entropy_mole();        //J/Kmol/K
        cp_[NS_] = thermo_->cp_mole();             //J/Kmol/K
        MW_[NS_] = thermo_->meanMolecularWeight();
    }

    void canteraInterface::transportCalculate()
    {
        double mu[NS_];
        double diff[NS_*NS_];
        double diffM[NS_];
        
        transport_->getSpeciesViscosities(mu);
        transport_->getBinaryDiffCoeffs(NS_,diff);
        transport_->getMixDiffCoeffs(diffM);

        unsigned int counter = 0;
        for (unsigned int i=0;i<(NS_+1);i++)
        {
            if ( i == NS_ ) 
            {
                n_[i]     = "mix";
                mu_[i]    = transport_->viscosity();             //Pas
                cond_[i]  = transport_->thermalConductivity();   //W/m/K
                MW_[i]    = thermo_->meanMolecularWeight();
                for (unsigned int j=0;j<(NS_+1);j++)
                {
                    diff_[i][j] = -1.;
                }
            }
            else
            {
                n_[i]    = thermo_->speciesName(i);
                mu_[i]   = mu[i];
                MW_[i] = thermo_->molecularWeight(i);

                for (unsigned int j=0;j<(NS_+1);j++)
                {
                    if ( j == NS_ )
                    {
                        diff_[i][j] = diffM[i];
                    }
                    else
                    {
                        diff_[i][j] = diff[counter++];
                    }
                }
            }
        }
        
        
        {
            double X[NS_];
            for (unsigned int i=0;i<NS_;i++)
            {
                for (unsigned int j=0;j<NS_;j++)
                {
                    if (j==i)
                    {
                        X[j] = 1.;
                    }
                    else
                    {
                        X[j] = 0.;
                    }
                }

                thermo_->setState_TPY(T_,p_,X);
                cond_[i] = transport_->thermalConductivity();
            }
        }
    }


    canteraInterface::~canteraInterface()
    {
    }
}
