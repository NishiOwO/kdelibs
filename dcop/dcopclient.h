/*
   This file is part of the KDE libraries
   Copyright (c) 1999 Preston Brown <pbrown@kde.org>
   Copyright (c) 1999 Matthias Ettrich <ettrich@kde.org>
 
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

#ifndef _DCOPCLIENT_H
#define _DCOPCLIENT_H

#include <qobject.h>
#include <qcstring.h>
#include <qvaluelist.h>
#include <qstring.h>

class DCOPClientPrivate;

typedef QValueList<QCString> QCStringList;

/**
 * Provides inter-process communication and remote procedure calls
 * for KDE applications.
 *
 * This class provides IPC and RPC for KDE applications.  Usually you
 * will not have to instantiate one yourself, because KApplication 
 * contains a method to return a pointer to a DCOPClient object which
 * can be used for your whole application.
 *
 * Before being able to send or receive any DCOP messages, you will have
 * to attach your client object to the DCOP server, and then register
 * your application with a specific name. See #attach and #registerAs for
 * more information.
 *
 * Data to be sent should be serialized into a QDataStream which was
 * initialized with the QByteArray that you actually intend to send
 * the data in.  An example of how you might do this:
 *
 * <pre>
 *   QByteArray data;
 *   QDataStream dataStream(data, IO_WriteOnly);
 *   dataStream << QString("This is text I am serializing");
 *   client->send("someApp", "someObject", "someFunction", data);
 * </pre>
 *
 * @see KApplication::dcopClient
 */
class DCOPClient : public QObject
{
  Q_OBJECT

 public:
  /**
   * Creates a new DCOP client, but does not attach to any server.  */
  DCOPClient();

  /**
   * clean up any open connections and dynamic data.
   */
  virtual ~DCOPClient();

  /**
   * specify the address of a server to use upon attaching.
   * if no server address is ever specified, attach will try its best to
   * find the server anyway.
   */
  static void setServerAddress(const QCString &addr);

  /**
   * Attach to the DCOP server.  If the connection was already attached,
   * the connection will be re-established with the new appId.
   *
   * @param appId is a UNIQUE application/program id that the server
   * will use to associate requests with.  If no appId is specified
   * an anonymous connection will be made.
   *
   * @return true if attaching was successful.
   */
  bool attach(const QCString &appId = 0);

  /**
   * Detach from the DCOP server.
   */
  bool detach();

  /**
   * Query whether or not the client is attached to the server.
   */
  bool isAttached() const;

  /**
   * return the socket over which DCOP is communicating with the server.
   */
  int socket() const;

  /**
   * send a data block to the server.
   * @param remApp the remote application id.
   * @param remObj the name of the remote object.
   * @param remFun the remote function in the specified object to call.
   * @param data the data to provide to the remote function.
   * @param fast if set to true, a "fast" form of IPC will be used.
   *        Fast connections are not guaranteed to be implemented, but
   *        if they are they work only on the local machine, not across
   *        the network.  "fast" is only a hint not an order.
   *
   * @return whether or not the server was able to accept the send.
   */
  bool send(const QCString &remApp, const QCString &remObj,
	    const QCString &remFun, const QByteArray &data,
	    bool fast=false);

  /**
   * This function acts exactly the same as the above, but the data
   * parameter can be specified as a QString for convenience.
   */
  bool send(const QCString &remApp, const QCString &remObj,
	    const QCString &remFun, const QString &data,
	    bool fast=false);

  /**
   * performs a synchronous send and receive.  The parameters are
   * the same as for send, with the exception of another QByteArray
   * being provided for results to be (optionally) returned in.
   *
   * @see #send
   */
  bool call(const QCString &remApp, const QCString &remObj,
	    const QCString &remFun, const QByteArray &data,
	    QByteArray &replyData, bool fast=false);


  /**
   * override to handle app-wide function calls unassociated w/an object.
   */
  virtual bool process(const QCString &fun, const QByteArray &data,
		       QByteArray &replyData);

  /**
   * Check whether @param remApp is registered with the DCOPServer.
   * @return true if the remote application is registered, otherwise false.
   */
  bool isApplicationRegistered( const QCString& remApp);

  /**
   * Return the list of all currently registered applications.
   */
  QCStringList registeredApplications();

  /**
   * receive a piece of data from the server.
   * @param app the application the data was intended for.  Should be
   *        equal to our appId that we passed when the DCOPClient was
   *        created.
   * @param obj the name of the object to pass the data on to.
   * @param fun the name of the function in the object to call.
   * @param data the arguments for the function.
   * @internal
   */
  bool receive(const QCString &app, const QCString &obj,
	       const QCString &fun, const QByteArray& data,
	       QByteArray &replyData);

 public slots:

 protected slots:
  void processSocketData(int socknum);

 protected:

 private:
  /**
   * this is only public so that ICE callbacks work.  Don't touch.
   */
  DCOPClientPrivate *d;

};

#endif
