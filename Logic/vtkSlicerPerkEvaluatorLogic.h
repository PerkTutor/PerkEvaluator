
// .NAME vtkSlicerPerkEvaluatorLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerPerkEvaluatorLogic_h
#define __vtkSlicerPerkEvaluatorLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSmartPointer.h"
#include "vtkXMLDataParser.h"

#include "vtkSlicerPerkEvaluatorModuleLogicExport.h"

#include "vtkTransformTimeSeries.h"



/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_PERKEVALUATOR_MODULE_LOGIC_EXPORT vtkSlicerPerkEvaluatorLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerPerkEvaluatorLogic *New();
  vtkTypeMacro(vtkSlicerPerkEvaluatorLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  void ImportFile( std::string fileName );
  
  double GetTotalTime() const;
  double GetMinTime() const;
  double GetMaxTime() const;
  double GetPlaybackTime() const;
  
  void SetPlaybackTime( double time );
  
  
protected:
  vtkSlicerPerkEvaluatorLogic();
  virtual ~vtkSlicerPerkEvaluatorLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);

private:

  vtkSlicerPerkEvaluatorLogic(const vtkSlicerPerkEvaluatorLogic&); // Not implemented
  void operator=(const vtkSlicerPerkEvaluatorLogic&);               // Not implemented
  
  void ClearData();
  double GetTimestampFromElement( vtkXMLDataElement* element );
  vtkTransformTimeSeries* UpdateToolList( std::string name );
  void CreateTransformNodes();
  
  typedef std::vector< vtkSmartPointer< vtkTransformTimeSeries > > TrajectoryContainerType;
  TrajectoryContainerType ToolTrajectories;
  
  typedef std::pair< double, std::string > AnnotationType;
  typedef std::vector< AnnotationType > AnnotationVectorType;
  AnnotationVectorType Annotations;
  
  double PlaybackTime;
};

#endif
