/* ANSI-C code produced by gperf version 2.7.1 (19981006 egcs) */
/* Command-line: gperf -a -L ANSI-C -E -C -c -o -t -k * -NfindAttr -Hhash_attr -Wwordlist_attr -D -s 2 htmlattrs.gperf  */
/* This file is automatically generated from
#htmlattrs.in by makeattrs, do not edit */
/* Copyright 1999 Lars Knoll */
#include "htmlattrs.h"
struct attrs {
    const char *name;
    int id;
};
/* maximum key range = 699, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
hash_attr (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728,   0, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728,  30,  15,   0,
      238,   0,  40,  85,  10,   0,   0,  65,   0,  15,
        0,   0, 125,   0,  40, 120,  25,  20, 200,  20,
       35,  50,   5, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728, 728, 728, 728, 728,
      728, 728, 728, 728, 728, 728
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
      TOTAL_KEYWORDS = 127,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 14,
      MIN_HASH_VALUE = 29,
      MAX_HASH_VALUE = 727
    };

  static const struct attrs wordlist_attr[] =
    {
      {"cite", ATTR_CITE},
      {"rel", ATTR_REL},
      {"color", ATTR_COLOR},
      {"object", ATTR_OBJECT},
      {"name", ATTR_NAME},
      {"label", ATTR_LABEL},
      {"html", ATTR_HTML},
      {"title", ATTR_TITLE},
      {"content", ATTR_CONTENT},
      {"alt", ATTR_ALT},
      {"action", ATTR_ACTION},
      {"link", ATTR_LINK},
      {"onclick", ATTR_ONCLICK},
      {"face", ATTR_FACE},
      {"clear", ATTR_CLEAR},
      {"onblur", ATTR_ONBLUR},
      {"for", ATTR_FOR},
      {"char", ATTR_CHAR},
      {"text", ATTR_TEXT},
      {"href", ATTR_HREF},
      {"nohref", ATTR_NOHREF},
      {"alink", ATTR_ALINK},
      {"abbr", ATTR_ABBR},
      {"lang", ATTR_LANG},
      {"align", ATTR_ALIGN},
      {"cols", ATTR_COLS},
      {"size", ATTR_SIZE},
      {"frame", ATTR_FRAME},
      {"onchange", ATTR_ONCHANGE},
      {"height", ATTR_HEIGHT},
      {"bgcolor", ATTR_BGCOLOR},
      {"scheme", ATTR_SCHEME},
      {"onselect", ATTR_ONSELECT},
      {"plain", ATTR_PLAIN},
      {"src", ATTR_SRC},
      {"charoff", ATTR_CHAROFF},
      {"noresize", ATTR_NORESIZE},
      {"rows", ATTR_ROWS},
      {"rules", ATTR_RULES},
      {"accept", ATTR_ACCEPT},
      {"onfocus", ATTR_ONFOCUS},
      {"axis", ATTR_AXIS},
      {"onreset", ATTR_ONRESET},
      {"multiple", ATTR_MULTIPLE},
      {"style", ATTR_STYLE},
      {"compact", ATTR_COMPACT},
      {"onsubmit", ATTR_ONSUBMIT},
      {"type", ATTR_TYPE},
      {"enctype", ATTR_ENCTYPE},
      {"maxlength", ATTR_MAXLENGTH},
      {"onmouseout", ATTR_ONMOUSEOUT},
      {"target", ATTR_TARGET},
      {"profile", ATTR_PROFILE},
      {"hreflang", ATTR_HREFLANG},
      {"wrap", ATTR_WRAP},
      {"nowrap", ATTR_NOWRAP},
      {"charset", ATTR_CHARSET},
      {"id", ATTR_ID},
      {"code", ATTR_CODE},
      {"rev", ATTR_REV},
      {"start", ATTR_START},
      {"scope", ATTR_SCOPE},
      {"scrolling", ATTR_SCROLLING},
      {"value", ATTR_VALUE},
      {"language", ATTR_LANGUAGE},
      {"onkeyup", ATTR_ONKEYUP},
      {"vlink", ATTR_VLINK},
      {"onload", ATTR_ONLOAD},
      {"class", ATTR_CLASS},
      {"span", ATTR_SPAN},
      {"dir", ATTR_DIR},
      {"colspan", ATTR_COLSPAN},
      {"archive", ATTR_ARCHIVE},
      {"media", ATTR_MEDIA},
      {"shape", ATTR_SHAPE},
      {"hspace", ATTR_HSPACE},
      {"method", ATTR_METHOD},
      {"ismap", ATTR_ISMAP},
      {"onunload", ATTR_ONUNLOAD},
      {"summary", ATTR_SUMMARY},
      {"width", ATTR_WIDTH},
      {"pluginurl", ATTR_PLUGINURL},
      {"onmouseup", ATTR_ONMOUSEUP},
      {"marginheight", ATTR_MARGINHEIGHT},
      {"declare", ATTR_DECLARE},
      {"usemap", ATTR_USEMAP},
      {"checked", ATTR_CHECKED},
      {"valign", ATTR_VALIGN},
      {"defer", ATTR_DEFER},
      {"data", ATTR_DATA},
      {"ondblclick", ATTR_ONDBLCLICK},
      {"prompt", ATTR_PROMPT},
      {"border", ATTR_BORDER},
      {"datetime", ATTR_DATETIME},
      {"rowspan", ATTR_ROWSPAN},
      {"tabindex", ATTR_TABINDEX},
      {"readonly", ATTR_READONLY},
      {"version", ATTR_VERSION},
      {"cellspacing", ATTR_CELLSPACING},
      {"onmousemove", ATTR_ONMOUSEMOVE},
      {"onkeydown", ATTR_ONKEYDOWN},
      {"bordercolor", ATTR_BORDERCOLOR},
      {"selected", ATTR_SELECTED},
      {"accesskey", ATTR_ACCESSKEY},
      {"coords", ATTR_COORDS},
      {"noshade", ATTR_NOSHADE},
      {"onmouseover", ATTR_ONMOUSEOVER},
      {"oversrc", ATTR_OVERSRC},
      {"codebase", ATTR_CODEBASE},
      {"http-equiv", ATTR_HTTP_EQUIV},
      {"accept-charset", ATTR_ACCEPT_CHARSET},
      {"onmousedown", ATTR_ONMOUSEDOWN},
      {"headers", ATTR_HEADERS},
      {"codetype", ATTR_CODETYPE},
      {"longdesc", ATTR_LONGDESC},
      {"valuetype", ATTR_VALUETYPE},
      {"frameborder", ATTR_FRAMEBORDER},
      {"marginwidth", ATTR_MARGINWIDTH},
      {"pluginpage", ATTR_PLUGINPAGE},
      {"vspace", ATTR_VSPACE},
      {"standby", ATTR_STANDBY},
      {"hidden", ATTR_HIDDEN},
      {"background", ATTR_BACKGROUND},
      {"classid", ATTR_CLASSID},
      {"onkeypress", ATTR_ONKEYPRESS},
      {"disabled", ATTR_DISABLED},
      {"cellpadding", ATTR_CELLPADDING}
    };

  static const signed char lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,   1,  -1,   2,   3,  -1,  -1,   4,
        5,  -1,  -1,  -1,   6,   7,  -1,   8,   9,  -1,
       -1,  10,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  11,
       -1,  -1,  12,  -1,  13,  14,  -1,  -1,  -1,  -1,
       -1,  15,  -1,  16,  17,  -1,  -1,  -1,  -1,  18,
       -1,  -1,  -1,  -1,  19,  -1,  20,  -1,  -1,  -1,
       21,  -1,  -1,  -1,  22,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  23,
       24,  -1,  -1,  -1,  25,  -1,  -1,  -1,  -1,  26,
       27,  -1,  -1,  28,  -1,  -1,  29,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  30,  -1,  -1,
       -1,  31,  -1,  32,  -1,  -1,  -1,  -1,  -1,  -1,
       33,  -1,  -1,  34,  -1,  -1,  -1,  35,  -1,  -1,
       -1,  -1,  -1,  36,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  37,  38,  39,  40,  -1,  41,
       -1,  -1,  42,  43,  -1,  -1,  -1,  -1,  -1,  -1,
       44,  -1,  45,  46,  47,  -1,  -1,  48,  -1,  49,
       50,  51,  52,  53,  -1,  -1,  -1,  -1,  -1,  54,
       -1,  55,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  56,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       57,  -1,  58,  59,  -1,  60,  -1,  -1,  -1,  -1,
       61,  -1,  -1,  -1,  62,  63,  -1,  -1,  64,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  65,  -1,  -1,
       66,  -1,  -1,  -1,  67,  68,  -1,  -1,  -1,  69,
       -1,  70,  71,  -1,  -1,  -1,  -1,  72,  73,  -1,
       74,  75,  -1,  -1,  76,  77,  78,  79,  80,  81,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  82,
       -1,  -1,  83,  -1,  -1,  84,  85,  -1,  -1,  -1,
       86,  87,  -1,  88,  -1,  -1,  -1,  89,  90,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  91,  -1,  -1,  92,
       -1,  93,  94,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  95,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  96,  97,  -1,  -1,
       -1,  98,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  99, 100,  -1, 101,  -1,  -1,  -1,  -1,  -1,
       -1, 102,  -1,  -1, 103,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 104, 105, 106, 107,  -1,  -1,
       -1, 108,  -1,  -1,  -1, 109,  -1,  -1,  -1, 110,
       -1,  -1,  -1,  -1, 111,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 112, 113,  -1,  -1,  -1,
       -1, 114,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 115,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 116,
       -1,  -1,  -1,  -1, 117,  -1,  -1,  -1,  -1,  -1,
      118, 119,  -1,  -1,  -1, 120,  -1,  -1,  -1,  -1,
       -1,  -1, 121,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 122,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 123,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      124,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 125,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 126
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash_attr (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist_attr[index].name;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
                return &wordlist_attr[index];
            }
          else if (index < -TOTAL_KEYWORDS)
            {
              register int offset = - 1 - TOTAL_KEYWORDS - index;
              register const struct attrs *wordptr = &wordlist_attr[TOTAL_KEYWORDS + lookup[offset]];
              register const struct attrs *wordendptr = wordptr + -lookup[offset + 1];

              while (wordptr < wordendptr)
                {
                  register const char *s = wordptr->name;

                  if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
                    return wordptr;
                  wordptr++;
                }
            }
        }
    }
  return 0;
}


static const DOMString attrList[] = {
    "ABBR",
    "ACCEPT-CHARSET",
    "ACCEPT",
    "ACCESSKEY",
    "ACTION",
    "ALIGN",
    "ALINK",
    "ALT",
    "ARCHIVE",
    "AXIS",
    "BACKGROUND",
    "BGCOLOR",
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
    "PLAIN",
    "PLUGINPAGE",
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
    "TYPE",
    "USEMAP",
    "VALIGN",
    "VALUE",
    "VALUETYPE",
    "VERSION",
    "VLINK",
    "VSPACE",
    "WIDTH",
    "WRAP",
    0
};
const DOMString &getAttrName(unsigned short id)
{
    return attrList[id-1];
};
