#ifndef Tools_H
#define Tools_H
#include <vector>
#include <math.h>
#include <iostream>
#include<fstream>
#include <string>
#include<algorithm>
// header for various tools i might or might not use further, still underdeveloped
namespace tools
{
class UserInputFile // class for reading vectors from file
{
    public:

    enum VectorType {VA, VB, VC, VD, VE, VF, VG, VH};
    std::vector<float> vA,vB,vC,vD,vE, vF, vG, vH;
    std::vector<float> *VectorPointer[8] {&vA, &vB, &vC, &vD, &vE, &vF, &vG, &vH};
    float A, B, C, D, E, F, G, H;
    UserInputFile(){};
    void ReadFile()
    { 

    std::ifstream infile("./test_file.txt");
    infile >> A >> B >> C >> D >> E >> F >> G >>H; 
     while(infile >> A >> B >> C >> D >> E >> F >> G>>H)
 {
    vA.push_back(A);
    vB.push_back(B);
    vC.push_back(C);
    vD.push_back(D);
    vE.push_back(E);
    vF.push_back(F);
    vG.push_back(G);
    vH.push_back(H);
 }
 if(!infile.is_open())
 
 {
    std::cout<< "no fille, sorry"<<std::endl;
 }

    }
     std::vector<float> GetVector(VectorType type)
 {
   return *VectorPointer[type];
 }
};

class FindMeMin // UNUSED min finding class, 
{
private:
std::vector<float>::const_iterator minElement;
public:
void find_me_min(std::vector<float> Master)
    {
    minElement = std::min_element(Master.begin(), Master.end());
    } 
float get_me_min()
    {
    return *minElement;
    }
};

class GetVector // generic GetVector
{
    public:
    enum Type {a, b, c, d, e};
    std::vector<float> *vecPtr[5] {&A, &B, &C, &D, &E};
    std::vector<float> A, B, C, D, E;
    GetVector(std::vector<float> A_, std::vector<float> B_, 
    std::vector<float> C_,std::vector<float> D_, 
    std::vector<float> E_):A(A_), B(B_), C(C_), D(D_), E(E_)
    {

    }
    std::vector<float> return_vector(Type type)
    {
        return *vecPtr[type];
    }
};


class GradientSearch // underdeveloped gradient search, not sure if i will use it
{
    private:
    std::vector<float> *function;
    std::vector<float> *derivative = new std::vector<float>;
    std::vector<float> *function_variable;
    float delta;
    float contain[1]; 
    public:
    GradientSearch(std::vector<float> Function, std::vector<float> Function_Variable)
    :function(new std::vector<float>), function_variable(new std::vector<float>)
    {
        *function = Function;
        *function_variable = Function_Variable;
    }


    float search_me_for_global_min() // UNUSED another approach for minima defining
    {
        derivative->clear();
        derivative->resize(function->size());
        for (int i = 1; i<(*function).size()-2; i++)
        {


            (*derivative)[i] = ((*function)[i+1] - (*function)[i])/ 
            ((*function_variable)[i+1] - (*function_variable)[i]);

        }
        
    }
    std::vector<float> return_derivative()
    {
        return *derivative;
    }

};

class ReturnFunctionValue // UNUSED filtering values idea, not implemented nor fully developed yet
{
    private:
    std::vector<float> *master, *slave;
    float variable, foundIt, temp;
    public:
ReturnFunctionValue(std::vector<float> Master, std::vector<float> Slave):
master(new std::vector<float>), slave(new std::vector<float>)
{
    *master = Master;
    *slave = Slave;
}

void find_value()
{
        int i = 0;
        while((*master)[i] != variable)
        {
            i++;
        }
        (*slave)[i] = foundIt;
}
float return_value(){return foundIt;}


};

class FindMinima // Just another idea, or even rewritten previous class
{
    private:
    std::vector<float> *main_vector, store_vector;
    public:
    FindMinima(std::vector<float> mainVector):
    main_vector(new std::vector<float>){*main_vector = mainVector;}

void get_me_min()
{
    for(int i=0; i < (*main_vector).size(); i++)
    {
        if((*main_vector)[i] == 0)
        {
            float k = (*main_vector)[i];
            store_vector.push_back(k);
        }
    }
}
};
// class to improve further, for now its a baseline class for half a period of usual phi 

class JustADebug // generic flot debug class
{
public:
void debug_class(std::vector<float> Whatever)
{
    for(float W: Whatever)
    {
        std::cout<< W <<std::endl;
    }
}

}; 
class JustADebugDouble //generic debug class for pbPlots
{
public:
void debug_class(std::vector<double> Whatever)
{
    for(double W: Whatever)
    {
        std::cout << W << std::endl;
    }
}

}; 

class ReverseVector // generic float reverse for further modeling
{
private:
    std::vector<float> *V1;
    std::vector<float> contain_it;
public:
ReverseVector(std::vector<float> V1_):V1(new std::vector<float>(V1_))
{}
void reverse_it()
{
    for(int i = V1->size() -1; i >= 0; i--)
    {
        float Container = (*V1)[i];
        contain_it.push_back(Container);
    }
}
std::vector<float> return_reversed()
{
    return contain_it;
}
};
}
#endif