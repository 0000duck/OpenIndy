#include "featureoverviewdelegate.h"

FeatureOverviewDelegate::FeatureOverviewDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief FeatureOverviewDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* FeatureOverviewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QLineEdit* myEditor = new QLineEdit(parent);
    const FeatureOvserviewProxyModel *myModel = static_cast<const FeatureOvserviewProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
        return myEditor;
    }
    return NULL;
}

/*!
 * \brief FeatureOverviewDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void FeatureOverviewDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    const FeatureOvserviewProxyModel *myModel = static_cast<const FeatureOvserviewProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
         QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
         if(myEditor != NULL){
             myEditor->setText(index.data().toString());
         }
    }
}

/*!
 * \brief FeatureOverviewDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void FeatureOverviewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    const FeatureOvserviewProxyModel *myModel = static_cast<const FeatureOvserviewProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            model->setData(index, myEditor->text());
        }
    }
}
