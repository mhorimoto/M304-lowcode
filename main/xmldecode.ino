#include <yxml.h>

//int c;
//yxml_ret_t r;
//yxml_t yx[1];
char stack[32];

void y_printchar(char c) {
  if(c == '\x7F' || (c >= 0 && c < 0x20)) {
    Serial.print("\\x");
    Serial.print(c, HEX); 
  }
  else
    Serial.print(c); 
}

void y_printstring(const char *str) {
  while(*str) {
    y_printchar(*str);
    str++;
  }
}

void y_printtoken(yxml_t *yx, char *str) {
  Serial.println("");
  Serial.print(str);
}

void y_printres(yxml_t *yx, yxml_ret_t r) {
  static int indata;
  int verbose = 0;
  int nextdata = 0;

  switch(r) {
  case YXML_OK:
    if (verbose) {
      y_printtoken(yx, "ok");
      nextdata = 0;
    }
    else
      nextdata = indata;
    break;
  case YXML_ELEMSTART:
    y_printtoken(yx, "elemstart ");
    y_printstring(yx->elem);
    if (yxml_symlen(yx, yx->elem) != strlen(yx->elem))
      y_printtoken(yx, "assertfail: elem lengths don't match");
    if (r & YXML_CONTENT)
      y_printtoken(yx, "content");
    break;
  case YXML_ELEMEND:
    y_printtoken(yx, "elemend");
    break;
  case YXML_ATTRSTART:
    y_printtoken(yx, "attrstart ");
    y_printstring(yx->attr);
    if (yxml_symlen(yx, yx->attr) != strlen(yx->attr))
      y_printtoken(yx, "assertfail: attr lengths don't match");
    break;
  case YXML_ATTREND:
    y_printtoken(yx, "attrend");
    break;
  case YXML_PICONTENT:
  case YXML_CONTENT:
  case YXML_ATTRVAL:
    if (!indata)
      y_printtoken(yx, r == YXML_CONTENT ? "content " : r == YXML_PICONTENT ? "picontent " : "attrval ");
    y_printstring(yx->data);
    nextdata = 1;
    break;
  case YXML_PISTART:
    y_printtoken(yx, "pistart ");
    y_printstring(yx->pi);
    if (yxml_symlen(yx, yx->pi) != strlen(yx->pi))
      y_printtoken(yx, "assertfail: pi lengths don't match");
    break;
  case YXML_PIEND:
    y_printtoken(yx, "piend");
    break;
  default:
    y_printtoken(yx, "error\n");
    exit(0);
  }
  indata = nextdata;
}

void xmldecode(char *xml) {
  yxml_t *yx;
  yxml_ret_t r;
  yxml_init(yx, stack, sizeof(stack));

  while (*xml) {
    r = yxml_parse(yx, *xml);
    y_printres(yx, r);
    xml++;
  }
  y_printtoken(yx, yxml_eof(yx) < 0 ? "error\n" : "ok\n");
}
