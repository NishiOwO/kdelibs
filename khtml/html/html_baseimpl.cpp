/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org))
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
// -------------------------------------------------------------------------

#include "dom_string.h"

#include <kapp.h>

#include "html_baseimpl.h"
#include "html_documentimpl.h"
using namespace DOM;

#include "khtmlview.h"
#include "khtml_part.h"
#include "htmlhashes.h"
#include <stdio.h>
#include <kurl.h>

#include <qcursor.h>
#include <qnamespace.h>

#include <kapp.h>

#include "rendering/render_object.h"
#include "rendering/render_frames.h"
#include "css/cssstyleselector.h"
#include "css/cssproperties.h"
#include "misc/loader.h"
#include <qframe.h>
#include <qstring.h>
#include <qscrollview.h>

#include <assert.h>


HTMLBodyElementImpl::HTMLBodyElementImpl(DocumentImpl *doc)
    : HTMLElementImpl(doc)
{
}

HTMLBodyElementImpl::~HTMLBodyElementImpl()
{
    if(m_style && m_style->backgroundImage()) m_style->backgroundImage()->deref(this);
}

const DOMString HTMLBodyElementImpl::nodeName() const
{
    return "BODY";
}

ushort HTMLBodyElementImpl::id() const
{
    return ID_BODY;
}

void HTMLBodyElementImpl::parseAttribute(Attribute *attr)
{
    switch(attr->id)
    {

    case ATTR_BACKGROUND:
    {
	KURL u = khtml::Cache::completeURL(attr->value(), static_cast<HTMLDocumentImpl *>(document)->URL());
        addCSSProperty(CSS_PROP_BACKGROUND_IMAGE, u.url(), false);
	break;
    }
    case ATTR_MARGINWIDTH:
    {
	QString str;
	str += attr->value().string() += "px";
        addCSSProperty(CSS_PROP_PADDING_LEFT, str, false);
        addCSSProperty(CSS_PROP_PADDING_RIGHT, str, false);
	break;	
    }
    case ATTR_MARGINHEIGHT:
    {
	QString str;
	str += attr->value().string() += "px";    
        addCSSProperty(CSS_PROP_PADDING_TOP, str, false);
        addCSSProperty(CSS_PROP_PADDING_BOTTOM, str, false);
	break;
    }
    case ATTR_BGCOLOR:
	addCSSProperty(CSS_PROP_BACKGROUND_COLOR, attr->value(), false);
	break;
    case ATTR_TEXT:
	addCSSProperty(CSS_PROP_COLOR, attr->value(), false);
	break;
    case ATTR_LINK:
    case ATTR_VLINK:
	// ### has to be added as stylesheet
    case ATTR_ALINK:
      break;
    default:
	HTMLElementImpl::parseAttribute(attr);
    }
}

// -------------------------------------------------------------------------

HTMLFrameElementImpl::HTMLFrameElementImpl(DocumentImpl *doc)
    : HTMLElementImpl(doc)
{
    view = 0;
    parentWidget = 0;

    frameBorder = true;
    marginWidth = -1;
    marginHeight = -1;
    scrolling = QScrollView::Auto;
    noresize = false;
}

HTMLFrameElementImpl::~HTMLFrameElementImpl()
{
}

const DOMString HTMLFrameElementImpl::nodeName() const
{
    return "FRAME";
}

ushort HTMLFrameElementImpl::id() const
{
    return ID_FRAME;
}

void HTMLFrameElementImpl::parseAttribute(Attribute *attr)
{
    printf("parsing attribute %d=%s\n", attr->id, attr->value().string().ascii());

    switch(attr->id)
    {
    case ATTR_SRC:
	url = attr->value();
	break;
    case ATTR_NAME:
	name = attr->value();
	break;
    case ATTR_FRAMEBORDER:
	if(attr->value() == "0" || strcasecmp( attr->value(), "no" ) == 0 )
	    frameBorder = false;
	break;
    case ATTR_MARGINWIDTH:
	marginWidth = attr->val()->toInt();
	break;
    case ATTR_MARGINHEIGHT:
	marginHeight = attr->val()->toInt();
	break;
    case ATTR_NORESIZE:
	noresize = true;
	break;
    case ATTR_SCROLLING:
	if( strcasecmp( attr->value(), "auto" ) == 0 )
	    scrolling = QScrollView::Auto;
	else if( strcasecmp( attr->value(), "yes" ) == 0 )
	    scrolling = QScrollView::AlwaysOn;
	else if( strcasecmp( attr->value(), "no" ) == 0 )
	    scrolling = QScrollView::AlwaysOff;

    default:
	HTMLElementImpl::parseAttribute(attr);
    }
}

void HTMLFrameElementImpl::attach(KHTMLView *w)
{
    m_style = document->styleSelector()->styleForElement( this );

    khtml::RenderObject *r = _parent->renderer();

    if ( !r )
      return;

    khtml::RenderFrame *renderFrame = new khtml::RenderFrame( m_style, w, this );
    m_render = renderFrame;
    m_render->ref();
    r->addChild( m_render );

    // we need a unique name for every frame in the frameset. Hope that's unique enough.
    if(name.isEmpty())
    {
      QString tmp;
      tmp.sprintf("0x%p", this);
      name = DOMString(tmp) + url;
      printf("creating frame name: %s\n",name.string().ascii());
    }

    w->part()->childRequest( renderFrame, url.string(), name.string() );

    NodeBaseImpl::attach( w );
    return;
#if 0
    printf("Frame::attach\n");
    m_style = document->styleSelector()->styleForElement(this);

    // needed for restoring frames
    bool open = true;

    parentWidget = w;
    if(w)
    {	
	// we need a unique name for every frame in the frameset. Hope that's unique enough.
	if(name.isEmpty())
	{
	    QString tmp;
	    tmp.sprintf("0x%p", this);
	    name = DOMString(tmp) + url;
	    printf("creating frame name: %s\n",name.string().ascii());
	}
	view = w->getFrame(name.string());
	if(view)
	    open = false;
	else
	    view = w->createFrame(w->viewport(), name.string());
	view->setIsFrame(true);
    }
    if(url != 0 && open)
    {
      //KURL u(w->url(), url.string());
      QString str = url.string();
	view->openURL(w->completeURL(str));
    }

    if(!parentWidget || !view) return;

    int x,y;
    getAbsolutePosition(x, y);
#ifdef DEBUG_LAYOUT
    printf("adding frame at %d/%d\n", x, y);
    printf("frame size %d/%d\n", width, descent);
#endif
    w->addChild(view, x, y);
    view->resize(width, descent);

    if(!frameBorder || !((static_cast<HTMLFrameSetElementImpl *>(_parent))->frameBorder()))
	view->setFrameStyle(QFrame::NoFrame);
    view->setVScrollBarMode(scrolling);
    view->setHScrollBarMode(scrolling);
    if(marginWidth != -1) view->setMarginWidth(marginWidth);
    if(marginHeight != -1) view->setMarginHeight(marginHeight);

    view->show();
    printf("adding frame\n");
#endif
}

void HTMLFrameElementImpl::detach()
{
    delete view;
    parentWidget = 0;
    NodeBaseImpl::detach();
}

// -------------------------------------------------------------------------

HTMLFrameSetElementImpl::HTMLFrameSetElementImpl(DocumentImpl *doc)
    : HTMLElementImpl(doc)
{
    // default value for rows and cols...
    m_totalRows = 1;
    m_totalCols = 1;

    m_rows = m_cols = 0;

    frameborder = true;
    m_border = 4;
    noresize = false;

    m_resizing = false;

    view = 0;

    setBlocking();
}

HTMLFrameSetElementImpl::~HTMLFrameSetElementImpl()
{
  if ( m_rows ) delete m_rows;
  if ( m_cols ) delete m_cols;
}

const DOMString HTMLFrameSetElementImpl::nodeName() const
{
    return "FRAMESET";
}

ushort HTMLFrameSetElementImpl::id() const
{
    return ID_FRAMESET;
}

void HTMLFrameSetElementImpl::parseAttribute(Attribute *attr)
{
    switch(attr->id)
    {
    case ATTR_ROWS:
	m_rows = attr->val()->toLengthList();
	m_totalRows = m_rows->count();
	break;
    case ATTR_COLS:
	m_cols = attr->val()->toLengthList();
	m_totalCols = m_cols->count();
	break;
    case ATTR_FRAMEBORDER:
	if(attr->value() == "0" || strcasecmp( attr->value(), "no" ) == 0 )
	    frameborder = false;
	break;
    case ATTR_NORESIZE:
	noresize = true;
	break;
    case ATTR_BORDER:
	m_border = attr->val()->toInt();
	break;
    default:
	HTMLElementImpl::parseAttribute(attr);
    }
}
/*
void HTMLFrameSetElementImpl::positionFrames(bool deep)
{
    int r;
    int c;
    NodeImpl *child = _first;
    if(!child) return;

    int yPos = 0;

    for(r = 0; r < totalRows; r++)
    {
	int xPos = 0;
	for(c = 0; c < totalCols; c++)
	{
	    HTMLElementImpl *e = static_cast<HTMLElementImpl *>(child);
	
	    if ( !e->renderer() )
	      continue;
	
	    e->renderer()->setXPos(xPos);
	    e->renderer()->setYPos(yPos);
	    e->setWidth(colWidth[c]);
	    e->setAvailableWidth(colWidth[c]);
	    e->setDescent(rowHeight[r]);
	    if(deep)
		e->layout(deep);
	    xPos += colWidth[c] + border;
	    child = child->nextSibling();
	    if(!child) return;
	}
	yPos += rowHeight[r] + border;
    }
}
*/
void HTMLFrameSetElementImpl::attach(KHTMLView *w)
{
    m_style = document->styleSelector()->styleForElement( this );
    view = w;

    // view->layout()

    khtml::RenderObject *r = _parent->renderer();

    if ( !r )
      return;

    khtml::RenderFrameSet *renderFrameSet = new khtml::RenderFrameSet( m_style, this, w, m_rows, m_cols );
    m_render = renderFrameSet;
    m_render->ref();
    r->addChild( m_render );

    NodeBaseImpl::attach( w );
#if 0
    m_style = document->styleSelector()->styleForElement(this);

    // ensure the htmlwidget knows we have a frameset, and adjusts the width accordingly
    w->layout();
    view = w;
    NodeBaseImpl::attach(w);
#endif
}

NodeImpl *HTMLFrameSetElementImpl::addChild(NodeImpl *child)
{
#ifdef DEBUG_LAYOUT
    printf("%s(FrameSet)::addChild( %s )\n", nodeName().string().ascii(), child->nodeName().string().ascii());
#endif

    return NodeBaseImpl::addChild(child);
}

bool HTMLFrameSetElementImpl::mouseEvent( int _x, int _y, int button, MouseEventType type,
				  int _tx, int _ty, DOMString &url,
                                          NodeImpl *&innerNode, long &offset)
{
    _x-=_tx;
    _y-=_ty;
    printf("mouseEvent\n");

    NodeImpl *child = _first;
    while(child)
    {
	if(child->id() == ID_FRAMESET)
	    if(child->mouseEvent( _x, _y, button, type, _tx, _ty, url, innerNode, offset)) return true;
	child = child->nextSibling();
    }

    qDebug( "children done.." );

    if(noresize) return true;

    if ( !m_render || !m_render->layouted() )
    {
      qDebug( "ugh, not layouted or not attached?!" );
      return true;
    }

    return static_cast<khtml::RenderFrameSet *>(m_render)->userResize( _x, _y, type );

    /*
    {
      printf("mouseEvent:check\n");
	
	hSplit = -1;
	vSplit = -1;
	//bool resizePossible = true;

	// check if we're over a horizontal or vertical boundary
	int pos = m_colWidth[0];
	for(int c = 1; c < m_totalCols; c++)
	{
	    if(_x >= pos && _x <= pos+m_border)
	    {
		if(vSplitVar && vSplitVar[c-1] == true) vSplit = c-1;
		printf("vsplit!\n");
		break;
	    }
	    pos += m_colWidth[c] + m_border;
	}
	pos = m_rowHeight[0];
	for(int r = 1; r < m_totalRows; r++)
	{
	    if( _y >= pos && _y <= pos+m_border)
	    {
		if(hSplitVar && hSplitVar[r-1] == true) hSplit = r-1;
		printf("hsplitvar = %p\n", hSplitVar);
		printf("hsplit!\n");
		break;
	    }
	    pos += m_rowHeight[r] + m_border;
	}
	printf("%d/%d\n", hSplit, vSplit);

	QCursor cursor;
	if(hSplit != -1 && vSplit != -1)
	{
	    cursor = Qt::sizeAllCursor;
	}
	else if( vSplit != -1 )
	{
	    cursor = Qt::splitHCursor;
	}	
	else if( hSplit != -1 )
	{
	    cursor = Qt::splitVCursor;
	}

	if(type == MousePress)
	{
	    resizing = true;
	    KApplication::setOverrideCursor(cursor);
	    vSplitPos = _x;
	    hSplitPos = _y;
	}
	else
	    view->setCursor(cursor);
    }

    // ### need to draw a nice movin indicator for the resize.
    // ### check the resize is not going out of bounds.
    if(resizing && type == MouseRelease)
    {
	resizing = false;
	KApplication::restoreOverrideCursor();

	if(hSplit)
	{
	    printf("split xpos=%d\n", _x);
	    int delta = vSplitPos - _x;
	    m_colWidth[vSplit] -= delta;
	    m_colWidth[vSplit+1] += delta;
	}	
	if(vSplit)
	{
	    printf("split ypos=%d\n", _y);
	    int delta = hSplitPos - _y;
	    m_rowHeight[hSplit] -= delta;
	    m_rowHeight[hSplit+1] += delta;
	}
	qDebug( "starting relayout!" );
	m_render->layout( true );
    }
    */
}
// -------------------------------------------------------------------------

HTMLHeadElementImpl::HTMLHeadElementImpl(DocumentImpl *doc)
    : HTMLElementImpl(doc)
{
    setBlocking();
}

HTMLHeadElementImpl::~HTMLHeadElementImpl()
{
}

const DOMString HTMLHeadElementImpl::nodeName() const
{
    return "HEAD";
}

ushort HTMLHeadElementImpl::id() const
{
    return ID_HEAD;
}

// -------------------------------------------------------------------------

HTMLHtmlElementImpl::HTMLHtmlElementImpl(DocumentImpl *doc)
    : HTMLElementImpl(doc)
{
}

HTMLHtmlElementImpl::~HTMLHtmlElementImpl()
{
}

const DOMString HTMLHtmlElementImpl::nodeName() const
{
    return "HTML";
}

ushort HTMLHtmlElementImpl::id() const
{
    return ID_HTML;
}

