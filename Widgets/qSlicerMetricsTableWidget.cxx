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

// FooBar Widgets includes
#include "qSlicerMetricsTableWidget.h"

#include <QtGui>


// Helper functions -------------------------------------------------------------

#include "qSlicerApplication.h"
#include "qSlicerModuleManager.h"
#include "qSlicerAbstractCoreModule.h"

// TODO: This should really be a helper function
vtkMRMLAbstractLogic* qSlicerMetricsTableWidget
::GetSlicerModuleLogic( std::string moduleName )
{
  qSlicerAbstractCoreModule* Module = qSlicerApplication::application()->moduleManager()->module( moduleName.c_str() );
  if ( Module != NULL )
  {
    return Module->logic();
  }
  return NULL;
}


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_CreateModels
class qSlicerMetricsTableWidgetPrivate
  : public Ui_qSlicerMetricsTableWidget
{
  Q_DECLARE_PUBLIC(qSlicerMetricsTableWidget);
protected:
  qSlicerMetricsTableWidget* const q_ptr;

public:
  qSlicerMetricsTableWidgetPrivate( qSlicerMetricsTableWidget& object);
  ~qSlicerMetricsTableWidgetPrivate();
  virtual void setupUi(qSlicerMetricsTableWidget*);
};

// --------------------------------------------------------------------------
qSlicerMetricsTableWidgetPrivate
::qSlicerMetricsTableWidgetPrivate( qSlicerMetricsTableWidget& object) : q_ptr(&object)
{
}

qSlicerMetricsTableWidgetPrivate
::~qSlicerMetricsTableWidgetPrivate()
{
}


// --------------------------------------------------------------------------
void qSlicerMetricsTableWidgetPrivate
::setupUi(qSlicerMetricsTableWidget* widget)
{
  this->Ui_qSlicerMetricsTableWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerMetricsTableWidget methods

//-----------------------------------------------------------------------------
qSlicerMetricsTableWidget
::qSlicerMetricsTableWidget(QWidget* parentWidget) : Superclass( parentWidget ) , d_ptr( new qSlicerMetricsTableWidgetPrivate(*this) )
{
  this->MetricsTableNode = NULL;
  this->PerkEvaluatorLogic = vtkSlicerPerkEvaluatorLogic::SafeDownCast( qSlicerMetricsTableWidget::GetSlicerModuleLogic( "PerkEvaluator" ) );
  this->setup();
}


qSlicerMetricsTableWidget
::~qSlicerMetricsTableWidget()
{
}


void qSlicerMetricsTableWidget
::setup()
{
  Q_D(qSlicerMetricsTableWidget);

  d->setupUi(this);

  connect( d->MetricsTableNodeComboBox, SIGNAL( currentNodeChanged( vtkMRMLNode* ) ), this, SLOT( onMetricsTableNodeChanged( vtkMRMLNode* ) ) );
  
  connect( d->ClipboardButton, SIGNAL( clicked() ), this, SLOT( onClipboardButtonClicked() ) );
  d->ClipboardButton->setIcon( QIcon( ":/Icons/Small/SlicerEditCopy.png" ) );

  this->updateWidget();  
}

vtkMRMLTableNode* qSlicerMetricsTableWidget
::getMetricsTableNode()
{
  Q_D(qSlicerMetricsTableWidget);

  return this->MetricsTableNode;
}


void qSlicerMetricsTableWidget
::setMetricsTableNode( vtkMRMLNode* newMetricsTableNode )
{
  Q_D(qSlicerMetricsTableWidget);

  d->MetricsTableNodeComboBox->setCurrentNode( newMetricsTableNode );
  // If it is a new table node, then the onTransformBufferNodeChanged will be called automatically
}


void qSlicerMetricsTableWidget
::onMetricsTableNodeChanged( vtkMRMLNode* newMetricsTableNode )
{
  Q_D(qSlicerMetricsTableWidget);

  this->qvtkDisconnectAll();

  this->MetricsTableNode = vtkMRMLTableNode::SafeDownCast( newMetricsTableNode );

  this->qvtkConnect( this->MetricsTableNode, vtkCommand::ModifiedEvent, this, SLOT( onMetricsTableNodeModified() ) );

  this->updateWidget();

  emit metricsTableNodeChanged( this->MetricsTableNode );
}


void qSlicerMetricsTableWidget
::onMetricsTableNodeModified()
{
  this->updateWidget();
  emit metricsTableNodeModified(); // This should allows parent widgets to update themselves
}


void qSlicerMetricsTableWidget
::OnClipboardButtonClicked()
{
  Q_D( qSlicerMetricsTableWidget );

  // TODO: Should this be taken directly from the metrics table node?
  // Grab all of the contents from whatever is currently on the metrics table
  QString clipString = QString( "" );

  for ( int i = 0; i < d->MetricsTable->rowCount(); i++ )
  {
    for ( int j = 0; j < d->MetricsTable->columnCount(); j++ )
    {
      QTableWidgetItem* currentItem = d->MetricsTable->item( i, j );
      clipString.append( currentItem->text() );
      clipString.append( QString( "\t" ) );
    }
    clipString.append( QString( "\n" ) );
  }

  QApplication::clipboard()->setText( clipString );
}


void qSlicerMetricsTableWidget
::updateWidget()
{
  Q_D(qSlicerMetricsTableWidget);

  d->MetricsTableNodeComboBox->setCurrentNode( this->MetricsTableNode );

  // Set up the table
  d->MetricsTable->clear();
  d->MetricsTable->setRowCount( 0 );
  d->MetricsTable->setColumnCount( 0 );

  if ( this->MetricsTableNode == NULL )
  {
    return;
  }

  QStringList MetricsTableHeaders;
  MetricsTableHeaders << "Metric" << "Value";
  d->MetricsTable->setRowCount( this->MetricsTableNode->GetTable()->GetNumberOfRows() );
  d->MetricsTable->setColumnCount( 2 );
  d->MetricsTable->setHorizontalHeaderLabels( MetricsTableHeaders );
  d->MetricsTable->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
  
  // Add the computed values to the table
  for ( int i = 0; i < this->MetricsTableNode->GetTable()->GetNumberOfRows(); i++ )
  {
    QString nameString;
    nameString = nameString + QString( this->MetricsTableNode->GetTable()->GetValueByName( i, "TransformName" ).ToString() );
    nameString = nameString + QString( " " );
    nameString = nameString + QString( this->MetricsTableNode->GetTable()->GetValueByName( i, "MetricName" ).ToString() );
    QTableWidgetItem* nameItem = new QTableWidgetItem( nameString );
    d->MetricsTable->setItem( i, 0, nameItem );

    QString valueString;
    valueString = valueString + QString( this->MetricsTableNode->GetTable()->GetValueByName( i, "MetricValue" ).ToString() );
    valueString = valueString + QString( " " );
    valueString = valueString + QString( this->MetricsTableNode->GetTable()->GetValueByName( i, "MetricUnit" ).ToString() );
    QTableWidgetItem* valueItem = new QTableWidgetItem( valueString );    
    d->MetricsTable->setItem( i, 1, valueItem );
  }

}