/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _KJS_WINDOW_H_
#define _KJS_WINDOW_H_

#include <kjs/object.h>
#include <kjs/function.h>

#include "kjs_binding.h"

class KHTMLView;

namespace KJS {

  class Window : public DOMObject {
  public:
    Window(KHTMLView *w) : widget(w) { }
    virtual KJSO tryGet(const UString &p) const;
    virtual void tryPut(const UString &p, const KJSO& v);
  private:
    KHTMLView *widget;
  };

  class WindowFunc : public DOMFunction {
  public:
    WindowFunc(KHTMLView *w, int i) : widget(w), id(i) { };
    Completion tryExecute(const List &);
    enum { Alert, Confirm, Open };
  private:
    KHTMLView *widget;
    int id;
  };

}; // namespace

#endif
