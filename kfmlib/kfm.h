#ifndef kfm_h
#define kfm_h

#include <qobject.h>
#include <qdialog.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qlabel.h>

#include "kfmclient_ipc.h"

class KFM : public QObject
{
    Q_OBJECT
public:
    KFM();
    ~KFM();
    
    bool isOK() { return ok; }

    void refreshDesktop();
    void openURL();
    void openURL( const char *_url );
    void refreshDirectory( const char *_url );
    void openProperties( const char *_url );
    void exec( const char *_url, const char *_binding );
    void copy( const char *_src, const char *_dest );
    void move( const char *_src, const char *_dest );    

signals:    
    void finished();
    
public slots:
    void slotFinished();
    
protected:
    void init();
    
    char flag;
    bool ok;

    KfmIpc *ipc;
};

/// Asking for a location
/**
  This class can be used to ask for a new filename or for
  an URL.
  */
class DlgLocation : public QDialog
{
    Q_OBJECT
public:
    /// Constructor
    /**
      Create a dialog that asks for a single line of text. _value is the initial
      value of the line. _text appears as label on top of the entry box.
      */
    DlgLocation( const char *_text, const char *_value, QWidget *parent = 0L );

    /// Return the value the user entered
    const char * getText() { return edit->text(); }
    
protected:
    /// The line edit widget
    QLineEdit *edit;
};

#endif
