from ROOT import *
import sys, os

xmlparser = TDOMParser()
xmlparser.SetValidate(False)
xmlparser.ParseFile(sys.argv[1])
doc = xmlparser.GetXMLDocument()
root_node = doc.GetRootNode()
print "root node name: ", root_node.GetNodeName()
node = root_node.GetChildren()
while node:
   if node.GetNodeType()== TXMLNode.kXMLElementNode:
      print "node Type:", node.GetNodeType(), ",", 
      print "node Name:", node.GetNodeName()
   node = node.GetNextNode()

# see content
node= root_node.GetChildren()
while node:
   if node.GetNodeName() == "Config":
      nodeList = node.GetAttributes()
      print nodeList
      for item in nodeList:
         print item.GetName(), item.GetValue()
   node = node.GetNextNode()
