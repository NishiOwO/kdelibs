#include <qtimer.h>
#include <qheader.h>

#include <kglobalsettings.h>
#include <kcursor.h>
#include <kapp.h>
#include <kipc.h>
#include <kdebug.h>

#include "klistview.h"

#include <X11/Xlib.h>

KListView::KListView( QWidget *parent, const char *name )
    : QListView( parent, name )
{
    oldCursor = viewport()->cursor();
    connect( this, SIGNAL( onViewport() ),
	     this, SLOT( slotOnViewport() ) );
    connect( this, SIGNAL( onItem( QListViewItem * ) ),
	     this, SLOT( slotOnItem( QListViewItem * ) ) );

    slotSettingsChanged(KApplication::SETTINGS_MOUSE);
    connect( kapp, SIGNAL( settingsChanged(int) ), SLOT( slotSettingsChanged(int) ) );
    kapp->addKipcEventMask( KIPC::SettingsChanged );

    m_pCurrentItem = 0L;

    m_pAutoSelect = new QTimer( this );
    connect( m_pAutoSelect, SIGNAL( timeout() ),
    	     this, SLOT( slotAutoSelect() ) );
}

bool KListView::isExecuteArea( const QPoint& point )
{
   if ( itemAt( point ) )
   {
     if( allColumnsShowFocus() )
       return true;
     else {

      int x = point.x();
      int pos = header()->mapToActual( 0 );
      int offset = 0;
      int width = columnWidth( pos );

      for ( int index = 0; index < pos; index++ )
         offset += columnWidth( index );

      return ( x > offset && x < ( offset + width ) );
     }
   }
   return false;
}

void KListView::slotOnItem( QListViewItem *item )
{
  if ( item && (m_autoSelectDelay > -1) && m_bUseSingle ) {
    m_pAutoSelect->start( m_autoSelectDelay, true );
    m_pCurrentItem = item;
  }
}

void KListView::slotOnViewport()
{
  if ( m_bChangeCursorOverItem )
    viewport()->setCursor( oldCursor );
  
  m_pAutoSelect->stop();
  m_pCurrentItem = 0L;
}

void KListView::slotSettingsChanged(int category)
{
    if (category != KApplication::SETTINGS_MOUSE)
        return;
    m_bUseSingle = KGlobalSettings::singleClick();

    disconnect( this, SIGNAL( mouseButtonClicked( int, QListViewItem *, 
						  const QPoint &, int ) ),
		this, SLOT( slotMouseButtonClicked( int, QListViewItem *, 
						    const QPoint &, int ) ) );
//       disconnect( this, SIGNAL( doubleClicked( QListViewItem *, 
// 					       const QPoint &, int ) ),
// 		  this, SLOT( slotExecute( QListViewItem *, 
// 					   const QPoint &, int ) ) );

    if( m_bUseSingle )
    {
      connect( this, SIGNAL( mouseButtonClicked( int, QListViewItem *, 
						 const QPoint &, int ) ),
	       this, SLOT( slotMouseButtonClicked( int, QListViewItem *, 
						   const QPoint &, int ) ) );
    }
    else
    {
//       connect( this, SIGNAL( doubleClicked( QListViewItem *, 
// 					    const QPoint &, int ) ),
// 	       this, SLOT( slotExecute( QListViewItem *, 
// 					const QPoint &, int ) ) );
    }

    m_bChangeCursorOverItem = KGlobalSettings::changeCursorOverIcon();
    m_autoSelectDelay = KGlobalSettings::autoSelectDelay();

    if( !m_bUseSingle || !m_bChangeCursorOverItem )
	viewport()->setCursor( oldCursor );
}

void KListView::slotAutoSelect()
{
  //Give this widget the keyboard focus.
  if( !hasFocus() )
    setFocus();

  Window root;
  Window child;
  int root_x, root_y, win_x, win_y;
  uint keybstate;
  XQueryPointer( qt_xdisplay(), qt_xrootwin(), &root, &child,
		 &root_x, &root_y, &win_x, &win_y, &keybstate );

  QListViewItem* previousItem = currentItem();
  setCurrentItem( m_pCurrentItem );

  if( m_pCurrentItem ) {
    //Shift pressed?
    if( (keybstate & ShiftMask) ) {
      bool block = signalsBlocked();
      blockSignals( true );

      //No Ctrl? Then clear before!
      if( !(keybstate & ControlMask) )  
	clearSelection(); 

      bool select = !m_pCurrentItem->isSelected();
      bool update = viewport()->isUpdatesEnabled();
      viewport()->setUpdatesEnabled( false );

      bool down = previousItem->itemPos() < m_pCurrentItem->itemPos();
      QListViewItemIterator lit( down ? previousItem : m_pCurrentItem );
      for ( ; lit.current(); ++lit ) {
	if ( down && lit.current() == m_pCurrentItem ) {
	  m_pCurrentItem->setSelected( select );
	  break;
	}
	if ( !down && lit.current() == previousItem ) {
	  previousItem->setSelected( select );
	  break;
	}
	lit.current()->setSelected( select );
      }
      
      blockSignals( block );
      viewport()->setUpdatesEnabled( update );
      triggerUpdate();

      emit selectionChanged();

      if( selectionMode() == QListView::Single )
	emit selectionChanged( m_pCurrentItem );
    }
    else if( (keybstate & ControlMask) )
      setSelected( m_pCurrentItem, !m_pCurrentItem->isSelected() );
    else {
      bool block = signalsBlocked();
      blockSignals( true );

      if( !m_pCurrentItem->isSelected() )
	clearSelection(); 

      blockSignals( block );

      setSelected( m_pCurrentItem, true );
    }
  }
  else
    kdDebug() << "That�s not supposed to happen!!!!" << endl;
}

void KListView::emitExecute( QListViewItem *item, const QPoint &pos, int c )
{
  if( isExecuteArea( viewport()->mapFromGlobal(pos) ) ) {

    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint keybstate;
    XQueryPointer( qt_xdisplay(), qt_xrootwin(), &root, &child,
		   &root_x, &root_y, &win_x, &win_y, &keybstate );
    
    m_pAutoSelect->stop();
  
    //Don�t emit executed if in SC mode and Shift or Ctrl are pressed
    if( !( m_bUseSingle && ((keybstate & ShiftMask) || (keybstate & ControlMask)) ) ) {
      emit executed( item );
      emit executed( item, pos, c );
    }
  }
}

void KListView::focusOutEvent( QFocusEvent *fe )
{
  m_pAutoSelect->stop();

  QListView::focusOutEvent( fe );
}

void KListView::leaveEvent( QEvent *e ) 
{
  m_pAutoSelect->stop();

  QListView::leaveEvent( e );
}

void KListView::contentsMousePressEvent( QMouseEvent *e )
{
  if( (selectionMode() == Extended) && (e->state() & ShiftButton) && !(e->state() & ControlButton) ) {
    bool block = signalsBlocked();
    blockSignals( true );

    clearSelection();

    blockSignals( block );
  }

  QListView::contentsMousePressEvent( e );
}

void KListView::contentsMouseMoveEvent( QMouseEvent *e )
{
  QPoint vp = contentsToViewport(e->pos());
  QListViewItem *item = itemAt( vp );
  
  //do we process cursor changes at all?
  if ( item && m_bChangeCursorOverItem && m_bUseSingle ) {
    //Cursor moved on a new item or in/out the execute area
    if( (item != m_pCurrentItem) || 
	(isExecuteArea(vp) != m_cursorInExecuteArea) ) {

      m_cursorInExecuteArea = isExecuteArea(vp);

      if( m_cursorInExecuteArea ) //cursor moved in execute area
	viewport()->setCursor( KCursor().handCursor() );
      else //cursor moved out of execute area
	viewport()->setCursor( oldCursor );
    }
  }

  QListView::contentsMouseMoveEvent( e );
}

void KListView::contentsMouseDoubleClickEvent ( QMouseEvent *e )
{
  QListView::contentsMouseDoubleClickEvent( e );

  QPoint vp = contentsToViewport(e->pos());
  QListViewItem *item = itemAt( vp );
  int col = item ? header()->mapToLogical( header()->cellAt( vp.x() ) ) : -1;

  if( item ) {
    emit doubleClicked( item, e->globalPos(), col );

    if( (e->button() == LeftButton) && !m_bUseSingle )
      emitExecute( item, e->globalPos(), col );
  }
}

void KListView::slotMouseButtonClicked( int btn, QListViewItem *item, const QPoint &pos, int c )
{
  if( (btn == LeftButton) && item )
    emitExecute( item, pos, c );
}


