/* ANSI-C code produced by gperf version 2.7.2 */
/* Command-line: gperf -c -a -L ANSI-C -E -C -o -t -k '*' -NfindAttr -Hhash_attr -Wwordlist_attr -s 2 htmlattrs.gperf  */
/* This file is automatically generated from
#htmlattrs.in by makeattrs, do not edit */
/* Copyright 1999 Lars Knoll */
#include "htmlattrs.h"
struct attrs {
    const char *name;
    int id;
};
/* maximum key range = 769, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash_attr (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793,   5, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793,  20,  65,   0,
       70,   0,  80,  95, 180,  25,   0,  50,   0,  10,
        0,   0,  70,   0, 135,  20,  15, 110, 105,  70,
       25, 135,   0, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793, 793, 793, 793, 793,
      793, 793, 793, 793, 793, 793
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
const struct attrs *
findAttr (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 137,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 14,
      MIN_HASH_VALUE = 24,
      MAX_HASH_VALUE = 792
    };

  static const struct attrs wordlist_attr[] =
    {
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"cols", ATTR_COLS},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"name", ATTR_NAME},
      {"", 0}, {"", 0},
      {"content", ATTR_CONTENT},
      {"alt", ATTR_ALT},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onselect", ATTR_ONSELECT},
      {"cite", ATTR_CITE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"size", ATTR_SIZE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"text", ATTR_TEXT},
      {"title", ATTR_TITLE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"class", ATTR_CLASS},
      {"action", ATTR_ACTION},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"code", ATTR_CODE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"link", ATTR_LINK},
      {"", 0}, {"", 0},
      {"onclick", ATTR_ONCLICK},
      {"", 0}, {"", 0}, {"", 0},
      {"object", ATTR_OBJECT},
      {"", 0},
      {"top", ATTR_TOP},
      {"", 0},
      {"label", ATTR_LABEL},
      {"", 0}, {"", 0}, {"", 0},
      {"axis", ATTR_AXIS},
      {"scope", ATTR_SCOPE},
      {"onload", ATTR_ONLOAD},
      {"id", ATTR_ID},
      {"", 0},
      {"left", ATTR_LEFT},
      {"alink", ATTR_ALINK},
      {"", 0}, {"", 0}, {"", 0},
      {"face", ATTR_FACE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"accept", ATTR_ACCEPT},
      {"", 0},
      {"selected", ATTR_SELECTED},
      {"span", ATTR_SPAN},
      {"", 0}, {"", 0},
      {"colspan", ATTR_COLSPAN},
      {"", 0},
      {"lang", ATTR_LANG},
      {"plain", ATTR_PLAIN},
      {"", 0},
      {"compact", ATTR_COMPACT},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"data", ATTR_DATA},
      {"media", ATTR_MEDIA},
      {"", 0},
      {"z-index", ATTR_Z_INDEX},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"rel", ATTR_REL},
      {"", 0},
      {"color", ATTR_COLOR},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"align", ATTR_ALIGN},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"ismap", ATTR_ISMAP},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"src", ATTR_SRC},
      {"", 0},
      {"clear", ATTR_CLEAR},
      {"", 0},
      {"classid", ATTR_CLASSID},
      {"datetime", ATTR_DATETIME},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0},
      {"style", ATTR_STYLE},
      {"", 0},
      {"onreset", ATTR_ONRESET},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"codebase", ATTR_CODEBASE},
      {"", 0},
      {"vlink", ATTR_VLINK},
      {"", 0}, {"", 0},
      {"noresize", ATTR_NORESIZE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"longdesc", ATTR_LONGDESC},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onunload", ATTR_ONUNLOAD},
      {"html", ATTR_HTML},
      {"start", ATTR_START},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"pagex", ATTR_PAGEX},
      {"scheme", ATTR_SCHEME},
      {"onfocus", ATTR_ONFOCUS},
      {"for", ATTR_FOR},
      {"", 0},
      {"ondblclick", ATTR_ONDBLCLICK},
      {"vspace", ATTR_VSPACE},
      {"", 0}, {"", 0},
      {"type", ATTR_TYPE},
      {"", 0}, {"", 0},
      {"enctype", ATTR_ENCTYPE},
      {"tabindex", ATTR_TABINDEX},
      {"rows", ATTR_ROWS},
      {"", 0},
      {"coords", ATTR_COORDS},
      {"declare", ATTR_DECLARE},
      {"dir", ATTR_DIR},
      {"", 0}, {"", 0},
      {"usemap", ATTR_USEMAP},
      {"unknown", ATTR_UNKNOWN},
      {"multiple", ATTR_MULTIPLE},
      {"", 0},
      {"value", ATTR_VALUE},
      {"cellspacing", ATTR_CELLSPACING},
      {"", 0},
      {"rev", ATTR_REV},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"frame", ATTR_FRAME},
      {"valign", ATTR_VALIGN},
      {"autocomplete", ATTR_AUTOCOMPLETE},
      {"onsubmit", ATTR_ONSUBMIT},
      {"accesskey", ATTR_ACCESSKEY},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0},
      {"onmousemove", ATTR_ONMOUSEMOVE},
      {"", 0}, {"", 0}, {"", 0},
      {"rules", ATTR_RULES},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onmouseout", ATTR_ONMOUSEOUT},
      {"", 0}, {"", 0},
      {"disabled", ATTR_DISABLED},
      {"", 0}, {"", 0},
      {"method", ATTR_METHOD},
      {"", 0}, {"", 0},
      {"scrolling", ATTR_SCROLLING},
      {"", 0},
      {"target", ATTR_TARGET},
      {"", 0}, {"", 0},
      {"abbr", ATTR_ABBR},
      {"defer", ATTR_DEFER},
      {"onmousedown", ATTR_ONMOUSEDOWN},
      {"version", ATTR_VERSION},
      {"", 0}, {"", 0},
      {"shape", ATTR_SHAPE},
      {"hspace", ATTR_HSPACE},
      {"noshade", ATTR_NOSHADE},
      {"codetype", ATTR_CODETYPE},
      {"wrap", ATTR_WRAP},
      {"", 0},
      {"nowrap", ATTR_NOWRAP},
      {"bgcolor", ATTR_BGCOLOR},
      {"onchange", ATTR_ONCHANGE},
      {"", 0}, {"", 0},
      {"prompt", ATTR_PROMPT},
      {"checked", ATTR_CHECKED},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onblur", ATTR_ONBLUR},
      {"profile", ATTR_PROFILE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"rowspan", ATTR_ROWSPAN},
      {"", 0}, {"", 0},
      {"pagey", ATTR_PAGEY},
      {"", 0}, {"", 0}, {"", 0},
      {"onmouseup", ATTR_ONMOUSEUP},
      {"", 0}, {"", 0},
      {"standby", ATTR_STANDBY},
      {"", 0},
      {"onkeydown", ATTR_ONKEYDOWN},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"char", ATTR_CHAR},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"language", ATTR_LANGUAGE},
      {"", 0}, {"", 0},
      {"hidden", ATTR_HIDDEN},
      {"", 0}, {"", 0},
      {"maxlength", ATTR_MAXLENGTH},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"cellpadding", ATTR_CELLPADDING},
      {"", 0}, {"", 0}, {"", 0},
      {"width", ATTR_WIDTH},
      {"", 0}, {"", 0},
      {"readonly", ATTR_READONLY},
      {"", 0}, {"", 0}, {"", 0},
      {"onkeyup", ATTR_ONKEYUP},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"charset", ATTR_CHARSET},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onmouseover", ATTR_ONMOUSEOVER},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"href", ATTR_HREF},
      {"", 0},
      {"nohref", ATTR_NOHREF},
      {"oversrc", ATTR_OVERSRC},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"border", ATTR_BORDER},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0},
      {"headers", ATTR_HEADERS},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"onkeypress", ATTR_ONKEYPRESS},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"summary", ATTR_SUMMARY},
      {"", 0}, {"", 0},
      {"visibility", ATTR_VISIBILITY},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"valuetype", ATTR_VALUETYPE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"archive", ATTR_ARCHIVE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0},
      {"accept-charset", ATTR_ACCEPT_CHARSET},
      {"pluginpage", ATTR_PLUGINPAGE},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"height", ATTR_HEIGHT},
      {"charoff", ATTR_CHAROFF},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"pluginspage", ATTR_PLUGINSPAGE},
      {"", 0},
      {"hreflang", ATTR_HREFLANG},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"http-equiv", ATTR_HTTP_EQUIV},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"bordercolor", ATTR_BORDERCOLOR},
      {"", 0}, {"", 0},
      {"pluginurl", ATTR_PLUGINURL},
      {"background", ATTR_BACKGROUND},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"bgproperties", ATTR_BGPROPERTIES},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"marginwidth", ATTR_MARGINWIDTH},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"frameborder", ATTR_FRAMEBORDER},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
      {"marginheight", ATTR_MARGINHEIGHT}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash_attr (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist_attr[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist_attr[key];
        }
    }
  return 0;
}


static const char *attrList[] = {
    "ABBR",
    "ACCEPT-CHARSET",
    "ACCEPT",
    "ACCESSKEY",
    "ACTION",
    "ALIGN",
    "ALINK",
    "ALT",
    "ARCHIVE",
    "AUTOCOMPLETE",
    "AXIS",
    "BACKGROUND",
    "BGCOLOR",
    "BGPROPERTIES",
    "BORDER",
    "BORDERCOLOR",
    "CELLPADDING",
    "CELLSPACING",
    "CHAR",
    "CHAROFF",
    "CHARSET",
    "CHECKED",
    "CITE",
    "CLASS",
    "CLASSID",
    "CLEAR",
    "CODE",
    "CODEBASE",
    "CODETYPE",
    "COLOR",
    "COLS",
    "COLSPAN",
    "COMPACT",
    "CONTENT",
    "COORDS",
    "DATA",
    "DATETIME",
    "DECLARE",
    "DEFER",
    "DIR",
    "DISABLED",
    "ENCTYPE",
    "FACE",
    "FOR",
    "FRAME",
    "FRAMEBORDER",
    "HEADERS",
    "HEIGHT",
    "HIDDEN",
    "HREF",
    "HREFLANG",
    "HSPACE",
    "HTML",
    "HTTP-EQUIV",
    "ID",
    "ISMAP",
    "LABEL",
    "LANG",
    "LANGUAGE",
    "LEFT",
    "LINK",
    "LONGDESC",
    "MARGINHEIGHT",
    "MARGINWIDTH",
    "MAXLENGTH",
    "MEDIA",
    "METHOD",
    "MULTIPLE",
    "NAME",
    "NOHREF",
    "NORESIZE",
    "NOSHADE",
    "NOWRAP",
    "OBJECT",
    "ONBLUR",
    "ONCHANGE",
    "ONCLICK",
    "ONDBLCLICK",
    "ONFOCUS",
    "ONKEYDOWN",
    "ONKEYPRESS",
    "ONKEYUP",
    "ONLOAD",
    "ONMOUSEDOWN",
    "ONMOUSEMOVE",
    "ONMOUSEOUT",
    "ONMOUSEOVER",
    "ONMOUSEUP",
    "ONRESET",
    "ONSELECT",
    "ONSUBMIT",
    "ONUNLOAD",
    "OVERSRC",
    "PAGEX",
    "PAGEY",
    "PLAIN",
    "PLUGINPAGE",
    "PLUGINSPAGE",
    "PLUGINURL",
    "PROFILE",
    "PROMPT",
    "READONLY",
    "REL",
    "REV",
    "ROWS",
    "ROWSPAN",
    "RULES",
    "SCHEME",
    "SCOPE",
    "SCROLLING",
    "SELECTED",
    "SHAPE",
    "SIZE",
    "SPAN",
    "SRC",
    "STANDBY",
    "START",
    "STYLE",
    "SUMMARY",
    "TABINDEX",
    "TARGET",
    "TEXT",
    "TITLE",
    "TOP",
    "TYPE",
    "UNKNOWN",
    "USEMAP",
    "VALIGN",
    "VALUE",
    "VALUETYPE",
    "VERSION",
    "VISIBILITY",
    "VLINK",
    "VSPACE",
    "WIDTH",
    "WRAP",
    "Z-INDEX",
    0
};
DOMString getAttrName(unsigned short id)
{
    return attrList[id-1];
};
