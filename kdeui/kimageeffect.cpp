/* This file is part of the KDE libraries
    Copyright (C) 1998, 1999 Christian Tibirna <ctibirna@total.net>
              (C) 1998, 1999 Daniel M. Duley <mosfet@kde.org>
              (C) 1998, 1999 Dirk A. Mueller <mueller@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

// $Id$

#include <math.h>

#include <dither.h>
#include <qimage.h>
#include <stdlib.h>

#include "kimageeffect.h"

//======================================================================
//
// Gradient effects
//
//======================================================================

QImage KImageEffect::gradient(const QSize &size, const QColor &ca,
	const QColor &cb, GradientType eff, int ncols)
{
    int rDiff, gDiff, bDiff;
    int rca, gca, bca, rcb, gcb, bcb;

    QImage image(size, 32);

    register int x, y;

    rDiff = (rcb = cb.red())   - (rca = ca.red());
    gDiff = (gcb = cb.green()) - (gca = ca.green());
    bDiff = (bcb = cb.blue())  - (bca = ca.blue());

    if( eff == VerticalGradient || eff == HorizontalGradient ){

        uint *p;
        uint rgb;

        register int rl = rca << 16;
        register int gl = gca << 16;
        register int bl = bca << 16;

        if( eff == VerticalGradient ) {

            int rcdelta = ((1<<16) / size.height()) * rDiff;
            int gcdelta = ((1<<16) / size.height()) * gDiff;
            int bcdelta = ((1<<16) / size.height()) * bDiff;

            for ( y = 0; y < size.height(); y++ ) {
                p = (uint *) image.scanLine(y);

                rl += rcdelta;
                gl += gcdelta;
                bl += bcdelta;

                rgb = qRgb( (rl>>16), (gl>>16), (bl>>16) );

                for( x = 0; x < size.width(); x++ ) {
                    *p = rgb;
                    p++;
                }
            }

        }
        else {                  // must be HorizontalGradient

            unsigned int *o_src = (unsigned int *)image.scanLine(0);
            unsigned int *src = o_src;

            int rcdelta = ((1<<16) / size.width()) * rDiff;
            int gcdelta = ((1<<16) / size.width()) * gDiff;
            int bcdelta = ((1<<16) / size.width()) * bDiff;

            for( x = 0; x < size.width(); x++) {

                rl += rcdelta;
                gl += gcdelta;
                bl += bcdelta;

                *src++ = qRgb( (rl>>16), (gl>>16), (bl>>16));
            }

            src = o_src;

            // Believe it or not, manually copying in a for loop is faster
            // than calling memcpy for each scanline (on the order of ms...).
            // I think this is due to the function call overhead (mosfet).

            for (y = 1; y < size.height(); ++y) {

                p = (unsigned int *)image.scanLine(y);
                src = o_src;
                for(x=0; x < size.width(); ++x)
                    *p++ = *src++;
            }
        }
    }

    else {

        float rfd, gfd, bfd;
        float rd = rca, gd = gca, bd = bca;

        unsigned char *xtable[3];
        unsigned char *ytable[3];

        unsigned int w = size.width(), h = size.height();
        xtable[0] = new unsigned char[w];
        xtable[1] = new unsigned char[w];
        xtable[2] = new unsigned char[w];
        ytable[0] = new unsigned char[h];
        ytable[1] = new unsigned char[h];
        ytable[2] = new unsigned char[h];
        w*=2, h*=2;

        if ( eff == DiagonalGradient || eff == CrossDiagonalGradient) {
            // Diagonal dgradient code inspired by BlackBox (mosfet)
            // BlackBox dgradient is (C) Brad Hughes, <bhughes@tcac.net> and
            // Mike Cole <mike@mydot.com>.

            rfd = (float)rDiff/w;
            gfd = (float)gDiff/w;
            bfd = (float)bDiff/w;

            int dir;
            for (x = 0; x < size.width(); x++, rd+=rfd, gd+=gfd, bd+=bfd) {
                dir = eff == DiagonalGradient? x : size.width() - x - 1;
                xtable[0][dir] = (unsigned char) rd;
                xtable[1][dir] = (unsigned char) gd;
                xtable[2][dir] = (unsigned char) bd;
            }
            rfd = (float)rDiff/h;
            gfd = (float)gDiff/h;
            bfd = (float)bDiff/h;
            rd = gd = bd = 0;
            for (y = 0; y < size.height(); y++, rd+=rfd, gd+=gfd, bd+=bfd) {
                ytable[0][y] = (unsigned char) rd;
                ytable[1][y] = (unsigned char) gd;
                ytable[2][y] = (unsigned char) bd;
            }

            for (y = 0; y < size.height(); y++) {
                unsigned int *scanline = (unsigned int *)image.scanLine(y);
                for (x = 0; x < size.width(); x++) {
                    scanline[x] = qRgb(xtable[0][x] + ytable[0][y],
                                       xtable[1][x] + ytable[1][y],
                                       xtable[2][x] + ytable[2][y]);
                }
            }
        }

        else if (eff == RectangleGradient ||
                 eff == PyramidGradient ||
                 eff == PipeCrossGradient ||
                 eff == EllipticGradient)
        {
            int rSign = rDiff>0? 1: -1;
            int gSign = gDiff>0? 1: -1;
            int bSign = bDiff>0? 1: -1;

            rfd = (float)rDiff / size.width();
            gfd = (float)gDiff / size.width();
            bfd = (float)bDiff / size.width();

            rd = (float)rDiff/2;
            gd = (float)gDiff/2;
            bd = (float)bDiff/2;

            for (x = 0; x < size.width(); x++, rd-=rfd, gd-=gfd, bd-=bfd)
            {
                xtable[0][x] = (unsigned char) abs((int)rd);
                xtable[1][x] = (unsigned char) abs((int)gd);
                xtable[2][x] = (unsigned char) abs((int)bd);
            }

            rfd = (float)rDiff/size.height();
            gfd = (float)gDiff/size.height();
            bfd = (float)bDiff/size.height();

            rd = (float)rDiff/2;
            gd = (float)gDiff/2;
            bd = (float)bDiff/2;

            for (y = 0; y < size.height(); y++, rd-=rfd, gd-=gfd, bd-=bfd)
            {
                ytable[0][y] = (unsigned char) abs((int)rd);
                ytable[1][y] = (unsigned char) abs((int)gd);
                ytable[2][y] = (unsigned char) abs((int)bd);
            }
            unsigned int rgb;
            int h = (size.height()+1)>>1;
            for (y = 0; y < h; y++) {
                unsigned int *sl1 = (unsigned int *)image.scanLine(y);
                unsigned int *sl2 = (unsigned int *)image.scanLine(QMAX(size.height()-y-1, y));

                int w = (size.width()+1)>>1;
                int x2 = size.width()-1;

                for (x = 0; x < w; x++, x2--) {
		    rgb = 0;
                    if (eff == PyramidGradient) {
                        rgb = qRgb(rcb-rSign*(xtable[0][x]+ytable[0][y]),
                                   gcb-gSign*(xtable[1][x]+ytable[1][y]),
                                   bcb-bSign*(xtable[2][x]+ytable[2][y]));
                    }
                    if (eff == RectangleGradient) {
                        rgb = qRgb(rcb - rSign *
                                   QMAX(xtable[0][x], ytable[0][y]) * 2,
                                   gcb - gSign *
                                   QMAX(xtable[1][x], ytable[1][y]) * 2,
                                   bcb - bSign *
                                   QMAX(xtable[2][x], ytable[2][y]) * 2);
                    }
                    if (eff == PipeCrossGradient) {
                        rgb = qRgb(rcb - rSign *
                                   QMIN(xtable[0][x], ytable[0][y]) * 2,
                                   gcb - gSign *
                                   QMIN(xtable[1][x], ytable[1][y]) * 2,
                                   bcb - bSign *
                                   QMIN(xtable[2][x], ytable[2][y]) * 2);
                    }
                    if (eff == EllipticGradient) {
                        rgb = qRgb(rcb - rSign *
                                   (int)sqrt((xtable[0][x]*xtable[0][x] +
                                              ytable[0][y]*ytable[0][y])*2),
                                   gcb - gSign *
                                   (int)sqrt((xtable[1][x]*xtable[1][x] +
                                              ytable[1][y]*ytable[1][y])*2),
                                   bcb - bSign *
                                   (int)sqrt((xtable[2][x]*xtable[2][x] +
                                              ytable[2][y]*ytable[2][y])*2));
                    }

                    sl1[x] = sl2[x] = rgb;
                    sl1[x2] = sl2[x2] = rgb;
                }
            }
        }

        delete [] xtable[0];
        delete [] xtable[1];
        delete [] xtable[2];
        delete [] ytable[0];
        delete [] ytable[1];
        delete [] ytable[2];
    }

    // dither if necessary
    if (ncols && (QPixmap::defaultDepth() < 15 )) {
	if ( ncols < 2 || ncols > 256 )
	    ncols = 3;
	QColor *dPal = new QColor[ncols];
	for (int i=0; i<ncols; i++) {
	    dPal[i].setRgb ( rca + rDiff * i / ( ncols - 1 ),
			     gca + gDiff * i / ( ncols - 1 ),
			     bca + bDiff * i / ( ncols - 1 ) );
	}
	kFSDither dither(dPal, ncols);
	image = dither.dither(image);
	delete [] dPal;
    }

    return image;
}


// -----------------------------------------------------------------------------

//CT this was (before Dirk A. Mueller's speedup changes)
//   merely the same code as in the above method, but it's supposedly
//   way less performant since it introduces a lot of supplementary tests
//   and simple math operations for the calculus of the balance.
//      (surprizingly, it isn't less performant, in the contrary :-)
//   Yes, I could have merged them, but then the excellent performance of
//   the balanced code would suffer with no other gain than a mere
//   source code and byte code size economy.

QImage KImageEffect::unbalancedGradient(const QSize &size, const QColor &ca,
	const QColor &cb, GradientType eff, int xfactor, int yfactor,
	int ncols)
{
    int dir; // general parameter used for direction switches

    bool _xanti = false , _yanti = false;

    if (xfactor < 0) _xanti = true; // negative on X direction
    if (yfactor < 0) _yanti = true; // negative on Y direction

    xfactor = abs(xfactor);
    yfactor = abs(yfactor);

    if (!xfactor) xfactor = 1;
    if (!yfactor) yfactor = 1;

    if (xfactor > 200 ) xfactor = 200;
    if (yfactor > 200 ) yfactor = 200;


    float xbal = xfactor/5000.;
    float ybal = yfactor/5000.;
    float rat;

    int rDiff, gDiff, bDiff;
    int rca, gca, bca, rcb, gcb, bcb;

    QImage image(size, 32);

    register int x, y;
    unsigned int *scanline;

    rDiff = (rcb = cb.red())   - (rca = ca.red());
    gDiff = (gcb = cb.green()) - (gca = ca.green());
    bDiff = (bcb = cb.blue())  - (bca = ca.blue());

    if( eff == VerticalGradient || eff == HorizontalGradient){
        QColor cRow;

        uint *p;
        uint rgbRow;

	if( eff == VerticalGradient) {
	  for ( y = 0; y < size.height(); y++ ) {
	    dir = _yanti ? y : size.height() - 1 - y;
            p = (uint *) image.scanLine(dir);
            rat =  1 - exp( - (float)y  * ybal );
	
            cRow.setRgb( rcb - (int) ( rDiff * rat ),
                         gcb - (int) ( gDiff * rat ),
                         bcb - (int) ( bDiff * rat ) );
	
            rgbRow = cRow.rgb();
	
            for( x = 0; x < size.width(); x++ ) {
	      *p = rgbRow;
	      p++;
            }
	  }
	}
	else {

	  unsigned int *src = (unsigned int *)image.scanLine(0);
	  for(x = 0; x < size.width(); x++ )
	    {
	      dir = _xanti ? x : size.width() - 1 - x;
	      rat = 1 - exp( - (float)x  * xbal );
	
	      src[dir] = qRgb(rcb - (int) ( rDiff * rat ),
			    gcb - (int) ( gDiff * rat ),
			    bcb - (int) ( bDiff * rat ));
	    }
	
	  // Believe it or not, manually copying in a for loop is faster
	  // than calling memcpy for each scanline (on the order of ms...).
	  // I think this is due to the function call overhead (mosfet).
	
	  for(y = 1; y < size.height(); ++y)
	    {
	      scanline = (unsigned int *)image.scanLine(y);
	      for(x=0; x < size.width(); ++x)
		scanline[x] = src[x];
	    }
	}
    }

    else {
      int w=size.width(), h=size.height();

      unsigned char *xtable[3];
      unsigned char *ytable[3];
      xtable[0] = new unsigned char[w];
      xtable[1] = new unsigned char[w];
      xtable[2] = new unsigned char[w];
      ytable[0] = new unsigned char[h];
      ytable[1] = new unsigned char[h];
      ytable[2] = new unsigned char[h];

      if ( eff == DiagonalGradient || eff == CrossDiagonalGradient)
	{
	  for (x = 0; x < w; x++) {
              dir = _xanti ? x : w - 1 - x;
              rat = 1 - exp( - (float)x * xbal );

              xtable[0][dir] = (unsigned char) ( rDiff/2 * rat );
              xtable[1][dir] = (unsigned char) ( gDiff/2 * rat );
              xtable[2][dir] = (unsigned char) ( bDiff/2 * rat );
          }
	
	  for (y = 0; y < h; y++) {
              dir = _yanti ? y : h - 1 - y;
              rat =  1 - exp( - (float)y  * ybal );

              ytable[0][dir] = (unsigned char) ( rDiff/2 * rat );
              ytable[1][dir] = (unsigned char) ( gDiff/2 * rat );
              ytable[2][dir] = (unsigned char) ( bDiff/2 * rat );
          }
	
	  for (y = 0; y < h; y++) {
              unsigned int *scanline = (unsigned int *)image.scanLine(y);
              for (x = 0; x < w; x++) {
                  scanline[x] = qRgb(rcb - (xtable[0][x] + ytable[0][y]),
                                     gcb - (xtable[1][x] + ytable[1][y]),
                                     bcb - (xtable[2][x] + ytable[2][y]));
              }
          }
        }

      else if (eff == RectangleGradient ||
               eff == PyramidGradient ||
               eff == PipeCrossGradient ||
               eff == EllipticGradient)
      {
          int rSign = rDiff>0? 1: -1;
          int gSign = gDiff>0? 1: -1;
          int bSign = bDiff>0? 1: -1;

          for (x = 0; x < w; x++)
	    {
                dir = _xanti ? x : w - 1 - x;
                rat =  1 - exp( - (float)x * xbal );

                xtable[0][dir] = (unsigned char) abs((int)(rDiff*(0.5-rat)));
                xtable[1][dir] = (unsigned char) abs((int)(gDiff*(0.5-rat)));
                xtable[2][dir] = (unsigned char) abs((int)(bDiff*(0.5-rat)));
            }

          for (y = 0; y < h; y++)
          {
              dir = _yanti ? y : h - 1 - y;

              rat =  1 - exp( - (float)y * ybal );

              ytable[0][dir] = (unsigned char) abs((int)(rDiff*(0.5-rat)));
              ytable[1][dir] = (unsigned char) abs((int)(gDiff*(0.5-rat)));
              ytable[2][dir] = (unsigned char) abs((int)(bDiff*(0.5-rat)));
          }

          for (y = 0; y < h; y++) {
              unsigned int *scanline = (unsigned int *)image.scanLine(y);
              for (x = 0; x < w; x++) {
                  if (eff == PyramidGradient)
                  {
                      scanline[x] = qRgb(rcb-rSign*(xtable[0][x]+ytable[0][y]),
                                         gcb-gSign*(xtable[1][x]+ytable[1][y]),
                                         bcb-bSign*(xtable[2][x]+ytable[2][y]));
                  }
                  if (eff == RectangleGradient)
                  {
                      scanline[x] = qRgb(rcb - rSign *
                                         QMAX(xtable[0][x], ytable[0][y]) * 2,
                                         gcb - gSign *
                                         QMAX(xtable[1][x], ytable[1][y]) * 2,
                                         bcb - bSign *
                                         QMAX(xtable[2][x], ytable[2][y]) * 2);
                  }
                  if (eff == PipeCrossGradient)
                  {
                      scanline[x] = qRgb(rcb - rSign *
                                         QMIN(xtable[0][x], ytable[0][y]) * 2,
                                         gcb - gSign *
                                         QMIN(xtable[1][x], ytable[1][y]) * 2,
                                         bcb - bSign *
                                         QMIN(xtable[2][x], ytable[2][y]) * 2);
                  }
                  if (eff == EllipticGradient)
                  {
                      scanline[x] = qRgb(rcb - rSign *
                                         (int)sqrt((xtable[0][x]*xtable[0][x] +
                                                    ytable[0][y]*ytable[0][y])*2),
                                         gcb - gSign *
                                         (int)sqrt((xtable[1][x]*xtable[1][x] +
                                                    ytable[1][y]*ytable[1][y])*2),
                                         bcb - bSign *
                                         (int)sqrt((xtable[2][x]*xtable[2][x] +
                                                    ytable[2][y]*ytable[2][y])*2));
                  }
              }
          }
      }

      if (ncols && (QPixmap::defaultDepth() < 15 )) {
          if ( ncols < 2 || ncols > 256 )
              ncols = 3;
          QColor *dPal = new QColor[ncols];
          for (int i=0; i<ncols; i++) {
              dPal[i].setRgb ( rca + rDiff * i / ( ncols - 1 ),
                               gca + gDiff * i / ( ncols - 1 ),
                               bca + bDiff * i / ( ncols - 1 ) );
          }
          kFSDither dither(dPal, ncols);
          image = dither.dither(image);
          delete [] dPal;
      }

      delete [] xtable[0];
      delete [] xtable[1];
      delete [] xtable[2];
      delete [] ytable[0];
      delete [] ytable[1];
      delete [] ytable[2];

    }

    return image;
}


//======================================================================
//
// Intensity effects
//
//======================================================================


/* This builds a 256 byte unsigned char lookup table with all
 * the possible percent values prior to applying the effect, then uses
 * integer math for the pixels. For any image larger than 9x9 this will be
 * less expensive than doing a float operation on the 3 color components of
 * each pixel. (mosfet)
 */

QImage& KImageEffect::intensity(QImage &image, float percent)
{
    int i, tmp, r, g, b;
    int segColors = image.depth() > 8 ? 256 : image.numColors();
    unsigned char *segTbl = new unsigned char[segColors];
    int pixels = image.depth() > 8 ? image.width()*image.height() :
        image.numColors();
    unsigned int *data = image.depth() > 8 ? (unsigned int *)image.bits() :
        (unsigned int *)image.colorTable();

    bool brighten = (percent >= 0);
    if(percent < 0)
        percent = -percent;

    if(brighten){ // keep overflow check out of loops
        for(i=0; i < segColors; ++i){
            tmp = (int)(i*percent);
            if(tmp > 255)
                tmp = 255;
            segTbl[i] = tmp;
        }
    }
    else{
        for(i=0; i < segColors; ++i){
            tmp = (int)(i*percent);
            if(tmp < 0)
                tmp = 0;
            segTbl[i] = tmp;
        }
    }

    if(brighten){ // same here
        for(i=0; i < pixels; ++i){
            r = qRed(data[i]);
            g = qGreen(data[i]);
            b = qBlue(data[i]);
            r = r + segTbl[r] > 255 ? 255 : r + segTbl[r];
            g = g + segTbl[g] > 255 ? 255 : g + segTbl[g];
            b = b + segTbl[b] > 255 ? 255 : b + segTbl[b];
            data[i] = qRgb(r, g, b);
        }
    }
    else{
        for(i=0; i < pixels; ++i){
            r = qRed(data[i]);
            g = qGreen(data[i]);
            b = qBlue(data[i]);
            r = r - segTbl[r] < 0 ? 0 : r - segTbl[r];
            g = g - segTbl[g] < 0 ? 0 : g - segTbl[g];
            b = b - segTbl[b] < 0 ? 0 : b - segTbl[b];
            data[i] = qRgb(r, g, b);
        }
    }
    delete [] segTbl;

    return image;
}

QImage& KImageEffect::channelIntensity(QImage &image, float percent,
                                       RGBComponent channel)
{
    int i, tmp, c;
    int segColors = image.depth() > 8 ? 256 : image.numColors();
    unsigned char *segTbl = new unsigned char[segColors];
    int pixels = image.depth() > 8 ? image.width()*image.height() :
        image.numColors();
    unsigned int *data = image.depth() > 8 ? (unsigned int *)image.bits() :
        (unsigned int *)image.colorTable();
    bool brighten = (percent >= 0);
    if(percent < 0)
        percent = -percent;

    if(brighten){ // keep overflow check out of loops
        for(i=0; i < segColors; ++i){
            tmp = (int)(i*percent);
            if(tmp > 255)
                tmp = 255;
            segTbl[i] = tmp;
        }
    }
    else{
        for(i=0; i < segColors; ++i){
            tmp = (int)(i*percent);
            if(tmp < 0)
                tmp = 0;
            segTbl[i] = tmp;
        }
    }

    if(brighten){ // same here
        if(channel == Red){ // and here ;-)
            for(i=0; i < pixels; ++i){
                c = qRed(data[i]);
                c = c + segTbl[c] > 255 ? 255 : c + segTbl[c];
                data[i] = qRgb(c, qGreen(data[i]), qBlue(data[i]));
            }
        }
        if(channel == Green){
            for(i=0; i < pixels; ++i){
                c = qGreen(data[i]);
                c = c + segTbl[c] > 255 ? 255 : c + segTbl[c];
                data[i] = qRgb(qRed(data[i]), c, qBlue(data[i]));
            }
        }
        else{
            for(i=0; i < pixels; ++i){
                c = qBlue(data[i]);
                c = c + segTbl[c] > 255 ? 255 : c + segTbl[c];
                data[i] = qRgb(qRed(data[i]), qGreen(data[i]), c);
            }
        }

    }
    else{
        if(channel == Red){
            for(i=0; i < pixels; ++i){
                c = qRed(data[i]);
                c = c - segTbl[c] < 0 ? 0 : c - segTbl[c];
                data[i] = qRgb(c, qGreen(data[i]), qBlue(data[i]));
            }
        }
        if(channel == Green){
            for(i=0; i < pixels; ++i){
                c = qGreen(data[i]);
                c = c - segTbl[c] < 0 ? 0 : c - segTbl[c];
                data[i] = qRgb(qRed(data[i]), c, qBlue(data[i]));
            }
        }
        else{
            for(i=0; i < pixels; ++i){
                c = qBlue(data[i]);
                c = c - segTbl[c] < 0 ? 0 : c - segTbl[c];
                data[i] = qRgb(qRed(data[i]), qGreen(data[i]), c);
            }
        }
    }
    delete [] segTbl;

    return image;
}


//======================================================================
//
// Blend effects
//
//======================================================================

QImage& KImageEffect::blend(QImage &image, float initial_intensity,
                            const QColor &bgnd, GradientType eff,
                            bool anti_dir)
{
    int r_bgnd = bgnd.red(), g_bgnd = bgnd.green(), b_bgnd = bgnd.blue();
    int r, g, b;
    int ind;

    unsigned int xi, xf, yi, yf;
    unsigned int a;

    // check the boundaries of the initial intesity param
    float unaffected = 1;
    if (initial_intensity >  1) initial_intensity =  1;
    if (initial_intensity < -1) initial_intensity = -1;
    if (initial_intensity < 0) {
        unaffected = 1. + initial_intensity;
        initial_intensity = 0;
    }


    float intensity = initial_intensity;
    float var = 1. - initial_intensity;

    if (anti_dir) {
        initial_intensity = intensity = 1.;
        var = -var;
    }

    register int x, y;

    unsigned int *data =  (unsigned int *)image.bits();

    if( eff == VerticalGradient || eff == HorizontalGradient ) {

        // set the image domain to apply the effect to
        xi = 0, xf = image.width();
        yi = 0, yf = image.height();
        if (eff == VerticalGradient) {
            if (anti_dir) yf = (int)(image.height() * unaffected);
            else yi = (int)(image.height() * (1 - unaffected));
        }
        else {
            if (anti_dir) xf = (int)(image.width() * unaffected);
            else xi = (int)(image.height() * (1 - unaffected));
        }

        var /= (eff == VerticalGradient?yf-yi:xf-xi);

        for (y = yi; y < (int)yf; y++) {
            intensity = eff == VerticalGradient? intensity + var :
                initial_intensity;
            for (x = xi; x < (int)xf ; x++) {
                if (eff == HorizontalGradient) intensity += var;
                ind = x + image.width()  * y ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);
            }
        }
    }
    else if (eff == DiagonalGradient  || eff == CrossDiagonalGradient) {
        float xvar = var / 2 / image.width();  // / unaffected;
        float yvar = var / 2 / image.height(); // / unaffected;
        float tmp;

        for (x = 0; x < image.width() ; x++) {
            tmp =  xvar * (eff == DiagonalGradient? x : image.width()-x-1);
            for (y = 0; y < image.height() ; y++) {
                intensity = initial_intensity + tmp + yvar * y;
                ind = x + image.width()  * y ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);
            }
        }
    }

    else if (eff == RectangleGradient || eff == EllipticGradient) {
        float xvar;
        float yvar;

        for (x = 0; x < image.width() / 2 + image.width() % 2; x++) {
            xvar = var / image.width()  * (image.width() - x*2/unaffected-1);
            for (y = 0; y < image.height() / 2 + image.height() % 2; y++) {
                yvar = var / image.height()   * (image.height() - y*2/unaffected -1);

                if (eff == RectangleGradient)
                    intensity = initial_intensity + QMAX(xvar, yvar);
                else
                    intensity = initial_intensity + sqrt(xvar * xvar + yvar * yvar);
                if (intensity > 1) intensity = 1;
                if (intensity < 0) intensity = 0;

                //NW
                ind = x + image.width()  * y ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);

                //NE
                ind = image.width() - x - 1 + image.width()  * y ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);
            }
        }

        //CT  loop is doubled because of stupid central row/column issue.
        //    other solution?
        for (x = 0; x < image.width() / 2; x++) {
            xvar = var / image.width()  * (image.width() - x*2/unaffected-1);
            for (y = 0; y < image.height() / 2; y++) {
                yvar = var / image.height()   * (image.height() - y*2/unaffected -1);

                if (eff == RectangleGradient)
                    intensity = initial_intensity + QMAX(xvar, yvar);
                else
                    intensity = initial_intensity + sqrt(xvar * xvar + yvar * yvar);
                if (intensity > 1) intensity = 1;
                if (intensity < 0) intensity = 0;

                //SW
                ind = x + image.width()  * (image.height() - y -1) ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);

                //SE
                ind = image.width()-x-1 + image.width() * (image.height() - y - 1) ;
                r = qRed  (data[ind]) + (int)(intensity *
                                              (r_bgnd - qRed  (data[ind])));
                g = qGreen(data[ind]) + (int)(intensity *
                                              (g_bgnd - qGreen(data[ind])));
                b = qBlue (data[ind]) + (int)(intensity *
                                              (b_bgnd - qBlue (data[ind])));
                if (r > 255) r = 255; if (r < 0 ) r = 0;
                if (g > 255) g = 255; if (g < 0 ) g = 0;
                if (b > 255) b = 255; if (b < 0 ) b = 0;
                a = qAlpha(data[ind]);
                data[ind] = qRgb(r, g, b) | ((uint)(a & 0xff) << 24);
            }
        }
    }

    else debug("not implemented");

    return image;
}

//======================================================================
//
// Hash effects
//
//======================================================================

unsigned int KImageEffect::lHash(unsigned int c)
{
    unsigned char r = qRed(c), g = qGreen(c), b = qBlue(c), a = qAlpha(c);
    unsigned char nr, ng, nb;
    nr =(r >> 1) + (r >> 2); nr = nr > r ? 0 : nr;
    ng =(g >> 1) + (g >> 2); ng = ng > g ? 0 : ng;
    nb =(b >> 1) + (b >> 2); nb = nb > b ? 0 : nb;

    return qRgb(nr, ng, nb) | (uint) ((0xff & a) << 24);
}


// -----------------------------------------------------------------------------

unsigned int KImageEffect::uHash(unsigned int c)
{
    unsigned char r = qRed(c), g = qGreen(c), b = qBlue(c), a = qAlpha(c);
    unsigned char nr, ng, nb;
    nr = r + (r >> 3); nr = nr < r ? ~0 : nr;
    ng = g + (g >> 3); ng = ng < g ? ~0 : ng;
    nb = b + (b >> 3); nb = nb < b ? ~0 : nb;

    return qRgb(nr, ng, nb) | (uint) ((0xff & a) << 24);
}


// -----------------------------------------------------------------------------

QImage& KImageEffect::hash(QImage &image, Lighting lite, unsigned int spacing)
{
    register int x, y;
    unsigned int *data =  (unsigned int *)image.bits();
    unsigned int ind;

    //CT no need to do it if not enough space
    if ((lite == NorthLite ||
         lite == SouthLite)&&
        (unsigned)image.height() < 2+spacing) return image;
    if ((lite == EastLite ||
         lite == WestLite)&&
        (unsigned)image.height() < 2+spacing) return image;

    if (lite == NorthLite || lite == SouthLite) {
        for (y = 0 ; y < image.height(); y = y + 2 + spacing) {
            for (x = 0; x < image.width(); x++) {
                ind = x + image.width() * y;
                data[ind] = lite==NorthLite?uHash(data[ind]):lHash(data[ind]);

                ind = ind + image.width();
                data[ind] = lite==NorthLite?lHash(data[ind]):uHash(data[ind]);
            }
        }
    }

    else if (lite == EastLite || lite == WestLite) {
        for (y = 0 ; y < image.height(); y++) {
            for (x = 0; x < image.width(); x = x + 2 + spacing) {
                ind = x + image.width() * y;
                data[ind] = lite==EastLite?uHash(data[ind]):lHash(data[ind]);

                ind++;
                data[ind] = lite==EastLite?lHash(data[ind]):uHash(data[ind]);
            }
        }
    }

    else if (lite == NWLite || lite == SELite) {
        for (y = 0 ; y < image.height(); y++) {
            for (x = 0;
                 x < (int)(image.width() - ((y & 1)? 1 : 0) * spacing);
                 x = x + 2 + spacing) {
                ind = x + image.width() * y + ((y & 1)? 1 : 0);
                data[ind] = lite==NWLite?uHash(data[ind]):lHash(data[ind]);

                ind++;
                data[ind] = lite==NWLite?lHash(data[ind]):uHash(data[ind]);
            }
        }
    }

    else if (lite == SWLite || lite == NELite) {
        for (y = 0 ; y < image.height(); y++) {
            for (x = 0  + ((y & 1)? 1 : 0); x < image.width(); x = x + 2 + spacing) {
                ind = x + image.width() * y - ((y & 1)? 1 : 0);
                data[ind] = lite==SWLite?uHash(data[ind]):lHash(data[ind]);

                ind++;
                data[ind] = lite==SWLite?lHash(data[ind]):uHash(data[ind]);
            }
        }
    }

    return image;
}


//======================================================================
//
// Flatten effects
//
//======================================================================

QImage& KImageEffect::flatten(QImage &img, const QColor &ca,
                            const QColor &cb, int ncols)
{
    // a bitmap is easy...
    if (img.depth() == 1) {
	img.setColor(0, ca.rgb());
	img.setColor(1, cb.rgb());
	return img;
    }

    int r1 = ca.red(); int r2 = cb.red();
    int g1 = ca.green(); int g2 = cb.green();
    int b1 = ca.blue(); int b2 = cb.blue();
    int min = 0, max = 255;

    int mean;
    QRgb col;

    // Get minimum and maximum greylevel.
    if (img.numColors()) {
	// pseudocolor
	for (int i = 0; i < img.numColors(); i++) {
	    col = img.color(i);
	    mean = (qRed(col) + qGreen(col) + qBlue(col)) / 3;
	    min = QMIN(min, mean);
	    max = QMAX(max, mean);
	}
    } else {
	// truecolor
	for (int y=0; y < img.height(); y++)
	    for (int x=0; x < img.width(); x++) {
		col = img.pixel(x, y);
		int mean = (qRed(col) + qGreen(col) + qBlue(col)) / 3;
		min = QMIN(min, mean);
		max = QMAX(max, mean);
	    }
    }
	
    // Conversion factors
    int r, g, b;
    float sr = ((float) r2 - r1) / (max - min);
    float sg = ((float) g2 - g1) / (max - min);
    float sb = ((float) b2 - b1) / (max - min);


    // Repaint the image
    if (img.numColors()) {
	for (int i=0; i < img.numColors(); i++) {
	    col = img.color(i);
	    mean = (qRed(col) + qGreen(col) + qBlue(col)) / 3;
	    r = (int) (sr * (mean - min) + r1 + 0.5);
	    g = (int) (sg * (mean - min) + g1 + 0.5);
	    b = (int) (sb * (mean - min) + b1 + 0.5);
	    img.setColor(i, qRgb(r, g, b));
	}
    } else {
	for (int y=0; y < img.height(); y++)
	    for (int x=0; x < img.width(); x++) {
		col = img.pixel(x, y);
		mean = (qRed(col) + qGreen(col) + qBlue(col)) / 3;
		r = (int) (sr * (mean - min) + r1 + 0.5);
		g = (int) (sg * (mean - min) + g1 + 0.5);
		b = (int) (sb * (mean - min) + b1 + 0.5);
		img.setPixel(x, y, qRgb(r, g, b));
	    }
    }


    // Dither if necessary
    if ( (ncols <= 0) || ((img.numColors() != 0) && (img.numColors() <= ncols)))
	return img;

    if (ncols == 1) ncols++;
    if (ncols > 256) ncols = 256;

    QColor *pal = new QColor[ncols];
    sr = ((float) r2 - r1) / (ncols - 1);
    sg = ((float) g2 - g1) / (ncols - 1);
    sb = ((float) b2 - b1) / (ncols - 1);

    for (int i=0; i<ncols; i++)
	pal[i] = QColor(r1 + sr*i, g1 + sg*i, b1 + sb*i);
	
    kFSDither dither(pal, ncols);
    img = dither.dither(img);

    delete[] pal;
    return img;
}


//======================================================================
//
// Fade effects
//
//======================================================================

QImage& KImageEffect::fade(QImage &img, float val, const QColor &color)
{
    // We don't handle bitmaps
    if (img.depth() == 1)
	return img;

    int red = color.red();
    int green = color.green();
    int blue = color.blue();

    QRgb col;
    int r, g, b;

    if (img.depth() <= 8) {
	// pseudo color

	for (int i=0; i<img.numColors(); i++) {
	    col = img.color(i);
	    r = (int) ((1 - val) * qRed(col) + val * red + 0.5);
	    g = (int) ((1 - val) * qGreen(col) + val * green + 0.5);
	    b = (int) ((1 - val) * qBlue(col) + val * blue + 0.5);
	    img.setColor(i, qRgb(r, g, b));
	}

    } else {
	// truecolor

	for (int y=0; y<img.height(); y++) {
	    for (int x=0; x<img.width(); x++) {
		col = img.pixel(x, y);
		r = (int) ((1 - val) * qRed(col) + val * red + 0.5);
		g = (int) ((1 - val) * qGreen(col) + val * green + 0.5);
		b = (int) ((1 - val) * qBlue(col) + val * blue + 0.5);
		img.setPixel(x, y, qRgb(r, g, b));
	    }
        }
 }

    return img;
}

//======================================================================
//
// Color effects
//
//======================================================================

// This code is adapted from code (C) Rik Hemsley <rik@kde.org>
//
// The formula used (r + b + g) /3 is different from the qGray formula
// used by Qt.  This is because our formula is much much faster.  If,
// however, it turns out that this is producing sub-optimal images,
// then it will have to change (kurt)
//
// It does produce lower quality grayscale ;-) Use fast == true for the fast
// algorithm, false for the higher quality one (mosfet).
QImage& KImageEffect::toGray(QImage &img, bool fast)
{
    if(fast){
        if (img.depth() == 32) {
            register uchar * r(img.bits());
            register uchar * g(img.bits() + 1);
            register uchar * b(img.bits() + 2);

            uchar * end(img.bits() + img.numBytes());

            while (r != end) {

                *r = *g = *b = (((*r + *g) >> 1) + *b) >> 1; // (r + b + g) / 3

                r += 4;
                g += 4;
                b += 4;
            }
        }
        else
        {
            for (int i = 0; i < img.numColors(); i++)
            {
                register uint r = qRed(img.color(i));
                register uint g = qGreen(img.color(i));
                register uint b = qBlue(img.color(i));

                register uint gray = (((r + g) >> 1) + b) >> 1;
                img.setColor(i, qRgb(gray, gray, gray));
            }
        }
    }
    else{
        int pixels = img.depth() > 8 ? img.width()*img.height() :
            img.numColors();
        unsigned int *data = img.depth() > 8 ? (unsigned int *)img.bits() :
            (unsigned int *)img.colorTable();
        int val, i;
        for(i=0; i < pixels; ++i){
            val = qGray(data[i]);
            data[i] = qRgb(val, val, val);
        }
    }
    return img;
}

// CT 29Jan2000 - desaturation algorithms
QImage& KImageEffect::desaturate(QImage &img, float desat)
{
    
    if (desat < 0) desat = 0.;
    if (desat > 1) desat = 1.;
    int pixels = img.depth() > 8 ? img.width()*img.height() :
        img.numColors();
    unsigned int *data = img.depth() > 8 ? (unsigned int *)img.bits() :
        (unsigned int *)img.colorTable();
    int h, s, v, i;
    QColor clr; // keep constructor out of loop (mosfet)
    for(i=0; i < pixels; ++i){
        clr.setRgb(data[i]);
	clr.hsv(&h, &s, &v);
	clr.setHsv(h, (int)(s * (1. - desat)), v);
	data[i] = clr.rgb();
    }
    return img;
}

// Contrast stuff (mosfet)
QImage& KImageEffect::contrast(QImage &img, int c)
{
    if(c > 255)
        c = 255;
    if(c < -255)
        c =  -255;
    int pixels = img.depth() > 8 ? img.width()*img.height() :
        img.numColors();
    unsigned int *data = img.depth() > 8 ? (unsigned int *)img.bits() :
        (unsigned int *)img.colorTable();
    int i, r, g, b;
    for(i=0; i < pixels; ++i){
        r = qRed(data[i]);
        g = qGreen(data[i]);
        b = qBlue(data[i]);
        if(qGray(data[i]) <= 127){
            if(r - c <= 255)
                r -= c;
            if(g - c <= 255)
                g -= c;
            if(b - c <= 255)
                b -= c;
        }
        else{
            if(r + c <= 255)
                r += c;
            if(g + c <= 255)
                g += c;
            if(b + c <= 255)
                b += c;
        }
        data[i] = qRgb(r, g, b);
    }
    return(img);
}

