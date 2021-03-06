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
#ifndef PMOLSTACKOBJECT_H
#define PMOLSTACKOBJECT_H

#include <QtGui>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "pMolCmd.h"
#include "pMolObject.h"
#include "pMolKernelInterface.h"

///Stack object level of pMol objects.
///Needs a lot of work, probably needs to be merged with pMolObject
class pMolStackObject :public pMolObject
{
  public:
    pMolStackObject();
    pMolStackObject(pMolXMLbase *p_pMolXMLbaseParent);

    virtual void renderBase(const QString &p_param);
    virtual void render(QPaintEvent *);
    virtual bool type(int);
    virtual bool save();
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual bool loadDependencies(pMolKernelInterface* interface);
    virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* makeError(pMolKernelInterface* interface, const QString &member, const QString &error);
    static pMolStackObject* makeError(pMolKernelInterface* interface, const QString &name, const QString &member, const QString &error);
    static pMolStackObject* makeError(const QString &name, const QString &member, const QString &error);
};

///A stack object which represents a string
///When pMolCmdString gets interpreted it leaves this object
///@see pMolCmdString
class pMolStackString : public pMolStackObject
{
  public :
    pMolStackString(const QString &p_string);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int);
    QString string;

};

///A stack object which represents a string
///When pMolCmdString gets interpreted it leaves this object
///@see pMolCmdString
class pMolStackError : public pMolStackObject
{
  public :
    pMolStackError(const QString &p_error);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int);
    QString error;

};


///A stack object which represents a number
///When pMolCmdNumber gets interpreted it leaves this object
class pMolStackNumber : public pMolStackObject
{
  public :
    pMolStackNumber(int p_n);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int);
    int n;
};

///A stack object which represents a double precision real number
///When pMolCmdDouble gets interpreted it leaves this object
class pMolStackDouble : public pMolStackObject
{
  public :
    pMolStackDouble(double p_n);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int);
    double n;
};

///A stack object which represents a list of objects
///When pMolCmdList gets interpreted each child gets interpreted
///and the resulting stack objects get contained by this class
class pMolStackList : public pMolStackObject
{
  public :
    virtual ~pMolStackList();
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int);

    bool checkParameter(int i, int type, QString &error);
    bool checkParameters(int type0, QString &error);
    bool checkParameters(int type0, int type1, QString &error);
    bool checkParameters(int type0, int type1, int type2, QString &error);
    bool checkParameters(int type0, int type1, int type2, int type3, QString &error);
    bool checkParameters(int type0, int type1, int type2, int type3, int type4, QString &error);
    static pMolStackList* interpretParameter(pMolCmd* cmd, pMolKernelInterface*, QString &error);
    static pMolStackList* interpretParameter(pMolCmd* cmd, pMolKernelInterface* interface, pMolObject* parent, QString &error);

    void append(pMolStackObject* p_cmd);
    int length();
    pMolStackObject* at(int i);
    QList <pMolStackObject*> list;

  private:
    bool checkParameter(pMolStackObject* object, int type);
};

#endif
