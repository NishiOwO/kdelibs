#include "ipprequest.h"
#include "cupsinfos.h"

#include <stdlib.h>
#include <cups/language.h>
#include <cups/cups.h>

void dumpRequest(ipp_t *req)
{
	debug("------------------------------");
	debug("state = 0x%x",req->state);
	debug("current tag = 0x%x",req->curtag);
	debug("request operation ID = 0x%x",req->request.op.operation_id);
	debug("request ID = 0x%x",req->request.op.request_id);
	debug("request version = %c%c",req->request.op.version[0],req->request.op.version[1]);
	ipp_attribute_t	*attr = req->attrs;
	while (attr)
	{
		debug(" ");
		debug("attribute: %s",attr->name);
		debug("group tag = 0x%x",attr->group_tag);
		debug("value tag = 0x%x",attr->value_tag);
		debug("number of values = %d",attr->num_values);
		if (attr->value_tag >= IPP_TAG_TEXT)
			for (int i=0;i<attr->num_values;i++)
				debug("value[%d] = %s",i,attr->values[i].string.text);
		attr = attr->next;
	}
	debug("------------------------------");
}

//*************************************************************************************

IppRequest::IppRequest()
{
	request_ = 0;
	init();
}

IppRequest::~IppRequest()
{
	ippDelete(request_);
}

void IppRequest::init()
{
	if (request_)
	{
		ippDelete(request_);
		request_ = 0;
	}
	request_ = ippNew();
	cups_lang_t*	lang = cupsLangDefault();
	ippAddString(request_, IPP_TAG_OPERATION, IPP_TAG_CHARSET, "attributes-charset", NULL, cupsLangEncoding(lang));
	ippAddString(request_, IPP_TAG_OPERATION, IPP_TAG_LANGUAGE, "attributes-natural-language", NULL, lang->language);
}

void IppRequest::addString_p(int group, int type, const QString& name, const QString& value)
{
	if (!name.isEmpty())
		ippAddString(request_,(ipp_tag_t)group,(ipp_tag_t)type,name.latin1(),NULL,(value.isEmpty() ? QString::fromLatin1("") : value.local8Bit()));
}

void IppRequest::addStringList_p(int group, int type, const QString& name, const QStringList& values)
{
	if (!name.isEmpty())
	{
		ipp_attribute_t	*attr = ippAddStrings(request_,(ipp_tag_t)group,(ipp_tag_t)type,name.latin1(),(int)(values.count()),NULL,NULL);
		int	i(0);
		for (QStringList::ConstIterator it=values.begin(); it != values.end(); ++it, i++)
			attr->values[i].string.text = strdup((*it).local8Bit());
	}
}

void IppRequest::addInteger_p(int group, int type, const QString& name, int value)
{
	if (!name.isEmpty()) ippAddInteger(request_,(ipp_tag_t)group,(ipp_tag_t)type,name.latin1(),value);
}

void IppRequest::addIntegerList_p(int group, int type, const QString& name, const QValueList<int>& values)
{
	if (!name.isEmpty())
	{
		ipp_attribute_t	*attr = ippAddIntegers(request_,(ipp_tag_t)group,(ipp_tag_t)type,name.latin1(),(int)(values.count()),NULL);
		int	i(0);
		for (QValueList<int>::ConstIterator it=values.begin(); it != values.end(); ++it, i++)
			attr->values[i].integer = *it;
	}
}

void IppRequest::addBoolean(int group, const QString& name, bool value)
{
	if (!name.isEmpty()) ippAddBoolean(request_,(ipp_tag_t)group,name.latin1(),(char)value);
}

void IppRequest::addBoolean(int group, const QString& name, const QValueList<bool>& values)
{
	if (!name.isEmpty())
	{
		ipp_attribute_t	*attr = ippAddBooleans(request_,(ipp_tag_t)group,name.latin1(),(int)(values.count()),NULL);
		int	i(0);
		for (QValueList<bool>::ConstIterator it=values.begin(); it != values.end(); ++it, i++)
			attr->values[i].boolean = (char)(*it);
	}
}

void IppRequest::setOperation(int op)
{
	request_->request.op.operation_id = (ipp_op_t)op;
}

int IppRequest::status()
{
	return (request_ ? request_->request.status.status_code : IPP_OK);
}

bool IppRequest::integerValue_p(const QString& name, int& value, int type)
{
	if (!request_ || name.isEmpty()) return false;
	ipp_attribute_t	*attr = ippFindAttribute(request_, name.latin1(), (ipp_tag_t)type);
	if (attr)
	{
		value = attr->values[0].integer;
		return true;
	}
	else return false;
}

bool IppRequest::stringValue_p(const QString& name, QString& value, int type)
{
	if (!request_ || name.isEmpty()) return false;
	ipp_attribute_t	*attr = ippFindAttribute(request_, name.latin1(), (ipp_tag_t)type);
	if (attr)
	{
		value = QString::fromLocal8Bit(attr->values[0].string.text);
		return true;
	}
	else return false;
}

bool IppRequest::stringListValue_p(const QString& name, QStringList& values, int type)
{
	if (!request_ || name.isEmpty()) return false;
	ipp_attribute_t	*attr = ippFindAttribute(request_, name.latin1(), (ipp_tag_t)type);
	values.clear();
	if (attr)
	{
		for (int i=0;i<attr->num_values;i++)
			values.append(QString::fromLocal8Bit(attr->values[i].string.text));
		return true;
	}
	else return false;
}

bool IppRequest::boolean(const QString& name, bool& value)
{
	if (!request_ || name.isEmpty()) return false;
	ipp_attribute_t	*attr = ippFindAttribute(request_, name.latin1(), IPP_TAG_BOOLEAN);
	if (attr)
	{
		value = (bool)attr->values[0].boolean;
		return true;
	}
	else return false;
}

bool IppRequest::doFileRequest(const QString& res, const QString& filename)
{
	http_t	*HTTP = httpConnect(CupsInfos::self()->host().latin1(),CupsInfos::self()->port());

	if (HTTP == NULL)
		return false;
	request_ = cupsDoFileRequest(HTTP, request_, (res.isEmpty() ? "/" : res.latin1()), (filename.isEmpty() ? NULL : filename.latin1()));
	httpClose(HTTP);
	if (!request_ || request_->state == IPP_ERROR)
		return false;
#if 0
dumpRequest(request_);
#endif
	return true;
}
