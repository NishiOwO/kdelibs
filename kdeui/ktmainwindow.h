/*
    This file is part of the KDE libraries

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

    $Id$

*/

#ifndef _KWIDGET_H
#define _KWIDGET_H

#include <stdlib.h>
#include <qlist.h>
#include <qpopupmenu.h>
#include <qwidget.h>
#include <kstatusbar.h>
#include <ktoolbar.h>

class KConfig;
class KHelpMenu;
class KMenuBar;
class KTMLayout;

/**
 * Top level widget that provides toolbars, a status line and a frame.
 * It should be used as a toplevel (parent-less) widget and
 * manages the geometry for all its children, including your
 * main widget.
 *
 * Normally, you will inherit from KTMainWindow (known also as KTMW).
 * Then you must construct (or use some existing) widget that will be
 * your main view. You can set only one main view.
 *
 * You can add as many toolbar(s) as you like. There can be only one menubar
 * and only one statusbar.
 *
 * Toolbars, Menubar, and Statusbar can be generated by the
 * KTMainWindow and - unlike old KTMainWindow - may, but do not
 * have to be deleted by you. KTMainWindow will handle that internaly.
 *
 * Height and width can be operated independantly from each other. Simply
 * define the minimum/maximum height/width of your main widget and
 * KTMainWindow will take this into account. For fixed size windows set
 * your main widget to a fixed size.
 *
 * Fixed aspect ratios (heightForWidth()) and fixed width widgets are
 * not supported.
 *
 * KTMainWindow will set icon, mini icon and caption, which it gets
 * from KApplication. It provides full session management, and will save
 * its position, geometry and positions of toolbar(s) and menubar on
 * logout. If you want to save aditional data, overload saveProperties and
 * (to read them again on next login) readProperties. To save special
 * data about your data, overload saveData. To warn user
 * that application has unsaved data on logout, use setUnsavedData.
 *
 * There is also a macro RESTORE which can restore all your windows
 * on next login.
 *
 * This class is now merged with KTMainWindow (KTW). Biggest difference
 * is that KTMainWindow implements closeEvent and calls needed functios
 * in case when window is closed. By overloading these (virtual)
 * functions, you controll what happens in these cases.
 *
 * KTMainWindow might be replaced/extended in the future to
 * KMainWindow which will be a child of QMainWindow. Anyway,
 * the current interface  will be supported for compatibility
 * reasons.
 *
 * @see KApplication
 * @see KTMainWindow
 * @short KDE top level main window
   @author Stephan Kulow (coolo@kde.org), Matthias Ettrich (ettrich@kde.org), Chris Schlaeger (cs@kde.org), Sven Radej (radej@kde.org) .Maintained by Sven Radej (radej@kde.org)

 */

class KTMainWindow : public QWidget {
    Q_OBJECT

        friend class KToolBar;
        friend class KTLWSessionManaged;

public:
    /**
     * Constructor.
     * Note that for session management to work, KTMainWindow widget
     * must be created with 'new'.
     */
    KTMainWindow( const char *name = 0L, WFlags f= WDestructiveClose );
    /**
     * Destructor. Will also destroy the toolbars, and menubar if
     * needed.
     */
    ~KTMainWindow();

    /**
     * Add a toolbar to the widget.
     * A toolbar added to this widget will be automatically laid out
     * by it.
     *
     * The toolbar must have been created with this instance of
     * KTMainWindow as its parent.
     *
     * Usually you do not need this function. Just refer to a toolbar
     * with @ref #toolBar (index) instead and the KTMainWindow will
     * create it for you. Anyway addToolBar() is usefull if you want
     * to pass additional arguments to the toolbar's constructor.
     */
    int addToolBar( KToolBar *toolbar, int index = -1 );

    /**
     * Set the main client widget.
     * This is the main widget for your application; it's geometry
     * will be automatically managed by KTMainWindow to fit the
     * client area, constrained by the positions of the menu, toolbars
     * and status bar. It can be fixed-width or Y-fixed.
     *
     * Only one client widget can be handled at a time; multiple calls
     * of setView will cause only the last widget to be added to be
     * properly handled. The layout management will not start before this
	 * function has been called. It increases the application start
	 * speed to call this function after all bars have been registered. The
	 * presence of the view widget is mandatory for the class to operate.
     *
     * The widget must have been created with this instance of
     * KTMainWindow as its parent.
     */
    void setView( QWidget *view, bool show_frame = TRUE );

    QWidget *view() const { return kmainwidget; }

    /**
     *Enable or disable the status bar.
     */
    void enableStatusBar( KStatusBar::BarStatus stat = KStatusBar::Toggle );

    /**
     * Enable or disable the toolbar with the ID specified.
     * If no ID is specified, the default ID is 0.
     */
    void enableToolBar( KToolBar::BarStatus stat = KToolBar::Toggle,
                        int ID = 0 );

    /**
     * Set the width of the view frame.
     * If you request a frame around your view with @ref #setView (...,TRUE),
     * you can use this function to set the border width of the frame.
     * The default is 1 pixel. You should call this function before
     * @ref #setView ().
     */
    void setFrameBorderWidth( int );

	/**
	 * Set the maximum number of wraps for a single block of
	 * subsequent non-full-size tool bars. If more wraps would be
	 * necessary to properly layout the tool bars the bars will extend
	 * outside of the window. This behaviour is helpful when having
	 * many toolbars on small displays. Not all toolbars are
	 * accessible any longer but at least the main view keeps
	 * reasonably visible and is not squished by all the tool
	 * bars. Since the user cannot easyly distinguish between
	 * full-size and non full-size bars, they should not be mixed when
	 * using this function.  Technically there is no reason but it is
	 * very confusing when some bars automatically wrap (full-size
	 * bars) while other extend out of sight.  See @ref KTMLayout for
	 * more details. The toolbar wrapping limitation is disabled by
	 * default.
	 */
	void setMaximumToolBarWraps(unsigned int wraps);

    /**
     * Returns a pointer to the toolbar with the specified ID.
     * If there is no such tool bar yet, it will be generated
     */
    KToolBar *toolBar( int ID = 0 );

    /**
     * Returns a pointer to the menu bar. If there is no
     * menu bar yet, it will be generated
     */
    KMenuBar *menuBar();

    /**
     * If you constructed menuBar yourself, you must set it with this
     * function. You can use it also if you want to replace old menu bar
     * with a new one. There can be only one menu bar at a time. After this
     * function, layout will be updated.
     * @see #menuBar
     */
    void setMenu (KMenuBar *menuBar);

    /**
     * Returns a pointer to the status bar. If there is no
     * status bar yet, it will be generated
     */
    KStatusBar *statusBar();

    /**
     * If you constructed statusBar yourself, you must set it with this
     * function. You can use it also if you want to replace old status bar
     * with a new one. There can be only one status bar at a time. After this
     * function layout will be updated.
     * @see #statusBar
     */
    void setStatusBar (KStatusBar *statusBar);

    /**
     * You probably do not need this. Anyway, if you are porting code
     * which have been written for the former @ref KTMainWindow you may
     * find the following three boolean has-functions useful:
     *
     * This function returns wether the menubar is existing
     */
    bool hasMenuBar();

    /**
     * Returns wether the statusbar is existing
     */
    bool hasStatusBar();

    /**
     * Returns wether the specified toolbar  is existing
     */
    bool hasToolBar( int ID = 0);

    /**
     * Shows toplevel widget. Reimplemented from QWidget, and calls
     * @ref #updateRects (i.e. updates layout)
     */
    virtual void show ();

    /**
     * Returns the geometry of the main view widget. This function is provided
	 * for legacy reasons. Do not use it! It might be removed.
     */
    QRect mainViewGeometry() const;

    /**
     * This function tries to restore the toplevel widget as defined number (1..X)
     * If the session did not contain that high number, the configuration
     * is not changed and False returned.
     *
     * That means clients could simply do the following:
     * <pre>
     * if (kapp->isRestored()){
     *   int n = 1;
     *   while (KTMainWindow::canBeRestored(n)){
     *     (new childTLW)->restore(n);
     *     n++;
     *   }
     * } else {
     * // create default application as usual
     * }
     * </pre>
     * Note that "show()" is called implicit in restore.
     *
     * With this you can easily restore all toplevel windows of your
     * application.
     *
     * If your application uses different kinds of toplevel
     * windows, then you can use KTMainWindow::classNameOfToplevel(n)
     * to determine the exact type before calling the childTLW
     * constructor in the example from above.
     *
     * If your client has only one kind of toplevel widgets (which should
     * be pretty usual) then you should use the RESTORE-macro:
     *
     * <pre>
     * if (kapp->isRestored())
     *   RESTORE(childTLW)
     * else {
     * // create default application as usual
     * }
     * </pre>
     *
     * The macro expands to the term above but is easier to use and
     * less code to write.
     *
     * @see #restore
     * @see #classNameOfToplevel
     *
     */
    static bool canBeRestored(int number);

    /**
     * Returns the className of the numberth toplevel window which
     * should be restored. This is only usefull if you application uses
     * different kinds of toplevel windows.
     */
    static const QString classNameOfToplevel(int number);

    /**
     * Restores the specified number. Returns "False" if this
     * fails, otherwise returns "True" and shows the window
     * You should call @ref canBeRestored first.
     */
    bool restore(int number);

    /**
     * Get the standard help menu.
     *
     * @param aboutAppText User definable string that is used in the 
     *        appication specific dialog box. Note: The help menu will
     *        not open this dialog box if you don't define a string.
     *
     * @return A standard help menu.
     */
    QPopupMenu* helpMenu( const QString &aboutAppText=QString::null );

protected:
    /**
     * Default implementation calls @ref #updateRects if main widget
     * is resizable. If mainWidget is not resizable it does
     * nothing. You shouldn't need to override this function.
     */
    virtual void resizeEvent( QResizeEvent *e);

    /**
     * We need to trap the layout hint. Otherwise we will miss when our
     * view widget or some bar changes the size constrains on it's own.
     */
    virtual bool event(QEvent *);

    /**
     * Default implementation just calls repaint (FALSE); You may
     * reimplement this function if you want to.
     */
    virtual void focusInEvent ( QFocusEvent *);

    /**
     * Default implementation just calls repaint (FALSE); You may
     * reimplement this function if you want to.
     */
    virtual void focusOutEvent ( QFocusEvent *);

    /**
     * Reimplemented to call the queryClose() and queryExit() handlers.
     * Please do not reimplement closeEvent directly but use queryClose() 
     * in your KDE applications. 
     */
    virtual void closeEvent ( QCloseEvent *);

    /**
     * KTMainWindow has the nice habbit that it will exit the
     * application when the very last KTMainWindow is
     * closed. Some applications may not want this default
     * behaviour, for example if the application wants to ask the user
     * wether he really wants to quit the application.  This can be
     * achived by overloading the @ref #queryExit () method.  The default
     * implementation simply returns TRUE, which means that the
     * application will be quitted. FALSE will cancel the exiting
     * process. 
     *
     * If you cancel exiting, your last window will remain visible.
     * @see #queryClose
     */
    virtual bool queryExit();

    /**
     * Called before window is closed, either by the user or indirectely by
     * the session manager in "safely quit all applications"-mode.
     *
     * Default implementation returns true. Returning false will cancel
     * the closing.
     *
       Reimplement this function to prevent the user from loosing data. 
       Example:
       
           switch ( QMessageBox::warning( this, "Appname",
				   i18n("Save changes to Document Foo?"),
				   i18n("&Yes"),
				   i18n("&No"),
				   i18n("Cancel"),
				   0, 2) ) {
           case 0: // yes
	// save document here. If saving fails, return FALSE;
	return TRUE;
           case 1: // no
	return TRUE;
           default: // cancel
	return FALSE;

     *
     * @see #queryExit
     */
    virtual bool queryClose();

     /**
     * Save your instance-specific properties.
     * You MUST NOT change the group of the kconfig object,
     * since KTMainWindow uses one group for each window.
     * Please overload these function in childclasses.
     *
     * Note that no user interaction is possible
     * in these functions!
     *
     */
    virtual void saveProperties(KConfig*){};

   /**
    * Read your instance-specific properties.
    */
   virtual void readProperties(KConfig*){};

   /**
    * This method is called, when @ref KApplication emits signal saveYourself
    * and after KTMainWindow has verified that it is "main" top-level window.
    * So this method will be called only once and not in every widget.
    * Override it if you need to save other data about your documents on
    * session end. sessionConfig is a config to which that data should be
    * saved. Normaly, you don't need this function. But if you want to save
    * data about your documents that are not in opened windows you might need
    * it.
    *
    * Default implementation does nothing.
    */
   virtual void saveData(KConfig* sessionConfig);

public slots:
    /**
     * Makes a KDE compliant caption.
     * 
     * @param caption Your caption. DO NOT include the application name
     * in this string. It will be added automatically according to the KDE
     * standard.
     */
    virtual void setCaption( const QString &caption );

    /**
     * Makes a plain caption without any modifications.
     * 
     * @param caption Your caption. This is the string that will be 
     * displayed in the window title.
     */
    virtual void setPlainCaption( const QString &caption );


protected slots:

   /**
    * This slot must be called whenever the arrangement of the child element
    * has been changed. It needs not to be called for a resize operation.
    * This is handled by Qt layout management.
    */
   virtual void updateRects();

private slots:
   /**
    * Notices when toolbar is deleted.
    */
   void toolbarKilled();

   /**
    * Notices when menubar is killed.
    */
   void menubarKilled();

public:

   /**
    * List of members of KTMainWindow class
    */
   static QList<KTMainWindow>* memberList;

private:
   /**
    * List of toolbars.
    */
   QList <KToolBar> toolbars;

   /**
    * Main widget. If you want fixed-widget just call setFixedSize(w.h)
    * on your mainwidget.
    * You should not setFixedSize on KTMainWindow.
    */
   QWidget *kmainwidget;

   /**
    * Menubar.
    */
   KMenuBar *kmenubar;

   /**
    * Statusbar
    */
   KStatusBar *kstatusbar;

   /**
    * Frame around main widget
    */
   QFrame *kmainwidgetframe;

   /**
    * Stores the width of the view frame
    */
    int borderwidth;

   /**
    * True if toolbars are killed by this destructor.
    */
   bool localKill;


   KTMLayout* layoutMgr;

   KHelpMenu    *mHelpMenu;

protected:

  void savePropertiesInternal (KConfig*, int);
  bool readPropertiesInternal (KConfig*, int);
};


#define RESTORE(type) { int n = 1;\
    while (KTMainWindow::canBeRestored(n)){\
      (new type)->restore(n);\
      n++;}}

#endif
