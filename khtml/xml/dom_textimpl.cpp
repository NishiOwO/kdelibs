/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
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
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * $Id$
 */
#include "dom_textimpl.h"

#include "dom_stringimpl.h"
#include "dom_exception.h"

#include "dom/dom_string.h"
#include "dom_docimpl.h"

#include "dom/dom_node.h"
#include "misc/htmlhashes.h"
#include "rendering/render_text.h"

#include <kdebug.h>

using namespace DOM;
using namespace khtml;


CharacterDataImpl::CharacterDataImpl(DocumentImpl *doc) : NodeWParentImpl(doc)
{
    str = 0;
}

CharacterDataImpl::CharacterDataImpl(DocumentImpl *doc, const DOMString &_text)
    : NodeWParentImpl(doc)
{
//    str = new DOMStringImpl(_text.impl->s,_text.impl->l);
    str = _text.impl;
    str->ref();
}

CharacterDataImpl::~CharacterDataImpl()
{
    if(str) str->deref();
}

DOMString CharacterDataImpl::data() const
{
    return str;
}

void CharacterDataImpl::setData( const DOMString &newStr )
{
    if(str == newStr.impl) return;
    if(str) str->deref();
    str = newStr.impl;
    if(str) str->ref();
    if (m_render)
      (static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
}

unsigned long CharacterDataImpl::length() const
{
    return str->l;
}

DOMString CharacterDataImpl::substringData( const unsigned long offset, const unsigned long count, int &exceptioncode )
{
    exceptioncode = 0;
    if (offset > str->l ) {
	exceptioncode = DOMException::INDEX_SIZE_ERR;
	return DOMString();
    }
    return str->substring(offset,count);
}

void CharacterDataImpl::appendData( const DOMString &arg )
{
    detachString();
    str->append(arg.impl);
    if (m_render)
      (static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
    _parent->setChanged(true);
}

void CharacterDataImpl::insertData( const unsigned long offset, const DOMString &arg, int &exceptioncode )
{
    detachString();
    exceptioncode = 0;
    if (offset > str->l) {
	exceptioncode = DOMException::INDEX_SIZE_ERR;
	return;
    }
    str->insert(arg.impl, offset);
    if (m_render)
      (static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
}

void CharacterDataImpl::deleteData( const unsigned long offset, const unsigned long count, int &exceptioncode )
{
    detachString();
    exceptioncode = 0;
    if (offset > str->l) {
	exceptioncode = DOMException::INDEX_SIZE_ERR;
	return;
    }

    str->remove(offset,count);
    if (m_render)
      (static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
}

void CharacterDataImpl::replaceData( const unsigned long offset, const unsigned long count, const DOMString &arg, int &exceptioncode )
{
    detachString();
    exceptioncode = 0;
    if (offset > str->l) {
	exceptioncode = DOMException::INDEX_SIZE_ERR;
	return;
    }

    unsigned long realCount;
    if (offset + count > str->l)
	realCount = str->l-offset;
    else
	realCount = count;

    str->remove(offset,realCount);
    str->insert(arg.impl, offset);
    if (m_render)
      (static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
}


void CharacterDataImpl::detachString()
{
    // make a copy of our string so as not to interfere with other texts that use it
    DOMStringImpl *newStr = str->copy();
    newStr->ref();
    str->deref();
    str = newStr;
}

// ---------------------------------------------------------------------------

CommentImpl::CommentImpl(DocumentImpl *doc, const DOMString &_text)
    : CharacterDataImpl(doc, _text)
{
}

CommentImpl::CommentImpl(DocumentImpl *doc)
    : CharacterDataImpl(doc)
{
}

CommentImpl::~CommentImpl()
{
}

const DOMString CommentImpl::nodeName() const
{
    return "#comment";
}

DOMString CommentImpl::nodeValue() const
{
    return str;
}

unsigned short CommentImpl::nodeType() const
{
    return Node::COMMENT_NODE;
}

ushort CommentImpl::id() const
{
    return ID_COMMENT;
}

NodeImpl *CommentImpl::cloneNode(bool /*deep*/)
{
    CommentImpl *newImpl = document->createComment( str );

    newImpl->setParent(0);
    newImpl->setFirstChild(0);
    newImpl->setLastChild(0);

    return newImpl;
}


// ---------------------------------------------------------------------------

// ### allow having children in text nodes for entities, comments etc.

TextImpl::TextImpl(DocumentImpl *doc, const DOMString &_text)
    : CharacterDataImpl(doc, _text)
{
    m_style = 0;
}

TextImpl::TextImpl(DocumentImpl *doc)
    : CharacterDataImpl(doc)
{
    m_style = 0;
}

TextImpl::~TextImpl()
{
    // we don't delete m_style, since it's just a pointer to the parents
    // style object
}

TextImpl *TextImpl::splitText( const unsigned long offset, int &exceptioncode )
{
    exceptioncode = 0;
    if (offset > str->l || (long)offset < 0)
	exceptioncode = DOMException::INDEX_SIZE_ERR;

    if (!_parent) // ### should	we still return the splitted text, and just not insert it into the tree?
	exceptioncode = DOMException::HIERARCHY_REQUEST_ERR;

    if ( exceptioncode )
	return 0;

    TextImpl *newText = static_cast<TextImpl*>(cloneNode(true));
    newText->deleteData(0,offset,exceptioncode);
    if ( exceptioncode )
	return 0;
    deleteData(offset,str->l-offset,exceptioncode);
    if ( exceptioncode )
	return 0;
    _parent->insertBefore(newText,_next, exceptioncode );
    if ( exceptioncode )
	return 0;

    if (m_render)
	(static_cast<RenderText*>(m_render))->setText(str);
    setChanged(true);
    return newText;
}

const DOMString TextImpl::nodeName() const
{
  return "#text";
}
DOMString TextImpl::nodeValue() const
{
    return str;
}


unsigned short TextImpl::nodeType() const
{
    return Node::TEXT_NODE;
}

void TextImpl::attach(KHTMLView *w)
{
    m_style = parentNode()->style();
    RenderObject *r = _parent->renderer();
    if(r)
    {
	m_render = new RenderText(str);
	m_render->setStyle(m_style);
	r->addChild(m_render, _next ? _next->renderer() : 0);
    }
    CharacterDataImpl::attach(w);
}

void TextImpl::detach()
{
    m_render = 0;
    CharacterDataImpl::detach();
}

void TextImpl::applyChanges(bool,bool force)
{
    if (force || changed())
	recalcStyle();
    setChanged(false);
}

bool TextImpl::mouseEvent( int _x, int _y,
			   int _tx, int _ty,
                           MouseEvent *ev)
{
    //kdDebug( 6020 ) << "Text::mouseEvent" << endl;

    if(!m_render) return false;

    if(m_render->parent() && m_render->parent()->isAnonymousBox())
    {
	// we need to add the offset of the anonymous box
	_tx += m_render->parent()->xPos();
	_ty += m_render->parent()->yPos();
    }

    int off = 0;
    if( static_cast<RenderText *>(m_render)->checkPoint(_x, _y, _tx, _ty, off) )
    {
	ev->offset = off;
	ev->innerNode = this;
        ev->nodeAbsX = _tx;
        ev->nodeAbsY = _ty;
	return true;
    }
    return false;
}

int TextImpl::findSelectionNode( int _x, int _y, int _tx, int _ty,
                                 DOM::Node & node, int & offset )
{
    kdDebug(6030) << "TextImpl::findSelectionNode " << this << " _x=" << _x << " _y=" << _y
               << " _tx=" << _tx << " _ty=" << _ty << endl;
    if(!m_render) return -2;

    if(m_render->parent() && m_render->parent()->isAnonymousBox())
    {
	// we need to add the offset of the anonymous box
	_tx += m_render->parent()->xPos();
	_ty += m_render->parent()->yPos();
    }

    node = this;
    // Will return -2 (before), -1 (after) or 0 (hit), and will set offset
    return static_cast<RenderText *>(m_render)->checkSelectionPoint(_x, _y, _tx, _ty, offset);
}

ushort TextImpl::id() const
{
    return ID_TEXT;
}

NodeImpl *TextImpl::cloneNode(bool /*deep*/)
{
    TextImpl *newImpl = document->createTextNode(str);

    newImpl->setParent(0);
    newImpl->setFirstChild(0);
    newImpl->setLastChild(0);

    return newImpl;
}

void TextImpl::recalcStyle()
{
    if (!parentNode())
	return;
    m_style = parentNode()->activeStyle();
    if(m_render) m_render->setStyle(m_style);
}

// ---------------------------------------------------------------------------

CDATASectionImpl::CDATASectionImpl(DocumentImpl *impl, const DOMString &_text) : TextImpl(impl,_text)
{
}

CDATASectionImpl::CDATASectionImpl(DocumentImpl *impl) : TextImpl(impl)
{
}

CDATASectionImpl::~CDATASectionImpl()
{
}

const DOMString CDATASectionImpl::nodeName() const
{
  return "#cdata-section";
}

unsigned short CDATASectionImpl::nodeType() const
{
    return Node::CDATA_SECTION_NODE;
}

NodeImpl *CDATASectionImpl::cloneNode(bool /*deep*/)
{
    TextImpl *newImpl = document->createCDATASection(str);

    newImpl->setParent(0);
    newImpl->setFirstChild(0);
    newImpl->setLastChild(0);

    return newImpl;
}
