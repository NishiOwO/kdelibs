/*-
 * B3Style is 99.9999% B2Style code
 * B2Style (C)2000 Daniel M. Duley <mosfet@kde.org>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#ifndef INCLUDE_MENUITEM_DEF
#define INCLUDE_MENUITEM_DEF
#endif

#include <qmenudata.h>
#include "b3style.h"
#include <kapp.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kdrawutil.h>
#include <qpalette.h>
#include <qbitmap.h>
#include <qtabbar.h>
#include <kglobalsettings.h>

//#include "bitmaps.h"

B3Style::B3Style()
    :B2Style()
{
}

B3Style::~B3Style()
{
}

void B3Style::drawScrollBarControls(QPainter *p, const QScrollBar *sb,
                                       int sliderStart, uint controls,
                                       uint activeControl)
{
    int sliderMin, sliderMax, sliderLength, buttonDim;
    scrollBarMetrics( sb, sliderMin, sliderMax, sliderLength, buttonDim );
 
    if (sliderStart > sliderMax) 
        sliderStart = sliderMax;

    bool horiz = sb->orientation() == QScrollBar::Horizontal;
    QColorGroup g = sb->colorGroup();
    QRect addB, subB3, subB;
    QRect addPageR, subPageR, sliderR;
    int addX, addY, subX, subY;
    int len = horiz ? sb->width() : sb->height();
    int extent = horiz ? sb->height() : sb->width();
 
    if (horiz) {
        subY = addY = ( extent - buttonDim ) / 2;
        subX = 0;
        addX = len - buttonDim;
    }
    else {
        subX = addX = ( extent - buttonDim ) / 2;
        subY = 0;
        addY = len - buttonDim;
    }
    subB.setRect( subX,subY,buttonDim,buttonDim );
    addB.setRect( addX,addY,buttonDim,buttonDim );
    if(horiz)
        subB3.setRect(addX-buttonDim,addY,buttonDim,buttonDim );
    else
        subB3.setRect(addX,addY-buttonDim,buttonDim,buttonDim );
        
    int sliderEnd = sliderStart + sliderLength;
    int sliderW = extent;

    if (horiz) {
        subPageR.setRect( subB.right() + 1, 0,
                          sliderStart - subB.right() - 1 , sliderW );
        addPageR.setRect( sliderEnd, 0, addX - sliderEnd - buttonDim, sliderW );
        sliderR .setRect( sliderStart, 0, sliderLength, sliderW );
    }
    else {
        subPageR.setRect( 0, subB.bottom() + 1, sliderW,
                          sliderStart - subB.bottom() - 1 );
        addPageR.setRect( 0, sliderEnd, sliderW, addY - buttonDim - sliderEnd);
        sliderR .setRect( 0, sliderStart, sliderW, sliderLength );
    }
    
    bool maxed = sb->maxValue() == sb->minValue();
    
    if ( controls & AddLine ) {
        drawSBButton(p, addB, g);
        if (horiz)
          drawSBArrow( p, RightArrow,
                       activeControl == AddLine, addB.x()+3, addB.y()+4,
                       addB.width()-7, addB.height()-10, g, !maxed);
        else
          drawSBArrow( p, DownArrow,
                       activeControl == AddLine, addB.x()+5, addB.y()+3,
                       addB.width()-10, addB.height()-7, g, !maxed);
    }
    if ( controls & SubLine ) {
        drawSBButton(p, subB, g);
        if (horiz)
          drawSBArrow( p, LeftArrow,
                       activeControl == SubLine, subB.x()+3, subB.y()+4,
                       subB.width()-7, subB.height()-10, g, !maxed);
        else
          drawSBArrow( p, UpArrow,
                       activeControl == SubLine, subB.x()+5, subB.y()+3,
                       subB.width()-10, subB.height()-7, g, !maxed);

        drawSBButton(p, subB3, g);
        if (horiz)
          drawSBArrow( p, LeftArrow,
                       activeControl == SubLine, subB3.x()+3, subB3.y()+4,
                       subB3.width()-7, subB3.height()-10, g, !maxed);
        else
          drawSBArrow( p, UpArrow,
                       activeControl == SubLine, subB3.x()+5, subB3.y()+3,
                       subB3.width()-10, subB3.height()-7, g, !maxed);

    }
    if(controls & AddPage){
        if(addPageR.width()){
            p->fillRect(addPageR, activeControl == AddPage ?
                        g.brush(QColorGroup::Button) :
                        g.brush(QColorGroup::Midlight));
            p->setPen(g.mid());
            if(horiz){
                p->drawLine(addPageR.x(), addPageR.y(), addPageR.right(),
                            addPageR.y());
                p->drawLine(addPageR.x(), addPageR.bottom(), addPageR.right(),
                            addPageR.bottom());
                p->setPen(activeControl==AddPage ? g.button() : g.midlight());
                p->drawLine(addPageR.x(), addPageR.y()+1, addPageR.right(),
                            addPageR.y()+1);
            }
            else{
                p->drawLine(addPageR.x(), addPageR.y(), addPageR.x(),
                            addPageR.bottom());
                p->drawLine(addPageR.right(), addPageR.y(), addPageR.right(),
                            addPageR.bottom());
                p->setPen(activeControl==AddPage ? g.button() : g.midlight());
                p->drawLine(addPageR.x()+1, addPageR.y(), addPageR.x()+1,
                            addPageR.bottom());
            }
        }
    }
    if(controls & SubPage){
        if(subPageR.height()){
            p->fillRect(subPageR, activeControl == SubPage ?
                        g.brush(QColorGroup::Button) :
                        g.brush(QColorGroup::Midlight));
            p->setPen(g.mid());
            if(horiz){
                p->drawLine(subPageR.x(), subPageR.y(), subPageR.right(),
                            subPageR.y());
                p->drawLine(subPageR.x(), subPageR.bottom(), subPageR.right(),
                            subPageR.bottom());
                p->setPen(activeControl==SubPage ? g.button() : g.midlight());
                p->drawLine(subPageR.x(), subPageR.y()+1, subPageR.right(),
                            subPageR.y()+1);
                p->drawLine(subPageR.x(), subPageR.y()+1, subPageR.x(),
                            subPageR.bottom()-1);
            }
            else{
                p->drawLine(subPageR.x(), subPageR.y(), subPageR.x(),
                            subPageR.bottom());
                p->drawLine(subPageR.right(), subPageR.y(), subPageR.right(),
                            subPageR.bottom());
                p->setPen(activeControl==SubPage ? g.button() : g.midlight());
                p->drawLine(subPageR.x()+1, subPageR.y(), subPageR.x()+1,
                            subPageR.bottom());
                p->drawLine(subPageR.x()+1, subPageR.y(), subPageR.right()-1,
                            subPageR.y());
            }
        }
    }
    if ( controls & Slider ) {
        drawSBButton(p, sliderR, g, activeControl == Slider);
        drawSBDeco(p, sliderR, g, horiz);
    }
}

void B3Style::drawSBButton(QPainter *p, const QRect &r, const QColorGroup &g,
                           bool down)
{
    int x = r.x();
    int y = r.y();
    int w = r.width();
    int h = r.height();
    int x2 = x+w-1;
    int y2 = y+h-1;
    p->setPen(g.dark());
    p->drawRect(x, y, w, h);
    p->fillRect(x+1, y+1, w-2, h-2, down ? g.brush(QColorGroup::Midlight) :
                                           g.brush(QColorGroup::Button));

    p->setPen(down? g.light() : g.mid());
    p->drawLine(x2-1, y+2, x2-1, y2-1);
    p->drawLine(x+2, y2-1, x2-1, y2-1);

    p->setPen(down ? g.mid() : g.light());
    p->drawLine(x+1, y+1, x2-1, y+1);
    p->drawLine(x+1, y+2, x+1, y2-1);
}

void B3Style::drawSBDeco(QPainter *p, const QRect &r, const QColorGroup &g,
                         bool horiz)
{
    if(horiz){
        int y = r.y() + (r.height()-7)/2;
        if(r.width() >= 36){
            int x = r.x() + (r.width()-16)/2;
            drawSBDecoButton(p, x,    y, 4, 7, g);
            drawSBDecoButton(p, x+6,  y, 4, 7, g);
            drawSBDecoButton(p, x+12, y, 4, 7, g);
        }
        else if(r.width() >= 24 ){
            int x = r.x() + (r.width()-10)/2;
            drawSBDecoButton(p, x,   y, 4, 7, g);
            drawSBDecoButton(p, x+6, y, 4, 7, g);
        }
        else if(r.width() >= 16)
            drawSBDecoButton(p, r.x()+(r.width()-4)/2, y, 4, 7, g);
    }
    else{
        int x = r.x() + (r.width()-7)/2;
        if(r.height() >= 36 ){
            int y = r.y() + (r.height()-16)/2;
            drawSBDecoButton(p, x, y,    7, 4, g);
            drawSBDecoButton(p, x, y+6,  7, 4, g);
            drawSBDecoButton(p, x, y+12, 7, 4, g);
        }
        else if(r.height() >= 24 ){
            int y = r.y() + (r.height()-10)/2;
            drawSBDecoButton(p, x, y,   7, 4, g);
            drawSBDecoButton(p, x, y+6, 7, 4, g);
        }
        else if(r.height() >= 16)
            drawSBDecoButton(p, x, r.y()+(r.height()-4)/2, 7, 4, g);
    }
}

void B3Style::drawSBDecoButton(QPainter *p, int x, int y, int w, int h,
                               const QColorGroup &g)
{
    int x2 = w+x;
    int y2 = h+y;

    p->setPen(g.mid());
    p->drawLine(x+1, y,   x2-1, y);
    p->drawLine(x,   y+1, x,    y2-1);

    p->setPen(g.light());
    p->drawLine(x+1, y+1, x2-1, y+1);
    p->drawLine(x+1, y+1, x+1,  y2-1);

    p->setPen(g.dark());
    p->drawLine(x+1, y2,  x2-1, y2);
    p->drawLine(x2,  y+1, x2,   y2-1);
}

void B3Style::drawSBArrow(QPainter *p, Qt::ArrowType type, bool down, int x,
                          int y, int w, int h, const QColorGroup &g,
                          bool enabled, const QBrush *fill)
{
    int x2 = x+w;
    int y2 = y+h;

    switch (type)
    {
    case UpArrow:
      p->setPen(enabled ? g.dark() : g.mid());
      p->drawLine(x+3,  y,  x+3, y+1);

      p->drawLine(x+2,  y+2,  x+2, y+3);
      p->drawLine(x2-2, y+2, x2-2, y+3);

      p->drawLine(x+1,  y+4, x+1,  y+6);
      p->drawLine(x2-1, y+4, x2-1, y+6);

      p->drawLine(x,  y+7, x,  y2);
      p->drawLine(x2, y+7, x2, y2);

      p->drawLine(x, y2, x2, y2);

      p->setPen(enabled ? g.light() : g.mid());
      p->drawLine(x+3, y+2, x+3, y+3);
      p->drawLine(x+2, y+4, x+2, y+6);
      p->drawLine(x+1, y+7, x+1, y2-1);

      p->setPen(g.mid());
      p->drawLine(x2-2, y+4, x2-2, y+6);
      p->drawLine(x2-1, y+7, x2-1, y2-1);
      p->drawLine(x+2, y2-1, x2-1, y2-1);
      break;
    case DownArrow:
    default:
      p->setPen(enabled ? g.dark() : g.mid());
      p->drawLine(x+3,  y2,  x+3, y2-1);

      p->drawLine(x+2,  y2-2,  x+2, y2-3);
      p->drawLine(x2-2, y2-2, x2-2, y2-3);

      p->drawLine(x+1,  y2-4, x+1,  y2-6);
      p->drawLine(x2-1, y2-4, x2-1, y2-6);

      p->drawLine(x,  y2-7, x,  y);
      p->drawLine(x2, y2-7, x2, y);

      p->drawLine(x, y, x2, y);

      p->setPen(enabled ? g.light() : g.mid());
      p->drawLine(x+3, y2-2, x+3, y2-3);
      p->drawLine(x+2, y2-4, x+2, y2-6);
      p->drawLine(x+1, y2-7, x+1, y+1);

      p->setPen(g.mid());
      p->drawLine(x2-2, y2-4, x2-2, y2-6);
      p->drawLine(x2-1, y2-7, x2-1, y+1);
      p->drawLine(x+2, y+1, x2-1, y+1);
      break;
    case LeftArrow:
      p->setPen(enabled ? g.dark() : g.mid());
      p->drawLine(x,  y+3,  x+1, y+3);

      p->drawLine(x+2, y+2, x+3, y+2);
      p->drawLine(x+2, y+4, x+3, y+4);

      p->drawLine(x+4, y+1, x+6, y+1);
      p->drawLine(x+4, y+5, x+6, y+5);

      p->drawLine(x+7, y,   x2, y);
      p->drawLine(x+7, y+6, x2, y+6);

      p->drawLine(x2, y, x2, y+6);

      p->setPen(enabled ? g.light() : g.mid());
      p->drawLine(x+2, y+3,  x+3, y+3);
      p->drawLine(x+4, y+2,  x+6, y+2);
      p->drawLine(x+7, y+1, x2-1, y+1);

      p->setPen(g.mid());
      p->drawLine(x+4,  y+4, x+6,  y+4);
      p->drawLine(x+7,  y+5, x2-1, y+5);
      p->drawLine(x2-1, y+2, x2-1, y+5);
      break;
    case RightArrow:
      p->setPen(enabled ? g.dark() : g.mid());
      p->drawLine(x2,  y+3,  x2-1, y+3);

      p->drawLine(x2-2, y+2, x2-3, y+2);
      p->drawLine(x2-2, y+4, x2-3, y+4);

      p->drawLine(x2-4, y+1, x2-6, y+1);
      p->drawLine(x2-4, y+5, x2-6, y+5);

      p->drawLine(x2-7, y,   x, y);
      p->drawLine(x2-7, y+6, x, y+6);

      p->drawLine(x, y, x, y+6);

      p->setPen(enabled ? g.light() : g.mid());
      p->drawLine(x2-7, y+1, x+1, y+1);
      p->drawLine(x+1,  y+1, x+1, y+5);

      p->setPen(g.mid());
      p->drawLine(x2-2, y+3, x2-3, y+3);
      p->drawLine(x2-4, y+2, x2-6, y+2);
      p->drawLine(x2-4, y+4, x2-6, y+4);
      p->drawLine(x2-7, y+5, x+2,  y+5);
      break;

    }
}

void B3Style::drawArrow(QPainter *p, Qt::ArrowType type, bool down, int x,
                            int y, int w, int h, const QColorGroup &g,
                            bool enabled, const QBrush *fill)
{
    qDrawArrow(p, type, Qt::WindowsStyle, down, x, y, w, h, g, enabled);
}

void B3Style::drawKBarHandle(QPainter *p, int x, int y, int w, int h,
                                const QColorGroup &g, bool, QBrush *)
{
    p->setPen(g.mid());
    p->drawLine(w, y, w, h-1);

    p->setPen(g.light());
    p->drawLine(w+1, y, w+1, h-2);

    if(h > w)
        qDrawArrow(p, RightArrow, WindowsStyle, false, x+3, y+4, w-4, w-4, g,
                   true);
    else
        qDrawArrow(p, DownArrow, WindowsStyle, false, x+4, y+1, h-4, h-4, g,
                   true);
}

void B3Style::drawKMenuBar(QPainter *p, int x, int y, int w, int h,
                           const QColorGroup &g, QBrush *)
{
    int x2 = x+w-1;
    int y2 = y+h;

    p->fillRect(x, y, w, h, g.brush(QColorGroup::Button));

    p->setPen(g.dark());
    p->drawRect(x, y, w, h);

    p->setPen(g.mid());
    p->drawRect(x+1, y+1, w-2, h-1);

    p->setPen(g.light());
    p->drawLine(x+2, y+2,  x+2, y2-2);
    p->drawLine(x+2, y+2, x2-2, y+2);

    // and a little touch for roundedness
    p->setPen(g.button());
    p->drawPoint(x, y);
    p->drawPoint(x2, y);
}

void B3Style::drawKToolBar(QPainter *p, int x, int y, int w, int h,
                           const QColorGroup &g, bool)
{
    int x2 = x+w-1;
    int y2 = y+h;

    p->fillRect(x, y, w, h, g.brush(QColorGroup::Button));

    p->setPen(g.dark());
    p->drawLine(x, y, x, y2);
    p->drawLine(x2, y, x2, y2);

    p->setPen(g.light());
    p->drawLine(x+1,  y,  x2-1, y);
    p->drawLine(x+2, y+1, x+2,  y2-1);

    p->setPen(g.mid());
    p->drawLine(x+1,  y, x+1,  y2);
    p->drawLine(x2-1, y, x2-1, y2);
    p->drawLine(x+1, y2-1, x2-1, y2-1);
}

void B3Style::drawKToolBarButton(QPainter *p, int x, int y, int w, int h,
                                    const QColorGroup &g, bool sunken,
                                    bool raised, bool enabled, bool popup,
                                    KToolButtonType icontext,
                                    const QString& btext, const QPixmap *pixmap,
                                    QFont *font, QWidget *)
{
    int dx, dy;

    QFontMetrics fm(*font);

    if(raised)
    {
        int x2 = x+w;
        int y2 = y+h;

        p->fillRect(x, y, w, h, g.brush(QColorGroup::Midlight));

        p->setPen(g.dark());
        p->drawLine(x+1, y+1, x2-2, y+1);
        p->drawLine(x, y+2, x, y2-3);
        p->drawLine(x2-1, y+2, x2-1, y2-3);
        p->drawLine(x+1, y2-2, x2-2, y2-2);

        p->setPen(sunken ? g.mid() : g.light());
        p->drawLine(x+1, y+2, x2-2, y+2);
        p->drawLine(x+1, y+2, x+1, y2-3);
        p->setPen(sunken ? g.light() : g.mid());
        p->drawLine(x2-2, y+3, x2-2, y2-3);
        p->drawLine(x+2, y2-3, x2-2, y2-3);
    }
    else
    {
        if(sunken)
            qDrawShadePanel(p, x, y, w, h, g, true, 1,
                            &g.brush(QColorGroup::Midlight));
        else
            p->fillRect(x, y, w, h, g.brush(QColorGroup::Button));
    }
    p->setPen(g.text());

    if (icontext == Icon){ // icon only
        if (pixmap){
            dx = ( w - pixmap->width() ) / 2;
            dy = ( h - pixmap->height() ) / 2;
            if ( sunken )
            {
                ++dx;
                ++dy;
            }
            p->drawPixmap( x+dx, y+dy, *pixmap );
        }
    }
    else if (icontext == IconTextRight){ // icon and text (if any)
        if (pixmap){
            dx = 4;
            dy = ( h - pixmap->height() ) / 2;
            if ( sunken ){
                ++dx;
                ++dy;
            }
            p->drawPixmap( x+dx, y+dy, *pixmap );
        }
        if (!btext.isNull()){
            int tf = AlignVCenter|AlignLeft;
            if (pixmap)
                dx= 4 + pixmap->width() + 2;
            else
                dx= 4;
            dy = 0;
            if ( sunken ){
                ++dx;
                ++dy;
            }
            if (font)
                p->setFont(*font);
            if(raised)
                p->setPen(KGlobalSettings::toolBarHighlightColor());
            p->drawText(x+dx, y+dy, w-dx, h, tf, btext);
        }
    }
    else if (icontext == Text){ // only text, even if there is a icon
        if (!btext.isNull()){
            int tf = AlignVCenter|AlignLeft;
            if (!enabled)
                p->setPen(g.dark());
            dx= (w - fm.width(btext)) / 2;
            dy= (h - fm.lineSpacing()) / 2;
            if ( sunken ){
                ++dx;
                ++dy;
            }
            if (font)
                p->setFont(*font);
            if(raised)
                p->setPen(KGlobalSettings::toolBarHighlightColor());
            p->drawText(x+dx, y+dy, fm.width(btext), fm.lineSpacing(), tf, btext);
        }
    }
    else if (icontext == IconTextBottom){
        if (pixmap){
            dx = (w - pixmap->width()) / 2;
            dy = (h - fm.lineSpacing() - pixmap->height()) / 2;
            if ( sunken ){
                ++dx;
                ++dy;
            }
            p->drawPixmap( x+dx, y+dy, *pixmap );
        }
        if (!btext.isNull()){
            int tf = AlignBottom|AlignHCenter;
            dy= pixmap->height();
            dx = 2;
            if ( sunken ){
                ++dx;
                ++dy;
            }
            if (font)
                p->setFont(*font);
            if(raised)
                p->setPen(KGlobalSettings::toolBarHighlightColor());
            p->drawText(x, y, w, h-3, tf, btext);
        }
    }
    if (popup){
        if (enabled)
            qDrawArrow (p, Qt::DownArrow, Qt::WindowsStyle, false, w-5, h-5, 0, 0,
                        g, true);
        else
            qDrawArrow (p, Qt::DownArrow, Qt::WindowsStyle, false, w-5, h-5,
                        0, 0, g, false);
    }
}

void B3Style::drawKMenuItem(QPainter *p, int x, int y, int w, int h,
                            const QColorGroup &g, bool active, QMenuItem *mi,
                            QBrush *)
{
    if(active){
        qDrawShadePanel(p, x, y+2, w, h, g, true, 1,
                        &g.brush(QColorGroup::Midlight));
        QApplication::style().drawItem(p, x, y+2, w, h,
                                       AlignCenter|ShowPrefix|DontClip|SingleLine,
                                       g, mi->isEnabled(), mi->pixmap(), mi->text(),
                                       -1, &g.text());
    }
    else
        QApplication::style().drawItem(p, x, y+2, w, h,
                                       AlignCenter|ShowPrefix|DontClip|SingleLine,
                                       g, mi->isEnabled(), mi->pixmap(), mi->text(),
                                       -1, &g.text() );
}
