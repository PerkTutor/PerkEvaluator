/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkMRMLTransformRecorderNode.h,v $
  Date:      $Date: 2006/03/19 17:12:28 $
  Version:   $Revision: 1.6 $

=========================================================================auto=*/

#ifndef __vtkMRMLPerkEvaluatorNode_h
#define __vtkMRMLPerkEvaluatorNode_h

// Standard includes
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>

// VTK includes
#include "vtkCommand.h"
#include "vtkTransform.h"
#include "vtkObject.h"
#include "vtkObjectBase.h"
#include "vtkObjectFactory.h"


// Slicer includes
#include "vtkMRMLTransformableNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLLinearTransformNode.h"


// PerkEvaluator includes
#include "vtkSlicerPerkEvaluatorModuleMRMLExport.h"

// Includes from Transform Recorder
#include "vtkMRMLTransformBufferNode.h"

struct TransformTrajectory
{
  vtkMRMLTransformBufferNode* Buffer;
  vtkMRMLLinearTransformNode* Node;
};


class VTK_SLICER_PERKEVALUATOR_MODULE_MRML_EXPORT
vtkMRMLPerkEvaluatorNode : public vtkMRMLTransformableNode
{
public:
  vtkTypeMacro( vtkMRMLPerkEvaluatorNode, vtkMRMLTransformableNode );

  // Standard MRML node methods  
  static vtkMRMLPerkEvaluatorNode* New();  
  virtual vtkMRMLNode* CreateNodeInstance();
  virtual const char* GetNodeTagName() { return "PerkEvaluator"; };
  void PrintSelf( ostream& os, vtkIndent indent );
  virtual void ReadXMLAttributes( const char** atts );
  virtual void WriteXML( ostream& of, int indent );
  virtual void Copy( vtkMRMLNode *node );

  
protected:

  // Constructor/desctructor
  vtkMRMLPerkEvaluatorNode();
  virtual ~vtkMRMLPerkEvaluatorNode();
  vtkMRMLPerkEvaluatorNode ( const vtkMRMLPerkEvaluatorNode& ); // Required to prevent linking error
  void operator=( const vtkMRMLPerkEvaluatorNode& ); // Required to prevent linking error
  
  
public:

  // Analysis start/end times
  double GetMarkBegin();
  void SetMarkBegin( double newBegin );
  
  double GetMarkEnd();
  void SetMarkEnd( double newEnd );

  // Enumerate all of the possible needle orientations
  enum NeedleOrientationEnum{ PlusX, MinusX, PlusY, MinusY, PlusZ, MinusZ };  
  // Needle orientation
  void GetNeedleBase( double needleBase[ 4 ] );
  NeedleOrientationEnum GetNeedleOrientation();
  void SetNeedleOrientation( NeedleOrientationEnum needleOrietation );
  
  // Metrics directory
  std::string GetMetricsDirectory();
  void SetMetricsDirectory( std::string newMetricsDirectory );
  

  // Getters/setters associated with roles
  std::string GetTransformRole( std::string transformNodeName );
  std::string GetFirstTransformNodeName( std::string transformRole );
  void SetTransformRole( std::string transformNodeName, std::string newTransformRole );

  std::string GetAnatomyNodeName( std::string anatomyRole );
  std::string GetFirstAnatomyRole( std::string anatomyNodeName );
  void SetAnatomyNodeName( std::string anatomyRole, std::string newAnatomyNodeName );
  
  


  
private:

/* To store:
TransformRoleMap
AnatomyRoleMap
MarkEnd
MarkBegin
NeedleOrientation
MetricsDirectory
*/

  double MarkBegin;
  double MarkEnd;
  
  NeedleOrientationEnum NeedleOrientation;
  
  std::string MetricsDirectory;

  std::map< std::string, std::string > TransformRoleMap; // From transform node names to roles
  std::map< std::string, std::string > AnatomyNodeMap; // From roles to anatomy node names
  
};  

#endif