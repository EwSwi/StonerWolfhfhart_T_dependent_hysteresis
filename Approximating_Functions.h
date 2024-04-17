#ifndef Approximating_Functions_H
#define Approximating_Functions_H
#include <math.h>
#include <vector>
#include <Tools.h>
namespace Approximations
{
    // this is a simple Bloch's model for bulk ferromagnets, 
    //sufficient for the models needs. Saturation magnetisation approximation
class ApproximateMagnetisationSaturation 
{
private:
std::vector<float> Ms, *T;
float TC, exponenta, Ms0;
public:
ApproximateMagnetisationSaturation(std::vector<float> Temp, float Ms0_, float TC_, float exponenta_):
Ms0(Ms0_), TC(TC_), exponenta(exponenta_)
    {
     T = new std::vector<float>;
    *T = Temp; 
    }

void make_me_a_Ms_approximation()
{
       Ms.clear(); 
       Ms.resize(T->size());
for(int i=0; i<(*T).size(); i++)
    {
    Ms[i] = Ms0*(1-pow(((*T)[i]/TC), exponenta));
    }
}

std::vector<float> return_me_a_Ms_approximation()
{
    return Ms;
}
float just_give_me_a_M0(){return Ms0;}
};
}
#endif