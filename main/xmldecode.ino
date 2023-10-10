// typedef struct st_UECSXML {
//   byte element;
//   char ver[21];
//   char type[21];
//   byte page,room,region,priority;
//   int  order;
//   char textval[21];
//   float fval;
// };

void y_printchar(char c) {
  if(c == '\x7F' || (c >= 0 && c < 0x20)) {
    Serial.print("\\x");
    Serial.print(c, HEX); 
  }
  else
    Serial.print(c); 
}

void y_printstring(char *str) {
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

bool xmldecode(char *xml) {
  byte element,attr;
  char *attr_val,aval[100];
  static char stack[32];
  yxml_t yx[1];
  yxml_ret_t r;
  extern st_UECSXML *ptr_uecsxmldata;

  attr_val = &aval[0];
  element = 0;
  attr = 0;
  yxml_init(yx, stack, sizeof(stack));
  uecsxmldata_init();
  while (*xml) {
    r = yxml_parse(yx, *xml);
    switch(r) {
    case YXML_OK:
      break;
    case YXML_ELEMSTART:
      element = chooseElement(yx->elem);
      switch(element) {
      case ELE_UECS:
      case ELE_NODESCAN:
      case ELE_CCMSCAN:
      case ELE_SEARCH:
        ptr_uecsxmldata->element = element;
        break;
      case ELE_DATA:
        break;
      case ELE_REQUEST:
        break;
      default:
        Serial.println("UNKNOWN ELEMENT");
        return;
      }
    case YXML_ELEMEND:
      break;
    case YXML_ATTRSTART:
      attr = chooseAttr(yx->attr);
      break;
    case YXML_ATTREND:
      *attr_val = NULL;
      if ((element==ELE_UECS)&&(attr==ATTR_VER)) {
        //        Serial.println("UECS-VER");
        //        Serial.println(aval);
        strncpy(ptr_uecsxmldata->ver,aval,LEN_UECSXML_VER);
      } else if ((element==ELE_CCMSCAN)&&(attr==ATTR_PAGE)) {
        Serial.println("CCMSCAN-PAGE");
        //        Serial.println(aval);
        ptr_uecsxmldata->page = (byte)(atoi(aval));
      } else if (element==ELE_SEARCH) {
        switch(attr) {
        case ATTR_TYPE:
          Serial.println("SEARCH-TYPE");
          //          Serial.println(aval);
          strncpy(ptr_uecsxmldata->type,aval,LEN_UECSXML_TYPE);
          break;
        case ATTR_ROOM:
          Serial.println("SEARCH-ROOM");
          //        Serial.println(aval);
          ptr_uecsxmldata->room = (byte)(atoi(aval));
          break;
        case ATTR_REGION:
          Serial.println("SEARCH-REGION");
          //        Serial.println(aval);
          ptr_uecsxmldata->region = (byte)(atoi(aval));
          break;
        case ATTR_ORDER:
          Serial.println("SEARCH-ORDER");
          //          Serial.println(aval);
          ptr_uecsxmldata->order = (int)(atoi(aval));
          break;
        }
      }
      attr_val = &aval[0];
      break;
    case YXML_PICONTENT:
    case YXML_CONTENT:
    case YXML_ATTRVAL:
      *attr_val = *yx->data;
      attr_val++;
      break;
    case YXML_PISTART:
    case YXML_PIEND:
    default:
      break;
    }
    xml++;
  }
  if (yxml_eof(yx) < 0) {
    return(false);
  } else {
    return(true);
  }
}

byte chooseElement(char *ce) {
  if (!strcmp(ce,"UECS"))     return(ELE_UECS);
  if (!strcmp(ce,"NODESCAN")) return(ELE_NODESCAN);
  if (!strcmp(ce,"CCMSCAN"))  return(ELE_CCMSCAN);
  if (!strcmp(ce,"DATA"))     return(ELE_DATA);
  if (!strcmp(ce,"REQUEST"))  return(ELE_REQUEST);
  if (!strcmp(ce,"SEARCH"))   return(ELE_SEARCH);
  return(0);  
}

byte chooseAttr(char *ca) {
  if (!strcmp(ca,"ver"))      return(ATTR_VER);
  if (!strcmp(ca,"page"))     return(ATTR_PAGE);
  if (!strcmp(ca,"type"))     return(ATTR_TYPE);
  if (!strcmp(ca,"room"))     return(ATTR_ROOM);
  if (!strcmp(ca,"region"))   return(ATTR_REGION);
  if (!strcmp(ca,"order"))    return(ATTR_ORDER);
  if (!strcmp(ca,"priority")) return(ATTR_PRIORITY);  
  return(0);
}

void uecsxmldata_init(void) {
  extern st_UECSXML *ptr_uecsxmldata;
  int i;
  
  ptr_uecsxmldata->element = 0;
  for (i=0;i<21;i++) {
    ptr_uecsxmldata->ver[i] = NULL;
    ptr_uecsxmldata->type[i] = NULL;
    ptr_uecsxmldata->textval[i] = NULL;
  }
  ptr_uecsxmldata->page = 255;
  ptr_uecsxmldata->room = 255;
  ptr_uecsxmldata->region = 255;
  ptr_uecsxmldata->priority = 255;
  ptr_uecsxmldata->order = 0;
  ptr_uecsxmldata->fval=0.0;
}
    
