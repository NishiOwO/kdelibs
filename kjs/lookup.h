/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
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

#ifndef _KJSLOOKUP_H_
#define _KJSLOOKUP_H_

#include "ustring.h"

namespace KJS {

  struct HashEntry {
    unsigned int len;
    const void *c; // unicode data
    int value;
    const HashEntry *next;
  };

  struct HashTable {
    int type;
    int size;
    const HashEntry *entries;
    int hashSize;
  };

  /**
   * @short Fast keyword lookup.
   */
  class Lookup {
  public:
    static int find(const struct HashTable *table, const UString &s);
    static int find(const struct HashTable *table,
		    const UChar *c, unsigned int len);
    static unsigned int hash(const UChar *c, unsigned int len);
    static unsigned int hash(const UString &key);
  private:
    HashTable *table;
  };

  void lookupTest();

}; // namespace

#endif
