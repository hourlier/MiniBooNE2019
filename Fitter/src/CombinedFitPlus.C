#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <math.h>

#include "FitMaster.h"
#include "SampleROOT.h"
#include "ErrorMatrixROOT.h"
#include "Util.h"
#include "Algo.h"
#include "AlgoSensitivity.h"

#include "WeightCalc.h"
#include "OscWeightCalc.h"
#include "POTWeightCalc.h"
#include "MsgLog.h"
#include "FuncStopwatch.h"

#include "TH2F.h"
#include "TFile.h"
#include "TList.h"
#include "TObjString.h"
#include "TObjArray.h"

#include "TDOMParser.h"
#include "TXMLDocument.h"
#include "TXMLNode.h"
#include "TXMLAttr.h"


using namespace std;

int main(int argc, char *argv[])
{

	if ( argc<2 ) {
		cout << endl;
		cout << Form("\t %s input1.xml [input2.xml]...",argv[0])<<endl;
		cout << endl;
		exit (0);
	}
	MsgLog::SetPrintType(MsgLog::kInfo, false);
	MsgLog::SetPrintPrefix(MsgLog::kInfo, false);
	string lineDivider(50, '-');

	for (Int_t ixml=1;ixml<argc;ixml++) {

		FitMaster* myFitMaster=FitMaster::Instance();
		string input_xml = argv[ixml];
		MsgInfo( "Parsing input file "+ input_xml);

		TDOMParser xmlparser;
		xmlparser.SetValidate(false);
		xmlparser.ParseFile(input_xml.c_str());
		TXMLDocument* doc=xmlparser.GetXMLDocument();
		TXMLNode* root_node = doc->GetRootNode();
		TString node_name(root_node->GetNodeName());
		if (node_name != "CombinedFitPlus") {
			cout << "File "<<input_xml<<" does not have a CombinedFitPlus root node"<<endl;
			cout << root_node->GetNodeName()<<endl;
			cout << "Skipping... "<<endl;
			exit (1);
		}

		TXMLNode* node = root_node->GetChildren();

		while (node) {
			if ( node->GetNodeType() == TXMLNode::kXMLElementNode ) {
				node_name=TString(node->GetNodeName());

  /****************************************************************************************
   *
   * Config
   *
   ****************************************************************************************/
   if ( node_name == "Config" ) {
   	MsgInfo(lineDivider);
   	MsgInfo("Configuring Combined Fit");
   	map<string, string> attr_map = Util::GetAttributeMap(node);
		string output_path      = attr_map["output_path"];
		string output_path_root = attr_map["output_file_stem"];
		string output_path_log  = attr_map["output_file_stem"] + ".log";
		string output_path_xml  = attr_map["output_file_stem"] + ".xml";
   	if ( output_path != "" ){
   		output_path_root = output_path + "/" + output_path_root;
   		output_path_log  = output_path + "/logs/" + output_path_log;
   		output_path_xml  = output_path + "/output_xml/" + output_path_xml;
   	}
   	else{
   		if(output_path_xml.rfind("/")!=string::npos){
   			output_path_xml.insert(output_path_xml.rfind("/"), "/output_xml");
   		}
   		else{
   			output_path_xml = "./output_path_xml/" + output_path_xml;
   		}
   	}
   	myFitMaster -> SetFitType(attr_map["fit_type"]);
   	myFitMaster -> SetOutputFileStem( output_path_root);
   	myFitMaster -> SetVerbosity(atoi(attr_map["verbosity"].c_str()));


   	MsgLog::SetFileOutput(output_path_log.c_str());
   	MsgLog::SetGlobalLogLevel(static_cast<MsgLog::EType>(myFitMaster->GetVerbosity()));
   	if(static_cast<int>(myFitMaster->GetVerbosity()>=4)) MsgLog::SetGlobalDebugLevel(stoi(attr_map["debugmode"]));
   	MsgInfo("Set GlobalLogLevel to " + MsgLog::GetGlobalLogLevelString());
   	MsgInfo(MsgLog::Form("Set GlobalDebuglevel to %d if log level is larger or equal than debug(4)", MsgLog::GetGlobalDebugLevel()));
   	MsgInfo(MsgLog::Form("Doing a %s fit", myFitMaster->GetFitType().c_str() ));
   	MsgInfo("ROOT output = " + output_path_root);
   	MsgInfo("Log  output = " + output_path_log);
   	MsgInfo("XML  backup = " + output_path_xml);
		system(Form("cp %s %s", input_xml.c_str(), (output_path_xml).c_str() ));


 /****************************************************************************************
   *
   * External Data
   *
   ****************************************************************************************/
} else if(node_name == "ExternalData"){
	MsgInfo(lineDivider);
	MsgInfo("Loading ExternalData(Only LSND is allowed now!)");
	map<string, string> attr_map = Util::GetAttributeMap(node);
	TString topdir(attr_map.find("path")->second);
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if ( node_child_name == "Sample" ) {
				map<string, string> child_attr_map = Util::GetAttributeMap(node_child);
				if(child_attr_map.find("name")->second != "LSND"){
					MsgFatal("Only LSND External Data should be included!");
					continue;
				}

				TXMLNode* node_grandchild = node_child->GetChildren();
				while (node_grandchild) {
					if ( node_grandchild->GetNodeType() == TXMLNode::kXMLElementNode ) {
						TString node_grandchild_name = TString(node_grandchild->GetNodeName());
						if ( node_grandchild_name == "WeightCalc" ) {
							map<string, string> wc_attr_map = Util::GetAttributeMap(node_grandchild);
							std::shared_ptr<WeightCalc> wc = myFitMaster->GetWeightCalc(wc_attr_map.find("name")->second);
							for(auto const& wc_attr : wc_attr_map){
								if (wc_attr.first != "name")
									wc->SetParameter(wc_attr.first, atof(wc_attr.second.c_str()));
							}		  
							myFitMaster->SetLSNDWeightCalc(wc);
						} else
							MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_grandchild_name.Data()));
					}
					node_grandchild= node_grandchild->GetNextNode();
				}
			} // end if input is Sample 
			else
			MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
		} // end of if(kXMLElementNode)
		node_child = node_child->GetNextNode();
	} // end of while(node_child)

  /****************************************************************************************
   *
   * MC
   *
   ****************************************************************************************/
} else if ( node_name == "MC" ) {
	MsgInfo(lineDivider);
	MsgInfo("Loading MC");
	map<string, string> attr_map = Util::GetAttributeMap(node);
	TString topdir(attr_map.find("path")->second);
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if ( node_child_name == "Sample" ) {
				map<string, string> child_attr_map = Util::GetAttributeMap(node_child);
				std::shared_ptr<Sample> s = std::make_shared<Sample>();
				s->SetName(child_attr_map.find("name")->second);
				s->SetInputNtuple(Form("%s/%s",topdir.Data(),(child_attr_map.find("file")->second).c_str()));
				s->SetNtupleID(atoi((child_attr_map.find("ntuple")->second).c_str()));
				if (child_attr_map.find("downsample")!=child_attr_map.end()) 
					s->SetDownsample(atof((child_attr_map.find("downsample")->second).c_str()));

				TXMLNode* node_grandchild = node_child->GetChildren();
				while (node_grandchild) {
					if ( node_grandchild->GetNodeType() == TXMLNode::kXMLElementNode ) {
						TString node_grandchild_name = TString(node_grandchild->GetNodeName());
						if ( node_grandchild_name == "WeightCalc" ) {
							map<string, string> wc_attr_map = Util::GetAttributeMap(node_grandchild);
							std::shared_ptr<WeightCalc> wc = myFitMaster->GetWeightCalc(wc_attr_map.find("name")->second);
							for(auto const& wc_attr : wc_attr_map){
								if (wc_attr.first != "name")
									wc->SetParameter(wc_attr.first, atof(wc_attr.second.c_str()));
							}		  
							s->AddWeightCalc(wc);
						} else if ( node_grandchild_name == "Filter" ) {
							std::shared_ptr<Filter> filter = std::make_shared<Filter>();
							map<string, string> filter_attr_map = Util::GetAttributeMap(node_grandchild);
							filter -> SetName        (     filter_attr_map.find("name")->second);
							filter -> SetCutParameter(     filter_attr_map.find("par") ->second);
							filter -> SetCutParMin   (atof(filter_attr_map.find("min") ->second.c_str()));
							filter -> SetCutParMax   (atof(filter_attr_map.find("max") ->second.c_str()));
							s->AddFilter(filter);
						} else if (node_grandchild_name == "FillBackgroundHists" ) {
							map<string, string> fillbkg_attr_map = Util::GetAttributeMap(node_grandchild);
							s->FillBackgroundHists(Util::atob(fillbkg_attr_map.find("val")->second));
						} else
							MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_grandchild_name.Data()));
					}
					node_grandchild= node_grandchild->GetNextNode();
				}
				s->FillVector();
				myFitMaster->AddMCSample(s); 
			} // end if input is Sample 
			else
			MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
		} // end of if(kXMLElementNode)
		node_child = node_child->GetNextNode();
	} // end of while(node_child)
  /****************************************************************************************
   *
   * Data
   *
   ****************************************************************************************/
} else if (node_name == "Data") {
	MsgInfo(lineDivider);
	MsgInfo("Loading Data");
	map<string, string> attr_map = Util::GetAttributeMap(node);
	TString topdir(attr_map["path"]);
	myFitMaster->SetDataPOT(atof(attr_map["pot"].c_str()));
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if ( node_child_name == "Sample" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				std::shared_ptr<Sample> s = std::make_shared<Sample>();
				s->SetName(attr_map["name"]);
				s->SetInputNtuple(Form("%s/%s",topdir.Data(),attr_map["file"].c_str()));
				s->SetNtupleID(atoi(attr_map["ntuple"].c_str()));
				TXMLNode* node_grandchild = node_child->GetChildren();
				while (node_grandchild) {
					if ( node_grandchild->GetNodeType() == TXMLNode::kXMLElementNode ) {
						TString node_grandchild_name = TString(node_grandchild->GetNodeName());
						if ( node_grandchild_name == "WeightCalc" ) {
							map<string, string> wc_attr_map = Util::GetAttributeMap(node_grandchild);
							std::shared_ptr<WeightCalc> wc = myFitMaster->GetWeightCalc(wc_attr_map.find("name")->second);
							for(auto const& wc_attr : wc_attr_map){
								if (wc_attr.first != "name")
									wc->SetParameter(wc_attr.first, atof(wc_attr.second.c_str()));
							}
							s->AddWeightCalc(wc);
						} else if ( node_grandchild_name == "Filter" ) {
							std::shared_ptr<Filter> filter = std::make_shared<Filter>();
							map<string, string> filter_attr_map = Util::GetAttributeMap(node_grandchild);
							filter -> SetName        (     filter_attr_map.find("name")->second);
							filter -> SetCutParameter(     filter_attr_map.find("par") ->second);
							filter -> SetCutParMin   (atof(filter_attr_map.find("min") ->second.c_str()));
							filter -> SetCutParMax   (atof(filter_attr_map.find("max") ->second.c_str()));
							s->AddFilter(filter);
						} else
						MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_grandchild_name.Data()));
						
					}
					node_grandchild= node_grandchild->GetNextNode();
				}

				s->FillVector();
				myFitMaster->AddDataSample(s); 
			} else
			MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
		}
		node_child = node_child->GetNextNode();
	}
   /****************************************************************************************
   *
   * Fake Data
   *
   ****************************************************************************************/
} else if (node_name == "FakeData") {
	MsgInfo(lineDivider);
	MsgInfo("Setting Fake Data");
	map<string, string> attr_map = Util::GetAttributeMap(node);
	MsgInfo( "Doing " + attr_map["number_of_experiments"] + " fake experiment(s)");
	MsgInfo( "Random seed: " + attr_map["random_seed"]);
	myFitMaster->DoFakeDataFits(true);
	myFitMaster->SetNumberOfFakeDataFits(atoi(attr_map["number_of_experiments"].c_str()));
	myFitMaster->SetFakeDataRandomSeed(atoi(attr_map["random_seed"].c_str()));
	if (attr_map.find("total_pot") != attr_map.end())
		myFitMaster->SetTotalPOT(atof(attr_map["total_pot"].c_str()));
	if (attr_map.find("add_fake_data_to_real_data") != attr_map.end() && 
		Util::atob(attr_map["add_fake_data_to_real_data"]))
		myFitMaster->AddFakeDataToRealData(true);
	MsgInfo("Fake data parameters:");
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if ( node_child_name == "FakeParameter" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				MsgInfo( attr_map["name"] + " = " + attr_map["val"]);
				myFitMaster->SetFakeDataParameter(attr_map["name"],atof(attr_map["val"].c_str()));
			} else
			MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
		}
		node_child = node_child->GetNextNode();
	}

  /****************************************************************************************
   *
   * Matrices
   *
   ****************************************************************************************/
} else if (node_name == "Matrices") {
	MsgInfo(lineDivider);
	MsgInfo("Loading Matrices");
	map<string, string> attr_map = Util::GetAttributeMap(node);
	TString topdir(attr_map["path"]);
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			//---------- Binning -------------
			if ( node_child_name == "Binning" ) {
				MsgDebug(2, "Matrices::Binning");
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				TString sample_name = attr_map["sample_name"];
				TString order       = attr_map["order"];
				vector<Double_t> binvec=Util::MakeVectorD(attr_map["bins"]);
				std::shared_ptr<Sample> s=myFitMaster->GetMCSample(sample_name.Data());
				if ( s != NULL ) s->SetBins(binvec);
				else MsgWarning(MsgLog::Form("Sample %s not yet loaded. Binning might not work properly for this sample.", sample_name.Data()));

				if ( !sample_name.Contains("Signal") ) {
					sample_name.ReplaceAll("MC","Data");
					s=myFitMaster->GetDataSample(sample_name.Data());
					if ( s != NULL ) s->SetBins(binvec);
					else MsgWarning(MsgLog::Form("Can't set binning to %s. Sample not found.", sample_name.Data()));
				}
			//---------- Matrix -------------
			} else if ( node_child_name == "Matrix" ) {
				MsgDebug(2, "Matrices::Matrix");
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				std::shared_ptr<ErrorMatrix> em = std::make_shared<ErrorMatrix>();
				em->SetName(attr_map["name"]);
				em->FillMatrix(Form("%s/%s",topdir.Data(),attr_map["file"].c_str()));
				myFitMaster->AddFracErrorMatrix(em); 
			//---------- CorrectMSstat -------------
			} else if ( node_child_name == "CorrectMSstat" ) {
				MsgDebug(2, "Matrices::CorrectMSstat");
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				vector<Double_t>    mdp = Util::MakeVectorD(attr_map["ms_to_data_pot"]);
				vector<string>  sn = Util::MakeVectorS(attr_map["sample"]);
				map<string, Double_t> ms_to_data_pot_map;
				for (unsigned int i=0;i<sn.size();i++) {
					ms_to_data_pot_map[sn[i]]=mdp[i];
				}

	      		//need event vector for this
				MsgDebug(2, "generating event vector");
				myFitMaster->UpdateHistograms();
				myFitMaster->BuildEventVector();
				myFitMaster->CorrectMSstat(attr_map["matrix"],ms_to_data_pot_map);
			//---------- AddMCStatError -------------
			} else if ( node_child_name == "AddMCStatError" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				MsgInfo( MsgLog::Form("Setting %s to %s", node_child_name.Data() , attr_map["opt"].c_str() ));
				if ( Util::atob(attr_map["opt"]) ) {
					//need event vector for this
					myFitMaster->UpdateHistograms();
					myFitMaster->BuildEventVector();
					Util::AddMonteCarloStatError(myFitMaster);
				}
			//---------- AddSignalStatError -------------
			} else if ( node_child_name == "AddSignalStatError" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				MsgInfo( MsgLog::Form("Setting %s to %s", node_child_name.Data() , attr_map["opt"].c_str() ));
				if ( Util::atob(attr_map["opt"]) ) myFitMaster->AddSignalStatError(true);
			//---------- AddStatError -------------
			} else if ( node_child_name == "AddStatError" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				MsgInfo( MsgLog::Form("Setting %s to %s", node_child_name.Data() , attr_map["opt"].c_str() ));
				if ( Util::atob(attr_map["opt"]) ) myFitMaster->AddStatError(true);
			//---------- Multisim -------------
			} else if (node_child_name == "MultisimCV" ) {
              //this can be used to set the cv for multisims (otherwise it reads it from the file)
              //useful when the multisim in a file does not reflect the stat error and you want
              //to use the built in stat error
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				myFitMaster->GetFracErrorMatrix(attr_map["matrix"])->SetCV(Util::MakeVectorD(attr_map["cv"],(char*) ", "));
			} 
			else
				MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
		}
		node_child = node_child->GetNextNode();
	}
	myFitMaster->UpdateHistograms();
	myFitMaster->BuildEventVector();
	myFitMaster->RecalculateTotalFracErrorMatrix();
  /****************************************************************************************
   *
   * Rebin
   *
   ****************************************************************************************/

} else if (node_name == "Rebin") {
	// Example: <Sample name="Nuebar"  type="Data, MC" vec="1,  1,  2,  3,  4,  5,  5,  6,  6,  7,  8,  9,  9,  9,  10,  11,  11,  11" />
	// Each Sample will be saved into the following
	// rebin_map["Nuebar Data"] = vectorize(vec)
	// rebin_map["Nuebar MC"] = vectorize(vec)
	// where vectorize change the string into vector

	MsgInfo(lineDivider);
	MsgInfo("Setting rebin vectors");
	//before rebinning need to fill the histograms once
	map<string, vector<Int_t> > rebin_map;
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if ( node_child_name == "Sample" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);

				TString name(attr_map["name"]);
				TString type(attr_map["type"]);
				vector<Int_t> rebin_vec=Util::MakeVectorI(attr_map["vec"]);

				type.ReplaceAll(" ","");
				TObjArray* typearray = type.Tokenize(",");
				if (typearray->GetEntries() > 0 ) {
					TIter iType(typearray->MakeIterator());
					while (TObjString* os=(TObjString*)iType()) {
						TString stype = os->GetString();
						stype.Prepend(" ");
						stype.Prepend(name);
						string sample_name = stype.Data();
						rebin_map[sample_name] = rebin_vec;
					}
				} else {
					string sample_name = name.Data();
					rebin_map[sample_name] = rebin_vec;
				}
			} 
			else // node_child_name != "Sample"
			{ 
				MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
			}
		}
		node_child = node_child->GetNextNode(); 
	}
	
	if (rebin_map.size() > 0)
		myFitMaster->Rebin(rebin_map);

  /****************************************************************************************
   *
   * Algorithm
   *
   ****************************************************************************************/
} else if (node_name == "Algo") {
	MsgInfo(lineDivider);
	MsgInfo("Configuring fit algorithm");
	
	map<string, string> algo_attr = Util::GetAttributeMap(node);
	std::shared_ptr<Algo> algo = myFitMaster->GetAlgo((algo_attr.find("name")->second));
	TXMLNode* node_child = node->GetChildren();
	while (node_child) {
		if ( node_child->GetNodeType() == TXMLNode::kXMLElementNode ) {
			TString node_child_name = TString(node_child->GetNodeName());
			if (node_child_name == "Parameter" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				algo->AddParameter(attr_map["name"], atoi(attr_map["nbins"].c_str()), 
					atof(attr_map["min"].c_str()), atof(attr_map["max"].c_str()), 
					Util::atob(attr_map["logscale"]));
			} else if (node_child_name == "Option" ) {
				map<string, string> attr_map = Util::GetAttributeMap(node_child);
				algo->SetOption(attr_map["name"], Util::atob(attr_map["val"]));
			} else {
				MsgWarning(MsgLog::Form("Sample node child %s not recognized! Skipping... " , node_child_name.Data()));
			}
		}
		node_child= node_child->GetNextNode(); 
	}
	myFitMaster->AddAlgoToRun(algo);
} 
else {
	MsgWarning(MsgLog::Form("Unrecognized node name %s. Skipping...", node->GetNodeName()));
}
}
node = node->GetNextNode();  
} // while(node)

myFitMaster->Dump();
MsgLog::Flush();
myFitMaster->Run();


  } //loop over input files
  FuncStopwatch::Report();
}
