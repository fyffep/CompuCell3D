

#include <CompuCell3D/CC3D.h>

using namespace CompuCell3D;
using namespace std;

#include "PolygonInitializer.h"


PolygonInitializer::PolygonInitializer(): 
cellFieldG(0), sim(0), potts(0), 
xmlData(0), boundaryStrategy(0), automaton(0), cellInventoryPtr(0)
{}

PolygonInitializer::~PolygonInitializer() {
}

void PolygonInitializer::init(Simulator *simulator, CC3DXMLElement *_xmlData) {
    cerr<<"init of PolygonInitializer"<<endl;

    xmlData=_xmlData;
    potts = simulator->getPotts();
    cellInventoryPtr=& potts->getCellInventory();
    sim=simulator;
    cellFieldG = (WatchableField3D<CellG *> *)potts->getCellFieldG();
    fieldDim=cellFieldG->getDim();

    simulator->registerSteerableObject(this);



    CC3DXMLElementList regionVec = _xmlData->getElements("Region");

    CC3DXMLElement * gapXMLElem = _xmlData->getFirstElement("Gap");
    if (regionVec[i]->getFirstElement("Gap")) {
        cerr<<"agni gap=" << regionVec[i]->getFirstElement("Gap")->getUInt() << endl;
        // initData.gap = regionVec[i]->getFirstElement("Gap")->getUInt();
    }


    update(_xmlData,true);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PolygonInitializer::extraInit(Simulator *simulator){

    //PUT YOUR CODE HERE
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PolygonInitializer::start(){

  //PUT YOUR CODE HERE
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PolygonInitializer::step(const unsigned int currentStep){

    //REPLACE SAMPLE CODE BELOW WITH YOUR OWN

	CellInventory::cellInventoryIterator cInvItr;

	CellG * cell=0;

    cerr<<"currentStep="<<currentStep<<endl;

	for(cInvItr=cellInventoryPtr->cellInventoryBegin() ; cInvItr !=cellInventoryPtr->cellInventoryEnd() ;++cInvItr )

	{
		cell=cellInventoryPtr->getCell(cInvItr);
        cerr<<"cell.id="<<cell->id<<" vol="<<cell->volume<<endl;
    }

}

void PolygonInitializer::update(CC3DXMLElement *_xmlData, bool _fullInitFlag){

    //PARSE XML IN THIS FUNCTION

    //For more information on XML parser function please see CC3D code or lookup XML utils API

    automaton = potts->getAutomaton();

    ASSERT_OR_THROW("CELL TYPE PLUGIN WAS NOT PROPERLY INITIALIZED YET. MAKE SURE THIS IS THE FIRST PLUGIN THAT YOU SET", automaton)

    set<unsigned char> cellTypesSet;

    CC3DXMLElement * exampleXMLElem=_xmlData->getFirstElement("Example");

    if (exampleXMLElem){

        double param=exampleXMLElem->getDouble();

        cerr<<"param="<<param<<endl;

        if(exampleXMLElem->findAttribute("Type")){

            std::string attrib=exampleXMLElem->getAttribute("Type");

            // double attrib=exampleXMLElem->getAttributeAsDouble("Type"); //in case attribute is of type double

            cerr<<"attrib="<<attrib<<endl;

        }
    }

    //boundaryStrategy has information aobut pixel neighbors 

    boundaryStrategy=BoundaryStrategy::getInstance();

}

std::string PolygonInitializer::toString(){

   return "PolygonInitializer";
}

std::string PolygonInitializer::steerableName(){

   return toString();

}

