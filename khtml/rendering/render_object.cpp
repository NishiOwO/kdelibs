/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
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

#include "render_object.h"

#include "dom_nodeimpl.h"
#include "render_box.h"
#include "render_flow.h"
#include "render_style.h"
#include "render_table.h"
#include "render_list.h"
#include "render_style.h"
#include "render_root.h"
#include "render_hr.h"
#include "misc/loader.h"

#include <kdebug.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpen.h>
#include <qsize.h>
#include <qtextstream.h>

#include <assert.h>
using namespace DOM;
using namespace khtml;

RenderObject *RenderObject::createObject(DOM::NodeImpl *node)
{
    RenderStyle *style = node->style();
    RenderObject *o = 0;
    switch(style->display())
    {
    case INLINE:
    case BLOCK:
        o = new RenderFlow();
        break;
    case LIST_ITEM:
        o = new RenderListItem();
        break;
    case RUN_IN:
    case COMPACT:
    case MARKER:
        break;
    case KONQ_RULER:
        o = new RenderHR();
        break;
    case TABLE:
    case INLINE_TABLE:
        // ### set inline/block right
        //kdDebug( 6040 ) << "creating RenderTable" << endl;
        o = new RenderTable();
        break;
    case TABLE_ROW_GROUP:
    case TABLE_HEADER_GROUP:
    case TABLE_FOOTER_GROUP:
        o = new RenderTableSection();
        break;
    case TABLE_ROW:
        o = new RenderTableRow();
        break;
    case TABLE_COLUMN_GROUP:
    case TABLE_COLUMN:
        o = new RenderTableCol();
        break;
    case TABLE_CELL:
        o = new RenderTableCell();
        break;
    case TABLE_CAPTION:
        o = new RenderTableCaption();
        break;
    case NONE:
        return 0;
    }
    if(o) o->setStyle(style);
    return o;
}


RenderObject::RenderObject()
{
    m_style = 0;

    m_layouted = false;
    m_parsing = false;
    m_minMaxKnown = false;

    m_parent = 0;
    m_previous = 0;
    m_next = 0;

    m_floating = false;
    m_positioned = false;
    m_relPositioned = false;
    m_printSpecial = false;
    m_containsPositioned = false;
    m_isAnonymous = false;
    m_isText = false;
    m_inline = true;
    m_replaced = false;
    m_visible = true;
    m_containsWidget = false;
    m_containsOverhangingFloats = false;
    m_hasFirstLine = false;
    m_verticalPosition = PositionUndefined;
}

RenderObject::~RenderObject()
{
    if(m_style->backgroundImage())
        m_style->backgroundImage()->deref(this);

    if (m_style)
	m_style->deref();
}


void RenderObject::addChild(RenderObject* , RenderObject *)
{
    assert(0);
}

RenderObject* RenderObject::removeChildNode(RenderObject* )
{
    assert(0);
    return 0;
}

void RenderObject::removeChild(RenderObject* )
{
    assert(0);
}

void RenderObject::appendChildNode(RenderObject*)
{
    assert(0);
}

void RenderObject::insertChildNode(RenderObject*, RenderObject*)
{
    assert(0);
}

RenderObject *RenderObject::containingBlock() const
{
    if(isTableCell()) {
        return static_cast<const RenderTableCell *>(this)->table();
    }

    RenderObject *o = parent();
    if(m_style->position() == FIXED) {
        while ( o && !o->isRoot() )
            o = o->parent();
    }
    else if(m_style->position() == ABSOLUTE) {
        while (o && o->style()->position() == STATIC && !o->isHtml() && !o->isRoot())
            o = o->parent();
    } else {
        while(o && o->isInline())
            o = o->parent();
    }
    // this is just to make sure we return a valid element.
    // the case below should never happen...
    if(!o) {
        if(!isRoot()) {
            //assert ( false );
            kdDebug( 6040 ) << renderName() << "(RenderObject): No containingBlock!" << endl;
        }
        return const_cast<RenderObject *>(this);
    } else
        return o;
}

short RenderObject::containingBlockWidth() const
{
    // ###
    return containingBlock()->contentWidth();
}

int RenderObject::containingBlockHeight() const
{
    // ###
    return containingBlock()->contentHeight();
}

void RenderObject::drawBorder(QPainter *p, int x1, int y1, int x2, int y2,
                              BorderSide s, QColor c, const QColor& textcolor, EBorderStyle style,
                              int adjbw1, int adjbw2, bool invalidisInvert)
{
    int width = (s==BSTop||s==BSBottom?y2-y1:x2-x1);

    if(style == DOUBLE && width < 3)
        style = SOLID;

    if(!c.isValid()) {
        if(invalidisInvert)
        {
            p->setRasterOp(Qt::XorROP);
            c = Qt::white;
        }
        else {
            if(style == INSET || style == OUTSET || style == RIDGE || style ==
	    GROOVE)
                c.setRgb(238, 238, 238);
            else
                c = textcolor;
        }
    }

    switch(style)
    {
    case BNONE:
    case BHIDDEN:
        // should not happen
        if(invalidisInvert && p->rasterOp() == Qt::XorROP)
            p->setRasterOp(Qt::CopyROP);

        return;
    case DOTTED:
        p->setPen(QPen(c, width == 1 ? 0 : width, Qt::DotLine));
        /* nobreak; */
    case DASHED:
        if(style == DASHED)
            p->setPen(QPen(c, width == 1 ? 0 : width, Qt::DashLine));
        {
            switch(s)
            {
            case BSBottom:
            case BSTop:
                p->drawLine(x1, (y1+y2)/2, x2, (y1+y2)/2);
            case BSRight:
            case BSLeft:
                p->drawLine((x1+x2)/2, y1, (x1+x2)/2, y2);
            }
        }
        break;

    case DOUBLE:
    {
        int third = (width+1)/3;

	if (adjbw1 == 0 && adjbw2 == 0)
	{
	    p->setPen(Qt::NoPen);
	    p->setBrush(c);
	    switch(s)
	    {
	    case BSTop:
	    case BSBottom:
	        p->drawRect(x1, y1      , x2-x1, third);
		p->drawRect(x1, y2-third, x2-x1, third);
		break;
	    case BSLeft:
	        p->drawRect(x1      , y1+1, third, y2-y1-1);
		p->drawRect(x2-third, y1+1, third, y2-y1-1);
		break;
	    case BSRight:
	        p->drawRect(x1      , y1+1, third, y2-y1-1);
		p->drawRect(x2-third, y1+1, third, y2-y1-1);
		break;
	    }
	}
	else
	{
	    int adjbw1bigthird;
	    if (adjbw1>0) adjbw1bigthird = adjbw1+1;
	    else adjbw1bigthird = adjbw1 - 1;
	    adjbw1bigthird /= 3;

	    int adjbw2bigthird;
	    if (adjbw2>0) adjbw2bigthird = adjbw2 + 1;
	    else adjbw2bigthird = adjbw2 - 1;
	    adjbw2bigthird /= 3;

	  switch(s)
	    {
	    case BSTop:
	      drawBorder(p, x1+QMAX((-adjbw1*2+1)/3,0), y1        , x2-QMAX((-adjbw2*2+1)/3,0), y1 + third, s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      drawBorder(p, x1+QMAX(( adjbw1*2+1)/3,0), y2 - third, x2-QMAX(( adjbw2*2+1)/3,0), y2        , s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      break;
	    case BSLeft:
	      drawBorder(p, x1        , y1+QMAX((-adjbw1*2+1)/3,0), x1+third, y2-QMAX((-adjbw2*2+1)/3,0), s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      drawBorder(p, x2 - third, y1+QMAX(( adjbw1*2+1)/3,0), x2      , y2-QMAX(( adjbw2*2+1)/3,0), s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      break;
	    case BSBottom:
	      drawBorder(p, x1+QMAX(( adjbw1*2+1)/3,0), y1      , x2-QMAX(( adjbw2*2+1)/3,0), y1+third, s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      drawBorder(p, x1+QMAX((-adjbw1*2+1)/3,0), y2-third, x2-QMAX((-adjbw2*2+1)/3,0), y2      , s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      break;
	    case BSRight:
            drawBorder(p, x1      , y1+QMAX(( adjbw1*2+1)/3,0), x1+third, y2-QMAX(( adjbw2*2+1)/3,0), s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	    drawBorder(p, x2-third, y1+QMAX((-adjbw1*2+1)/3,0), x2      , y2-QMAX((-adjbw2*2+1)/3,0), s, c, textcolor, SOLID, adjbw1bigthird, adjbw2bigthird);
	      break;
	    default:
	      break;
	    }
	}
        break;
    }
    case RIDGE:
    case GROOVE:
    {
        EBorderStyle s1;
        EBorderStyle s2;
        if (style==GROOVE)
        {
            s1 = INSET;
            s2 = OUTSET;
        }
        else
        {
            s1 = OUTSET;
            s2 = INSET;
        }

	int adjbw1bighalf;
	int adjbw2bighalf;
	if (adjbw1>0) adjbw1bighalf=adjbw1+1;
	else adjbw1bighalf=adjbw1-1;
	adjbw1bighalf/=2;

	if (adjbw2>0) adjbw2bighalf=adjbw2+1;
	else adjbw2bighalf=adjbw2-1;
	adjbw2bighalf/=2;

        switch (s)
	{
	case BSTop:
	    drawBorder(p, x1+QMAX(-adjbw1  ,0)/2,  y1        , x2-QMAX(-adjbw2,0)/2, (y1+y2+1)/2, s, c, textcolor, s1, adjbw1bighalf, adjbw2bighalf);
	    drawBorder(p, x1+QMAX( adjbw1+1,0)/2, (y1+y2+1)/2, x2-QMAX( adjbw2+1,0)/2,  y2        , s, c, textcolor, s2, adjbw1/2, adjbw2/2);
	    break;
	case BSLeft:
            drawBorder(p,  x1        , y1+QMAX(-adjbw1  ,0)/2, (x1+x2+1)/2, y2-QMAX(-adjbw2,0)/2, s, c, textcolor, s1, adjbw1bighalf, adjbw2bighalf);
	    drawBorder(p, (x1+x2+1)/2, y1+QMAX( adjbw1+1,0)/2,  x2        , y2-QMAX( adjbw2+1,0)/2, s, c, textcolor, s2, adjbw1/2, adjbw2/2);
	    break;
	case BSBottom:
	    drawBorder(p, x1+QMAX( adjbw1  ,0)/2,  y1        , x2-QMAX( adjbw2,0)/2, (y1+y2+1)/2, s, c, textcolor, s2,  adjbw1bighalf, adjbw2bighalf);
	    drawBorder(p, x1+QMAX(-adjbw1+1,0)/2, (y1+y2+1)/2, x2-QMAX(-adjbw2+1,0)/2,  y2        , s, c, textcolor, s1, adjbw1/2, adjbw2/2);
	    break;
	case BSRight:
            drawBorder(p,  x1        , y1+QMAX( adjbw1  ,0)/2, (x1+x2+1)/2, y2-QMAX( adjbw2,0)/2, s, c, textcolor, s2, adjbw1bighalf, adjbw2bighalf);
	    drawBorder(p, (x1+x2+1)/2, y1+QMAX(-adjbw1+1,0)/2,  x2        , y2-QMAX(-adjbw2+1,0)/2, s, c, textcolor, s1, adjbw1/2, adjbw2/2);
	    break;
	}
        break;
    }
    case INSET:
        if(s == BSTop || s == BSLeft)
            c = c.dark();

        /* nobreak; */
    case OUTSET:
        if(style == OUTSET && (s == BSBottom || s == BSRight))
            c = c.dark();
        /* nobreak; */
    case SOLID:
        QPointArray quad(4);
        p->setPen(Qt::NoPen);
        p->setBrush(c);
#if QT_VERSION < 300
	ASSERT(x2>x1);
	ASSERT(y2>y1);
#else
	Q_ASSERT(x2>x1);
	Q_ASSERT(y2>y1);
#endif
	if (adjbw1==0 && adjbw2 == 0)
	  {
            p->drawRect(x1,y1,x2-x1,y2-y1);
	    return;
	  }
	switch(s) {
        case BSTop:
            quad.setPoints(4,
			   x1+QMAX(-adjbw1,0), y1,
                           x1+QMAX( adjbw1,0), y2,
                           x2-QMAX( adjbw2,0), y2,
                           x2-QMAX(-adjbw2,0), y1);
            break;
        case BSBottom:
            quad.setPoints(4,
			   x1+QMAX( adjbw1,0), y1,
                           x1+QMAX(-adjbw1,0), y2,
                           x2-QMAX(-adjbw2,0), y2,
                           x2-QMAX( adjbw2,0), y1);
            break;
        case BSLeft:
	  quad.setPoints(4,
			 x1, y1+QMAX(-adjbw1,0),
               		 x1, y2-QMAX(-adjbw2,0),
			 x2, y2-QMAX( adjbw2,0),
			 x2, y1+QMAX( adjbw1,0));
            break;
        case BSRight:
	  quad.setPoints(4,
			 x1, y1+QMAX( adjbw1,0),
               		 x1, y2-QMAX( adjbw2,0),
			 x2, y2-QMAX(-adjbw2,0),
			 x2, y1+QMAX(-adjbw1,0));
            break;
        }
	p->drawPolygon(quad);
        break;
    }

    if(invalidisInvert && p->rasterOp() == Qt::XorROP)
        p->setRasterOp(Qt::CopyROP);
}

void RenderObject::printBorder(QPainter *p, int _tx, int _ty, int w, int h, const RenderStyle* style, bool begin, bool end)
{
    const QColor& tc = style->borderTopColor();
    const QColor& bc = style->borderBottomColor();

    EBorderStyle ts = style->borderTopStyle();
    EBorderStyle bs = style->borderBottomStyle();
    EBorderStyle ls = style->borderLeftStyle();
    EBorderStyle rs = style->borderRightStyle();

    bool render_t = ts > BHIDDEN;
    bool render_l = ls > BHIDDEN && begin;
    bool render_r = rs > BHIDDEN && end;
    bool render_b = bs > BHIDDEN;

    if(render_t)
        drawBorder(p, _tx, _ty, _tx + w, _ty +  style->borderTopWidth(), BSTop, tc, style->color(), ts,
                   (render_l && ls<=DOUBLE?style->borderLeftWidth():0),
		   (render_r && rs<=DOUBLE?style->borderRightWidth():0));

    if(render_b)
        drawBorder(p, _tx, _ty + h - style->borderBottomWidth(), _tx + w, _ty + h, BSBottom, bc, style->color(), bs,
                   (render_l && ls<=DOUBLE?style->borderLeftWidth():0),
		   (render_r && rs<=DOUBLE?style->borderRightWidth():0));

    if(render_l)
    {
	const QColor& lc = style->borderLeftColor();

	bool ignore_top =
	  (tc == lc) &&
	  (ls <= OUTSET) &&
	  (ts == DOTTED || ts == DASHED || ts == SOLID || ts == OUTSET);

	bool ignore_bottom =
	  (bc == lc) &&
	  (ls <= OUTSET) &&
	  (bs == DOTTED || bs == DASHED || bs == SOLID || bs == INSET);

        drawBorder(p, _tx, _ty, _tx + style->borderLeftWidth(), _ty + h, BSLeft, lc, style->color(), ls,
		   ignore_top?0:style->borderTopWidth(),
		   ignore_bottom?0:style->borderBottomWidth());
    }

    if(render_r)
    {
	const QColor& rc = style->borderRightColor();

	bool ignore_top =
	  (tc == rc) &&
	  (rs <= SOLID || rs == INSET) &&
	  (ts == DOTTED || ts == DASHED || ts == SOLID || ts == OUTSET);

	bool ignore_bottom =
	  (bc == rc) &&
	  (rs <= SOLID || rs == INSET) &&
	  (bs == DOTTED || bs == DASHED || bs == SOLID || bs == INSET);

        drawBorder(p, _tx + w - style->borderRightWidth(), _ty, _tx + w, _ty + h, BSRight, rc, style->color(), rs,
		   ignore_top?0:style->borderTopWidth(),
		   ignore_bottom?0:style->borderBottomWidth());
    }
}

void RenderObject::printOutline(QPainter *p, int _tx, int _ty, int w, int h, const RenderStyle* style)
{
    int ow = style->outlineWidth();
    if(!ow) return;

    const QColor& oc = style->outlineColor();
    EBorderStyle os = style->outlineStyle();

    drawBorder(p, _tx-ow, _ty-ow, _tx, _ty+h+ow, BSLeft,
	       QColor(oc), style->color(),
               os, ow, ow, true);

    drawBorder(p, _tx-ow, _ty-ow, _tx+w+ow, _ty, BSTop,
	       QColor(oc), style->color(),
	       os, ow, ow, true);

    drawBorder(p, _tx+w, _ty-ow, _tx+w+ow, _ty+h+ow, BSRight,
	       QColor(oc), style->color(),
	       os, ow, ow, true);

    drawBorder(p, _tx-ow, _ty+h, _tx+w+ow, _ty+h+ow, BSBottom,
	       QColor(oc), style->color(),
	       os, ow, ow, true);

}

void RenderObject::print( QPainter *p, int x, int y, int w, int h, int tx, int ty)
{
    printObject(p, x, y, w, h, tx, ty);
}

void RenderObject::repaintRectangle(int x, int y, int w, int h, bool f)
{
    if(parent()) parent()->repaintRectangle(x, y, w, h, f);
}

void RenderObject::printTree(int indent) const
{
    QString ind;
    ind.fill(' ', indent);
    int childcount = 0;
    for(RenderObject* c = firstChild(); c; c = c->nextSibling())
        childcount++;

    kdDebug()    << ind << renderName()
                 << (childcount ?
                     (QString::fromLatin1("[") + QString::number(childcount) + QString::fromLatin1("]"))
                     : QString::null)
                 << "(" << (style() ? style()->refCount() : 0) << ")"
                 << ": " << (void*)this
                 << " il=" << (int)isInline() << " ci=" << (int) childrenInline()
                 << " fl=" << (int)isFloating() << " rp=" << (int)isReplaced()
                 << " an=" << (int)isAnonymousBox()
                 << " ps=" << (int)isPositioned()
                 << " cp=" << (int)containsPositioned()
                 << " lt=" << (int)layouted()
                 << " cw=" << (int)containsWidget()
                 << " pa=" << (int)parsing()
                 << " (" << xPos() << "," << yPos() << "," << width() << "," << height() << ")" << endl;
    RenderObject *child = firstChild();
    while( child != 0 )
    {
        child->printTree(indent+2);
        child = child->nextSibling();
    }
}

void RenderObject::dump(QTextStream *stream, QString ind) const
{
    if (isAnonymousBox()) { *stream << " anonymousBox"; }
    if (isFloating()) { *stream << " floating"; }
    if (isPositioned()) { *stream << " positioned"; }
    if (isRelPositioned()) { *stream << " relPositioned"; }
    if (isText()) { *stream << " text"; }
    if (isInline()) { *stream << " inline"; }
    if (isReplaced()) { *stream << " replaced"; }
    if (hasSpecialObjects()) { *stream << " specialObjects"; }
    if (isVisible()) { *stream << " visible"; }
    if (layouted()) { *stream << " layouted"; }
    if (parsing()) { *stream << " parsing"; }
    if (minMaxKnown()) { *stream << " minMaxKnown"; }
    if (containsPositioned()) { *stream << " containsPositioned"; }
    if (containsWidget()) { *stream << " containsWidget"; }
    if (hasFirstLine()) { *stream << " hasFirstLine"; }
    *stream << endl;

    RenderObject *child = firstChild();
    while( child != 0 )
    {
	*stream << ind << child->renderName() << ": ";
	child->dump(stream,ind+"  ");
	child = child->nextSibling();
    }
}

void RenderObject::selectionStartEnd(int& spos, int& epos)
{
    if (parent())
        parent()->selectionStartEnd(spos, epos);
}

void RenderObject::updateSize()
{
    containingBlock()->updateSize();
}

void RenderObject::setStyle(RenderStyle *style)
{
    if (m_style == style)
	return;

    // reset style flags
    m_floating = false;
    m_positioned = false;
    m_relPositioned = false;
    m_printSpecial = false;
    // no support for changing the display type dynamically... object must be
    // detached and re-attached as a different type
    //m_inline = true;
    m_visible = true;

    RenderStyle *oldStyle = m_style;
    m_style = style;

    CachedImage* ob = 0;
    CachedImage* nb = 0;

    if (m_style)
    {
	m_style->ref();
        nb = m_style->backgroundImage();
    }
    if (oldStyle)
    {
        ob = oldStyle->backgroundImage();
	oldStyle->deref();
    }

    if( ob != nb ) {
	if(ob) ob->deref(this);
	if(nb) nb->ref(this);
    }

    if( m_style->backgroundColor().isValid() || m_style->hasBorder() || nb )
        setSpecialObjects(true);
    else
        setSpecialObjects(false);

    if( m_style->visiblity() == HIDDEN || m_style->visiblity() == COLLAPSE )
	m_visible = false;

    m_hasFirstLine = (style->getPseudoStyle(RenderStyle::FIRST_LINE) != 0);

    setMinMaxKnown(false);
    setLayouted(false);
}

void RenderObject::setContainsPositioned(bool p)
{
    if (p)
    {
        m_containsPositioned = true;
        if (containingBlock()!=this)
            containingBlock()->setContainsPositioned(true);
    }
    else
    {
        RenderObject *n;
        bool c=false;

        for( n = firstChild(); n != 0; n = n->nextSibling() )
        {
            if (n->isPositioned() || n->containsPositioned())
                c=true;
        }

        if (c)
            return;
        else
        {
            m_containsPositioned = false;
            if (containingBlock()!=this)
                containingBlock()->setContainsPositioned(false);
        }
    }
}

QRect RenderObject::viewRect() const
{
    return containingBlock()->viewRect();
}

bool RenderObject::absolutePosition(int &xPos, int &yPos, bool f)
{
    if(parent())
        return parent()->absolutePosition(xPos, yPos, f);
    else
    {
        xPos = yPos = 0;
        return false;
    }
}

void RenderObject::cursorPos(int /*offset*/, int &_x, int &_y, int &height)
{
    _x = _y = height = -1;
}

int RenderObject::paddingTop() const
{
    int cw=0;
    if (style()->paddingTop().isPercent())
        cw = containingBlock()->contentWidth();
    return m_style->paddingTop().minWidth(cw);
}

int RenderObject::paddingBottom() const
{
    int cw=0;
    if (style()->paddingBottom().isPercent())
        cw = containingBlock()->contentWidth();
    return m_style->paddingBottom().minWidth(cw);
}

int RenderObject::paddingLeft() const
{
    int cw=0;
    if (style()->paddingLeft().isPercent())
        cw = containingBlock()->contentWidth();
    return m_style->paddingLeft().minWidth(cw);
}

int RenderObject::paddingRight() const
{
    int cw=0;
    if (style()->paddingRight().isPercent())
        cw = containingBlock()->contentWidth();
    return m_style->paddingRight().minWidth(cw);
}

RenderRoot* RenderObject::root() const
{
    RenderObject* o = const_cast<RenderObject*>( this );
    while ( o->parent() ) o = o->parent();

    assert( o->isRoot() );
    return static_cast<RenderRoot*>( o );
}

RenderObject *RenderObject::container() const
{
    EPosition pos = m_style->position();
    RenderObject *o = 0;
    if( pos == FIXED )
	o = root();
    else if ( pos == ABSOLUTE )
	o = containingBlock();
    else
	o = parent();
    return o;
}

void RenderObject::invalidateLayout()
{
    setLayouted(false);
    if (m_parent && m_parent->layouted())
        m_parent->invalidateLayout();
}

void RenderObject::removeFromSpecialObjects()
{
    if (isPositioned() || isFloating()) {
	RenderObject *p;
	for (p = parent(); p; p = p->parent()) {
	    if (p->isFlow())
		static_cast<RenderFlow*>(p)->removeSpecialObject(this);
	}
    }
}

void RenderObject::detach()
{
    remove();
    // by default no refcounting
    delete this;
}

bool RenderObject::containsPoint(int /*_x*/, int /*_y*/, int /*_tx*/, int /*_ty*/)
{
    return false;
}

short RenderObject::verticalPositionHint( bool firstLine ) const
{
    short vpos = m_verticalPosition;
    if ( m_verticalPosition == PositionUndefined || firstLine ) {
	vpos = getVerticalPosition( firstLine );
	if ( !firstLine )
	    const_cast<RenderObject *>(this)->m_verticalPosition = vpos;
    }
    return vpos;

}

short RenderObject::getVerticalPosition( bool firstLine ) const
{
    // vertical align for table cells has a different meaning
    int vpos = 0;
    if ( !isTableCell() ) {
	EVerticalAlign va = style()->verticalAlign();
	if ( va == TOP ) {
	    vpos = PositionTop;
	} else if ( va == BOTTOM ) {
	    vpos = PositionBottom;
	} else if ( va == LENGTH ) {
	    vpos = -style()->verticalAlignLength().width( lineHeight( firstLine ) );
	} else if ( parent() && parent()->childrenInline() ) {
	    vpos = parent()->verticalPositionHint( firstLine );
	    // don't allow elements nested inside text-top to have a different valignment.
	    if ( va == BASELINE || vpos == PositionBottom )
		return vpos;

            if ( vpos == PositionTop )
                vpos = 0;

	    QFont f = parent()->font( firstLine );
            int fontheight = parent()->lineHeight( firstLine );
            int fontsize = f.pixelSize();
            int halfleading = ( fontheight - fontsize ) / 2;

	    if ( va == SUB )
		vpos += fontsize/5 + 1;
	    else if ( va == SUPER )
		vpos -= fontsize/3 + 1;
	    else if ( va == TEXT_TOP ) {
//                 qDebug( "got TEXT_TOP vertical pos hint" );
//                 qDebug( "parent:" );
//                 qDebug( "CSSLH: %d, CSS_FS: %d, basepos: %d", fontheight, fontsize, parent()->baselinePosition( firstLine ) );
//                 qDebug( "this:" );
//                 qDebug( "CSSLH: %d, CSS_FS: %d, basepos: %d", lineHeight( firstLine ), style()->font().pixelSize(), baselinePosition( firstLine ) );
                vpos += baselinePosition( firstLine ) - parent()->baselinePosition( firstLine ) +
                        halfleading;
	    } else if ( va == MIDDLE ) {
		QRect b = QFontMetrics(f).boundingRect('x');
		vpos += -b.height()/2 - lineHeight( firstLine )/2 + baselinePosition( firstLine );
	    } else if ( va == TEXT_BOTTOM ) {
		vpos += QFontMetrics(f).descent();
		if ( !isReplaced() )
		    vpos -= QFontMetrics(font(firstLine)).descent();
	    } else if ( va == BASELINE_MIDDLE )
		vpos += - lineHeight( firstLine )/2 + baselinePosition( firstLine );
	}
    }
    return vpos;
}

int RenderObject::lineHeight( bool firstLine ) const
{
    Length lh;
    if( firstLine && hasFirstLine() ) {
	RenderStyle *pseudoStyle  = style()->getPseudoStyle(RenderStyle::FIRST_LINE);
	if ( pseudoStyle )
            lh = pseudoStyle->lineHeight();
    }
    else
        lh = style()->lineHeight();

    // its "unset", choose nice default
    if ( lh.value < 0 )
        return QFontMetrics( style()->font() ).height();

    if ( lh.isPercent() )
        return lh.minWidth( style()->font().pixelSize() );

    // its fixed
    return lh.value;
}

short RenderObject::baselinePosition( bool firstLine ) const
{
    QFont f = font( firstLine );
    return QFontMetrics( f ).ascent() + ( lineHeight( firstLine ) - QFontMetrics( f ).height() ) / 2;
}

QFont RenderObject::font(bool firstLine) const
{
    if( firstLine && hasFirstLine() ) {
	RenderStyle *pseudoStyle  = style()->getPseudoStyle(RenderStyle::FIRST_LINE);
	if ( pseudoStyle )
	    return pseudoStyle->font();
    }
    return style()->font();
}

void RenderObject::invalidateVerticalPositions()
{
    m_verticalPosition = PositionUndefined;
    RenderObject *child = firstChild();
    while( child ) {
	child->invalidateVerticalPositions();
	child = child->nextSibling();
    }
}

