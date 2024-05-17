#ifndef Kirschner_SW_BC_h_
#define Kirschner_SW_BC_h_
#include <vector>
#include <math.h>
#include <Tools.h>
#include <utility>
using namespace tools;
// Stoner Wolfhart boundary conditions with more demag field oriented Kirschner's model - magnetic field independent
class Stoner_Wolf_Fart_Kirschner
{
    private:
    float u0_;
    std::vector<float> K1_, phi_, K2_, Ms_, function;
    public:
// the equation has a dimension of J/ m^3
Stoner_Wolf_Fart_Kirschner(std::vector<float> K1, std::vector<float> phi, 
std::vector<float> K2, std::vector<float> Ms, float u0)
    : 
    K1_(std::move(K1)), phi_(std::move(phi)), K2_(std::move(K2)), 
    Ms_(std::move(Ms)), u0_(u0)
{}
void stoner_wolf_fart_kirschner_deafult()
{
function.resize(K1_.size());
for(int i= 0; i< K1_.size(); i++)
{
   function[i] = pow(sin((phi_)[i]),2)*(K1_)[i] 
   + pow(sin((phi_)[i]), 4)*((K2_)[i]) 
   - ((Ms_)[i]*(Ms_)[i]*(u0_))/2 
   *pow(cos((phi_)[i]), 2);
}
}
std::vector<float> get_me_SW_function()
{
    return function;
}
};

class Stoner_Wolf_Fart_Kirschner_1st_Derivative
{
    private:
    std::vector<float> derivative;
    public:
    enum type {k1, k2, c, phi};
    std::vector<float> *K1_, *K2_, *C_, *phi_;
    std::vector<float> *vecPtr[4] {K1_, K2_, C_, phi_};
Stoner_Wolf_Fart_Kirschner_1st_Derivative(std::vector<float> K1, 
std::vector<float> K2, std::vector<float>C, std::vector<float> phi)
:
K1_(new std::vector<float>(K1)), K2_(new std::vector<float>(K2)),
C_(new std::vector<float>(C)), phi_(new std::vector<float>(phi))

{}

void calculate_me_1st_derivative()
{
    derivative.clear();
    derivative.resize(K1_->size());
        for(int i=0; i<(*K1_).size(); i++)
    {
    derivative[i] =cos(2*(*phi_)[i])*
    ((2*pow(sin((*phi_)[i]),2))*
    (*K2_)[i] + (*K1_)[i] + (*C_)[i]);
    }
}
std::vector<float> get_me_1st_derivative()
{
    return derivative;
}
};

class Stoner_Wolf_Fart_Kirschner_2nd_Derivative
{
    private:
    std::vector<float> derivative2;
    public:
    std::vector<float> *K1, *K2, *C, *phi;
Stoner_Wolf_Fart_Kirschner_2nd_Derivative(Stoner_Wolf_Fart_Kirschner_1st_Derivative *SWKfirst)
{
    SWKfirst->K1_= K1;
    SWKfirst->K2_ = K2;
    SWKfirst->C_ = C;
    SWKfirst->phi_ = phi;
}
void calculate_me_2nd_derivative()
{
    for (int i = 0; i<(*K1).size(); i++)
    {
     derivative2[i] = -2*sin((2*(*phi)[i]))*
     ((*K1)[i]+(*K2)[i]*(sin((*phi)[i]))+(*C)[i])
     ;   
    }
}

};
#endif
