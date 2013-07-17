/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2013 Apple Inc. All rights reserved.
 *           (C) 2007 Eric Seidel (eric@webkit.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "core/dom/NamedNodeMap.h"

#include "core/dom/Attr.h"
#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/dom/ExceptionCode.h"

namespace WebCore {

using namespace HTMLNames;

static inline bool shouldIgnoreAttributeCase(const Element* e)
{
    return e && e->document()->isHTMLDocument() && e->isHTMLElement();
}

void NamedNodeMap::ref()
{
    m_element->ref();
}

void NamedNodeMap::deref()
{
    m_element->deref();
}

PassRefPtr<Node> NamedNodeMap::getNamedItem(const AtomicString& name) const
{
    return m_element->getAttributeNode(name);
}

PassRefPtr<Node> NamedNodeMap::getNamedItemNS(const AtomicString& namespaceURI, const AtomicString& localName) const
{
    return m_element->getAttributeNodeNS(namespaceURI, localName);
}

PassRefPtr<Node> NamedNodeMap::removeNamedItem(const AtomicString& name, ExceptionCode& ec)
{
    size_t index = m_element->hasAttributes() ? m_element->getAttributeItemIndex(name, shouldIgnoreAttributeCase(m_element)) : notFound;
    if (index == notFound) {
        ec = NotFoundError;
        return 0;
    }
    return m_element->detachAttribute(index);
}

PassRefPtr<Node> NamedNodeMap::removeNamedItemNS(const AtomicString& namespaceURI, const AtomicString& localName, ExceptionCode& ec)
{
    size_t index = m_element->hasAttributes() ? m_element->getAttributeItemIndex(QualifiedName(nullAtom, localName, namespaceURI)) : notFound;
    if (index == notFound) {
        ec = NotFoundError;
        return 0;
    }
    return m_element->detachAttribute(index);
}

PassRefPtr<Node> NamedNodeMap::setNamedItem(Node* node, ExceptionCode& ec)
{
    if (!node) {
        ec = NotFoundError;
        return 0;
    }

    // Not mentioned in spec: throw a HIERARCHY_REQUEST_ERROR if the user passes in a non-attribute node
    if (!node->isAttributeNode()) {
        ec = HierarchyRequestError;
        return 0;
    }

    return m_element->setAttributeNode(toAttr(node), ec);
}

PassRefPtr<Node> NamedNodeMap::setNamedItemNS(Node* node, ExceptionCode& ec)
{
    return setNamedItem(node, ec);
}

PassRefPtr<Node> NamedNodeMap::item(unsigned index) const
{
    if (index >= length())
        return 0;
    return m_element->ensureAttr(m_element->attributeItem(index)->name());
}

size_t NamedNodeMap::length() const
{
    if (!m_element->hasAttributes())
        return 0;
    return m_element->attributeCount();
}

} // namespace WebCore
