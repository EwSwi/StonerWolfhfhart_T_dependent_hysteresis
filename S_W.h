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

class IteratorSWFK1stD: public Stoner_Wolf_Fart_Kirschner_1st_Derivative 
{
private:
tools::FindMeMin FmM;
std::vector<float> IteratedDerivative, function, Minima;
std::vector<std::vector<float>> StoreIteratedDerivative;
std::vector<std::pair<float, float>> MinimaPhiPair; //= new std::vector<std::pair<float, float>>;
float *min = new float;
std::vector<float> *T_ = new std::vector<float>;
Operator op;
public:
IteratorSWFK1stD(const std::vector<float> K1, const std::vector<float> K2, const std::vector<float> C, const std::vector<float> Phi, const std::vector<float> T):
Stoner_Wolf_Fart_Kirschner_1st_Derivative(K1, K2, C, Phi)
{
    *K1_ = K1;
    *K2_ = K2;
    *C_ = C;
    *phi_ = Phi;
    *T_ = T;
};
std::vector<std::vector<float>> do_stuff_with_function(Stoner_Wolf_Fart_Kirschner_1st_Derivative::type Type)
{
    function.clear();
    //function->resize(K1_->size());
    Operator::operatorFunctionParam param;
    param.n = 8;
    *param.VEC = function;
    // MinimaPhiPair.clear();
    // MinimaPhiPair.resize(K1_->size());
    IteratedDerivative.clear();
    IteratedDerivative.resize((K1_->size()));
    T_->resize(K1_->size());
    K2_->resize(K1_->size());
    switch(Type)
    {
    case phi:
        for(size_t j = 0; j<(*K1_).size(); j++)
            {
            for(size_t i=0; i<(*phi_).size(); i++)
            {
                float functionFloat = pow(sin((*phi_)[i]),2)*(*K1_)[j] 
                  + pow(sin((*phi_)[i]), 4)*((*K2_)[j]) 
                 - ((*C_)[j]*(*C_)[j]*(2.1))/2 
                *pow(cos((*phi_)[i]), 2);
                function.push_back(functionFloat);
                
                 IteratedDerivative[i] = sin(2*(*phi_)[i])*(*K1_)[j] + 4*(*K2_)[j] 
                 * pow(sin((*phi_)[i]), 3) * cos((*phi_)[i])
                 + pow((*C_)[j], 2)*sin(2*(*phi_)[i]);
                 if (function[i] < function[i + param.n] && function[i] < function[i - param.n])
                {
                //  std::cout <<"im at the loop with if, SWFK, derivative:"<< IteratedDerivative[i]<< " function " << (function)[i] <<" K2: " << (*K2_)[j]<<" KT1: " << (*K1_)[j] <<" Phi: "  <<(*phi_)[i] <<" Temperature: "<<(*T_)[i] <<std::endl;
                MinimaPhiPair.push_back(std::make_pair((*T_)[i], (function)[i]));
                    // }
                 }
            }
    StoreIteratedDerivative.push_back(IteratedDerivative);
            }
    }
return StoreIteratedDerivative;
}
std::vector<std::vector<float>> operator*(Stoner_Wolf_Fart_Kirschner_1st_Derivative::type Type)
{
    return do_stuff_with_function(Type);
}
std::vector<std::pair<float, float>> get_me_pairs()
{
        return MinimaPhiPair;
}
std::vector<float> get_me_iterated_derivative()
{
    return IteratedDerivative;
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
    param.n = 2;
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
                 - ((*C_)[j]*(*C_)[j]*(2.1))/2 
                *pow(cos((*Phi_)[i]), 2);
                function.push_back(functionFloat);
              //  std::cout<< "SWFK, PRELOOP, FUNCTION:  " << function[i] << " KT1: " << (*KT1)[i] << " KT2: " <<j<<" phi:  "<< (*Phi_)[i] << std::endl;
                
                //  IteratedDerivative[i] = sin(2*(*Phi_)[i])*(*KT1)[j] + 4*(*KT2)[j] 
                //  * pow(sin((*Phi_)[i]), 3) * cos((*Phi_)[i])
                //  + pow((*C_)[j], 2)*sin(2*(*Phi_)[i]);
                 if (abs(function[i + param.n] - function[i]) < 0.001 && abs(function[i] - function[i - param.n]) > 0.001)
                {
                std::cout<< "SWFK, writin pairs, Phi_:  " << (function)[i] << " function: " << (*Phi_)[i]<<std::endl;
                MinimaPhiPair.push_back(std::make_pair((function)[i], ((*Phi_)[i])));
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
    std::vector<float> *theta_, *K_, *H_, *Ms_, *phi_;
    std::vector<float> IteratedDerivative;
    std::vector<std::pair<float, float>> MinimaPhiPair;
    float u0_;
    public:
IteratorSWFog(std::vector<float> theta,  std::vector<float> K,  std::vector<float> H, std::vector<float> Ms, std::vector<float> phi, float u0):
theta_(new std::vector<float>(theta)), K_(new std::vector<float>(K)), H_(new std::vector<float>(H)),
Ms_(new std::vector<float> (Ms)), phi_(new std::vector<float>(phi)), u0_(u0){}
void iterate_SWF_og()
{
    Operator op;
    Operator::operatorFunctionParam param;
    *param.n_ = 15;
    *param.VEC = IteratedDerivative;
    MinimaPhiPair.clear();
        IteratedDerivative.clear();

        for(size_t j = 0; j<(*H_).size(); j++)
        {
        
          IteratedDerivative.clear();
            for(size_t i=1; i<(*phi_).size()-1; i++)
                {
                float iteratedDerivativeFloat = pow(sin((*phi_)[i]),2)*(*K_)[j] - 
                (*H_)[j]*(*Ms_)[j]*(u0_)*cos((*phi_)[i] -(*theta_)[j]);
                IteratedDerivative.push_back(iteratedDerivativeFloat);
                // cout<< IteratedDerivative[i] << " <- function, j -> " << j << " i: " << i << " phi: " << (*phi_)[i] << endl;

                if (abs(IteratedDerivative[i + 1] - IteratedDerivative[i]) < 0.001 && abs(IteratedDerivative[i] - IteratedDerivative[i - 1]) > 0.001)
                {
                 cout<< IteratedDerivative[i] << " <- function, j -> " << j << " i: " << i << " phi: " << (*H_)[j] << " THE LOOP" << endl;
                    MinimaPhiPair.push_back(std::make_pair((*phi_)[i], (*H_)[j]));
                }
            }
        }

}
  std::vector<std::pair<float, float>> return_me_pairs()
{
 return MinimaPhiPair;
}
  ~IteratorSWFog() { // Destructor to deallocate memory
        delete theta_;
        delete K_;
        delete H_;
        delete phi_;
        delete Ms_;
  }

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