#include <Tools.h>
 #include <S_W.h>
#include <utility>
#include <pbPlots.hpp>
#include <supportLib.hpp>
#include <string>
#include<PhiFilters.h>

class StuckUpVectors
{
private:
    std::vector<double> result; 

public:
    struct operatorParam
    {
        const std::vector<double>* V1_;
        const std::vector<double>* V2_;
        operatorParam(const std::vector<double>* V1, const std::vector<double>* V2)
            : V1_(V1), V2_(V2) {}
    };

    std::vector<double> operator+(const operatorParam& OP)
    {
        result.clear();

        for (const double& elem : *(OP.V1_))
        {
            result.push_back(elem);
        }

        for (const double& elem : *(OP.V2_))
        {
            result.push_back(elem);
        }

        return result;
    }
};

int main()
{
    float u0{1}; // holder for magnetic permitivity constant (numerical)
    tools::UserInputFile *UIF = new tools::UserInputFile(); // declare  file to use
    UIF->ReadFile();

    std::vector<double> *SWK_first_from_pair = new std::vector<double>; // doubles for pbPlots if pbPlots are being used
    std::vector<double> *SWK_second_from_pair = new std::vector<double>;
 
    JustADebug *JAD = new JustADebug(); // debug class
    std::vector<float> K1, K2, C, phi, T, Ms, 
    Ms_SWFog, theta, H, Phi_, KT, KT2, 
    reversedT, reversedKT, reversedKT2, reversedPhi; // lengthy vector declaration, for further redo, some are necessary

    K1 = UIF->GetVector(UIF->VA); // vectors from file
    K2 = UIF->GetVector(UIF->VB);
    C = UIF->GetVector(UIF->VC);
    phi = UIF->GetVector(UIF->VD);
    T = UIF->GetVector(UIF->VE);
    theta = UIF->GetVector(UIF->VF);
    H = UIF->GetVector(UIF->VG);
    Ms_SWFog = UIF->GetVector(UIF->VH);

    tools::ReverseVector *RV = new tools::ReverseVector(T);
    RV->reverse_it();
    reversedT = RV->return_reversed();


    Approximations::ApproximateMagnetisationSaturation *AMS
    = new Approximations::ApproximateMagnetisationSaturation(T, 1, 570, 3.0); // Magnetisation saturation as temperature modeling
    AMS->make_me_a_Ms_approximation();
   Ms = (AMS->return_me_a_Ms_approximation());
    float M0 = AMS->just_give_me_a_M0(); 
// new anisotropy modelling approaches, start
    anisotropyModels::LinearTK1 *LKT = new anisotropyModels::LinearTK1(T);
    LKT->calc_linear_KT_dependency(10, 220); // KT1, linear approx.
    KT = LKT->get_linear_KT(); // save KT value for bool topbranch ==false
    delete LKT;

    anisotropyModels::PolynomialModelKT2 *pMKT2 = new anisotropyModels::PolynomialModelKT2(KT);
    pMKT2->calcPolynomialKT2(0.000005, -0.00002, -0.00035, 0.0007, 0.013, -0.03, -15); // polynomial approx KT2
    KT2 = pMKT2->get_me_KT2();  // save KT2 value for bool topbranch == false
    delete pMKT2;

// second branch: 
LKT = new anisotropyModels::LinearTK1(reversedT);
LKT->calc_linear_KT_dependency(20, 220); reversedKT = LKT->get_linear_KT(); // save KT value for bool topbranch == true

pMKT2 = new anisotropyModels::PolynomialModelKT2(reversedKT);
pMKT2->calcPolynomialKT2(0.000005, -0.00002, -0.00035, 0.0007, 0.013, -0.03, -15);
 reversedKT2 = pMKT2->get_me_KT2(); // save KT2 value for bool topbranch == true

IteratorSWFK *SWFKIterator = new IteratorSWFK(KT, KT2, C, phi, T);
SWFKIterator->IterateSWFK(); // temperature dependent changes in magnetisation direction, based on anisotropy temperature modelling 
std::vector<std::pair<float, float>> resultSWFK = SWFKIterator->return_pair();

bool bottom = false;
TemperaturePhiFilter *TPF = new TemperaturePhiFilter(SWFKIterator->return_pair());
TPF->filter_branch(bottom); TPF->make_vector();
std::vector<double> filteredT_bottombranch = TPF->return_filtered_T(); 
std::vector<double> filteredPhi_bottombranch = TPF->return_filtered_phi(); 
// deallocate before second run
delete SWFKIterator;

SWFKIterator = new IteratorSWFK(reversedKT, reversedKT2, C, phi, T);
SWFKIterator->IterateSWFK(); 

bool upper = true;
TPF = new TemperaturePhiFilter(SWFKIterator->return_pair());
TPF->filter_branch(upper); TPF->make_vector();
std::vector<double> filteredT_upperbranch = TPF->return_filtered_T();
std::vector<double> filteredPhi_upperbranch = TPF->return_filtered_phi();


// make double

std::vector<double> Temperature_first_from_pair, Temperature_second_from_pair;

           for(int i = 0; i<resultSWFK.size();i++)
    {
       double first = resultSWFK[i].first;
       Temperature_first_from_pair.push_back(first);

        double second = resultSWFK[i].second;
        Temperature_second_from_pair.push_back(second);
    }
    delete SWFKIterator;



// old approach below

    // anisotropyModels::AnisotropyModels *AM = // declare old approach to magnetistion KT1 
    // new anisotropyModels::AnisotropyModels(M0, K1, K2, T, phi, Ms, -100, 12); // first anisotropy constant calc 
    // AM->temperature_anisotropy();
    // KT = AM->return_KT();

    // *AM = anisotropyModels::AnisotropyModels(M0, K1, K2, T, phi, Ms, 20, 13); // second anisotropy constant calc
    // AM->temperature_anisotropy();
    // KT2 = AM->return_KT(); 

// end old approahc


IteratorSWFog *SWFogIterate = new IteratorSWFog(theta, K1, K2, H, Ms_SWFog, phi, 2.1); 
SWFogIterate->iterate_SWF_og(); // stoner wolffhart model for field induced changes in magnetisation direction
std::vector<std::pair<float, float>> resultSWFog  = SWFogIterate->return_me_pairs();

    delete AMS;
    delete UIF;


       for(int i = 0; i<resultSWFog.size();i++)
    {
       double first = resultSWFog[i].first;
       SWK_first_from_pair->push_back(first);

        double second = resultSWFog[i].second;
        SWK_second_from_pair->push_back(second);
    }
delete SWFogIterate; 

anisotropyModels::MagnetisationCalc *MC 
= new anisotropyModels::MagnetisationCalc(
    *SWK_first_from_pair, *SWK_second_from_pair);
MC->calc_me_magnetisation_scalar();

 std::vector<double> cosine_vec1, cosine_vec2;
for(int i=0; i<MC->return_cosine().size(); i++)
{
    double MCfirst = MC->return_cosine()[i].first;
    cosine_vec1.push_back(MCfirst);

    double MCsecond = MC->return_cosine()[i].second;
    cosine_vec2.push_back(MCsecond);
    std::cout << cosine_vec1[i] << " :cosine, H: " <<cosine_vec2[i]<<std::endl;

}
SeparateForCosineCalc *SFC = new SeparateForCosineCalc(MC->return_cosine(), *SWK_second_from_pair);
SFC->separate();
//debug of separate 
SFC->cos0_to_phi();
// first bool for PhiFilters::SHB class
bool select = true;
//declaring two return pairs, two filtered cosine gets
std::vector<std::pair<double, double>> returnedFiltheredCosPair1, returnedFiltheredCosPair2;
std::vector<double> philteredCosine1, philteredCosine2;
// evoking class
SelectHysteresisBranch *SHB = new SelectHysteresisBranch(SFC);
// first branch below
SHB->selectUpper(select);
std::vector<double> returnPhilteredH1 = SHB->return_H();
returnedFiltheredCosPair1 = SHB->return_cos();

for(size_t i=0; i<returnedFiltheredCosPair1.size(); i++)
{
    philteredCosine1.push_back(returnedFiltheredCosPair1[i].first);
}
// second branch below
bool select2 = false;
SHB->selectUpper(select2);
std::vector<double> returnPhilteredH2 = SHB->return_H();
returnedFiltheredCosPair2 = SHB->return_cos();

for(size_t i=0; i<returnedFiltheredCosPair2.size(); i++)
{
    philteredCosine2.push_back(returnedFiltheredCosPair2[i].first);
}
// done with branches, stuck up vectors for uniform pbplot input

    
   StuckUpVectors SUP;

    // create operatorParam pointer
    StuckUpVectors::operatorParam* oP = nullptr;

    // declare op as new

    oP = new StuckUpVectors::operatorParam(&philteredCosine1, &philteredCosine2);
    std::vector<double> cosine_1st_plus_2nd_periods = SUP + *oP;

    // delete op to deallocate
    delete oP;
    // now able to redeclare
    oP = new StuckUpVectors::operatorParam(&returnPhilteredH1, &returnPhilteredH2);
    std::vector<double> H1st_plus_H2nd_periods = SUP + *oP;
    // delete to deallocate
    delete oP;

    oP = new StuckUpVectors::operatorParam(&filteredPhi_bottombranch, &filteredPhi_upperbranch);
    std::vector<double> temperature_phi_plus_phi = SUP + *oP;
    delete oP;

    oP = new StuckUpVectors::operatorParam(&filteredT_bottombranch, &filteredT_upperbranch);
    std::vector<double> temperature_T_plus_T = SUP + *oP;
    delete oP;

anisotropyModels::MagnetisationCalcButNotMD *MCnotretarded 
= 
new anisotropyModels::MagnetisationCalcButNotMD(temperature_phi_plus_phi);
MCnotretarded->calc_me_magnetisation_scalar();
std::vector<double> cosineTemperatureFlow = MCnotretarded->return_cosine();



ofstream outputFile;
outputFile.open("output.txt");
outputFile.clear();
std::cout << "filtered T bot size: " << temperature_phi_plus_phi.size() << " phi size: " << temperature_T_plus_T.size() << std::endl;
for(int i = 0; i<temperature_phi_plus_phi.size();i++)
{

    outputFile 
    << filteredPhi_bottombranch[i] 
   << " " 
    << filteredT_bottombranch[i] 
    << " " << temperature_phi_plus_phi[i] 
    << " " <<temperature_T_plus_T[i] 
    <<std::endl;

}


	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = CreateStringReferenceLengthValue(0, L' ');
    std::vector<wchar_t> v = {'d', 'o', 't', 's'};
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 1000;
	settings->height = 700;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    settings->title = toVector(L" ");
	settings->xLabel = toVector(L"phi (rad)");
	settings->yLabel = toVector(L"Magnetic Field (mT)");

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = &temperature_T_plus_T;
	series->ys = &cosineTemperatureFlow;
    series->pointType = &v;
    series->linearInterpolation = false;

    
    settings->scatterPlotSeries->push_back(series);

    DrawScatterPlotFromSettings(imageReference, settings, errorMessage);
 		std::vector<double> *pngdata = ConvertToPNG(imageReference->image);
		WriteToFile(pngdata, "example2.png");
		DeleteImage(imageReference->image);

    return 0;

}

