/***************************************************************************
 *   Copyright (C) 2006 by Martin Pule   *
 *   martin@pulecyte.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "pMolTerminal.h"

pMolTerminal::pMolTerminal(pMolKernelInterface* p_interface)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("pMol Terminal"));
  insertPlainText(tr("pMol:Obj:>"));
  interface = p_interface;
  logger("");
  logger("############START#############");
}

///I've no clue what this is for
QSize pMolTerminal::minimumSizeHint() const
{
   return QSize(512, 256);
}

///I've no clue what this is for either
QSize pMolTerminal::sizeHint() const
{
   return QSize(512, 256);
}

bool pMolTerminal::event(QEvent *e)
{
  if (e->type() == QEvent::KeyPress)
  {
    QKeyEvent* ke = static_cast<QKeyEvent *>(e);

    if (ke->key()==Qt::Key_Return || ke->key()==Qt::Key_Enter)
    {
      ke->accept();
      QTextEdit::event(e);
      QTextCursor cursor = textCursor();
      cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1 );
      cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1 );
      cursor.setPosition(cursor.position()+10, QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1 );
      //cursor.select(QTextCursor::LineUnderCursor);
      QString inputString = cursor.selectedText();

      cacheIndex = 1;					// when you press the up key
      commandCache.append(inputString);			// remembers what you've typed

      insertPlainText(tr("pMol:Obj:>"));
  //  updateMicroFocus();
      logger(inputString);
      interface->command(inputString);
      return true;
    };

    if (ke->key()==Qt::Key_Up)
    { 
      int i;
      if ((i=commandCache.size())<1) return true;
      if (cacheIndex > i) return true;
      qDebug() << "terminal" << i << cacheIndex;
      if (cacheIndex>i) cacheIndex = 1; else i-=cacheIndex++;
      //if (i<1) i=1;
      qDebug() << "terminal" << i;
      QTextCursor cursor = textCursor();
      cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1 );
      cursor.setPosition(cursor.position()+10, QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1 );
      cursor.removeSelectedText();
      setTextCursor(cursor);
      insertPlainText(commandCache.at(i));
      return true;
    };

  };

  return QTextEdit::event(e);
}


void pMolTerminal::closeEvent(QCloseEvent *event)
{
   event->accept();
}


void pMolTerminal::logger(const QString &text)
{
   QString filename = "log.txt";
   QString directory = "/home/mxpule/Dropbox/data/pMolSynch/";

   QFile file(directory+filename);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
   {
     qDebug() << "couldn't open log file:" << directory << filename;
     return;
   };

   qDebug() << "logged" << text;
   QTextStream out(&file);
   out << text << "\n";
   file.close();
   return;
}


void pMolTerminal::msg(const QString &message)
{
  insertPlainText(message);
  insertPlainText(tr("\n"));
  insertPlainText(tr("pMol:Obj:>"));
  ensureCursorVisible ();
  logger(message);
}
