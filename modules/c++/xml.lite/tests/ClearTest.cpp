/* =========================================================================
 * This file is part of xml.lite-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2009, General Dynamics - Advanced Information Systems
 *
 * xml.lite-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include <import/xml/lite.h>
#include <import/io.h>
#include <vector>
#include <iostream>
const char* xmldata =
    "<root okay=\"1\"><A>This</A><A>is</A><A>an</A><B>otter</B></root>";
void printCD( std::vector<xml::lite::Element*>& e )
{
    for (int i = 0; i < e.size(); ++i)
    {
        std::cout << e[i]->getCharacterData();
        std::cout << " ";
    }
    std::cout << "*";
}

int main()
{
    io::StringStream ss;
    ss.write(xmldata, strlen(xmldata));
    xml::lite::MinidomParser p;
    p.parse( ss );
    xml::lite::Element* elem = p.getDocument()->getRootElement();
    std::vector<xml::lite::Element*> vec;
    elem->getElementsByTagName("A", vec);
    printCD( vec );
    elem->getElementsByTagName("B", vec);
    xml::lite::AttributeNode attr;
    attr.setQName("better");
    attr.setValue("1");
    xml::lite::Attributes atts;
    atts.add( attr );
    elem->setAttributes( atts );

    printCD( vec );
    std::cout << std::endl;

    io::StandardOutStream sout;
    elem->print( sout );
}

