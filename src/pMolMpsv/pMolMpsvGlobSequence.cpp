
#include "pMolMpsv/pMolMpsvGlobSequence.h"

pMolMpsvGlobSequence::pMolMpsvGlobSequence(pMolMpsvWrap* p_parentWrapper, int p_index, QGraphicsItem* parent)
        : QGraphicsTextItem(parent)
{
  index = p_index;
  parentWrapper = p_parentWrapper;
}


void pMolMpsvGlobSequence::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  qDebug() << "globSequence::mousePressEvent says hello from "+parentWrapper->getName()+", about to emit signal";
  qDebug() << handlesChildEvents();
  QFontMetrics fm(font());	//need metrics
  int x = event->pos().x();
  x = 16*x/fm.width("cagtcagtcagtcagt");
  parentWrapper->userEvent(0x001, index+x);
};
