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
#include "pMolGeneSplicerTree.h"

//---------------------------------------------------------------------------------------------------
///
pMolGeneSplicerTree::pMolGeneSplicerTree(pMolXMLbase* p_pMolXMLbaseParent, const QString &p_name) : pMolStackObject(p_pMolXMLbaseParent)
{
  left = NULL;
  right = NULL;
  name = p_name;
};

pMolGeneSplicerTree::~pMolGeneSplicerTree()
{
  if (left!=NULL) delete left;
  if (right!=NULL) delete right;
};

//---------------------------------------------------------------------------------------------------
///
pMolXMLbase* pMolGeneSplicerTree::XMLopen(const QString &tag, QString &error)
{
  if (tag=="pMolGeneSplicerTreeLeft")
  { left = new pMolGeneSplicerTree((pMolXMLbase*) this, tag);
    return (pMolXMLbase*) left;
  }

  if (tag=="pMolGeneSplicerTreeRight") 
  { right = new pMolGeneSplicerTree((pMolXMLbase*) this, tag);
    return (pMolXMLbase*) right;
  };

  return (pMolXMLbase*) this;
};

//---------------------------------------------------------------------------------------------------
///Loads up the XML data
pMolXMLbase* pMolGeneSplicerTree::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolGeneSplicerTreeLeft")   		return pMolXMLbaseParent; 
  if (tag=="pMolGeneSplicerTreeRight")			return pMolXMLbaseParent;
  if (tag=="pMolGeneSplicerAcceptorTree")		return pMolXMLbaseParent;
  if (tag=="pMolGeneSplicerDonorTree")			return pMolXMLbaseParent;

  else if (tag=="val")               	XMLnumeric(&val, data, error);
  else if (tag=="consens")            	XMLnumeric(&consens, data, error);
  else if (tag=="poz")               	XMLnumeric(&poz, data, error);
  else if (tag=="no")               	XMLnumeric(&no, data, error);

  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

//---------------------------------------------------------------------------------------------------
///
void pMolGeneSplicerTree::treeOut(QTreeWidgetItem* tree)
{
  QTreeWidgetItem* branch = treeItem(tree, name);
  treeItem(branch, "val", val);
  treeItem(branch, "consens", consens);
  treeItem(branch, "poz", poz);
  treeItem(branch, "no", no);

  if (left!=NULL) left->treeOut(branch);
  if (right!=NULL) right->treeOut(branch);
}
