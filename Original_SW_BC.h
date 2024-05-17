#ifndef Original_SW_BC_H 
#define Original_SW_BC_H 
#include <vector>
#include <math.h>
#include <Tools.h>
#include <algorithm>
using namespace tools;
/// Stoner Wolfhart original model - neglegible magnetisation, still magnetic field dependant/////
class Stoner_Wolf_Fart_Original
{
    private:
    float *u0_;
   float *K_, *phi_, *theta_, *H_, *Ms_, function;
    public:
// the equation has a dimension of J/ m^3
Stoner_Wolf_Fart_Original(float Theta,float Phi, 
float AnisotropyConst, float H, 
float Ms, float u0)
{
 Phi = *phi_;
 AnisotropyConst = *K_;
 H = *H_;
 u0 = *u0_;
 Theta = *theta_;
 Ms = *Ms_;
}
void stoner_wolf_fart_deafult()
{

   function =pow(sin((*phi_)),2)*(*K_) - (*H_)*(*Ms_)*(*u0_)*cos((*theta_));
}
float get_me_SW_function()
{
    return function;
}
};

class Stoner_Wolf_Fart_OG_1st_Derivative
{
    
    private:
    std::vector<float> derivative;
    public:
    enum Type { k, h, ms, _phi};
    float *u0_ = new float;
    std::vector<float> *theta_, *K_, *H_, *Ms_, *phi_;
    std::vector<float> *vecPtr[4] {K_, H_, Ms_, phi_};
Stoner_Wolf_Fart_OG_1st_Derivative(const std::vector<float> theta, const std::vector<float> K, 
const std::vector<float> H, const std::vector<float> Ms, const std::vector<float> phi,
const float u0): theta_(new std::vector<float>(theta)), K_(new std::vector<float>(K)), H_(new std::vector<float>(H)), Ms_(new std::vector<float>(Ms)),
phi_(new std::vector<float> (phi))
{
        vecPtr[0] = K_;
        vecPtr[1] = H_;
        vecPtr[2] = Ms_;
        vecPtr[3] = phi_;
       // u0 = *u0_;
}

void calculate_me_1st_derivative()
{
    derivative.clear();
   // derivative.resize(K_->size());
    for(int i=0; i<(*K_).size(); i++)
    {
    float derivativeFloat =(*K_)[i]*cos(2*(*phi_)[i]) - (*u0_ * (*Ms_)[i] * (*H_)[i] * sin((*theta_)[i]));
    derivative.push_back(derivativeFloat);
    }
}
std::vector<float> get_me_1st_derivative()
{
    return derivative;
}
};

class Stoner_Wolf_Fart_OG_2nd_Derivative
{
    private:
    std::vector<float> derivative2;
    public:
    float u0{2};
    std::vector<float> *theta_, *phi_, *H_, *Ms_, *K_;
Stoner_Wolf_Fart_OG_2nd_Derivative(std::vector<float> theta, std::vector<float> K, 
std::vector<float> H, std::vector<float> Ms, std::vector<float> phi,
float u0): theta_(new std::vector<float>(theta)), K_(new std::vector<float>(K)), H_(new std::vector<float>(H)), Ms_(new std::vector<float>(Ms)),
phi_(new std::vector<float> (phi))
{}
void calculate_me_2nd_derivative()
{
    for (int i = 0; i<(*K_).size(); i++)
    {
     derivative2[i] =-(*K_)[i]*sin(2*(*phi_)[i]) - u0 * (*Ms_)[i] * (*H_)[i] * cos((*theta_)[i]);   
    }
}

};
#endif
//////////////////////////////////// FINISH SW ORIGINAL MODEL DERIVATIVE + MAIN FUNCTION CALC //////////////////////////////////
