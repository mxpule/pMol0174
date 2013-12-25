/*

#include <QWidget>
#include <QTreeWidget>

#include "pMolSql/pMolSql.h"
#include "pMolStackObject.h"
#include "pMolKernelInterface.h"

class QComboBox;
class QLineEdit;
class QPushButton;

class pLabSupesEntry : public QWidget, public pMolStackObject
{
    Q_OBJECT

public:
    pLabSupesEntry(pMolKernelInterface* p_interface);
    static pLabSupesEntry* create(pMolKernelInterface* interface, pMolCmd* cmd);


public slots:
    void allocateButtonClicked(bool);
    void finishButtonClicked(bool);
    void cancelButtonClicked(bool);
    void vectorComboActivated(int index);
    void commentEditingFinished();

private:
    QString vectorName;
    void setStatus();
    void searchSql(const QString &p_sql);

    int vectorIxix;
    int allocatedIx;

    QComboBox *pseudotype;
    QComboBox *vectorCombo;
    QLineEdit *commentString;

    QPushButton *allocateButton;// = new QPushButton("allocate");
    QPushButton *cancelButton;// = new QPushButton("cancel");
    QPushButton *finishButton;// =  new QPushButton("finish");
    QLabel* mpLabel;// = new QLabel("vector", this);

    pMolKernelInterface* interface;
};

#endif

*/
