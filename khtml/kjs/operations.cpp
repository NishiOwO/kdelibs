/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999 Harri Porten (porten@kde.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 */

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"
#include "operations.h"

using namespace KJS;

// ECMA 9.1
KJSO *KJS::toPrimitive(KJSO *obj, Type preferred)
{
  if (!obj->isA(Object)) {
    return obj->ref();
  }

  /* TODO */
}

// ECMA 9.2
KJSO *KJS::toBoolean(KJSO *obj)
{
  bool b;

  switch (obj->type())
    {
    case Undefined:
    case Null:
      b = false;
      break;
    case Boolean:
      return obj->ref();
    case Number:
      b = !((obj->dVal() == 0) /* || (obj->iVal() == N0) */ ||
	(obj->dVal() == NaN));
      break;
    case String:
      b = (obj->sVal().length() > 0);
      break;
    case Object:
      b = true;
      break;
    default:
      b = false;
      assert(!"unhandled object type in switch");
    }

  return new KJSBoolean(b);
}

// ECMA 9.3
KJSO *KJS::toNumber(KJSO *obj)
{
  double d;
  Ptr tmp, res;

  switch (obj->type())
    {
    case Undefined:
      d = NaN;
      break;
    case Null:
      d = 0;
      break;
    case Boolean:
      d = obj->bVal() ? 1 : 0;
      break;
    case Number:
      return obj->ref();
    case String:
      /* TODO */
      break;
    case Object:
      tmp = toPrimitive(obj, Number);
      res = toNumber(tmp);
      return res->ref();
    default:
      cerr << "type = " << obj->type() << endl;
      assert(!"toNumber: unhandled switch case");
      exit(1);
    }

  return new KJSNumber(d);
}

// ECMA 9.4
KJSO *KJS::toInteger(KJSO *obj)
{
  /* TODO */
}

// ECMA 9.5
KJSO *KJS::toInt32(KJSO *obj)
{
  /* TODO */
}

// ECMA 9.6
KJSO *KJS::toUInt32(KJSO *obj)
{
  /* TODO */
}

// ECMA 9.7
KJSO *KJS::toUInt16(KJSO *obj)
{
  /* TODO */
}

// ECMA 9.8
KJSO *KJS::toString(KJSO *obj)
{
  const char *c;
  double d;
  char buffer[40];
  Ptr res, tmp;

  switch (obj->type())
    {
    case Undefined:
      c = "undefined";
      break;
    case Null:
      c = "null";
      break;
    case Boolean:
      c = obj->bVal() ? "true" : "false";
      break;
    case Number:
      d = obj->dVal();
      // truncate decimal digits on round values
      if (d != (double)((int)d))
	sprintf(buffer, "%f", d);
      else
	sprintf(buffer, "%d", (int)d);
      c = buffer;
      break;
    case String:
      return obj->ref();
    case Object:
      tmp = toPrimitive(obj, String);
      res = toString(tmp);
      return res->ref();
    default:
      assert(!"toString: unhandled switch case");
      exit(1);
    }
  res = new KJSString(c);
  return res->ref();
}

// ECMA 9.9
KJSO *KJS::toObject(KJSO *obj)
{
  KJSO *o;

  switch (obj->type())
    {
    case Undefined:
      return new KJSError(ErrUndefToObject, o);
    case Null:
      return new KJSError(ErrNullToObject, o);
    case Boolean:
      o = new KJSBoolean(obj->bVal());
      break;
    case Number:
      o = new KJSNumber(obj->dVal());
      break;
    case String:
      o = new KJSString(obj->sVal().ascii());
      break;
    case Object:
    case DeclaredFunction:
    case InternalFunction:
    case AnonymousFunction:
      return obj->ref();
    default:
      assert(!"toObject: unhandled switch case");
      exit(1);
    }

  return o;
}


// ECMA 11.9.3
bool KJS::compare(KJSO *v1, Operator oper, KJSO *v2)
{
  if (!v1->isA(Number) || !v2->isA(Number)) {
    cerr << "Sorry. Can only handle numbers in compare() so far." << endl;
    abort();
  }
  if (oper != OpEqEq && oper != OpNotEq) {
    cerr << "Sorry. Can only handle == and != in compare() so far." << endl;
    abort();
  }

  Ptr n1 = toNumber(v1);
  Ptr n2 = toNumber(v2);

  bool eq = (n1->dVal() == n2->dVal());

  return (oper == OpEqEq) ? eq : !eq;
}
