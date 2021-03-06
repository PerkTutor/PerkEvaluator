/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerPerkEvaluatorAnatomyRolesWidget_h
#define __qSlicerPerkEvaluatorAnatomyRolesWidget_h

// Qt includes
#include "qSlicerWidget.h"
#include "qSlicerPerkEvaluatorModuleWidgetsExport.h"

#include "qSlicerPerkEvaluatorRolesWidget.h"

class qSlicerPerkEvaluatorAnatomyRolesWidgetPrivate;

/// \ingroup Slicer_QtModules_CreateModels
class Q_SLICER_MODULE_PERKEVALUATOR_WIDGETS_EXPORT 
qSlicerPerkEvaluatorAnatomyRolesWidget : public qSlicerPerkEvaluatorRolesWidget
{
  Q_OBJECT
public:
  qSlicerPerkEvaluatorAnatomyRolesWidget(QWidget *parent=0);
  virtual ~qSlicerPerkEvaluatorAnatomyRolesWidget();

protected slots:

  void onRolesChanged();

protected:
  QScopedPointer<qSlicerPerkEvaluatorAnatomyRolesWidgetPrivate> d_ptr;

  std::string getRolesHeader();
  std::string getCandidateHeader();
  
  std::vector< std::string > getAllRoles(); // Just a list of all roles
  std::string getNodeTypeForRole( std::string role ); // A list of the node types for that role
  std::string getNodeIDFromRole( std::string role ); // Get the ID of the node fulfilling the role

private:
  Q_DECLARE_PRIVATE(qSlicerPerkEvaluatorAnatomyRolesWidget);
  Q_DISABLE_COPY(qSlicerPerkEvaluatorAnatomyRolesWidget);

};

#endif
