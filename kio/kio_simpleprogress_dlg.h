#ifndef __kio_simpleprogress_dlg_h__
#define __kio_simpleprogress_dlg_h__

#include <qwidget.h>
#include <qlabel.h>

#include <kprogress.h>

class KIOJob;

class KIOSimpleProgressDlg : public QWidget {

  Q_OBJECT

public:
  KIOSimpleProgressDlg( KIOJob*, bool m_bStartIconified = false );
  ~KIOSimpleProgressDlg() {}
  
public slots:

  void slotTotalSize( int, unsigned long _bytes );
  void slotTotalFiles( int, unsigned long _files );
  void slotTotalDirs( int, unsigned long _dirs );
  void slotPercent( int, unsigned long _bytes );
  void slotProcessedSize( int, unsigned long _bytes );
  void slotProcessedFiles( int, unsigned long _files );
  void slotProcessedDirs( int, unsigned long _dirs );
  void slotScanningDir( int, const char *_dir );
  void slotSpeed( int, unsigned long _bytes_per_second );
  void slotCopyingFile( int, const char *_from, const char *_to );
  void slotMakingDir( int, const char *_dir );
  void slotGettingFile( int, const char *_url );
  void slotDeletingFile( int, const char *_url );
  void slotCanResume( int, bool );

protected slots:
  void quit();

protected:
  void closeEvent( QCloseEvent * );

  QLabel* sourceLabel;
  QLabel* destLabel;
  QLabel* speedLabel;
  QLabel* sizeLabel;
  QLabel* resumeLabel;

  KProgress* m_pProgressBar;

  unsigned long m_iTotalSize;
  unsigned long m_iTotalFiles;
  unsigned long m_iTotalDirs;

  KIOJob* m_pJob;
  
  enum ModeType { Copy, Delete, Create, Scan, Fetch };
  ModeType mode;
};

#endif
