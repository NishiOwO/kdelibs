/* This file is part of the KDE libraries

   Copyright (C) 1997 Sven Radej (sven.radej@iname.com)
   Copyright (c) 1999 Patrick Ward <PAT_WARD@HP-USA-om5.om.hp.com>
   Copyright (c) 1999 Preston Brown <pbrown@kde.org>
   Copyright (c) 1999 Dawit Alemayehu <adawit@earthlink.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <qclipboard.h>

#include <klocale.h>
#include <kstdaccel.h>

#include "klineedit.h"
#include "klineedit.moc"


KLineEdit::KLineEdit( const QString &string,
                      QWidget *parent,
                      const char *name,
                      bool showMenu,
                      bool showChanger ) : QLineEdit (string, parent, name)
{
    initialize( showMenu, showChanger );
}

KLineEdit::KLineEdit( QWidget *parent,
                      const char *name,
                      bool showMenu,
                      bool showChanger ) : QLineEdit (parent, name)
{
    initialize( showMenu, showChanger );
}

KLineEdit::~KLineEdit ()
{
    delete m_pContextMenu;
    delete m_pSubMenu;
    if( m_bAutoDelCompObj )
        delete m_pCompObj;
}

void KLineEdit::setCompletionObject ( KCompletion* obj, bool autoDelete )
{
    m_pCompObj = obj;
    m_bAutoDelCompObj = autoDelete;
    // By default makes this widget handle rotation signals.
    setHandleRotationSignals();
}

void KLineEdit::disableCompletion()
{
    setHandleRotationSignals( false );
    delete m_pCompObj;
    m_pCompObj = 0;
}

void KLineEdit::enableCompletion( bool autoDelete )
{
    if( m_pCompObj == 0 )
    {
        setCompletionObject( new KCompletion(), autoDelete );
        setCompletionMode( m_iCompletionMode );  // forces a completion mode sync w/ KCompletion.
    }
    m_bAutoDelCompObj = autoDelete;
}

void KLineEdit::setHandleRotationSignals( bool autoHandle )
{
    if( m_pCompObj != 0 )
    {
        if( autoHandle && !m_bHandleRotationSignals )
        {
            connect( this, SIGNAL( rotateUp() ), this, SLOT( iterateUpInList() ) );
            connect( this, SIGNAL( rotateDown() ), this, SLOT( iterateDownInList() ) );
            m_bHandleRotationSignals = true;
        }
        else if( !autoHandle && m_bHandleRotationSignals )
        {
            disconnect( this, SIGNAL( rotateUp() ), this, SLOT( iterateUpInList() ) );
            disconnect( this, SIGNAL( rotateDown() ), this, SLOT( iterateDownInList() ) );
            m_bHandleRotationSignals = false;
        }
    }
}

void KLineEdit::setEnabledModeChanger( bool showChanger )
{
    if ( !showChanger )
    {
        if( m_pSubMenu != 0 )
        {
            disconnect( m_pContextMenu, SIGNAL( highlighted( int ) ), this, SLOT( aboutToShowSubMenu( int ) ) );
            delete m_pSubMenu;
            m_pSubMenu = 0;
        }
    }
    m_bShowModeChanger = showChanger;
}

void KLineEdit::setEnabledContextMenu( bool showMenu )
{
    if( showMenu )
    {
        if( m_pContextMenu == 0 )
        {
            m_pContextMenu = new QPopupMenu();
            connect( m_pContextMenu, SIGNAL(aboutToShow()), this, SLOT( aboutToShowMenu() ) );
        }
    }
    else
    {
        if( m_pContextMenu != 0 )
        {
            disconnect ( m_pContextMenu, SIGNAL(aboutToShow()), this, SLOT( aboutToShowMenu() ) );
            if( m_pSubMenu != 0 )
            {
                disconnect( m_pContextMenu, SIGNAL( highlighted( int ) ), this, SLOT( aboutToShowSubMenu( int ) ) );
                delete m_pSubMenu;
                m_pSubMenu = 0;
            }
            delete m_pContextMenu;
            m_pContextMenu = 0;
        }
    }
    m_bShowContextMenu = showMenu;
}

void KLineEdit::useGlobalSettings()
{
    m_iCompletionKey = 0;
    m_iRotateUpKey = 0;
    m_iRotateDnKey = 0;
}

void KLineEdit::setCompletionMode( KGlobal::Completion mode )
{
    // If the widgets echo mode is not Normal, no completion
    // feature will be enabled even if one is requested.
    if ( echoMode() != QLineEdit::Normal )
        mode = KGlobal::CompletionNone; // override the request.

    m_iCompletionMode = mode;

    // Always sync up KCompletion mode with ours as long as we
    // are performing completions.
    if( m_pCompObj != 0 && m_iCompletionMode != KGlobal::CompletionNone )
    {
        m_pCompObj->setCompletionMode( m_iCompletionMode );
    }
}

bool KLineEdit::setCompletionKey( int ckey )
{
    if( ckey == 0 || (ckey > 0 && ckey != m_iRotateUpKey && ckey != m_iRotateDnKey) )
    {
        m_iCompletionKey = ckey;
        return true;
    }
    return false;
}

bool KLineEdit::setRotateUpKey( int rUpKey )
{
    if( rUpKey == 0 || (rUpKey > 0 && rUpKey != m_iRotateDnKey && rUpKey != m_iCompletionKey) )
    {
        m_iRotateUpKey = rUpKey;
        return true;
    }
    return false;
}

bool KLineEdit::setRotateDownKey( int rDnKey )
{
    if ( rDnKey == 0 || (rDnKey > 0 && rDnKey != m_iRotateUpKey && rDnKey != m_iCompletionKey) )
    {
        m_iRotateDnKey = rDnKey;
        return true;
    }
    return false;
}

void KLineEdit::aboutToShowSubMenu( int itemID )
{
    if( itemID == m_iSubMenuId )
    {
        int id;
        m_pSubMenu->clear();
        id = m_pSubMenu->insertItem( i18n("None"), this, SLOT( modeNone()) );
        m_pSubMenu->setItemChecked(id, m_iCompletionMode == KGlobal::CompletionNone );
        id = m_pSubMenu->insertItem( i18n("Manual"), this, SLOT( modeShell()) );
        m_pSubMenu->setItemChecked(id, m_iCompletionMode == KGlobal::CompletionShell );
        id = m_pSubMenu->insertItem( i18n("Automatic"), this, SLOT( modeAuto()) );
        m_pSubMenu->setItemChecked(id, m_iCompletionMode == KGlobal::CompletionAuto );
        id = m_pSubMenu->insertItem( i18n("Semi-Automatic"), this, SLOT( modeManual()) );
        m_pSubMenu->setItemChecked(id, m_iCompletionMode == KGlobal::CompletionMan );
    }
}

void KLineEdit::aboutToShowMenu()
{
    int id;
    bool isNotEmpty = ( text().length() != 0 );
    bool isNormal = (echoMode() == QLineEdit::Normal);
    m_pContextMenu->clear();
    id = m_pContextMenu->insertItem( i18n("Cut"), this, SLOT(slotCut()) );
    m_pContextMenu->setItemEnabled( id, hasMarkedText() && isNormal );
    id = m_pContextMenu->insertItem( i18n("Copy"), this, SLOT(slotCopy()) );
    m_pContextMenu->setItemEnabled( id, hasMarkedText() && isNormal );
    id = m_pContextMenu->insertItem( i18n("Paste"), this, SLOT(slotPaste()) );
    m_pContextMenu->setItemEnabled( id, QApplication::clipboard()->text().length() != 0 );
    id = m_pContextMenu->insertItem( i18n("Clear"), this, SLOT(clear()) );
    m_pContextMenu->setItemEnabled( id, isNotEmpty );
    m_pContextMenu->insertSeparator();
    if( m_bShowModeChanger && m_pCompObj != 0 )
    {
        if( m_pSubMenu == 0 )
            m_pSubMenu = new QPopupMenu();
        // Dummy place holder so that "-->" is shown !!!
        m_pSubMenu->insertItem( QString::null );
        m_iSubMenuId = m_pContextMenu->insertItem( i18n("Completion Mode"), m_pSubMenu );
        m_pContextMenu->insertSeparator();
        connect( m_pContextMenu, SIGNAL( highlighted( int ) ), this, SLOT( aboutToShowSubMenu( int ) ) );
    }
    id = m_pContextMenu->insertItem( i18n("Select All"), this, SLOT( selectAll() ) );
    m_pContextMenu->setItemEnabled( id, isNotEmpty );
    id = m_pContextMenu->insertItem( i18n("Unselect"), this, SLOT( deselect() ) );
    m_pContextMenu->setItemEnabled( id, hasMarkedText() );
}

void KLineEdit::rotateText( const QString& input )
{
    if( input.length() == 0 )
        return;

    if( m_iCompletionMode != KGlobal::CompletionNone &&
	    m_pCompObj != 0 && m_pCompObj->hasMultipleMatches() )
    {
        if( m_iCompletionMode == KGlobal::CompletionShell )
        {
            setText( input );
        }
        else
        {
            int pos = cursorPosition();
            int len = input.length();
            validateAndSet( input, pos, pos, len );
            m_iPrevlen = len;
            m_iPrevpos = pos;
        }
    }
}

void KLineEdit::iterateUpInList()
{
    if( m_pCompObj != 0 )
        rotateText( m_pCompObj->previousMatch() );
}

void KLineEdit::iterateDownInList()
{
    if( m_pCompObj != 0 )
        rotateText( m_pCompObj->nextMatch() );
}

void KLineEdit::entryChanged( const QString& text )
{
    if( m_iCompletionMode == KGlobal::CompletionAuto &&
        echoMode() == QLineEdit::Normal )
    {
        int pos = cursorPosition();
        int len = text.length();
        if( pos > m_iPrevpos && len >= m_iPrevlen )
            emit completion( text );
        m_iPrevpos = pos;
        m_iPrevlen = len;
    }
}

void KLineEdit::makeCompletion( const QString& text )
{
    if( m_pCompObj != 0 )
    {
        QString match = m_pCompObj->makeCompletion( text );

        // If no match or the same match, simply return
        // without completing.
        if( match.length() == 0 || match == text )
            return;

        if ( m_iCompletionMode == KGlobal::CompletionShell )
        {
            setText( match );
        }
        else
        {
            m_iPrevlen = match.length();
            m_iPrevpos = cursorPosition();
            validateAndSet( match, m_iPrevpos, m_iPrevpos, m_iPrevlen );
        }
    }
}

void KLineEdit::initialize( bool showMenu, bool showChanger )
{
    // Determines whether the completion object should
    // be deleted or not.
    m_bAutoDelCompObj = false;

    // Determines whether the rotation keys signals are
    // being handled by this widget automatically
    m_bHandleRotationSignals = false;

    // Initialize all key-bindings to 0 by default so that
    // the event filter will use the global settings.
    m_iCompletionKey = 0;
    m_iRotateUpKey = 0;
    m_iRotateDnKey = 0;

    // Initalize Variables used in auto-completion mode.
    // These values greatly simplify the logic used to
    // determine whether completion is needed when the
    // widget is in in auto-completion mode.
    m_iPrevpos = 0; // keeps cursor position whenever it changes.
    m_iPrevlen = 0; // keeps length of text as it changes.

    // Initialize the context Menu.  By default the popup
    // menu as well as the mode switching entry are enabled.
    m_pContextMenu = 0;
    m_pSubMenu = 0;
    setEnabledContextMenu( showMenu );
    setEnabledModeChanger( showChanger );

    // Assign the default completion type to use.
    m_iCompletionMode = KGlobal::completionMode();

    // Initialize the pointer to the completion object.
    m_pCompObj = 0;

    // Connect the signals and slots.
    connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( entryChanged( const QString& ) ) );
    connect( this, SIGNAL( completion( const QString& ) ), this, SLOT( makeCompletion( const QString& ) ) );
    connect( this, SIGNAL( returnPressed() ), this, SLOT( slotReturnPressed() ) );
}

void KLineEdit::slotReturnPressed()
{
    // DO NOT EMIT AT ALL if the widget is not
    // in normal
    if( echoMode() == QLineEdit::Normal ||
        echoMode() == QLineEdit::NoEcho )
        emit returnPressed( displayText() );
}

void KLineEdit::keyPressEvent( QKeyEvent *ev )
{
    // Filter key-events if EchoMode is normal and
    // the completion mode is not CompletionNone.
    if(  m_iCompletionMode != KGlobal::CompletionNone &&
         echoMode() == QLineEdit::Normal )
    {
        // Handles completion.
        int key = ( m_iCompletionKey == 0 ) ? KStdAccel::completion() : m_iCompletionKey;
        if( KStdAccel::isEqual( ev, key ) )
        {
            // Emit completion if the completion mode is NOT
            // CompletionAuto and if the mode is CompletionShell,
            // the cursor is at the end of the string.
            int len = text().length();
            if( m_iCompletionMode == KGlobal::CompletionMan ||
                (m_iCompletionMode == KGlobal::CompletionShell &&
                len != 0 && len == cursorPosition() ) )
            {
                emit completion( text() );
                ev->accept(); // oh how I hate event filtering!!! - Remove at your own risk!!!
                return;
            }
        }
        // Handles rotateUp.
        key = ( m_iRotateUpKey == 0 ) ? KStdAccel::rotateUp() : m_iRotateUpKey;
        if( KStdAccel::isEqual( ev, key ) )
        {
            emit rotateUp ();
            ev->accept();
            return;
        }
        // Handles rotateDown.
        key = ( m_iRotateDnKey == 0 ) ? KStdAccel::rotateDown() : m_iRotateDnKey;
        if( KStdAccel::isEqual( ev, key ) )
        {
            emit rotateDown();
            ev->accept();
            return;
        }
    }
    // Let QLineEdit handle the other keys events.
    QLineEdit::keyPressEvent ( ev );
    // Always update the position holder if the user
    // pressed the END key in auto completion mode.
    if( m_iCompletionMode == KGlobal::CompletionAuto )
    {
        int pos = cursorPosition();
        int len = text().length();
        if( m_iPrevpos != pos && pos == len )
            m_iPrevpos = pos;
    }
}

void KLineEdit::mousePressEvent( QMouseEvent *ev )
{
    if( ev->button() == Qt::RightButton )
    {
        if( m_bShowContextMenu )
            m_pContextMenu->popup ( QCursor::pos() );
        return;
    }
    QLineEdit::mousePressEvent( ev );
}
