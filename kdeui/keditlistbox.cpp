/* This file is part of the KDE libraries
    Copyright (C) 2000 David Faure <faure@kde.org>, Alexander Neundorf <neundorf@kde.org>

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

#include <qstringlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlistbox.h>
#include <qwhatsthis.h>
#include <qlabel.h>

#include <kdebug.h>
#include <kdialog.h>
#include <klineedit.h>
#include <klocale.h>
#include <kapp.h>
#include <knotifyclient.h>

#include "keditlistbox.h"

class KEditListBox::PrivateData
{
   public:
      bool m_checkAtEntering;
};

KEditListBox::KEditListBox(QWidget *parent, const char *name, bool checkAtEntering)
:QGroupBox(parent, name )
{
   d=new PrivateData;
   d->m_checkAtEntering=checkAtEntering;
   init();
}

KEditListBox::KEditListBox(const QString& title, QWidget *parent, const char *name, bool checkAtEntering)
:QGroupBox(title, parent, name )
{
   d=new PrivateData;
   d->m_checkAtEntering=checkAtEntering;
   init();
}

KEditListBox::~KEditListBox()
{
   delete d;
   d=0;
};

void KEditListBox::init()
{
   setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));

   QWidget * gb = this;
   QGridLayout * grid = new QGridLayout(gb, 7, 2, KDialog::marginHint(),
                                        KDialog::spacingHint());
   grid->addRowSpacing(0, fontMetrics().lineSpacing());
   grid->setRowStretch(1, 1);
   grid->setRowStretch(2, 1);
   grid->setRowStretch(3, 1);
   grid->setRowStretch(4, 1);
   grid->setRowStretch(5, 1);
   grid->setRowStretch(6, 1);
   grid->setMargin(15);

   m_lineEdit=new QLineEdit(gb);
   m_listBox = new QListBox(gb);

   servNewButton = new QPushButton(i18n("&Add..."), gb);
   servNewButton->setEnabled(false);

   servRemoveButton = new QPushButton(i18n("&Remove"), gb);
   servRemoveButton->setEnabled(false);

   servUpButton = new QPushButton(i18n("Move &Up"), gb);
   servUpButton->setEnabled(false);

   servDownButton = new QPushButton(i18n("Move &Down"), gb);
   servDownButton->setEnabled(false);

   grid->addMultiCellWidget(m_lineEdit,1,1,0,1);
   grid->addMultiCellWidget(m_listBox, 2, 6, 0, 0);
   grid->addWidget(servNewButton, 2, 1);
   grid->addWidget(servRemoveButton, 3, 1);
   grid->addWidget(servUpButton, 4, 1);
   grid->addWidget(servDownButton, 5, 1);

   connect(m_lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(enableAddButton(const QString&)));
   connect(m_listBox, SIGNAL(highlighted(int)), SLOT(enableMoveButtons(int)));
   connect(servNewButton, SIGNAL(clicked()), SLOT(addItem()));
   connect(servRemoveButton, SIGNAL(clicked()), SLOT(removeItem()));
   connect(servUpButton, SIGNAL(clicked()), SLOT(moveItemUp()));
   connect(servDownButton, SIGNAL(clicked()), SLOT(moveItemDown()));
};

void KEditListBox::enableAddButton(const QString& text)
{
   if (!d->m_checkAtEntering)
      servNewButton->setEnabled(!text.isEmpty());
   else
   {
      if (text.isEmpty())
      {
         servNewButton->setEnabled(false);
      }
      else
      {
         const QString& currentText=m_lineEdit->text();
         for (int i=0; i<count(); i++)
         {
            if (m_listBox->text(i)==currentText)
            {
               servNewButton->setEnabled(false);
               return;
            };
         };
         servNewButton->setEnabled(true);
      };
   };
};

void KEditListBox::moveItemUp()
{
   if (!m_listBox->isEnabled())
   {
      KNotifyClient::beep();
      return;
   }

   unsigned int selIndex = m_listBox->currentItem();
   if (selIndex == 0)
   {
      KNotifyClient::beep();
      return;
   }

   QListBoxItem *selItem = m_listBox->item(selIndex);
   m_listBox->takeItem(selItem);
   m_listBox->insertItem(selItem, selIndex-1);
   m_listBox->setCurrentItem(selIndex - 1);

   emit changed();
}

void KEditListBox::moveItemDown()
{
   if (!m_listBox->isEnabled())
   {
      KNotifyClient::beep();
      return;
   }

   unsigned int selIndex = m_listBox->currentItem();
   if (selIndex == m_listBox->count() - 1)
   {
      KNotifyClient::beep();
      return;
   }

   QListBoxItem *selItem = m_listBox->item(selIndex);
   m_listBox->takeItem(selItem);
   m_listBox->insertItem(selItem, selIndex+1);
   m_listBox->setCurrentItem(selIndex + 1);

   emit changed();
}

void KEditListBox::addItem()
{
   const QString& currentTextLE=m_lineEdit->text();
   bool alreadyInList(false);
   //if we didn't check for dupes at the inserting we have to do it now
   if (!d->m_checkAtEntering)
   {
      for (int i=0; i<count(); i++)
      {
         if (m_listBox->text(i)==currentTextLE)
         {
            alreadyInList=true;
            break;
         };
      };
   };

   servNewButton->setEnabled(false);
   if (!alreadyInList)
   {
      m_listBox->insertItem(currentTextLE);
      emit changed();
   };
}

void KEditListBox::removeItem()
{
   int selected = m_listBox->currentItem();

   if ( selected >= 0 )
   {
      m_listBox->removeItem( selected );
      emit changed();
   }

   if ( servRemoveButton && m_listBox->currentItem() == -1 )
      servRemoveButton->setEnabled(false);
}

void KEditListBox::enableMoveButtons(int index)
{
   if (m_listBox->count() <= 1)
   {
      servUpButton->setEnabled(false);
      servDownButton->setEnabled(false);
   }
   else if ((uint) index == (m_listBox->count() - 1))
   {
      servUpButton->setEnabled(true);
      servDownButton->setEnabled(false);
   }
   else if (index == 0)
   {
      servUpButton->setEnabled(false);
      servDownButton->setEnabled(true);
   }
   else
   {
      servUpButton->setEnabled(true);
      servDownButton->setEnabled(true);
   }

   if ( servRemoveButton )
      servRemoveButton->setEnabled(true);
}

void KEditListBox::clear()
{
   m_lineEdit->clear();
   m_listBox->clear();
   emit changed();
};

void KEditListBox::insertStringList(const QStringList& list, int index)
{
   m_listBox->insertStringList(list,index);
};

void KEditListBox::insertStrList(const QStrList* list, int index)
{
   m_listBox->insertStrList(list,index);
};

void KEditListBox::insertStrList(const QStrList& list, int index)
{
   m_listBox->insertStrList(list,index);
};

void KEditListBox::insertStrList(const char ** list, int numStrings, int index)
{
   m_listBox->insertStrList(list,numStrings,index);
};

#include "keditlistbox.moc"
