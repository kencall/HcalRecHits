void plot2D(const char *infile, const char *histName, const char *histTitle, const char *xtitle, const char *ytitle, const char *outfile)
{

   gROOT->SetBatch(kTRUE);

   TCanvas *Canvas = new TCanvas("Canvas", "Canvas",260,124,998,761);

   Canvas->SetGridx();
   Canvas->SetGridy();

   gStyle->SetOptStat(0);

   TFile *f1 = new TFile(infile);

   if(f1->IsZombie()){
     cout << "Cannot open " << infile << "!" << endl;
     return;
   }

   TH2D *map = (TH2D*)f1->Get(histName);
   if(!histName){
     cout << "Cannot load " << histName << " from " << infile << "!" << endl;
     return;
   }

   map->SetTitle(histTitle);

   map->GetXaxis()->SetTitle(xtitle);
   map->GetYaxis()->SetTitle(ytitle);

   map->Draw("colorz");

   Canvas->SetLogz();
	
  Canvas->Modified();
  Canvas->cd();
  Canvas->SetSelected(Canvas);
  
  TImage *img = TImage::Create();
  
  img->FromPad(Canvas);
  img->WriteImage(outfile);

  delete img;
  delete Canvas;
  delete map;
  f1->Close();
  delete f1;

}
