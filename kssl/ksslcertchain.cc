/* This file is part of the KDE project
 *
 * Copyright (C) 2001 George Staikos <staikos@kde.org>
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
 */
#ifdef HAVE_CONFIG_H 
#include <config.h>
#endif

#include "kssldefs.h"
#include "ksslcertificate.h"
#include "ksslcertchain.h"

// this hack provided by Malte Starostik to avoid glibc/openssl bug
// on some systems
#ifdef HAVE_SSL
#define crypt _openssl_crypt
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/stack.h>
#include <openssl/safestack.h>
#undef crypt
#endif

#include <kopenssl.h>
#include <kdebug.h>
#include <qstringlist.h>



#ifdef HAVE_SSL
#define sk_new d->kossl->sk_new
#define sk_push d->kossl->sk_push
#define sk_free d->kossl->sk_free
#define sk_value d->kossl->sk_value
#define sk_num d->kossl->sk_num
#endif

class KSSLCertChainPrivate {
public:
  KSSLCertChainPrivate() {
     kossl = KOSSL::self();
  }

  ~KSSLCertChainPrivate() {
  }

  KOSSL *kossl;
};

KSSLCertChain::KSSLCertChain() {
  d = new KSSLCertChainPrivate;
  _chain = NULL;
}


KSSLCertChain::~KSSLCertChain() {
  delete d;
#ifdef HAVE_SSL
  if (_chain) sk_X509_free((STACK_OF(X509)*)_chain);
#endif
}


bool KSSLCertChain::isValid() {
  return (_chain && depth() > 0);
}


KSSLCertChain *KSSLCertChain::replicate() {
KSSLCertChain *x = new KSSLCertChain;
QList<KSSLCertificate> ch = getChain();

  x->setChain(ch);   // this will do a deep copy for us
return x;
}


int KSSLCertChain::depth() {
#ifdef HAVE_SSL
  return sk_X509_num((STACK_OF(X509)*)_chain);
#endif
return 0;
}


QList<KSSLCertificate> KSSLCertChain::getChain() {
QList<KSSLCertificate> cl;
if (!_chain) return cl;
#ifdef HAVE_SSL
STACK_OF(X509) *x = (STACK_OF(X509) *)_chain;

   for (int i = 0; i < sk_X509_num(x); i++) {
     X509* x5 = sk_X509_value(x, i);
     KSSLCertificate *nc = new KSSLCertificate;
     nc->setCert(d->kossl->X509_dup(x5));
     cl.append(nc);
   }
#endif
return cl;
}


void KSSLCertChain::setChain(QList<KSSLCertificate>& chain) {
#ifdef HAVE_SSL
  if (_chain) sk_X509_free((STACK_OF(X509) *) _chain);

  _chain = (void *)sk_new(NULL);
  for (KSSLCertificate *x = chain.first(); x != 0; x = chain.next()) {
    sk_X509_push((STACK_OF(X509)*)_chain, d->kossl->X509_dup(x->getCert()));
  }

#endif
}

 
void KSSLCertChain::setChain(void *stack_of_x509) {
#ifdef HAVE_SSL
if (_chain) {
      sk_X509_free((STACK_OF(X509)*)_chain);
}
#endif
_chain = stack_of_x509;
}


void KSSLCertChain::setChain(QStringList chain) {
    QList<KSSLCertificate> cl;
    cl.setAutoDelete(true);
    for (QStringList::Iterator s = chain.begin(); s != chain.end(); ++s) {
       cl.append(KSSLCertificate::fromString((*s).local8Bit()));
    }
    setChain(cl);
}


