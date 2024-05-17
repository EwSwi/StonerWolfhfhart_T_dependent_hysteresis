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
        if(cosPhiPair_[i].second >= 0 && cosPhiPair_[i].second <= 3.14) //separate first minima set
        {
            tempPair = std::make_pair(cosPhiPair_[i].first, cosPhiPair_[i].second);
            cos1stPhiPair.push_back(tempPair);
            H_1st.push_back(H_[i]); // mag field info
        }
        else
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
std::vector<std::pair<double, double>> cosPhiPair_, cosPhiPair_2nd, filteredCosPhiPair;
std::vector<double> H_, H_2, H_save;
public:
SelectHysteresisBranch( SeparateForCosineCalc *SFC)
{
   cosPhiPair_ = SFC->cos0_to_phi();
   H_ = SFC->return_H1st();
   H_2 = SFC->return_H2nd();
   cosPhiPair_2nd = SFC->cosphi_to_2phi();
}


void selectUpper(bool select)
{
    filteredCosPhiPair.clear();
    H_save.clear();
    if(select == true)
    {
        for(size_t i=0; i<cosPhiPair_.size();i++)
        {
            if(cosPhiPair_[i].first >= 0)
            {
            filteredCosPhiPair.push_back(cosPhiPair_[i]);
            H_save.push_back(H_[i]);
            }
        }
    }
   else
    {
        for(size_t i=0; i<cosPhiPair_2nd.size();i++)
        {
            if(cosPhiPair_2nd[i].first <= 0)
            {
           filteredCosPhiPair.push_back(cosPhiPair_2nd[i]);
            H_save.push_back(H_2[i]);
            }
        }

    }
}
std::vector<std::pair<double, double>> return_cos() {return filteredCosPhiPair;}
std::vector<double> return_H() {return H_save;}
};

// for temperature loops
class TemperaturePhiFilter
{
    private:
    std::vector<std::pair<float, float>> TempPhiPair_;
    std::vector<std::pair<float,float>> filteredTempPhiPair;
    std::vector<double> T, phi;
    public:
    // first from pair phi, second from pair T
    TemperaturePhiFilter(std::vector<std::pair<float, float>> TempPhiPair)
    :
    TempPhiPair_(TempPhiPair)
    {}

    void filter_branch(bool topBranch)
    {
    filteredTempPhiPair.clear();
        if (topBranch == true)
        {
             for(size_t i=0; i<TempPhiPair_.size(); i++)
             {
                if(TempPhiPair_[i].first >= 0 && TempPhiPair_[i].first <= 1.57)
                {
                    std::pair<float, float> TempoaryTempPhi = 
                    std::make_pair(TempPhiPair_[i].first, TempPhiPair_[i].second);
                    filteredTempPhiPair.push_back(TempoaryTempPhi);
                }
             }
        }
        if (topBranch == false)
        {
             for(int i=0; i<TempPhiPair_.size(); i++)
            {
                if(TempPhiPair_[i].first > 1.57 && TempPhiPair_[i].first <= 3.14)
                {

                    filteredTempPhiPair.push_back(std::make_pair(TempPhiPair_[i].first, TempPhiPair_[i].second));
                }
            }
        }
          
}

    void make_vector()
    {
        for(int i=0; i<filteredTempPhiPair.size(); i++)
        {
            double tempT = filteredTempPhiPair[i].second;
            double tempPhi = filteredTempPhiPair[i].first;
            T.push_back(tempT); // make vector for temperature corresponding to calculated cosine
            phi.push_back(tempPhi); // make vector for cosine calc 

        }
    }

    std::vector<std::pair<float, float>> return_filtered_T_phi_pairs(){return filteredTempPhiPair;}
    std::vector<double> return_filtered_T(){return T;}
    std::vector<double> return_filtered_phi(){return phi;}
};

#endif
