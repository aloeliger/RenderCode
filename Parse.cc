#include "TROOT.h"
#include <string>
#include "HelperFunctions.cc"

TH1F* ParseBackground(TFile* TheOpenFile, string Line)
{
  string HistogramName = PopNextToken(&Line);
  TH1F* TheHistogram = (TH1F*) TheOpenFile->Get(HistogramName.c_str());  
  TheHistogram->SetLineColor(kBlack);
  string ColorToken = PopNextToken(&Line);
  SetHistogramFillColor(TheHistogram, ColorToken);
  // in the future I picture some sort of while loop here looping over the raminder of the modifier tokens for the backgrounds
  // and sorting them effectively and then doing what they say.
  // for now the only other option I want to add in is  is color. Just assume that everything from here until the 
  // end of the line is a valid root color
  
  return TheHistogram;
}

TH1F* ParseSignal(TFile* TheOpenFile, string Line)
{
  string HistogramName = PopNextToken(&Line);  
  TH1F* TheHistogram = (TH1F*) TheOpenFile->Get(HistogramName.c_str());
  /*
  TheHistogram->SetMarkerStyle(20);
  TheHistogram->SetLineColor(kBlack);
  TheHistogram->Sumw2();
  */
  return TheHistogram;
}

TH1F* AddExistingHistograms(std::vector<TH1F*> BackgroundHistograms, string Line)
{
  string FirstHistogramName = PopNextToken(&Line);  
  string SecondHistogramName = PopNextToken(&Line);  
  string NewName = PopNextToken(&Line);
  string NewTitle = PopNextToken(&Line);
  //now we search for these histograms in the vector we were given
  TH1F* FirstHistogram = RetrieveHistogramFromVector(BackgroundHistograms,FirstHistogramName);
  TH1F* SecondHistogram = RetrieveHistogramFromVector(BackgroundHistograms,SecondHistogramName);
  
  if(!FirstHistogram || !SecondHistogram) std::cout<<"One or more added histograms couldn't be found!"<<std::endl;  
  
  TH1F* ResultHistogram = new TH1F(NewName.c_str(),NewTitle.c_str(),FirstHistogram->GetNbinsX(),FirstHistogram->GetXaxis()->GetXmin(),FirstHistogram->GetXaxis()->GetXmax());
  ResultHistogram->Add(FirstHistogram,SecondHistogram);
  ResultHistogram->SetLineColor(kBlack);
  
  return ResultHistogram;
}

void ModifyExistingHistograms(std::vector<TH1F*> BackgroundHistograms, string Line)
{  
  string HistoName = PopNextToken(&Line);  
  string ColorToken = PopNextToken(&Line);
  TH1F* TheHistogram = RetrieveHistogramFromVector(BackgroundHistograms,HistoName);
  
  if(!TheHistogram) std::cout<<"Failed to find histogram to modify!"<<std::endl;
  
  TheHistogram->SetLineColor(kBlack);
  SetHistogramFillColor(TheHistogram,ColorToken);
}

TH1F* EnableDrawing(std::vector<TH1F*> BackgroundHistograms, string Line)
{
  string HistoName = PopNextToken(&Line);  
  TH1F* TheHistogram = RetrieveHistogramFromVector(BackgroundHistograms,HistoName);
  
  if(!TheHistogram) std::cout<<"Failed to find histogram to draw!"<<std::endl;
  return TheHistogram;
}

TLegend* ParseLegendDeclaration(string Line)
{  
  string DimOneS = PopNextToken(&Line);
  string DimTwoS = PopNextToken(&Line);
  string DimThreeS = PopNextToken(&Line);
  string DimFourS = PopNextToken(&Line);
  float DimOne = std::stof(DimOneS);
  float DimTwo = std::stof(DimTwoS);
  float DimThree = std::stof(DimThreeS);
  float DimFour = std::stof(DimFourS);
  TLegend* ReturnLegend = new TLegend(DimOne,DimTwo,DimThree,DimFour);
  return ReturnLegend;
}

void AddEntryToLegend(TLegend* TheLegend, std::vector<TH1F*> CompleteHistograms, string Line)
{
  string BaseHistogramName = PopNextToken(&Line);
  string LegendEntry = PopNextToken(&Line);
  string LegendOptions = PopNextToken(&Line);
  TH1F* BaseHistogram = RetrieveHistogramFromVector(CompleteHistograms, BaseHistogramName);
  TheLegend->AddEntry(BaseHistogram,LegendEntry.c_str(),LegendOptions.c_str());
}
