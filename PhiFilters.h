#ifndef PhiFilters_H_ 
#define PhiFilters_H_
#include <vector>
#include <utility>
#include <iostream>

/// this header is yet to be developed

// class Interpolation
// {
//     private:
//     std::vector<float> *x_, *y_;
//     int *n = new int;
//     int *m = new int;
//     public:
// Interpolation(std::vector<float> x, std::vector<float> y):
// x_(new std::vector<float>(x)), y_(new std::vector<float>(y))
// {}
// void interpolate_with_linear()
// {
//     float a =y_->at(*n)/(x_->at(*n) - (x_->at(*m)/y_->at(*m)));

// }
// };
class MinimaAmountAsess
//// this filters out these random shits at phi = 0///////////
{
    private:
    std::vector<double> Vector1, Vector2;
   std::vector<std::pair<double,double>> storeIt;
    public:
MinimaAmountAsess(std::vector<double> V1,  std::vector<double> V2):Vector1(V1), Vector2(V2)

{}
void pair_to_value()
{
    //storeIt.resize()
    std::vector<std::vector<double>> storeItLocal;
  for (int j = 0; j < Vector2.size(); j++) {
    double tempV2plus, tempV2minus;
            for(int i = j + 1; i < Vector1.size() -1; i++) {
                if(Vector2[j] <= Vector2[i] + 0.01 && Vector2[j] >= Vector2[i] - 0.01) 
                {
                    storeIt.push_back(std::make_pair(Vector1[i],Vector2[j])); 
                    std::cout<< Vector1[i] << " <- V1, V2->  " << Vector2[j] <<std::endl;
                }
            }
        }
}

std::vector<std::pair<double, double>>  return_vector_vector()
    {
        return storeIt;
    }

};


#endif