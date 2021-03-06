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
#include "pMolLanguageEngine.h"

///this returns a cue to what's coming next down the command line
int pMolLanguageEngine::getCue(QString* cmdStr)
{
  while (cmdStr->startsWith(" ")) cmdStr->remove(0,1);			// trim any white space
  if (cmdStr->length()==0)      	return CUE_EMPTY;		// string finished
  if (cmdStr->startsWith(";"))  	return CUE_TERM;		// semi-colon indicates end of input
  if (cmdStr->startsWith("("))  	return CUE_OPEN_ROUND;			
  if (cmdStr->startsWith(")"))  	return CUE_CLOSE_ROUND;
  if (cmdStr->startsWith("->")) 	return CUE_CHILD;
  if (cmdStr->startsWith(","))  	return CUE_COMMA;
  if (cmdStr->startsWith("'")) 		return CUE_STRING;
  if ((cmdStr->at(0)).isDigit())     	return CUE_NUMBER;
  if (cmdStr->startsWith("-"))		return CUE_NUMBER;
  if ((cmdStr->at(0)).isLetter())    	return CUE_NAME;

  return CUE_UNKNOWN;
};

///gets the next name type segment from the command line
///i.e. up to the next ( or ) or , or ; or ->
QString pMolLanguageEngine::getNext(QString *cmdStr)
{
  int j, i = cmdStr->length();
  j=cmdStr->indexOf("(");  if ((j>0) && (j<i)) i = j;
  j=cmdStr->indexOf(")");  if ((j>0) && (j<i)) i = j;
  j=cmdStr->indexOf(",");  if ((j>0) && (j<i)) i = j;
  j=cmdStr->indexOf(" ");  if ((j>0) && (j<i)) i = j;
  j=cmdStr->indexOf("'");  if ((j>0) && (j<i)) i = j;
  j=cmdStr->indexOf("->"); if ((j>0) && (j<i)) i = j;
  QString temp = cmdStr->left(i);
  cmdStr->remove(0,i);
  return temp;
};

///this makes an error pMolcmd Object with helpful information about what went wrong
pMolCmd* pMolLanguageEngine::parseError(const QString &errorMsg, QString* cmdStr)
{
  pMolCmdError* error = new pMolCmdError(*cmdStr+ ":" + errorMsg);	// create the error class
  return (pMolCmd*) error;						// return it
}


///this parses in a string
pMolCmd* pMolLanguageEngine::parseString(QString* cmdStr)
{
  cmdStr->remove(0,1);							// remove the opening '
  int i = cmdStr->indexOf("'");						// find the closing '

  if (i<1) return parseError("was expecting end to String",cmdStr);	// command didn't have closing '

  pMolCmdString* string = new pMolCmdString(cmdStr->left(i));		// make the container
  cmdStr->remove(0,i+1);							// remove string from command line
  return (pMolCmd*) string;						// return the container containing string
};


///this parses in a number
pMolCmd* pMolLanguageEngine::parseNumber(QString* cmdStr)
{
  bool ok;								// need this for QString.int call
  QString str = getNext(cmdStr);					// ok get the next bit
  int n = str.toInt(&ok, 10);						// try to make a number from it

  if (!ok) return parseError("couldn't understand number "+str, cmdStr);
									// went wrong!
  pMolCmdNumber* number = new pMolCmdNumber(n);				// otherwise make a container for this number
  return (pMolCmd*) number;						// and return it.
};


///parses a name object and goes forward fromparseName(cmdStr) there
pMolCmd* pMolLanguageEngine::parseName(QString* cmdStr)
{
  pMolCmdName* name = new pMolCmdName(getNext(cmdStr));			// make a container for the name
  name->child = parsePostName(cmdStr);					// go forward
  if (name->child==NULL) return (pMolCmd*) name;			// end of the input reached
  if (name->child->type()==pMolCmd::ERROR) return name->child;	// if not as expected bail with error
  return (pMolCmd*) name;						// otherwise return this new object
};


///parse command line after "->" expecting name anything else is an error
pMolCmd* pMolLanguageEngine::parseChild(QString* cmdStr)
{
  cmdStr->remove(0,2);							// pop off the "->"		
  int code = getCue(cmdStr);						// get what the next bit of input is
	
  if (code!=CUE_NAME) 							// if it's a name great go get it
    return parseError("was expecting a child object name after '->'",cmdStr);
									// otherwise exit with an error object
  pMolCmdChild* child = new pMolCmdChild(getNext(cmdStr));		// make a container for the name
  child->child = parsePostName(cmdStr);					// go forward
  if (child->child==NULL) return (pMolCmd*) child;			// end of the input reached
  if (child->child->type(pMolCmd::ERROR)) return child->child;	// if not as expected bail with error
  return (pMolCmd*) child;	
};



///parse command line, expecting follow-on from an object name
///i.e. we are expecting subcommands or parameter but nothing else
///not necessarily an error, e.g. a ")" etc, but anything else end this object
pMolCmd* pMolLanguageEngine::parsePostName(QString* cmdStr)
{
  int code = getCue(cmdStr);						// get what the next bit of input is

  if (code==CUE_OPEN_ROUND)  	return parseList(cmdStr); 		// looks like a list so go for it
  if (code==CUE_CHILD)		return parseChild(cmdStr);		// looks like a subobject, so go for it
  return NULL;
  qDebug() << "parsePostName" << code;
}


///round brackets have been open, put next stuff in a list!
///separated by ","
pMolCmd* pMolLanguageEngine::parseList(QString *cmdStr)
{
  cmdStr->remove(0,1);							// pop off the "("
  int code = getCue(cmdStr); 						// get what the next bit of input is
  pMolCmdList* list = new pMolCmdList();
 

  while (code!=CUE_CLOSE_ROUND)
  {
    pMolCmd* next = parse(cmdStr);
    if (next==NULL) return parseError("terminated before list end", cmdStr);
    if (next->type()==pMolCmd::ERROR) return next;

    list->append(next);

    code = getCue(cmdStr);
    if ((code==CUE_COMMA) || (code==CUE_CLOSE_ROUND)) cmdStr->remove(0,1);
    else return parseError("expecting ',' or ')'", cmdStr);
  };
  return (pMolCmd*) list;
};



///parses anything much really
pMolCmd* pMolLanguageEngine::parse(QString *cmdStr)
{
  int code = getCue(cmdStr);						// what's next?
  qDebug() << "parse" << code;
  qDebug() << "CUE_NAME" << CUE_NAME;
  if (code==CUE_EMPTY)		return NULL;				// nothing so return 
  if (code==CUE_TERM) 		return NULL;				// the end so return
  if (code==CUE_STRING) 	return parseString(cmdStr);		// looks like a String so get it
  if (code==CUE_NUMBER) 	return parseNumber(cmdStr);		// looks like a number so get it
  if (code==CUE_NAME)   	return parseName(cmdStr);		// looks like a name so get it
  if (code==CUE_OPEN_ROUND)  	return parseList(cmdStr);		// looks like a list so get it
  if (code==CUE_CHILD)		return parseChild(cmdStr);

  return parseError("I don't know what this is", cmdStr);		// unknown text
};
