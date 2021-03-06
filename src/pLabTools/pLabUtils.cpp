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
/*
#include "pLabTools/pLabUtils.h"

#include <QtGui>
#include <QChar>

//----------------------------------------------------------------
///determine what the next entry bit is based on first character
int pLabUtils::parseNextType(const QString &s, int *i)
{
  //remove any leading spaces
  while (s.at(*i).isSpace())		//flow through spaces
    if (*i >= s.length()) return -1;	//nothing but spaces, return failed
    else (*i)++;				//otherwise go to the next one

  QChar c = s.at(*i);
  if (c.isDigit()) return pLabUtils::NUMBER;
  if (c.isLetter()) return pLabUtils::STRING;
  if (c == '[') return pLabUtils::BRACE;
  if (c == '!') return pLabUtils::NOT;
  if (c == '&') return pLabUtils::AND;
  if (c == '|') return pLabUtils::OR;
  if (c == ',') return pLabUtils::OR;
  if (c == '(') return pLabUtils::OPEN;
  if (c == ')') return pLabUtils::CLOSE;
  if (c == '-') return pLabUtils::TO;
  if (c == '>') return pLabUtils::GT;
  if (c == '<') return pLabUtils::LT;
  return -1;
};

//------------------------------------------------------------------------
///Parses a substring from the leading edge
QString pLabUtils::parseGetString(const QString &s, int *i, bool *ok)
{
  //prepare for parsing next bit to string
  QString a = "";			//accumulating string
  *ok = false;				//pessimistic 

  //remove any leading spaces
  while (s.at(*i).isSpace())		//flow through spaces
    if (*i >= s.length()) return "";	//nothing but spaces, return failed
    else (*i)++;				//otherwise go to the next one

  while (*i < s.length())		//while within the string
  {
    QChar c = s.at(*i);			//next character
    if (c == '!') break;		//these characters break a string
    if (c == '&') break;
    if (c == '|') break;
    if (c == ')') break;
    if (c == ',') break;
    if (c.isSpace()) break;		//as well as a space
    a+=c;				//extend accumulator by this character
    (*i)++;				//move to the next one
  };

  //get ready to return
  if (a.length()<1) return "";	//if string empty, return failed

  *ok = true;				//otherwise success!!!
  return a;				//return substring
};

//------------------------------------------------------------------------
///Parses a substring from the leading edge to a number
int pLabUtils::parseGetNumber(const QString &s, int *i, bool *ok)
{
  //prepare for parsing next bit to string
  QString a = "";			//accumulating string
  *ok = false;				//pessimistic 

  //remove any leading spaces
  while (s.at(*i).isSpace())		//flow through spaces
    if (*i >= s.length()) return -1;	//nothing but spaces, return failed
    else (*i)++;				//otherwise go to the next one

  while (*i < s.length())		//while within the string
  {
    if (!s.at(*i).isDigit()) break;	//next character not a number break
    a+=s.at(*i);				//extend accumulator by this character
    (*i)++;				//move to the next one
  };

  //get ready to return
  if (a.length()<1) return -1;	//if string empty, return failed
  return a.toInt(ok);			//try to convert to number and return
};

//------------------------------------------------------------------------
///Parses a human input command to sql
QString pLabUtils::parseToSql(const QString &search, const QString &table)
{
  int i = 0; 				//counts where in search string we are
  bool ok;				//are we ok?
  int nest = 0;				//counts how many bracketed commands
  int notCount = 0;
  QString sql = "";

  while (i<search.length())
  {
    bool logic = false;

    //braced number, special case of a number within a square brace
    if (parseNextType(search, &i)==pLabUtils::BRACE)
    {
      i++;						//go past the brace
      parseNextType(search, &i);				//remove any spaces
      i+=2;						//go past the 'MP'
      int n = parseGetNumber(search, &i, &ok);		//get the number contained by the brace
      if (!ok) return NULL;				//if there's a problem, bail out
      QString s;					//string to convert number to string
      sql+=table+".ixix="+s.setNum(n);			//make sql
      break;						//special case, no more input
    };

    //simple string
    if (parseNextType(search, &i)==pLabUtils::STRING)
    {
      QString s = parseGetString(search, &i, &ok);	//get the string
      if (!ok) return NULL;				//if there's a problem, bail out
      s = s.replace('*', '%');				//convert to SQL format
      sql+=table+".name LIKE '%"+s+"%' ";		//make SQL
    } else

    //number or range of numbers
    if (parseNextType(search, &i) == pLabUtils::NUMBER)
    {
      int n0 = parseGetNumber(search, &i, &ok);
      qDebug() << "found number" << n0;
      if (!ok) return NULL;
      if (parseNextType(search, &i) == pLabUtils::TO)
      { 
        int n1 = parseGetNumber(search, &(++i), &ok);
        if (!ok) return NULL;
        int n;
        if (n1<n0) {n=n0; n0 = n1; n1=n0;};
        QString s0,s1;
        sql+="("+table+".ixix>"+s0.setNum(n0-1)+" AND ";
        sql+=table+".ixix<"+s1.setNum(n1+1)+") ";
      } else
      {
        QString s;
        sql+=table+".ixix="+s.setNum(n0)+" ";
      }; 
    } else

    //greater than
    if (parseNextType(search, &i) == pLabUtils::GT)
    {
      int n = parseGetNumber(search, &(++i), &ok);	//next must be a number
      if (!ok) return NULL;				//isn't then an error
      QString s;					//somewhere to convert to string
      sql+=table+".ixix>"+s.setNum(n)+" ";		//add to sql command
    } else

    //less than
    if (parseNextType(search, &i) == pLabUtils::LT)
    {
      int n = parseGetNumber(search, &(++i), &ok);	//next must be a number
      if (!ok) return NULL;				//isn't then an error
      QString s;					//somewhere to convert to string
      sql+=table+".ixix<"+s.setNum(n)+" ";		//add to sql command
    } else

    //open bracket
    if (parseNextType(search, &i)==pLabUtils::OPEN)
    { nest++;   sql+="(";  i++; logic = true;
    } else

    //close bracket
    if (parseNextType(search, &i)==pLabUtils::CLOSE)
    { nest--;	sql+=")";  i++;
    } else

    //logical AND
    if (parseNextType(search, &i)==pLabUtils::AND)
    { sql+=" AND "; i++; logic = true;
    } else

    //logical NAND
    if (parseNextType(search, &i)==pLabUtils::NOT)
    { sql+=" AND NOT ( "; i++; logic = true;
      notCount+=2;
    } else

    //logical OR
    if (parseNextType(search, &i)==pLabUtils::OR)
    { sql+=" OR "; i++; logic = true;
    };

    if (notCount>0) 
      if ((notCount-- & 1) == 1) sql+=")";

    int link = parseNextType(search, &i);		//if user hasn't put in logical connection
    if ((link>0) && (!logic) && ((link==pLabUtils::STRING) || (link==pLabUtils::NUMBER)))
      sql+=" AND ";					//assume they want an AND
  };

  return sql;
};

//------------------------------------------------------------------------
///changes occurence of 'table' in text into what the appropriate table is
QString pLabUtils::parseTableSql(const QString &query, int type)
{
  QString temp = query;
  if (type == type_OLIGOS)   return temp.replace(QString("table"), QString("oligos"));
  if (type == type_PLASMIDS) return temp.replace("table", "plasmids");
  if (type == type_SUPES)    return temp.replace("table", "supes");
  if (type == type_CELLS)    return temp.replace("table", "cells");
  throw pMolError("pLabUtils::parseTableSql - unknown sample type!");
  return NULL;
};

//------------------------------------------------------------------------
///get the details of a sample into a human readable summary
QString pLabUtils::getSqlDetails(const QSqlQuery &query, int type)
{
  if (type == type_OLIGOS)
  {
    int ixix = query.value(0).toInt();
    QString ixString; ixString.setNum(ixix);
    QString slot = query.value(1).toString();
    QString details = "[MP"+ixString+"]";
           details += "["+query.value(3).toString()+"]\n";
           details += "["+query.value(2).toString()+"]\n";
           details += "["+query.value(4).toString()+"] ";
    return details;
  };
  
  if (type == type_PLASMIDS)
  {
    int ixix = query.value(0).toInt();
    QString ixString; ixString.setNum(ixix);
    QString slot = query.value(1).toString();
    QString details = "[MP"+ixString+"]";
            details+= "["+query.value(2).toString()+"]\n";
            details+= "["+query.value(3).toString().left(64)+"] ";
  };

  if (type == type_SUPES)
  {
    int ixix = query.value(0).toInt();
    QString ixString; ixString.setNum(ixix);
    QString slot = query.value(1).toString();
    QString details = "[MP"+ixString+"]";
            details+= "["+query.value(2).toString()+"]\n";
            details+= "["+query.value(3).toString().left(64)+"] ";
  };

  if (type == type_CELLS)
  {
    int ixix = query.value(0).toInt();
    QString ixString; ixString.setNum(ixix);
    QString slot = query.value(1).toString();
    QString details = "[MP"+ixString+"]";
            details+= "["+query.value(2).toString()+"]\n";
            details+= "["+query.value(3).toString().left(64)+"] ";
  };

  throw pMolError("pLabUtils::getSqlDetails - unknown sample type");
};
*/
