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

#include "kprinterimpl.h"
#include "kprinter.h"
#include "kmfactory.h"
#include "kmmanager.h"
#include "kmuimanager.h"
#include "kmfiltermanager.h"
#include "kmthreadjob.h"
#include "kmprinter.h"
#include "kprintfilter.h"

#include <qfile.h>
#include <qregexp.h>
#include <klocale.h>
#include <dcopclient.h>
#include <kapp.h>
#include <kstddirs.h>
#include <kdatastream.h>
#include <kdebug.h>
#include <kmimemagic.h>
#include <kmessagebox.h>

#include <stdlib.h>

void dumpOptions(const QMap<QString,QString>&);
void initEditPrinter(KMPrinter *p)
{
	if (!p->isEdited())
	{
		p->setEditedOptions(p->defaultOptions());
		p->setEdited(true);
	}
}

//****************************************************************************************

KPrinterImpl::KPrinterImpl(QObject *parent, const char *name)
: QObject(parent,name)
{
}

KPrinterImpl::~KPrinterImpl()
{
}

void KPrinterImpl::preparePrinting(KPrinter*)
{
}

bool KPrinterImpl::setupCommand(QString&, KPrinter*)
{
	return false;
}

bool KPrinterImpl::printFiles(KPrinter *p, const QStringList& f, bool flag)
{
	QString	cmd;
	if (p->option("kde-isspecial") == "1")
	{
		if (p->option("kde-special-command").isEmpty() && p->outputToFile())
		{
			if (f.count() > 1)
			{
				p->setErrorMessage(i18n("Cannot copy multiple files into one file."));
				return false;
			}
			else if (system(QString::fromLatin1("%1 %2 %3").arg((flag?"mv":"cp")).arg(f[0]).arg(p->outputFileName()).latin1()) != 0)
			{
				p->setErrorMessage(i18n("Cannot save print file. Check that you have write access to it."));
				return false;
			}
			return true;
		}
		if (!setupSpecialCommand(cmd,p,f))
			return false;
	}
	else if (!setupCommand(cmd,p))
		return false;
	return startPrinting(cmd,p,f,flag);
}

void KPrinterImpl::broadcastOption(const QString& key, const QString& value)
{
	// force printer listing if not done yet (or reload needed)
	QPtrList<KMPrinter>	*printers = KMFactory::self()->manager()->printerList(false);
	if (printers)
	{
		QPtrListIterator<KMPrinter>	it(*printers);
		for (;it.current();++it)
		{
			initEditPrinter(it.current());
			it.current()->setEditedOption(key,value);
		}
	}
}

int KPrinterImpl::dcopPrint(const QString& cmd, const QStringList& files, bool removeflag)
{
	kdDebug() << "kdeprint: print command: " << cmd << endl;

	int result = 0;
	DCOPClient	*dclient = kapp->dcopClient();
	if (!dclient || (!dclient->isAttached() && !dclient->attach()))
	{
		return result;
	}

	QByteArray data, replyData;
	QCString replyType;
	QDataStream arg( data, IO_WriteOnly );
	arg << cmd;
	arg << files;
	arg << removeflag;
	if (dclient->call( "kded", "kdeprintd", "print(QString,QStringList,bool)", data, replyType, replyData ))
	{
		if (replyType == "int")
		{
			QDataStream _reply_stream( replyData, IO_ReadOnly );
			_reply_stream >> result;
		}
	}
	return result;
}

bool KPrinterImpl::startPrinting(const QString& cmd, KPrinter *printer, const QStringList& files, bool flag)
{
	QString	command(cmd), filestr;
	QStringList	printfiles;
	if (command.find("%in") == -1) command.append(" %in");

	for (QStringList::ConstIterator it=files.begin(); it!=files.end(); ++it)
		if (QFile::exists(*it))
		{
			// quote and encode filenames
			filestr.append("'").append(QFile::encodeName(*it)).append("' ");
			printfiles.append(*it);
		}
		else
			kdDebug() << "File not found: " << (*it) << endl;

	if (printfiles.count() > 0)
	{
		command.replace(QRegExp("%in"),filestr);
		int pid = dcopPrint(command,files,flag);
		if (pid > 0)
		{
			if (printer)
				KMThreadJob::createJob(pid,printer->printerName(),printer->docName(),getenv("USER"),0);
			return true;
		}
		else
		{
			QString	msg = i18n("Unable to start child print process. ");
			if (pid == 0)
				msg += i18n("The KDE print server (<b>kdeprintd</b>) could not be contacted. Check that this server is running.");
			else
				msg += i18n("1 is the command that <files> is given to", "Check the command syntax:\n%1 <files>").arg(cmd);
			printer->setErrorMessage(msg);
			return false;
		}
	}
	else
	{
		printer->setErrorMessage(i18n("No valid file was found for printing. Operation aborted."));
		return false;
	}
}

QString KPrinterImpl::tempFile()
{
	QString	f;
	// be sure the file doesn't exist
	do f = locateLocal("tmp","kdeprint_") + KApplication::randomString(8); while (QFile::exists(f));
	return f;
}

int KPrinterImpl::filterFiles(KPrinter *printer, QStringList& files, bool flag)
{
	QStringList	flist = QStringList::split(',',printer->option("_kde-filters"),false);
	QMap<QString,QString>	opts = printer->options();

	// generic page selection mechanism (using psselect filter)
	// do it only if:
	//	- using system-side page selection
	//	- special printer or regular printer without page selection support in current plugin
	//	- one of the page selection option has been selected to non default value
	// Action -> add the psselect filter to the filter chain.
	if (printer->pageSelection() == KPrinter::SystemSide &&
	    (printer->option("kde-isspecial") == "1" || !(KMFactory::self()->uiManager()->pluginPageCap() & KMUiManager::PSSelect)) &&
	    (printer->pageOrder() == KPrinter::LastPageFirst ||
	     !printer->option("kde-range").isEmpty() ||
	     printer->pageSet() != KPrinter::AllPages))
	{
		if (flist.findIndex("psselect") == -1)
		{
			int	index = KMFactory::self()->filterManager()->insertFilter(flist, "psselect", false);
			if (index == -1 || !KMFactory::self()->filterManager()->checkFilter("psselect"))
			{
				printer->setErrorMessage(i18n("<p>Unable to perform the requested page selection. The filter <b>psselect</b> "
							      "cannot be inserted in the current filter chain. See <b>Filter</b> tab in the "
							      "printer properties dialog for further information.</p>"));
				return -1;
			}
		}
		if (printer->pageOrder() == KPrinter::LastPageFirst)
			opts["_kde-psselect-order"] = "r";
		if (!printer->option("kde-range").isEmpty())
			opts["_kde-psselect-range"] = printer->option("kde-range");
		if (printer->pageSet() != KPrinter::AllPages)
			opts["_kde-psselect-set"] = (printer->pageSet() == KPrinter::OddPages ? "-o" : "-e");
	}

	return doFilterFiles(printer, files, flist, opts, flag);
}

int KPrinterImpl::doFilterFiles(KPrinter *printer, QStringList& files, const QStringList& flist, const QMap<QString,QString>& opts, bool flag)
{
	// nothing to do
	if (flist.count() == 0)
		return 0;

	QString	filtercmd;
	KMFilterManager	*fmgr = KMFactory::self()->filterManager();
	for (uint i=0;i<flist.count();i++)
	{
		KPrintFilter	*filter = fmgr->filter(flist[i]);
		QString		subcmd = filter->buildCommand(opts,(i>0),(i<(flist.count()-1)));
		if (!subcmd.isEmpty())
		{
			filtercmd.append(subcmd);
			if (i < flist.count()-1)
				filtercmd.append("| ");
		}
		else
		{
			printer->setErrorMessage(i18n("Error while reading filter description for <b>%1</b>. Empty command line received.").arg(filter->idName()));
			return -1;
		}
	}
	kdDebug() << "kdeprint: filter command: " << filtercmd << endl;

	QRegExp	rin("%in"), rout("%out"), rps("%psl");
	QString	ps = pageSizeToPageName(printer->pageSize());
	for (QStringList::Iterator it=files.begin(); it!=files.end(); ++it)
	{
		QString	tmpfile = tempFile();
		QString	cmd(filtercmd);
		cmd.replace(rin,*it);
		cmd.replace(rout,tmpfile);
		cmd.replace(rps,ps.lower());
		if (system(cmd.latin1()) != 0)
		{
			printer->setErrorMessage(i18n("Error while filtering. Command was: <b>%1</b>.").arg(filtercmd));
			return -1;
		}
		if (flag) QFile::remove(*it);
		*it = tmpfile;
	}
	return 1;
}

int KPrinterImpl::autoConvertFiles(KPrinter *printer, QStringList& files, bool flag)
{
	PluginInfo	info = KMFactory::self()->pluginInfo(KMFactory::self()->printSystem());
	int		status(0), result;
	for (QStringList::Iterator it=files.begin(); it!=files.end(); )
	{
		QString	mime = KMimeMagic::self()->findFileType(*it)->mimeType();
		if (info.mimeTypes.findIndex(mime) == -1)
		{
			if ((result=KMessageBox::warningYesNoCancel(NULL,
					       i18n("The file format <b>%1</b> is not directly supported by the current print system. "
					            "KDE can try to convert automatically this file to a supported format. But you can "
						    "still try to send the file to the printer without any conversion. Do you want KDE "
						    "to try to convert this file to <b>%2</b>?").arg(mime).arg(info.primaryMimeType),
					       QString::null,
					       i18n("Convert"),
					       i18n("Keep"),
					       QString::fromLatin1("kdeprintAutoConvert"))) == KMessageBox::Yes)
			{
				// find the filter chain
				QStringList	flist = KMFactory::self()->filterManager()->autoFilter(mime, info.primaryMimeType);
				if (flist.count() == 0)
				{
					if (KMessageBox::warningYesNo(NULL,
								      i18n("No appropriate filter was found to convert the file "
								           "format <b>%1</b> into <b>%2</b>. Do you want to print the "
									   "file using its original format?").arg(mime).arg(info.primaryMimeType),
								      QString::null,
								      i18n("Print"),
								      i18n("Skip")) == KMessageBox::No)
					{
						if (flag)
							QFile::remove(*it);
						it = files.remove(it);
					}
					else
						++it;
					continue;
				}
				QStringList	l(*it);
				switch (doFilterFiles(printer, l, flist, QMap<QString,QString>(), flag))
				{
					case -1:
						return -1;
					case 0:
						break;
					case 1:
						status = 1;
						*it = l[0];
						break;
				}
			}
			else if (result == KMessageBox::Cancel)
			{
				files.clear();
				return 0;
			}
		}
		++it;
	}
	return status;
}

bool KPrinterImpl::setupSpecialCommand(QString& cmd, KPrinter *p, const QStringList& files)
{
	QString	s(p->option("kde-special-command"));
	if (s.isEmpty())
	{
		p->setErrorMessage("Empty command.");
		return false;
	}
	s.replace(QRegExp("%out"),p->outputFileName());
	s.replace(QRegExp("%psl"),QString::fromLatin1(pageSizeToPageName(p->pageSize())).lower());
	cmd = s;
	return true;
}
#include "kprinterimpl.moc"
