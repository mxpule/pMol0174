#ifndef PMOLMPSVGLOBSEQUENCE_H
#define PMOLMPSVGLOBSEQUENCE_H

#include <QGraphicsScene>
#include <QtGui/QGraphicsTextItem>

#include "pMolMpsv/pMolMpsvWrap.h"


///of its bounding box is (0,0). Compact() frees up the considerable amount of memory needed (i.e. masks,
///and paths) to do the minimization.
class pMolMpsvGlobSequence : public QGraphicsTextItem
{
  public:
    pMolMpsvGlobSequence(pMolMpsvWrap* p_parentWrapper,int p_index, QGraphicsItem *parent);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int index;
    pMolMpsvWrap* parentWrapper;
};

#endif // PMOLMPSVGLOBSEQUENCE_H
