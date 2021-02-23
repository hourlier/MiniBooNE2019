#include<iostream>

#include "TDOMParser.h"
#include "TXMLDocument.h"
#include "TXMLNode.h"
#include "TXMLAttr.h"
#include "TString.h"


using namespace std;
//int main(int argc, char *argv[]){
int understandXML(){
  string input_xml="temp.xml";
  TDOMParser xmlparser;
  xmlparser.SetValidate(false);
  xmlparser.ParseFile(input_xml.c_str());
  TXMLDocument* doc=xmlparser.GetXMLDocument();
  TXMLNode* root_node = doc->GetRootNode();
  TString node_name(root_node->GetNodeName());
  cout << "root node:"<< node_name << endl;
  TXMLNode* node = root_node->GetChildren();

  while (node) {
    if ( node->GetNodeType() == TXMLNode::kXMLElementNode ) {
      node_name=TString(node->GetNodeName());
      cout << node_name << endl;
    }
  node = node->GetNextNode();
  }
  return 0;

}
