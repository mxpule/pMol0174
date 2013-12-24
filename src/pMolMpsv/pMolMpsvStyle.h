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

#ifndef PMOLMPSVSTYLE
#define PMOLMPSVSTYLE

#include <QFont>

///stores basic style data for pMolMpsv. The most essential purpose for this
///class is to store fonts used throughout the rendering. This way, the object
///doesn not need to keep performing the expensive QFont initializations again
///and again. Also calculates and stores <b>uc</b>, the "universal constant"
///which is the average width of a character of the sequenceFont.
class pMolMpsvStyle
{
  public :
    ///constructor
    pMolMpsvStyle(
      int sequenceFontSize, const QString &sequenceFontName, 
      int annotationFontSize, const QString &annotationFontName);

    ///destructor
    ~pMolMpsvStyle();

    ///universal constant
    float uc, uca;

   QFont sequenceFont;
   QFont annotationFont;
};

#endif
