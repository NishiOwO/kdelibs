/* This file is part of the KDE libraries
    Copyright (C) 1999 Ilya Baran (ibaran@mit.edu)

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

#include "MatrixDialog.h"
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlayout.h>

#define DEFAULT_SIZE 3
#define MAX_SIZE     20

//dialog constructor--make separate file!
MatrixDialog::MatrixDialog(QWidget *parent)
  : QDialog(parent, "Matrix Dialog", TRUE)
{
  w = h = DEFAULT_SIZE;

  zeroFill = FALSE;

  QPushButton *ok, *cancel;
  QLabel *rows, *columns;

  QGridLayout *grid = new QGridLayout(this, 4, 2, 10);

  rows = new QLabel("Rows:", this);
  columns = new QLabel("Columns:", this);

  grid->addWidget(rows, 0, 0);
  grid->addWidget(columns, 0, 1);

  QSpinBox *width, *height;

  height = new QSpinBox(1, MAX_SIZE, 1, this);
  grid->addWidget(height, 1, 0);
  height->setValue(DEFAULT_SIZE);
  connect(height, SIGNAL(valueChanged(int)), SLOT(setHeight(int)));

  width = new QSpinBox(1, MAX_SIZE, 1, this);
  grid->addWidget(width, 1, 1);
  width->setValue(DEFAULT_SIZE);
  connect(width, SIGNAL(valueChanged(int)), SLOT(setWidth(int)));

  QCheckBox *fill = new QCheckBox("Zero-Fill", this);
  grid->addMultiCellWidget(fill, 2, 2, 0, 1);
  connect(fill, SIGNAL(toggled(bool)), SLOT(setFill(bool)));

  ok = new QPushButton("OK", this);
  ok->setDefault(TRUE);
  grid->addWidget(ok, 3, 0);
  connect(ok, SIGNAL(clicked()), SLOT(accept()));
  
  cancel = new QPushButton("Cancel", this);
  grid->addWidget(cancel, 3, 1);
  connect(cancel, SIGNAL(clicked()), SLOT(reject()));

  height->setFocus();
}

void MatrixDialog::setHeight(int value)
{
  h = value;
}

void MatrixDialog::setWidth(int value)
{
  w = value;
}

void MatrixDialog::setFill(bool value)
{
  zeroFill = value;
}

#include "MatrixDialog.moc"
