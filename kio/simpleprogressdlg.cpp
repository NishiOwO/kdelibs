// $Id$

#include "kio_job.h"
#include <qpushbutton.h>
#include <qtimer.h>
#include <qlayout.h>

#include <kapp.h>
#include <kdialog.h>
#include <klocale.h>
#include <kwm.h>

#include "kio_simpleprogress_dlg.h"


KIOSimpleProgressDlg::KIOSimpleProgressDlg( KIOJob* _job, bool m_bStartIconified )
  : QWidget( 0L ) {

  m_pJob = _job;
  connect( m_pJob, SIGNAL( sigSpeed( int, unsigned long ) ),
	   SLOT( slotSpeed( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigTotalSize( int, unsigned long ) ),
	   SLOT( slotTotalSize( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigTotalFiles( int, unsigned long ) ),
	   SLOT( slotTotalFiles( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigTotalDirs( int, unsigned long ) ),
	   SLOT( slotTotalDirs( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigProcessedSize( int, unsigned long ) ),
	   SLOT( slotProcessedSize( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigPercent( int, unsigned long ) ),
	   SLOT( slotPercent( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigProcessedFiles( int, unsigned long ) ),
	   SLOT( slotProcessedFiles( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigProcessedDirs( int, unsigned long ) ),
	   SLOT( slotProcessedDirs( int, unsigned long ) ) );
  connect( m_pJob, SIGNAL( sigCopying( int, const char*, const char* ) ),
	   SLOT( slotCopyingFile( int, const char*, const char* ) ) );
  connect( m_pJob, SIGNAL( sigScanningDir( int, const char* ) ),
	   SLOT( slotScanningDir( int, const char* ) ) );
  connect( m_pJob, SIGNAL( sigMakingDir( int, const char* ) ),
	   SLOT( slotMakingDir( int, const char* ) ) );
  connect( m_pJob, SIGNAL( sigGettingFile( int, const char* ) ),
	   SLOT( slotGettingFile( int, const char* ) ) );
  connect( m_pJob, SIGNAL( sigDeletingFile( int, const char* ) ),
	   SLOT( slotDeletingFile( int, const char* ) ) );
  connect( m_pJob, SIGNAL( sigCanResume( int, bool ) ),
 	   SLOT( slotCanResume( int, bool ) ) );

  QVBoxLayout *topLayout = new QVBoxLayout( this, KDialog::marginHint(),
					    KDialog::spacingHint() );
  topLayout->addStrut( 360 );	// makes dlg at least that wide

  QGridLayout *grid = new QGridLayout(2, 3);
  topLayout->addLayout(grid);
  grid->setColStretch(2, 1);
  grid->addColSpacing(1, KDialog::spacingHint());
  // filenames or action name
  grid->addWidget(new QLabel(i18n("Source:"), this), 0, 0);
  
  sourceLabel = new QLabel(this);
  grid->addWidget(sourceLabel, 0, 2);

  grid->addWidget(new QLabel(i18n("Destination:"), this), 1, 0);

  destLabel = new QLabel(this);
  grid->addWidget(destLabel, 1, 2);
  
  topLayout->addSpacing( 10 );

  m_pProgressBar = new KProgress(0, 100, 0, KProgress::Horizontal, this);
  topLayout->addWidget( m_pProgressBar );

  // processed info
  QHBoxLayout *hBox = new QHBoxLayout();
  topLayout->addLayout(hBox);

  speedLabel = new QLabel(this);
  hBox->addWidget(speedLabel, 1);

  sizeLabel = new QLabel(this);
  hBox->addWidget(sizeLabel);
  
  resumeLabel = new QLabel(this);
  hBox->addWidget(resumeLabel);

  hBox = new QHBoxLayout();
  topLayout->addLayout(hBox);
  
  hBox->addStretch(1);

  QPushButton *pb = new QPushButton( i18n("Cancel"), this );
  connect( pb, SIGNAL( clicked() ), SLOT( quit() ) );
  hBox->addWidget( pb );

  resize( sizeHint() );

  // instead of showing immediately, we fire off a one shot timer to
  // show ourselves after 1.5 seconds.  This avoids massive window creation/
  // destruction on single file copies or other short operations.

  QTimer::singleShot(1500, this, SLOT(show()));

  if ( m_bStartIconified ) {
    KWM::setIconify( this->winId(), true );
  }
}


void KIOSimpleProgressDlg::closeEvent( QCloseEvent * )
{
  quit();
}


void KIOSimpleProgressDlg::quit() {
  if ( m_pJob ) {
    m_pJob->kill();
  }
}


void KIOSimpleProgressDlg::slotTotalSize( int, unsigned long _bytes ) 
{
  m_iTotalSize = _bytes;
}


void KIOSimpleProgressDlg::slotTotalFiles( int, unsigned long _files ) 
{
  m_iTotalFiles = _files;
}


void KIOSimpleProgressDlg::slotTotalDirs( int, unsigned long _dirs ) 
{
  m_iTotalDirs = _dirs;
}


void KIOSimpleProgressDlg::slotPercent( int, unsigned long _percent ) 
{
  QString tmp(i18n( "%1% of %2 ").arg( _percent ).arg( KIOJob::convertSize(m_iTotalSize)));
  m_pProgressBar->setValue( _percent );
  switch(mode) {
  case Copy:
    tmp.append(i18n(" (Deleting)"));
    break;
  case Delete:
    tmp.append(i18n(" (Deleting)"));
    break;
  case Create:
    tmp.append(i18n(" (Creating)"));
    break;
  case Scan:
    tmp.append(i18n(" (Scanning)"));
    break;
  case Fetch:
    tmp.append(i18n(" (Fetching)"));
    break;
  }

  setCaption( tmp );
}


void KIOSimpleProgressDlg::slotProcessedSize( int, unsigned long _bytes ) {
  QString tmp;

  tmp = i18n( "%1 of %2 ").arg( KIOJob::convertSize(_bytes) ).arg( KIOJob::convertSize(m_iTotalSize));
  sizeLabel->setText( tmp );
}


void KIOSimpleProgressDlg::slotProcessedDirs( int, unsigned long _dirs ) 
{
  sourceLabel->setText( i18n("%1/%2 directories created").arg( _dirs ).arg( m_iTotalDirs ) );
}


void KIOSimpleProgressDlg::slotProcessedFiles( int, unsigned long _files ) 
{
  sourceLabel->setText( i18n("%1/%2 files").arg( _files ).arg( m_iTotalFiles ) );
}


void KIOSimpleProgressDlg::slotSpeed( int, unsigned long _bytes_per_second ) 
{
  if ( _bytes_per_second == 0 ) {
    speedLabel->setText( i18n( "Stalled") );
  } else {
    speedLabel->setText( i18n( "%1/s %2").arg( KIOJob::convertSize( _bytes_per_second )).arg( m_pJob->getRemainingTime().toString()) );
  }
}


void KIOSimpleProgressDlg::slotScanningDir( int , const char *_dir) 
{
  setCaption(i18n("Scanning %1").arg( _dir ) );
  mode = Scan;
}


void KIOSimpleProgressDlg::slotCopyingFile( int, const char *_from, 
					    const char *_to ) 
{
  setCaption(i18n("Copy file(s) progress"));
  mode = Copy;
  sourceLabel->setText( _from );
  destLabel->setText( _to );
}


void KIOSimpleProgressDlg::slotMakingDir( int, const char *_dir ) 
{
  setCaption(i18n("Creating directory"));
  mode = Create;
  sourceLabel->setText( _dir );
}


void KIOSimpleProgressDlg::slotGettingFile( int, const char *_url ) 
{
  setCaption(i18n("Fetch file(s) progress"));
  mode = Fetch;
  sourceLabel->setText( _url );
}


void KIOSimpleProgressDlg::slotDeletingFile( int, const char *_url ) 
{
  setCaption(i18n("Delete file(s) progress"));
  mode = Delete;
  sourceLabel->setText( _url );
}


void KIOSimpleProgressDlg::slotCanResume( int, bool _resume )
{
  if ( _resume ) {
    resumeLabel->setText( i18n("Resumable") );
  } else {
    resumeLabel->setText( i18n("Not resumable") );
  }
}


#include "kio_simpleprogress_dlg.moc"
