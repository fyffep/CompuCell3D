
#include <iostream>

#include <CompuCell3D/Field3D/Dim3D.h>
#include <CompuCell3D/Potts3D/Cell.h>
#include <CompuCell3D/Field3D/Field3D.h>
#include <CompuCell3D/Field3D/WatchableField3D.h>

#include "CleaverDolfinUtil.h"
#include "CellFieldCleaverSimulator.h"

#include <Cleaver/Cleaver.h>
#include <Cleaver/InverseField.h>
#include <Cleaver/FloatField.h>


//dolfin includes
#include <dolfin/mesh/Mesh.h>
#include <dolfin/mesh/CellType.h>
#include <dolfin/mesh/MeshEditor.h>
#include <boost/shared_ptr.hpp>

using namespace std;


void dolfinMeshInfo(void *_obj){
  
     cerr<<"_objInfo="<<_obj<<endl;
    cout<<"INSIDE OBJECT INFO"<<endl;
//     PyNewPlugin * plPtr=(PyNewPlugin *)_obj;
//     plPtr->getX();
    
    
    boost::shared_ptr<dolfin::Mesh> *meshPtr=(boost::shared_ptr<dolfin::Mesh> *)_obj;
    cerr<<"shared ptr meshPtr="<<meshPtr<<endl;
    cerr<<"raw ptr meshPtr="<<meshPtr->get()<<endl;
    cerr<<"NUMBER OF CELLS="<<meshPtr->get()->num_cells()<<endl;
 
}

void simulateCleaverMesh(void *_cellField,std::vector<unsigned char> _includeTypesVec){
  
  CompuCell3D::WatchableField3D<CompuCell3D::CellG*> * cellField=(CompuCell3D::WatchableField3D<CompuCell3D::CellG*> *)_cellField;
  
  CompuCell3D::Dim3D fieldDim=cellField->getDim();
  cerr<<"THIS IS FIELD DIM="<<fieldDim<<endl;
  
  CompuCell3D::CellFieldCleaverSimulatorNew cfcs;
  cfcs.setFieldDim(fieldDim);
  cfcs.setCellFieldPtr(cellField);
  
  for (int i=0 ; i < _includeTypesVec.size() ; ++i){
      cerr<<"THIS IS TYPE="<<(int)_includeTypesVec[i]<<endl;
  }
  
  set<unsigned char> cellTypesSet(_includeTypesVec.begin(),_includeTypesVec.end());
  cfcs.setIncludeCellTypesSet(cellTypesSet);


  Cleaver::InverseField inverseField = Cleaver::InverseField(&cfcs);

  bool verbose=true;
  std::vector<Cleaver::ScalarField*> fields;

  fields.push_back(&cfcs);
  fields.push_back(&inverseField);

  Cleaver::Volume volume(fields);
  Cleaver::TetMesh *mesh = Cleaver::createMeshFromVolume(volume, verbose);
//   
// // //   delete mesh;  
}

void buildCellFieldDolfinMeshUsingCleaver(void *_cellField,void *_dolfinMesh ,std::vector<unsigned char> _includeTypesVec,bool _verbose){
  
  CompuCell3D::WatchableField3D<CompuCell3D::CellG*> * cellField=(CompuCell3D::WatchableField3D<CompuCell3D::CellG*> *)_cellField;
  boost::shared_ptr<dolfin::Mesh> *dolfinMesh=(boost::shared_ptr<dolfin::Mesh> *)_dolfinMesh; 
  dolfin::Mesh & dolfinMeshRef = **dolfinMesh;
  
  CompuCell3D::Dim3D fieldDim=cellField->getDim();

  
  CompuCell3D::CellFieldCleaverSimulatorNew cfcs;
  cfcs.setFieldDim(fieldDim);
  cfcs.setCellFieldPtr(cellField);
  
  
  set<unsigned char> cellTypesSet(_includeTypesVec.begin(),_includeTypesVec.end());
  cfcs.setIncludeCellTypesSet(cellTypesSet);
  

  Cleaver::InverseField inverseField = Cleaver::InverseField(&cfcs);


  std::vector<Cleaver::ScalarField*> fields;

  fields.push_back(&cfcs);
  fields.push_back(&inverseField);

  Cleaver::Volume volume(fields);
  Cleaver::TetMesh *cleaverMesh = Cleaver::createMeshFromVolume(volume, _verbose);
  
  Cleaver::TetMesh & cleaverMeshRef=*cleaverMesh;
  
  //building dolfinMesh
  
  dolfin::MeshEditor editor;

  cerr<<"cleaverMesh.verts.size()="<<cleaverMeshRef.verts.size()<<endl;
  cerr<<"cleaverMesh.tets.size()="<<cleaverMeshRef.tets.size()<<endl;
  cerr<<"cleaverMesh="<<cleaverMesh<<endl;
  cerr<<"nFaces="<<cleaverMesh->nFaces<<endl;
  
  editor.open(dolfinMeshRef, dolfin::CellType::tetrahedron, 3, 3);
  editor.init_vertices(cleaverMeshRef.verts.size());  
  editor.init_cells(cleaverMeshRef.tets.size());
  
  //writing vertices to dolfin mesh 
  for(unsigned int i=0; i < cleaverMeshRef.verts.size(); i++)
  {
    editor.add_vertex(i,cleaverMeshRef.verts[i]->pos().x,cleaverMeshRef.verts[i]->pos().y,cleaverMeshRef.verts[i]->pos().z);      
  }
  
  //writing tetrahedrons to dolfin mesh 
  for(unsigned int i=0; i < cleaverMeshRef.tets.size(); i++)
  {
    
//     editor.add_cell(i,cleaverMeshRef.verts[0]->tm_v_index,cleaverMeshRef.verts[1]->tm_v_index,cleaverMeshRef.verts[2]->tm_v_index,cleaverMeshRef.verts[3]->tm_v_index);      
    editor.add_cell(i,cleaverMeshRef.tets[i]->verts[0]->tm_v_index,cleaverMeshRef.tets[i]->verts[1]->tm_v_index,cleaverMeshRef.tets[i]->verts[2]->tm_v_index,cleaverMeshRef.tets[i]->verts[3]->tm_v_index);          
  }

  editor.close();
  
  delete cleaverMesh;
 
 
}