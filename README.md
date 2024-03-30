# SparseHistogram
The three dimensional histogram will take a lot of memory. To avoid this a sparse histogram is created   

The output.root file contains the corsika muon flux, The TH3D histograms were taking too much memory. But the data was sparse.  

THnSparseD is used to save the sparse data and the memory utilization went down by 90%
