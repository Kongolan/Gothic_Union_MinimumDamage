META
{
    Parser    = Menu;
    After     = zUnionMenu.d; // Wartet, bis die Union-Basis geladen ist
    Namespace = MinDamage;    // Setzt das saubere Präfix "MinDamage:"
};

// ==========================================
// 1. DER AUTOMATISCHE EINSPRUNG-BUTTON
// ==========================================
// Durch "C_MENU_ITEM_UNION_DEF" fügt Union diesen Button vollautomatisch 
// in das "Optionen -> Union" Menü ein! Kein C++ Hook mehr nötig!
INSTANCE MenuItem_Union_Auto_MinDamage(C_MENU_ITEM_UNION_DEF)
{
    text[0]        = "Minimum Damage Optionen"; 
    text[1]        = "Modus und Werte fuer den Mindestschaden konfigurieren.";
    onSelAction[0] = SEL_ACTION_STARTMENU;
    onSelAction_S[0] = "MinDamage:MENU_OPT_MINDAMAGE"; // Beachte das Namespace-Präfix!
};

// ==========================================
// 2. DEIN EIGENES MENÜ (Dynamisch befüllt)
// ==========================================
INSTANCE MENU_OPT_MINDAMAGE(C_MENU_DEF)
{
    backpic        = MENU_BACK_PIC;
    dimx           = 8192;
    dimy           = 8192;
    alpha          = 254;
    flags          = MENU_EXCLUSIVE;
    
    // DER MAGISCHE BEFEHL: Sucht alle Items, die mit "MENUITEM_OPT_MINDAMAGE_" anfangen 
    // und fügt sie automatisch ins Array ein. Keine Index-Crashes mehr!
    Menu_SearchItems("MinDamage:MENUITEM_OPT_MINDAMAGE_*");
};

// ==========================================
// 3. DIE MENÜ-INHALTE
// ==========================================

INSTANCE MENUITEM_OPT_MINDAMAGE_01_HEADLINE(C_MENU_ITEM_DEF)
{
    text[0]        = "MINIMUM DAMAGE EINSTELLUNGEN";
    type           = MENU_ITEM_TEXT;
    posx           = 0;
    posy           = 2000;
    dimx           = 8100;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

INSTANCE MENUITEM_OPT_MINDAMAGE_02_MODE(C_MENU_ITEM_DEF)
{
    text[0]        = "Schadens-Modus";
    text[1]        = "Dynamisch (nach Attributen) oder Fester Wert?";
    posx           = 1000;
    posy           = 4000;
    dimx           = 3000;
    dimy           = 750;
    fontName       = MENU_FONT_SMALL;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_OPT_MINDAMAGE_03_MODE_CHOICE(C_MENU_ITEM_DEF)
{
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

INSTANCE MENUITEM_OPT_MINDAMAGE_04_VAL(C_MENU_ITEM_DEF)
{
    text[0]        = "Fester Mindestschaden";
    text[1]        = "Greift nur, wenn Modus auf 'Fester Wert' steht.";
    posx           = 1000;
    posy           = 5000;
    dimx           = 3000;
    dimy           = 750;
    fontName       = MENU_FONT_SMALL;
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_OPT_MINDAMAGE_05_VAL_CHOICE(C_MENU_ITEM_DEF)
{
    type           = MENU_ITEM_CHOICEBOX;
    text[0]        = "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20";
    fontName       = MENU_FONT_SMALL;
    posx           = 5000;
    posy           = 5000; 
    dimx           = MENU_SLIDER_DX;
    dimy           = MENU_SLIDER_DY;
    onChgSetOption = "MinDamageValue";
    onChgSetOptionSection = "UNION_MINIMUM_DAMAGE";
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

INSTANCE MENUITEM_OPT_MINDAMAGE_99_BACK(C_MENU_ITEM_DEF)
{
    backpic        = MENU_ITEM_BACK_PIC;
    text[0]        = "Zurueck";
    type           = MENU_ITEM_BUTTON;
    posx           = 1000; 
    posy           = 8000; 
    dimx           = 6192;
    dimy           = 750;
    onSelAction[0] = SEL_ACTION_BACK; 
    flags          = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_TXT_CENTER;
};