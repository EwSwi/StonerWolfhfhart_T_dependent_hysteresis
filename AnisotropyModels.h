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

}