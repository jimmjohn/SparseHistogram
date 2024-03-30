void sparse_exa()
{
	Int_t bins[2] = {10, 20};
	Double_t xmin[2] = {0., -5.};
	Double_t xmax[2] = {10., 5.};
	auto hsd = new THnSparseD("hs", "hs", 2, bins, xmin, xmax);
	Double_t xx[2];
	xx[0] = 5.;
	xx[1] = 3;
	hsd->Fill(xx);
	xx[0] = 7.;
	xx[1] = -3;
	hsd->Fill(xx);
	cout << "hsd->GetNbins() " << hsd->GetNbins()<<endl;
	hsd->Fill(xx, 3.);
	cout << " hsd->GetNbins() " << hsd->GetNbins()<<endl;
	Int_t idx = hsd->GetBin(xx);
	cout << " hsd->GetBin() " << idx <<endl;
	Int_t ic[2];
	Double_t cc = hsd->GetBinContent(idx,ic);
	cout << "idx " << idx << " cont " << cc 
		<< " at " << ic[0]<< ", " << ic[1]<<endl;
}
