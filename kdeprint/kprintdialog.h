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

#ifndef KPRINTDIALOG_H
#define KPRINTDIALOG_H

#include <kdialog.h>
#include <qlist.h>

class KPrinter;
class KPrintDialogPage;
class QLabel;
class QComboBox;
class QCheckBox;
class QPushButton;
class QLineEdit;

class KPrintDialog : public KDialog
{
	Q_OBJECT
public:
	KPrintDialog(QWidget *parent = 0, const char *name = 0);
	~KPrintDialog();

	void setFlags(int f);
	void setDialogPages(QList<KPrintDialogPage> *pages);
	void initialize(KPrinter*);
	KPrinter* printer() const;

	static bool printerSetup(KPrinter*, QWidget*);

protected slots:
	void slotPrinterSelected(int);
	void slotBrowse();
	void slotProperties();
	void slotSetDefault();
	void slotOptions();
	virtual void done(int);
	void slotFilePrintToggled(bool);

protected:
	bool checkOutputFile();

protected:
	QLabel	*m_type, *m_state, *m_comment, *m_location;
	QPushButton	*m_properties, *m_default, *m_options, *m_filebrowse, *m_ok;
	QCheckBox	*m_preview, *m_printtofile;
	QLineEdit	*m_file;
	QComboBox	*m_printers;

	QList<KPrintDialogPage>	m_pages;
	KPrinter		*m_printer;
};

inline KPrinter* KPrintDialog::printer() const
{ return m_printer; }

#endif
