#include <Tools.h>
// #include <Kirschner_SW_BC.h>
 #include <S_W.h>
#include <utility>
#include <pbPlots.hpp>
#include <supportLib.hpp>
#include <string>
#include<PhiFilters.h>

class StuckUpVectors
{
    private:
    std::vector<double> *result = new std::vector<double>;
    int i{0};
    public:
struct operatorParam
{
    std::vector<double> *V1_ = new std::vector<double>;
    std::vector<double> *V2_ = new std::vector<double>;
    operatorParam(std::vector<double> *V1, std::vector<double> *V2):V1_(V1), V2_(V2){} 
};
std::vector<double> do_stuff_with_vectors(std::vector<double> *V1, std::vector<double> *V2)
{
    operatorParam oP(V1, V2);
    while(result->size() < oP.V1_->size())
    {
        i++;
        result->push_back((*oP.V1_)[i]);
    }
     i = 0;
        while (i < oP.V2_->size()) {
            result->push_back((*oP.V2_)[i++]);
        }
    return *result;

}
std::vector<double> operator+(const operatorParam *OP)
{
    return do_stuff_with_vectors(OP->V1_, OP->V2_);
}
}; // operator for adding vectors 

int main()
{
    float u0{2}; // holder for magnetic permitivity constant (numerical)
    tools::UserInputFile *UIF = new tools::UserInputFile(); // declare  file to use
    UIF->ReadFile();

    std::vector<double> *SWK_first_from_pair = new std::vector<double>; // doubles for pbPlots if pbPlots are being used
    std::vector<double> *SWK_second_from_pair = new std::vector<double>;
 
    JustADebug *JAD = new JustADebug(); // debug class
    std::vector<float> K1, K2, C, phi, T, Ms, 
    Ms_SWFog, theta, H, Phi_, KT, KT2, 
    reversedT, reversedKT, reversedMs, reversedPhi; // lengthy vector declaration, for further redo, some are necessary

    K1 = UIF->GetVector(UIF->VA); // vectors from file
    K2 = UIF->GetVector(UIF->VB);
    C = UIF->GetVector(UIF->VC);
    phi = UIF->GetVector(UIF->VD);
    T = UIF->GetVector(UIF->VE);
    theta = UIF->GetVector(UIF->VF);
    H = UIF->GetVector(UIF->VG);
    Ms_SWFog = UIF->GetVector(UIF->VH);

    Approximations::ApproximateMagnetisationSaturation *AMS
    = new Approximations::ApproximateMagnetisationSaturation(T, 1, 770, 3.0); // Magnetisation saturation as temperature modeling
    AMS->make_me_a_Ms_approximation();
   Ms = (AMS->return_me_a_Ms_approximation());


    float M0 = AMS->just_give_me_a_M0();
    anisotropyModels::AnisotropyModels *AM = 
    new anisotropyModels::AnisotropyModels(M0, K1, K2, T, phi, Ms, -5500, 3.1); // first anisotropy constant calc 
    AM->temperature_anisotropy();
    KT = AM->return_KT();

    *AM = anisotropyModels::AnisotropyModels(M0, K1, K2, T, phi, Ms, 3500, 1.8); // second anisotropy constant calc
    AM->temperature_anisotropy();
    KT2 = AM->return_KT(); 



//  StuckUpVectors SUP;
//  StuckUpVectors::operatorParam oP(SWK_first_from_pair, SWK_first_from_pair_2);
//  std::vector<double> SUPvectors1 = SUP+&oP;


IteratorSWFog *SWFogIterate = new IteratorSWFog(theta, K1, H, Ms_SWFog, phi, 2.1); 
SWFogIterate->iterate_SWF_og(); // stoner wolffhart model for field induced changes in magnetisation direction
std::vector<std::pair<float, float>> resultSWFog  = SWFogIterate->return_me_pairs();

IteratorSWFK *SWFKIterator = new IteratorSWFK(KT, KT2, C, phi, T);
SWFKIterator->IterateSWFK(); // temperature dependent changes in magnetisation direction, based on anisotropy temperature modelling 
std::vector<std::pair<float, float>> resultSWFK = SWFKIterator->return_pair();

       for(int i = 0; i<resultSWFK.size();i++)
    {
       double first = resultSWFK[i].first;
       SWK_first_from_pair->push_back(first);

        double second = resultSWFK[i].second;
        SWK_second_from_pair->push_back(second);
    }

    delete AMS;
    delete AM;
    delete UIF;
    delete SWFKIterator;
    delete SWFogIterate; 

// using pbPlots below

	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = CreateStringReferenceLengthValue(0, L' ');
    std::vector<wchar_t> v = {'d', 'o', 't', 's'};
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 1000;
	settings->height = 700;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    settings->title = toVector(L"x^2 - 2");
	settings->xLabel = toVector(L"X axis");
	settings->yLabel = toVector(L"Y axis");

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = SWK_first_from_pair;
	series->ys = SWK_second_from_pair;
    series->pointType = &v;
    series->linearInterpolation = false;

    
    settings->scatterPlotSeries->push_back(series);

    DrawScatterPlotFromSettings(imageReference, settings, errorMessage);
 		std::vector<double> *pngdata = ConvertToPNG(imageReference->image);
		WriteToFile(pngdata, "example2.png");
		DeleteImage(imageReference->image);

    return 0;

}

