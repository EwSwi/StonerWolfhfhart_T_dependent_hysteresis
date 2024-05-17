#include <vector>
#include <math.h>
#include "Tools.h"
#include <Approximating_Functions.h>
namespace anisotropyModels
{

class AnisotropyModels // modelling anisotropy constants, temperature dependent for now only
{
    private:
    std::vector<float> *K1, *K2, *d, *T, Kd, *Ms;
    std::vector<float> *KT = new std::vector<float>;
    float K0, Ms0, exponenta;
    public:
AnisotropyModels(float Ms0_, std::vector<float> K1_, std::vector<float> K2_,
std::vector<float> T_, std::vector<float> d_, std::vector<float> Ms_, 
float K0_, float exponenta_)
:
K1(new std::vector<float>(K1_)), K2(new std::vector<float>(K2_)), 
d(new std::vector<float>(d_)), T(new std::vector<float>(T_)), 
Ms0(Ms0_), Ms(new std::vector<float> (Ms_)) 
{
    K0 =K0_;
    exponenta = exponenta_;    // copy floats 
}

void temperature_anisotropy()
{
    KT->clear();
    for(int i=0; i<Ms->size(); i++)
    {
        float KT_ = pow(((*Ms)[i]/(Ms0)), exponenta)*K0;
       KT->push_back(KT_);
    }
}
void thickness_anisotropy()
{
  // empty for further implementation of thickness dependent model   
}
std::vector<float> return_KT()
{
    return *KT;
}

};

class MagnetisationCalc
{
private:
std::vector<double> phi_, H_;
std::vector<std::pair<double,double>> CosHPair;
public:
MagnetisationCalc(
const std::vector<double> phi,
const std::vector<double> H
)
:
phi_(phi),
H_(H){}


void calc_me_magnetisation_scalar()
{
    CosHPair.clear();
    for (size_t i=0; i <phi_.size(); i++)
    { 
    double cosine = cos(phi_[i]);
    std::cout<< "cosine: " <<cosine << " h: " << H_[i] << std::endl;
    std::pair<double, double> tempPair= std::make_pair(cosine, phi_[i]);
    CosHPair.push_back(tempPair);

    }
}
std::vector<std::pair<double, double>> return_cosine(){return CosHPair;}

};

class MagnetisationCalcButNotMD
{
private:
std::vector<double> phi_, cosine;
public:
MagnetisationCalcButNotMD(
const std::vector<double> phi)
:
phi_(phi){}


void calc_me_magnetisation_scalar()
{
    cosine.clear();
    for (size_t i=0; i <phi_.size(); i++)
    { 
    double cosin = cos(phi_[i]);
    cosine.push_back(cosin);

    }
}
std::vector<double> return_cosine(){return cosine;}

};

//linear KT1 dependency, more important is the latter KT2/KT1 anisotropy flow, so this might be sufficient as an approximation, we will see
class LinearTK1
{
    private:
    std::vector<float> copyT, KT1approx;
    public:
LinearTK1(const std::vector<float> T)
:
copyT(T)
{}
void calc_linear_KT_dependency(float a, float b)
{
    KT1approx.clear();
    for(size_t i=0; i<copyT.size(); i++)
    {
        float KTtemp = (copyT[i] - b)/a; // just a linear function with x = T, y = anisotropy constant, a being the ax term, b being the linear function shift
        KT1approx.push_back(KTtemp);
    }
}
std::vector<float> get_linear_KT()
{
    return KT1approx;
}
};

// addedd polynomial model for KT2, as previous did not work well enough
class PolynomialModelKT2
{
private:
std::vector<float> copyKT1;
std::vector<float> KT2;
public:
PolynomialModelKT2(const std::vector<float> KT1)
:
copyKT1(KT1)
{}
// SIX polynomial coefficients are needed so buckle up
void calcPolynomialKT2(double x6, double x5, double x4, double x3, double x2, double x, double theConstant)
{
    for(size_t i=0; i<copyKT1.size(); i++)
    {
    double tempKT2 = x6*pow(copyKT1[i], 6) + x5*pow(copyKT1[i],5) 
    // space for visibility
    + x4*pow(copyKT1[i],4) + x3*pow(copyKT1[i],3)
    // crying rn
    + x2*pow(copyKT1[i], 2) + x*copyKT1[i] + theConstant;
    KT2.push_back(tempKT2);
    }
}
std::vector<float> get_me_KT2()
{
    return KT2;
}

};

}