#include <vector>
#include <math.h>
#include <utility>
#include <Approximating_Functions.h>
#include <AnisotropyModels.h>
#include <Original_SW_BC.h>
#include <Kirschner_SW_BC.h>
#include <Tools.h>
#include <iostream>

using namespace std;
using namespace tools;

class Operator
{
public:
    struct operatorFunctionParam
    {
        int n;
        int *n_ = &n;
        std::vector<float> *VEC;
        operatorFunctionParam() : n_(new int), VEC(new std::vector<float>) {}
    };

    std::vector<float> operatorFunction(operatorFunctionParam *param)
    {
        std::vector<float> result;
        int vecSize = static_cast<int>(param->VEC->size());
        int min = std::min(*param->n_, vecSize);

            if ((*param).n >= 0)
            {
                for (int j = 0; j < min; j++)
                {
                    result.push_back((*param->VEC)[j]);
                }
            }
            else
            {
                int n = (std::abs(*param->n_) - 1);
                for (int j = n; j < vecSize - n; j++)
                {
                    result.push_back((*param->VEC)[j]);
                }
            }
        return result;
    }

    std::vector<float> operator[](const operatorFunctionParam &param)
    {
        return operatorFunction(const_cast<operatorFunctionParam *>(&param));
    }
};


class IteratorSWFK
{
    private:
    std::vector<float> *KT1, *KT2, *C_, *Phi_, *T_;
    std::vector<float> function, IteratedDerivative;
    std::vector<std::pair<float, float>> MinimaPhiPair;
    public:
    IteratorSWFK(const std::vector<float> K1, const std::vector<float> K2, const std::vector<float> C, 
    const std::vector<float> Phi, const std::vector<float> T):
    KT1(new std::vector<float>(K1)), 
    KT2(new std::vector<float>(K2)), 
    C_(new std::vector<float>(C)), 
    Phi_(new std::vector<float>(Phi)),
    T_(new std::vector<float>(T))
    {}
      /////////////// end constructor //////////////////
      void IterateSWFK()
{
    function.clear();
    Operator::operatorFunctionParam param;
    param.n = 5;
    *param.VEC = function;
    IteratedDerivative.clear();
    IteratedDerivative.resize((KT1->size()));
    T_->resize(KT1->size());
    KT2->resize(KT1->size());
        for(size_t j = 0; j<(*KT1).size(); j++)
    {
        function.clear();
            for(size_t i=0; i<(*Phi_).size(); i++)
            {
                float functionFloat = pow(sin((*Phi_)[i]),2)*(*KT1)[j] 
                + pow(sin((*Phi_)[i]), 4)*((*KT2)[j]) 
                 - ((2.1))/2 
                *pow(cos((*Phi_)[i]), 2);
                function.push_back(functionFloat);

                 if (abs(function[i + param.n] - function[i]) < 0.0003 && abs(function[i] - function[i - param.n]) > 0.0003)
                {
                MinimaPhiPair.push_back(std::make_pair((*Phi_)[i], ((*T_)[j])));
                }
            }
    }
}
    std::vector<std::pair<float, float>> return_pair()
    {
        return MinimaPhiPair;
    }

      ~IteratorSWFK() { 
        delete KT1;
        delete KT2;
        delete Phi_;
        delete C_;
        delete T_;
  }

};

class IteratorSWFog
{
    private:
    std::vector<float> *theta_, *K_, *H_, *Ms_, *phi_, *K2_;
    std::vector<float> IteratedDerivative;
    std::vector<std::pair<float, float>> MinimaPhiPair, FunctionPhiPair;
    float u0_;
    public:
IteratorSWFog
(std::vector<float> theta,  
std::vector<float> K, 
std::vector<float> K2, 
std::vector<float> H, 
std::vector<float> Ms, 
std::vector<float> phi,
 
float u0)
:
theta_(new std::vector<float>(theta)),

 K_(new std::vector<float>(K)), 
 H_(new std::vector<float>(H)),
Ms_(new std::vector<float> (Ms)), 
phi_(new std::vector<float>(phi)), 
K2_(new std::vector<float>(K2)),
u0_(u0){}
void iterate_SWF_og()
{
    Operator op;
    Operator::operatorFunctionParam param;
    *param.n_ = 10;
    *param.VEC = IteratedDerivative;
    MinimaPhiPair.clear();
        IteratedDerivative.clear();

        for(size_t j = 0; j<(*H_).size(); j++)
        {
        
          IteratedDerivative.clear();
            for(size_t i=0; i<(*phi_).size(); i++)
                {
                float iteratedDerivativeFloat = pow(sin((*phi_)[i]-(*theta_)[j]),2)*(*K_)[j] 
               + pow(sin((*phi_)[i]-(*theta_)[j]), 4)*(*K2_)[j]
                - (*H_)[j]*cos((*phi_)[i]);
                IteratedDerivative.push_back(iteratedDerivativeFloat);

                if (abs(IteratedDerivative[i + *param.n_] - IteratedDerivative[i]) < 0.005 && abs(IteratedDerivative[i] - IteratedDerivative[i - *param.n_]) > 0.005)
                {
                    MinimaPhiPair.push_back(std::make_pair((*phi_)[i], (*H_)[j]));

                }
            }
        }

}
  std::vector<std::pair<float, float>> return_me_pairs()
{
 return MinimaPhiPair;
}
  std::vector<std::pair<float, float>> return_me_M_pairs()
{
 return FunctionPhiPair;
}
//   ~IteratorSWFog() { // Destructor to deallocate memory
//         delete theta_;
//         delete K_;
//         delete H_;
//         delete phi_;
//         delete Ms_;
  //}

};

class ShiftFunction
{
    private:
std::vector<float> *KT_, *T_, result;
    public:
    ShiftFunction(std::vector<float> KT, std::vector<float> T):KT_(new std::vector<float>(KT)), T_(new std::vector<float>(T))
    {}
    std::vector<float> gradient_search()
    {
        tools::GradientSearch Gs(*KT_, *T_);
        Gs.search_me_for_global_min();
        result = Gs.return_derivative();
        return result;

    }
};

class CoerceModel
{

};