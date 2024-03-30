#include "math.h"
#include <iostream>
//#include "CLHEP/Vector/LorentzVector.h"

//#include "/products/GEANT4/sl7/CLHEP/clhep2404/include/CLHEP/Vector/LorentzVector.h"

using namespace std;

void createInput(){

  TH3D* muFlux = new TH3D("muFlux","muFlux",500,-25,25,500,-25,25,500,-25,0);
  TH3D* mupFlux = new TH3D("mupFlux","mupFlux",500,-25,25,500,-25,25,500,-25,0);
  TH3D* munFlux = new TH3D("munFlux","munFlux",500,-25,25,500,-25,25,500,-25,0);


  double id, px, py, pz, ek, x, y, t, w;

  TFile* FileFLUX = new TFile("output.root","read");
  FileFLUX->cd();
  TTree* data = (TTree*)FileFLUX->Get("data_1");
  data->SetBranchAddress("id",&id);
  data->SetBranchAddress("px",&px);
  data->SetBranchAddress("py",&py);
  data->SetBranchAddress("pz",&pz);
  data->SetBranchAddress("ek",&ek);
  data->SetBranchAddress("x",&x);
  data->SetBranchAddress("y",&y);
  data->SetBranchAddress("t",&t);
  data->SetBranchAddress("w",&w);

  Long64_t nentries = data->GetEntries();// mc has less entries

  double pival = acos(-1);

  for(Long64_t i=0 ; i<nentries; i++) {
    data->GetEntry(i);
    if(id==5 || id==6) {muFlux->Fill(px,py,-1.*pz);}
    if(id==5) {mupFlux->Fill(px,py,-1.*pz);}
    if(id==6) {munFlux->Fill(px,py,-1.*pz);}
  }

  //Convert to sparse histogram
  const Int_t dim = 3;
  Int_t nBins[dim] = {muFlux->GetNbinsX(), muFlux->GetNbinsY(), muFlux->GetNbinsZ()};
  Double_t xmin[dim] = {muFlux->GetXaxis()->GetXmin(), muFlux->GetYaxis()->GetXmin(), muFlux->GetZaxis()->GetXmin()};
  Double_t xmax[dim] = {muFlux->GetXaxis()->GetXmax(), muFlux->GetYaxis()->GetXmax(), muFlux->GetZaxis()->GetXmax()};

  THnSparseD* histnSparse = new THnSparseD(muFlux->GetName(), muFlux->GetTitle(), dim, nBins, xmin, xmax);
  THnSparseD* histnSparseP = new THnSparseD(mupFlux->GetName(), mupFlux->GetTitle(), dim, nBins, xmin, xmax);
  THnSparseD* histnSparseN = new THnSparseD(munFlux->GetName(), munFlux->GetTitle(), dim, nBins, xmin, xmax);
  // Fill the THnSparseD histogram from the TH3F histogram
    for (Int_t ix = 1; ix <= muFlux->GetNbinsX(); ++ix) {
        for (Int_t iy = 1; iy <= muFlux->GetNbinsY(); ++iy) {
            for (Int_t iz = 1; iz <= muFlux->GetNbinsZ(); ++iz) {
                Double_t content = muFlux->GetBinContent(ix, iy, iz);
                Double_t contentP = mupFlux->GetBinContent(ix, iy, iz);
                Double_t contentN = munFlux->GetBinContent(ix, iy, iz);
                if (content != 0.0) {
                    Double_t binCoords[dim] = {muFlux->GetXaxis()->GetBinCenter(ix), muFlux->GetYaxis()->GetBinCenter(iy), muFlux->GetZaxis()->GetBinCenter(iz)};
                    histnSparse->Fill(binCoords, content);
                }
                if (contentP != 0.0) {
                    Double_t binCoords[dim] = {mupFlux->GetXaxis()->GetBinCenter(ix), mupFlux->GetYaxis()->GetBinCenter(iy), mupFlux->GetZaxis()->GetBinCenter(iz)};
                    histnSparseP->Fill(binCoords, contentP);
                }
                if (contentN != 0.0) {
                    Double_t binCoords[dim] = {munFlux->GetXaxis()->GetBinCenter(ix), munFlux->GetYaxis()->GetBinCenter(iy), munFlux->GetZaxis()->GetBinCenter(iz)};
                    histnSparseN->Fill(binCoords, contentN);
                }
            }
        }
    }

  // Get the number of dimensions
  const Int_t ndims = histnSparse->GetNdimensions();
  std::cout << "Number of dimensions: " << ndims << std::endl;

  // Define an array to store the generated random coordinates
  Double_t randCoord[3];
  //double Pxx, Pyy, Pzz;

  for(int ij=0; ij<100; ij++){
    // Generate random coordinates using GetRandom method
    histnSparse->GetRandom(randCoord);
    //muFlux->GetRandom3(randCoord[0],randCoord[1],randCoord[2]);
    // Print the generated random coordinates
    std::cout << "Random Coordinates: (" << randCoord[0] << ", " << randCoord[1] << ", " << randCoord[2] << ")" << std::endl;
    // Get the bin corresponding to the coordinate
    //randCoord[0]=-24;randCoord[1]=-24;randCoord[2]=-24;
    Long64_t gBinP = histnSparseP->GetBin(randCoord);
    Long64_t gBinN = histnSparseN->GetBin(randCoord);
    // Get the bin content
    int gBin2 = muFlux->FindBin(randCoord[0],randCoord[1],randCoord[2]);

    cout<<"gBinN=" << gBinN <<"\tgBinP=" <<gBinP<<"\tgBin2="<<gBin2<<endl;
    

    Int_t binNum1[3];
    Int_t binNum2[3];

    Double_t mupCnt = histnSparseP->GetBinContent(gBinP,binNum1);
    Double_t munCnt = histnSparseN->GetBinContent(gBinN,binNum2);

    Double_t mupCntO = mupFlux->GetBinContent(gBin2);
    Double_t munCntO = munFlux->GetBinContent(gBin2);

    // Print the content
    std::cout << "Bin content at (" << randCoord[0] << ", " << randCoord[1] << ", " << randCoord[2] << "): " << "\tp="<<mupCnt << "\tn=" << munCnt<< std::endl;
    std::cout << "Bin content at (" << randCoord[0] << ", " << randCoord[1] << ", " << randCoord[2] << "): " << "\tp="<<mupCntO << "\tn=" << munCntO<< std::endl;

    std::cout << "Bin Number = (" << binNum1[0] << "," << binNum1[1] << "," << binNum1[2] <<")"<<endl;
    std::cout << "Bin Number = (" << binNum2[0] << "," << binNum2[1] << "," << binNum2[2] <<")"<<endl;

  }




  TFile *fileOut = new TFile("flux_qgsjeto1c_fluka_jim_sparse.root","RECREATE");
  fileOut->cd();
  histnSparse->Write(0,TObject::kOverwrite);
  histnSparseP->Write(0,TObject::kOverwrite);
  histnSparseN->Write(0,TObject::kOverwrite);
}
