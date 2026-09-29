META
{
    Parser = Menu;
    MergeMode = true;
};

// ==========================================
// 1. DEIN ISOLIERTES UNTERMENÜ
// ==========================================
INSTANCE MENU_OPT_UNION_MINDAMAGE(C_MENU_DEF)
{
    backpic        = MENU_BACK_PIC;
    dimx           = 8192;
    dimy           = 8192;
    alpha          = 254;
    flags          = MENU_EXCLUSIVE;
    
    // Dein eigenes, sicheres Array - hier crasht nichts beim Back-Button!
    items[0]       = "MENUITEM_UNION_MINDAMAGE_HEADLINE";
    items[1]       = "MENUITEM_UNION_MINDAMAGE_MODE";
    items[2]       = "MENUITEM_UNION_MINDAMAGE_MODE_CHOICE";
    items[3]       = "MENUITEM_UNION_MINDAMAGE_VAL";
    items[4]       = "MENUITEM_UNION_MINDAMAGE_VAL_CHOICE";
    
    // Dein eigener Zurück-Button auf einem sicheren Index
    items[14]      = "MENUITEM_UNION_MINDAMAGE_BACK";
};

// ==========================================
// 2. DIE INHALTE DEINES MENÜS
// ==========================================
INSTANCE MENUITEM_UNION_MINDAMAGE_HEADLINE(C_MENU_ITEM_DEF)
{
    text[0]        = "MINIMUM DAMAGE EINSTELLUNGEN";
    type           = MENU_ITEM_TEXT;
    posx           = 0;
    posy           = 2000;
    dimx           = 8100;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

INSTANCE MENUITEM_UNION_MINDAMAGE_MODE(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    text[0]        = "Schadens-Modus";
    text[1]        = "Dynamisch (nach Attributen) oder Fester Wert?";
    posx           = 1000;
    posy           = 4000;
    dimx           = 3000;
    dimy           = 750;
    fontName       = MENU_FONT_SMALL;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_UNION_MINDAMAGE_MODE_CHOICE(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    type           = MENU_ITEM_CHOICEBOX;
    text[0]        = "Fester Wert|Dynamisch";
    fontName       = MENU_FONT_SMALL;
    posx           = 5000;
    posy           = 4000;
    dimx           = MENU_SLIDER_DX;
    dimy           = MENU_SLIDER_DY;
    onChgSetOption = "DynamicMode";
    onChgSetOptionSection = "UNION_MINIMUM_DAMAGE";
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

INSTANCE MENUITEM_UNION_MINDAMAGE_VAL(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    text[0]        = "Fester Mindestschaden";
    text[1]        = "Greift nur, wenn Modus auf 'Fester Wert' steht.";
    posx           = 1000;
    posy           = 5000;
    dimx           = 3000;
    dimy           = 750;
    fontName       = MENU_FONT_SMALL;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_UNION_MINDAMAGE_VAL_CHOICE(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    type           = MENU_ITEM_CHOICEBOX;
    text[0]        = "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50";
    fontName       = MENU_FONT_SMALL;
    posx           = 5000;
    posy           = 5000; 
    dimx           = MENU_SLIDER_DX;
    dimy           = MENU_SLIDER_DY;
    onChgSetOption = "MinDamageValue";
    onChgSetOptionSection = "UNION_MINIMUM_DAMAGE";
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

INSTANCE MENUITEM_UNION_MINDAMAGE_BACK(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    text[0]        = "Zurueck";
    type           = MENU_ITEM_BUTTON;
    posx           = 1000; 
    posy           = 8000; 
    dimx           = 6192;
    dimy           = 750;
    onSelAction[0] = SEL_ACTION_BACK; // Schließt dein Untermenü sauber
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_TXT_CENTER;
};

// ==========================================
// 3. DER EINSPRUNG-BUTTON INS UNION-MENÜ
// ==========================================
INSTANCE MENUITEM_UNION_MINDAMAGE_ENTRY(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    text[0]        = "Minimum Damage Optionen"; 
    text[1]        = "Modus und Werte fuer den Mindestschaden konfigurieren.";
    type           = MENU_ITEM_BUTTON;
    posx           = 1000;
    posy           = 1000; 
    dimx           = 6192;
    dimy           = 750;
    onSelAction[0] = SEL_ACTION_STARTMENU;
    onEventAction[1] = "MENU_OPT_UNION_MINDAMAGE"; // Öffnet DEIN erstelltes Untermenü
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_TXT_CENTER;
};

// ==========================================
// 4. INJIZIERUNG IN DAS OFFIZIELLE UNION-MENÜ
// ==========================================
INSTANCE MENU_OPT_UNION(C_MENU_DEF)
{
    // Wir nutzen den Index 60, um uns ganz unten ungefährlich anzuhängen
    items[60] = "MENUITEM_UNION_MINDAMAGE_ENTRY";
};