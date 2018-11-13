#include "TROOT.h"
#include <string>

string PopNextToken(string* Line)
{
  string Token = Line->substr(0,Line->find_first_of(":"));
  Line->erase(0,Line->find_first_of(":")+1);
  return Token;
}

void SetHistogramFillColor(TH1F* TheHistogram, string ColorToken)
{
  if(ColorToken == "kRed")
    {
      TheHistogram->SetFillColor(kRed);
    }
  else if(ColorToken == "kPink")
    {
      TheHistogram->SetFillColor(kPink);
    }
  else if(ColorToken == "kMagenta")
    {
      TheHistogram->SetFillColor(kMagenta);
    }
  else if(ColorToken == "kViolet")
    {
      TheHistogram->SetFillColor(kViolet);
    }
  else if(ColorToken == "kBlue")
    {
      TheHistogram->SetFillColor(kBlue);
    }
  else if(ColorToken == "kAzure")
    {
      TheHistogram->SetFillColor(kAzure);
    }
  else if(ColorToken == "kCyan")
    {
      TheHistogram->SetFillColor(kCyan);
    }
  else if(ColorToken == "kTeal")
    {
      TheHistogram->SetFillColor(kTeal);
    }
  else if(ColorToken == "kGreen")
    {
      TheHistogram->SetFillColor(kGreen);
    }
  else if(ColorToken == "kSpring")
    {
      TheHistogram->SetFillColor(kSpring);
    }
  else if(ColorToken == "kYellow")
    {
      TheHistogram->SetFillColor(kYellow);
    }
  else if(ColorToken == "kOrange")
    {
      TheHistogram->SetFillColor(kOrange);
    }
}

TH1F* RetrieveHistogramFromVector(std::vector<TH1F*> HistogramCollection, string HistoName)
{
  TH1F* FoundHisto;
  for(auto Index = HistogramCollection.begin(); Index != HistogramCollection.end(); Index++)
    {
      if((*Index)->GetName() == HistoName)FoundHisto = *Index;
    }
  return FoundHisto;
}
