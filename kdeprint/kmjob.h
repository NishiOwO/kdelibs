/*
 *  This file is part of the KDE libraries
 *  Copyright (c) 2001 Michael Goffioul <goffioul@imec.be>
 *
 *  $Id$
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
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
 **/

#ifndef KMJOB_H
#define KMJOB_H

#include <qstring.h>
#include "kmobject.h"

class KMJob : public KMObject
{
public:
	enum JobAction {
		Remove        = 0x01,
		Move          = 0x02,
		Hold          = 0x04,
		Resume        = 0x08,
		Restart       = 0x10,
		ShowCompleted = 0x20,
		All           = 0xFF
	};
	enum JobState {
		Printing  = 1,
		Queued    = 2,
		Held      = 3,
		Error     = 4,
		Cancelled = 5,
		Aborted   = 6,
		Completed = 7,
		Unknown   = 8
	};
	enum JobType {
		System   = 0,
		Threaded = 1
	};

	KMJob();
	KMJob(const KMJob& j);

	void copy(const KMJob& j);
	QString pixmap();
	QString stateString();
	bool isCompleted() const		{ return (m_state >= Cancelled && m_state <= Completed); }
	bool isActive() const			{ return !isCompleted(); }

	// inline access functions
	int id() const				{ return m_ID; }
	void setId(int id)			{ m_ID = id; }
	const QString& name() const		{ return m_name; }
	void setName(const QString& s)		{ m_name = s; }
	const QString& printer() const		{ return m_printer; }
	void setPrinter(const QString& s)	{ m_printer = s; }
	const QString& owner() const		{ return m_owner; }
	void setOwner(const QString& s)		{ m_owner = s; }
	int state() const			{ return m_state; }
	void setState(int s)			{ m_state = s; }
	int size() const			{ return m_size; }
	void setSize(int s)			{ m_size = s; }
	const QString& uri() const		{ return m_uri; }
	void setUri(const QString& s)		{ m_uri = s; }
	int type() const			{ return m_type; }
	void setType(int t)			{ m_type = t; }
	int pages() const			{ return m_pages; }
	void setPages(int p)			{ m_pages = p; };
	int processedPages() const		{ return m_processedpages; }
	void setProcessedPages(int p)		{ m_processedpages = p; }
	int processedSize() const		{ return m_processedsize; }
	void setProcessedSize(int s)		{ m_processedsize = s; }

protected:
	void init();

protected:
	// normal members
	int	m_ID;
	QString	m_name;
	QString	m_printer;
	QString	m_owner;
	int	m_state;
	int	m_size;
	int	m_type;
	int	m_pages;
	int	m_processedsize;
	int	m_processedpages;

	// internal members
	QString	m_uri;
};

#endif
