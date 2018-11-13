#include "TROOT.h"
#include <string>
#include "Parse.cc"
#include "/afs/hep.wisc.edu/home/aloeliger/ScriptsAndMacros/MakeRatioPlot.cc"
#include "/afs/hep.wisc.edu/home/aloeliger/ScriptsAndMacros/tdrstyle.C"
#include "/afs/hep.wisc.edu/home/aloeliger/ScriptsAndMacros/CMS_lumi.C"

void RenderHistos(string FileName)
{
  setTDRStyle();

  gStyle->SetOptStat(0);
  
  string Line="";
  ifstream ConfigFile (FileName.c_str());
  TFile* OpenFile;
  TH1F* Data;
  std::vector<TH1F*> BackgroundDistributions;
  std::vector<TH1F*> HistogramsToBeDrawn;
  bool DrawTheLegend = false;
  bool DrawARatioPlot = false;
  bool DrawCMSLumiInfo = false;
  TLegend* TheLegend;
  string TheTitle = "";
  string XaxisTitle = "";
  string YaxisTitle = "";
  if(ConfigFile.is_open())
    {
      while(getline(ConfigFile,Line))
	{
	  string InitialToken = PopNextToken(&Line);
	  //maybe need to start splitting these up by major and minor tokens?
	  if(InitialToken == "File")
	    { 
	      std::cout<<"Opening a file..."<<std::endl;
	      string FilePath = Line.substr(Line.find_first_of(":")+1,Line.length());
	      OpenFile = new TFile(FilePath.c_str(),"READ");  	      
	    }
	  else if(InitialToken == "Background")
	    {
	      std::cout<<"Adding Background Distribution"<<std::endl;
	      if(OpenFile)
		{
		  BackgroundDistributions.push_back((TH1F*)ParseBackground(OpenFile,Line));
		}
	      else
		{
		  std::cout<<"Attempted to open a distribution with no open file!"<<std::endl;
		  std::cout<<"The line is: "+Line<<std::endl;
		}
	    }
	  else if(InitialToken == "Signal")
	    {
	      std::cout<<"Adding Signal Distribution"<<std::endl;
	      if(OpenFile)
		{
		  Data = (TH1F*)ParseSignal(OpenFile,Line);
		}
	      else
		{
		  std::cout<<"Attempted to open a distribution with no open file!"<<std::endl;
		  std::cout<<"The line is: "+Line<<std::endl;
		}
	    }
	  else if (InitialToken == "AddExisting")
	    {
	      std::cout<<"Adding Histograms Together..."<<std::endl;
	      BackgroundDistributions.push_back((TH1F*)AddExistingHistograms(BackgroundDistributions,Line));
	    }
	  else if(InitialToken == "ModifyExisting")
	    {
	      std::cout<<"Modifying Existing Histograms..."<<std::endl;
	      ModifyExistingHistograms(BackgroundDistributions,Line);
	    }
	  else if(InitialToken == "EnableDrawing")
	    {
	      std::cout<<"Enabling Histogram to be Drawn..."<<std::endl;
	      HistogramsToBeDrawn.push_back((TH1F*)EnableDrawing(BackgroundDistributions,Line));
	    }
	  else if(InitialToken == "EnableLegend")
	    {
	      std::cout<<"Enabling Legend..."<<std::endl;
	      DrawTheLegend = true;
	    }
	  else if(InitialToken == "DeclareLegend")
	    {
	      std::cout<<"Setting Up Legend..."<<std::endl;
	      TheLegend = ParseLegendDeclaration(Line);
	    }
	  else if(InitialToken == "AddEntryToLegend")
	    {
	      std::cout<<"Adding Entry to Legend..."<<std::endl;
	      AddEntryToLegend(TheLegend, HistogramsToBeDrawn, Line);
	    }
	  else if(InitialToken == "XAxisTitle")
	    {
	      std::cout<<"Setting X Axis Title..."<<std::endl;
	      XaxisTitle = PopNextToken(&Line);
	    }
	  else if(InitialToken == "YAxisTitle")
	    {
	      std::cout<<"Setting Y Axis Title..."<<std::endl;
	      YaxisTitle = PopNextToken(&Line);
	    }
	  else if(InitialToken == "Title")
	    {
	      std::cout<<"Setting Title..."<<std::endl;
	      TheTitle = PopNextToken(&Line);
	    }
	  else if(InitialToken == "MakeRatioPlot")
	    {
	      std::cout<<"Enabling Ratio Plot"<<std::endl;
	      DrawARatioPlot = true;
	    }
	  else if(InitialToken == "CMSLumi")
	    {
	      std::cout<<"Setting up CMS Lumi Info..."<<std::endl;
	      extraText = PopNextToken(&Line);
	      lumi_sqrtS = PopNextToken(&Line);
	      DrawCMSLumiInfo = true;	      
	      writeExtraText=true;
	    }
	  //empty line. nothing to do or see here.
	  else if(InitialToken.empty()){}
	  else
	    {
	      std::cout<<"Unrecognized Input in file!"<<std::endl;
	    }
	}//end of parse loop
      //just some temporary stuff to draw the stuff we just did      
      std::cout<<"Drawing..."<<std::endl;
      TCanvas* FinalCanvas = new TCanvas("TheCanvas","TheCanvas");	  
      TPad* CurrentPad = FinalCanvas;
      THStack* BackgroundStack=new THStack("hs","");
      std::cout<<"Drawing Backgrounds and Signal"<<std::endl;
      while(!HistogramsToBeDrawn.empty())
	{
	  TH1F* CurrentHistogram = HistogramsToBeDrawn.back();
	  HistogramsToBeDrawn.pop_back();
	  BackgroundStack->Add(CurrentHistogram,"hist");
	}	  
      Data->SetMarkerStyle(20);
      Data->SetLineColor(kBlack);
      Data->Sumw2();
      
      if(DrawARatioPlot) {CurrentPad = MakeRatioPlot(FinalCanvas,BackgroundStack,Data); CurrentPad->cd();}
      
      Data->Draw();
      BackgroundStack->Draw("SAME");
      Data->Draw("SAME");

      std::cout<<"Setting up axes"<<std::endl;
      Data->GetXaxis()->SetTitle(XaxisTitle.c_str());
      Data->GetYaxis()->SetTitle(YaxisTitle.c_str());
      Data->SetTitle(TheTitle.c_str());      
      
      if(DrawCMSLumiInfo) CMS_lumi(CurrentPad, 0 , 33);      

      std::cout<<"Drawing the legend"<<std::endl;      
      if(DrawTheLegend) TheLegend->Draw();

      std::cout<<"Drawing the canvas"<<std::endl;
      FinalCanvas->Draw();
            
    }// end  of successful file conditions
  else
    {
      std::cout<<"Failed to open file!"<<std::endl;
      std::cout<<"The file you told me to open was: "+FileName<<std::endl;
    }//end of failed file conditions
}
