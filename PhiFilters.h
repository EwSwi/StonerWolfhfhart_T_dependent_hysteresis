#ifndef PhiFilters_H_ 
#define PhiFilters_H_
#include <vector>
#include <utility>
#include <iostream>
class SeparateForCosineCalc
// Cosine is being the function interpreted as the normalised magnetisation
{
private:
std::vector<std::pair<double, double>> cosPhiPair_, cos1stPhiPair, cos2ndPhiPair;
std::vector<double> H_, H_1st, H_2nd;
public:
SeparateForCosineCalc(
const std::vector<std::pair<double, double>> cosPhiPair,
std::vector<double> H)
:
cosPhiPair_(cosPhiPair),
H_(H){} // declare H as an addition for debug

void separate()
{
    std::pair<double, double> tempPair;

    for(size_t i=0; i<cosPhiPair_.size(); i++)
    {
        if(cosPhiPair_[i].second >= -0.05 && cosPhiPair_[i].second <= 3.145) //separate first minima set
        {
            tempPair = std::make_pair(cosPhiPair_[i].first, cosPhiPair_[i].second);
            cos1stPhiPair.push_back(tempPair);
            H_1st.push_back(H_[i]); // mag field info
        }
        else if((cosPhiPair_[i].second <= -0.05 && cosPhiPair_[i].second >= -1.57)  // from second minima set
        || (cosPhiPair_[i].second >= 3.146 && cosPhiPair_[i].second <= 4.71))
        {
            tempPair = std::make_pair(cosPhiPair_[i].first, cosPhiPair_[i].second);
            cos2ndPhiPair.push_back(tempPair);
            H_2nd.push_back(H_[i]); // mag field info
        }
    }
}
// series of getits, i will rewrite them at some point to be more of a pair<pair,double> maybe
std::vector<std::pair<double, double>> cos0_to_phi(){return cos1stPhiPair;}
std::vector<std::pair<double, double>> cosphi_to_2phi(){return cos2ndPhiPair;}
std::vector<double> return_H1st(){return H_1st;}
std::vector<double> return_H2nd(){return H_2nd;}
};

// filter cos <0 and cos>0 to get an output reminding hysteresis loop

class SelectHysteresisBranch
{
private: 
std::vector<std::pair<double, double>> cosPhiPair_, filteredCosPhiPair;
public:
SelectHysteresisBranch(
const std::vector<std::pair<double, double>> cosPhiPair)
:
cosPhiPair_(cosPhiPair){}

void selectUpper(bool select)
{
    filteredCosPhiPair.clear();
    filteredCosPhiPair.resize(cosPhiPair_.size());
    if(select = 1)
    {
        for(size_t i=0; i<cosPhiPair_.size();i++)
        {
            if(cosPhiPair_[i].first >= 0)
            {
            filteredCosPhiPair[i] = cosPhiPair_[i];
            }
        }
    }
    if(select = 0)
    {
        for(size_t i=0; i<cosPhiPair_.size();i++)
        {
            if(cosPhiPair_[i].first <= 0)
            {
            filteredCosPhiPair[i] = cosPhiPair_[i];
            }
        }

    }
}


};
#endif
