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

#ifndef __XML_LITE_ELEMENT_H__
#define __XML_LITE_ELEMENT_H__

#include "io/InputStream.h"
#include "io/OutputStream.h"
#include "XMLException.h"
#include "Attributes.h"

#define CAST_ELEMENT(TYPE, XML_ELEMENT) dynamic_cast<TYPE*>(XML_ELEMENT)

/*!
 * \file  Element.h
 * \brief This contains the xml::lite data type for elements.
 *
 * This class is analogous to its DOM namesake.  It contains all of
 * the information that would be expected of it, including comment data.
 * The API, however, is much simpler and less developed than DOM.
 */

// xml::lite::Element
namespace xml
{
namespace lite
{
/*!
 * \class Element
 * \brief The class defining one element of an XML document
 *
 * This class stores all of the element information about an XML
 * document.
 */
class Element
{
public:
    //! Constructor
    Element()
    {}

    //! Destructor
    virtual ~Element()
    {
        destroyChildren();
    }

    //! Destroys any child elements.
    void destroyChildren();

    /*!
     * Copy constructor
     * \param element  Takes an element
     */
    Element(const Element & element);

    /*!
     *  Assignment operator
     *  \param element  Takes an element
     *  \return a reference to *this
     */
    Element & operator=(const Element & element);

    /*!
     *  Clone function performs deep copy
     *  of element
     *  \param element  Takes an element
     */
    void clone(const Element & element);

    /*!
     *  Get the attributes in a non-const way
     *  \return attributes
     */
    Attributes & getAttributes()
    {
        return mAttributes;
    }

    /*!
     *  Get the attributes in a const way
     *  \return attributes
     */
    const Attributes & getAttributes() const
    {
        return mAttributes;
    }

    /*!
     *  Set the attributes
     *  \param attributes The attributes to set
     */
    void setAttributes(const Attributes & attributes)
    {
        mAttributes.clear();
        for (int i = 0; i < attributes.getLength(); i++)
        {
            mAttributes.add(attributes.getNode(i));
        }
    }


    /*!
     *  Get the elements by tag name
     *  \param qname the QName
     *  \param elements the elements that match the QName
     */
    void getElementsByTagNameNS(const std::string & qname,
                                std::vector < Element * >&elements);


    /*!
     *  Sometimes we dont care about the qname or the uri -- just 
     *  the local name is good enough.  For those times, use this function
     *  \param localName The local name
     *  \param elements The elements
     */
    void getElementsByTagName(const std::string & localName,
                              std::vector < Element * >&elements);


    /*!
     *  1)  Find this child's attribute and change it
     *  2)  Recursively descend over children and fix all
     *  namespaces below using fixNodeNamespace()
     *  
     * 
     */
    void rewriteNamespacePrefix(const std::pair < std::string,
                                std::string > & prefixAndUri);
    void rewriteNamespaceUri(const std::pair < std::string,
                             std::string > & prefixAndUri);


    /*!
     *  Get the elements by tag name
     *  \param uri the URI
     *  \param localName the local name
     *  \param elements the elements that match the QName
     */
    void getElementsByTagName(const std::string & uri,
                              const std::string & localName,
                              std::vector < Element * >&elements);

    /*!
     *  Prints the element to the specified OutputStream
     *  \param stream the OutputStream to write to
     *  \param formatter  The formatter
    *  \todo Add format capability
     */
    void print(io::OutputStream & stream);

    void prettyPrint(io::OutputStream & stream, std::string formatter = "    ");

    /*!
     *  Determines if a child element exists
     *  \param localName the local name to search for
     *  \return true if it exists, false if not
     */
    bool hasElement(const std::string & localName) const;

    /*!
     *  Determines if a child element exists
     *  \param uri the URI to search for
     *  \param localName the local name to search for
     *  \return true if it exists, false if not
     */
    bool hasElement(const std::string & uri,
                    const std::string & localName) const;

    /*!
     *  Returns the character data of this element.
     *  \return the charater data
     */
    std::string getCharacterData() const
    {
        return mCharacterData;
    }

    /*!
     *  Sets the character data for this element.
     *  \param characters The data to add to this element
     */
    void setCharacterData(const std::string & characters)
    {
        mCharacterData = characters;
    }

    /*!
     *  Sets the local name for this element.
     *  \param localName the data to add to this element
     */
    void setLocalName(const std::string & localName)
    {
        mName.setName(localName);
    }

    /*!
     *  Returns the local name of this element.
     *  \return the local name
     */
    std::string getLocalName()const
    {
        return mName.getName();
    }

    /*!
     *  Sets the QName for this element.
     *  \param qname the data to add to this element
     */
    void setQName(const std::string& qname)
    {
        mName.setQName(qname);
    }

    /*!
     *  Returns the QName of this element.
     *  \return the QName
     */
    std::string getQName()const
    {
        return mName.toString();
    }

    /*!
     *  Sets the URI for this element.
     *  \param uri the data to add to this element
     */
    void setUri(const std::string & uri)
    {
        mName.setAssociatedUri( uri );
    }

    /*!
     *  Returns the URI of this element.
     *  \return the URI
     */
    std::string getUri() const
    {
        return mName.getAssociatedUri();
    }

    /*!
     *  Adds a child element to this element
     *  \param node the child element to add
     */
    virtual void addChild(Element * node);

    /*!
     *  Returns all of the children of this element
     *  \return the children of this element
     */
    std::vector < Element * >&getChildren()
    {
        return mChildren;
    }

    /*!
     *  Returns all of the children of this element
     *  \return the children of this element
     */
    const std::vector < Element * >&getChildren() const
    {
        return mChildren;
    }

protected:

    void changePrefix(Element* element,
                      const std::pair < std::string,
                      std::string > & prefixAndUri);
    void changeUri(Element* element,
                   const std::pair < std::string,
                   std::string > & prefixAndUri);

    void depthPrint(io::OutputStream & stream, int depth, std::string formatter);

    //! The children of this element
    std::vector < Element * >mChildren;
    //! QNames replace lname, pre, uri
    xml::lite::QName mName;
    //! The attributes for this element
    xml::lite::Attributes mAttributes;
    //! The character data
    std::string mCharacterData;
};
}
}

#endif
