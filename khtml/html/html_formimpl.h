/*
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * $Id$
 */
#ifndef HTML_FORMIMPL_H
#define HTML_FORMIMPL_H

// -------------------------------------------------------------------------
#include "dtd.h"
#include "html_elementimpl.h"
#include "khtmlview.h"

class KHTMLWidget;
class QWidget;
class QMultiLineEdit;
#include <qpixmap.h>
#include <qlist.h>

namespace khtml
{
    class RenderFormElement;
    class RenderTextArea;
    class RenderSelect;
}

namespace DOM {

class HTMLFormElement;
class DOMString;
class HTMLGenericFormElementImpl;

class HTMLFormElementImpl : public HTMLElementImpl
{
public:
    HTMLFormElementImpl(DocumentImpl *doc);
    virtual ~HTMLFormElementImpl();

    virtual const DOMString nodeName() const { return "FORM"; }
    virtual ushort id() const { return ID_FORM; }

    virtual tagStatus startTag() { return FORMStartTag; }
    virtual tagStatus endTag() { return FORMEndTag; }

    long length() const;
    void submit (  );
    void reset (  );

    virtual void parseAttribute(Attribute *attr);

    virtual void attach(KHTMLWidget *w);
    virtual void detach();

    void radioClicked( khtml::RenderFormElement *caller );
    void maybeSubmit();

    void registerFormElement(khtml::RenderFormElement *);
    void removeFormElement(khtml::RenderFormElement *);

protected:
    DOMString url;
    DOMString target;
    bool post;
    KHTMLWidget *view;
    QList<khtml::RenderFormElement> formElements;
};

// -------------------------------------------------------------------------

class HTMLGenericFormElementImpl : public HTMLElementImpl
{
    friend class HTMLFormElementImpl;

public:
    HTMLGenericFormElementImpl(DocumentImpl *doc);
    HTMLGenericFormElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);
    virtual ~HTMLGenericFormElementImpl();

    HTMLFormElementImpl *form() { return _form; }

    virtual NodeImpl *addChild(NodeImpl *newChild) { return HTMLElementImpl::addChild(newChild); }

    virtual void parseAttribute(Attribute *attr);

    virtual void attach(KHTMLWidget *w);
    virtual void detach();

    virtual void reset() {}
    bool disabled() const { return m_disabled; }

protected:
    HTMLFormElementImpl *getForm() const;

    DOMString _name;
    HTMLFormElementImpl *_form;
    KHTMLWidget *view;
    bool m_disabled, m_readonly;
};

// -------------------------------------------------------------------------

class HTMLButtonElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLButtonElementImpl(DocumentImpl *doc);
    HTMLButtonElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);

    virtual ~HTMLButtonElementImpl();

    enum typeEnum {
	SUBMIT,
	RESET,
	BUTTON
    };

    virtual const DOMString nodeName() const;
    virtual ushort id() const;

    virtual tagStatus startTag() { return BUTTONStartTag; }
    virtual tagStatus endTag() { return BUTTONEndTag; }

    long tabIndex() const;
    void setTabIndex( long );

    DOMString type() const;

    void parseAttribute(Attribute *attr);

    virtual void attach(KHTMLWidget *w);

    virtual NodeImpl *addChild(NodeImpl *newChild) { return HTMLElementImpl::addChild(newChild); }

protected:
    DOMString _value;
    bool _clicked;
    typeEnum _type;
    QString currValue;
    QPixmap pixmap;
    bool dirty;
};

// -------------------------------------------------------------------------

class HTMLFieldSetElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLFieldSetElementImpl(DocumentImpl *doc);
    HTMLFieldSetElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);

    virtual ~HTMLFieldSetElementImpl();

    virtual const DOMString nodeName() const;
    virtual ushort id() const;

    virtual tagStatus startTag() { return FIELDSETStartTag; }
    virtual tagStatus endTag() { return FIELDSETEndTag; }
};

// -------------------------------------------------------------------------

class HTMLInputElementImpl : public HTMLGenericFormElementImpl
{
public:
    enum typeEnum {
	TEXT,
	PASSWORD,
	CHECKBOX,
	RADIO,
	SUBMIT,
	RESET,
	FILE,
	HIDDEN,
	IMAGE,
	BUTTON
    };

    HTMLInputElementImpl(DocumentImpl *doc);
    HTMLInputElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);
    virtual ~HTMLInputElementImpl();

    virtual const DOMString nodeName() const;
    virtual ushort id() const;

    virtual tagStatus startTag() { return INPUTStartTag; }
    virtual tagStatus endTag() { return INPUTEndTag; }

    bool checked() const { return m_checked; }
    void setChecked(bool b) { m_checked = b; }

    long maxLength() const { return _maxLen; }
    void setMaxLength( long );

    long tabIndex() const;
    void setTabIndex( long );

    DOMString type() const;

    void blur();
    void focus();
    void select();
    void click();

    virtual void parseAttribute(Attribute *attr);

    virtual void attach(KHTMLWidget *w);
    virtual void detach();

protected:
    typeEnum _type;
    DOMString _value;
    QString currValue;
    bool m_checked;
    int _maxLen;
    int _size;
    DOMString _src;
    bool _clicked;
};

// -------------------------------------------------------------------------

class HTMLLabelElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLLabelElementImpl(DocumentImpl *doc);
    HTMLLabelElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);
    virtual ~HTMLLabelElementImpl();

    virtual const DOMString nodeName() const;
    virtual ushort id() const;

    virtual tagStatus startTag() { return LABELStartTag; }
    virtual tagStatus endTag() { return LABELEndTag; }
};

// -------------------------------------------------------------------------

class HTMLLegendElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLLegendElementImpl(DocumentImpl *doc);
    HTMLLegendElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);
    virtual ~HTMLLegendElementImpl();

    virtual const DOMString nodeName() const;
    virtual ushort id() const;

    virtual tagStatus startTag() { return LEGENDStartTag; }
    virtual tagStatus endTag() { return LEGENDEndTag; }
};


// -------------------------------------------------------------------------

class HTMLSelectElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLSelectElementImpl(DocumentImpl *doc);
    HTMLSelectElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);

    virtual const DOMString nodeName() const { return "SELECT"; }
    virtual ushort id() const { return ID_SELECT; }

    virtual tagStatus startTag() { return SELECTStartTag; }
    virtual tagStatus endTag() { return SELECTEndTag; }

    DOMString type() const;

    long selectedIndex() const;
    void setSelectedIndex( long );

    long length() const;

    long size() const { return m_size; }
    void setSize( long );

    long tabIndex() const;
    void setTabIndex( long );

    void add ( const HTMLElement &element, const HTMLElement &before );
    void remove ( long index );
    void blur (  );
    void focus (  );

    virtual void parseAttribute(Attribute *attr);

    virtual void attach(KHTMLWidget *w);

protected:
    int m_size;
    bool m_multiple;
};


// -------------------------------------------------------------------------

class HTMLOptGroupElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLOptGroupElementImpl(DocumentImpl *doc);
    HTMLOptGroupElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);
    virtual ~HTMLOptGroupElementImpl();

    virtual const DOMString nodeName() const { return "OPTGROUP"; }
    virtual ushort id() const { return ID_OPTGROUP; }

    virtual tagStatus startTag() { return OPTGROUPStartTag; }
    virtual tagStatus endTag() { return OPTGROUPEndTag; }

};


// ---------------------------------------------------------------------------

class HTMLOptionElementImpl : public HTMLGenericFormElementImpl
{
public:
    HTMLOptionElementImpl(DocumentImpl *doc);
    HTMLOptionElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);

    virtual const DOMString nodeName() const { return "OPTION"; }
    virtual ushort id() const { return ID_OPTION; }

    virtual tagStatus startTag() { return OPTIONStartTag; }
    virtual tagStatus endTag() { return OPTIONEndTag; }

    DOMString text() const;

    long index() const;
    void setIndex( long );
    virtual void parseAttribute(Attribute *attr);

    bool selected() const { return m_selected; }

protected:
    bool m_selected;
    DOMString m_value;

    friend khtml::RenderSelect;
};


// -------------------------------------------------------------------------

class HTMLTextAreaElementImpl : public HTMLGenericFormElementImpl
{
public:
    enum WrapMethod {
        ta_NoWrap,
        ta_Virtual,
        ta_Physical
    };

    HTMLTextAreaElementImpl(DocumentImpl *doc);
    HTMLTextAreaElementImpl(DocumentImpl *doc, HTMLFormElementImpl *f);

    virtual const DOMString nodeName() const { return "TEXTAREA"; }
    virtual ushort id() const { return ID_TEXTAREA; }

    virtual tagStatus startTag() { return TEXTAREAStartTag; }
    virtual tagStatus endTag() { return TEXTAREAEndTag; }

    long cols() const { return m_cols; }
    void setCols( long cols) { m_cols = cols; }

    long rows() const { return m_rows; }
    void setRows( long rows ) { m_rows = rows; }

    long tabIndex() const;
    void setTabIndex( long );

    DOMString type() const;

    void blur (  );
    void focus (  );
    void select (  );

    virtual void parseAttribute(Attribute *attr);
    virtual void attach(KHTMLWidget *w);

protected:
    int m_rows;
    int m_cols;
    WrapMethod m_wrap;

    friend khtml::RenderTextArea;
};

}; //namespace

#endif
