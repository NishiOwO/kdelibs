// $Id$

#include "kio_skip_dlg.h"

#include <stdio.h>
#include <assert.h>

#include <qmessagebox.h>

#include <kapp.h>
#include <klocale.h>
#include <kurl.h>

KIOSkipDlg::KIOSkipDlg(QWidget *parent, const char *_dest, bool _multi, QString _error_text, bool _modal ) :
  QDialog ( parent, "" , _modal )
{
  modal = _modal;
  
  b0 = b1 = b2 = 0L;
    
  setCaption( i18n( "Information" ) );

  b0 = new QPushButton( i18n( "Cancel" ), this );
  connect(b0, SIGNAL(clicked()), this, SLOT(b0Pressed()));
  
  b1 = new QPushButton( i18n( "Skip" ), this );
  connect(b1, SIGNAL(clicked()), this, SLOT(b1Pressed()));

  if ( _multi )
  {    
    b2 = new QPushButton( i18n( "Auto Skip" ), this );
    connect(b2, SIGNAL(clicked()), this, SLOT(b2Pressed()));
  }
  
  QVBoxLayout *vlayout = new QVBoxLayout( this, 10, 0 );
  // vlayout->addStrut( 360 );	makes dlg at least that wide
  
  QLabel *lb = new QLabel( i18n("Can not write"), this );
  lb->setFixedHeight( lb->sizeHint().height() );
  lb->setMinimumWidth( lb->sizeHint().width() );
  vlayout->addWidget( lb );
  vlayout->addSpacing( 5 );

  lb = new QLabel( _dest, this );
  lb->setFixedHeight( lb->sizeHint().height() );
  lb->setMinimumWidth( lb->sizeHint().width() );
  vlayout->addWidget( lb );

  lb = new QLabel( _error_text, this );
  lb->setFixedHeight( lb->sizeHint().height() );
  lb->setMinimumWidth( lb->sizeHint().width() );
  vlayout->addWidget( lb );

  vlayout->addSpacing( 10 );
    
  QHBoxLayout* layout = new QHBoxLayout();
  vlayout->addLayout( layout );
  if ( b0 )
  {    
    b0->setDefault( true );
    b0->setFixedSize( b0->sizeHint() );
    layout->addWidget( b0 );
    layout->addSpacing( 5 );
  }
  if ( b1 )
  {    
    b1->setFixedSize( b1->sizeHint() );
    layout->addWidget( b1 );
    layout->addSpacing( 5 );
  }
  if ( b2 )
  {    
    b2->setFixedSize( b2->sizeHint() );
    layout->addWidget( b2 );
    layout->addSpacing( 5 );
  }
  
  vlayout->addStretch( 10 );
  vlayout->activate();
  resize( sizeHint() );
}

KIOSkipDlg::~KIOSkipDlg()
{
}

void KIOSkipDlg::b0Pressed()
{
  if ( modal )
    done( 0 );
  else
    emit result( this, 0 );
}

void KIOSkipDlg::b1Pressed()
{
  if ( modal )
    done( 1 );
  else
    emit result( this, 1 );
}

void KIOSkipDlg::b2Pressed()
{
  if ( modal )
    done( 2 );
  else
    emit result( this, 2 );
}

SkipDlg_Result open_SkipDlg( const char *_dest, bool _multi, QString _error_text )
{
  if ( kapp == 0L )
  {
    (void)new KApplication( );
  }
  
  QString tmps = _error_text;
  if ( tmps.isEmpty() ) {
    tmps = i18n("Permission denied!");
  }
  KIOSkipDlg dlg( 0L, _dest, _multi, tmps, true );
  int i = dlg.exec();
  return (SkipDlg_Result)i;
}

#include "kio_skip_dlg.moc"
