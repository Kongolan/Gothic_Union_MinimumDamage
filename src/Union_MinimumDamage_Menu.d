META
{
    Parser = Menu;
    MergeMode = true;
};

// --- OPTION 1: MODUS (Fester Wert vs. Dynamisch) ---
INSTANCE MENUITEM_OPT_UNION_MINDAMAGE_MODE(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    text[0]     = "Schadens-Modus";
    text[1]     = "Dynamisch (nach Attributen) oder Fester Wert?";
    posX        = 1000;
    posY        = 7500;
    dimX        = 3000;
    dimY        = 750;
    fontName    = MENU_FONT_SMALL;
    flags       = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_OPT_UNION_MINDAMAGE_MODE_CHOICE(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    type        = MENU_ITEM_CHOICEBOX;
    text[0]     = "Fester Wert|Dynamisch"; // Index 0 = Fest, Index 1 = Dynamisch
    fontName    = MENU_FONT_SMALL;
    posX        = 5000;
    posY        = 7500;
    dimX        = MENU_SLIDER_DX;
    dimY        = MENU_SLIDER_DY;
    
    onChgSetOption = "DynamicMode";
    onChgSetOptionSection = "UNION_MINIMUM_DAMAGE";
    
    flags       = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

// --- OPTION 2: DER FESTE WERT (0 bis 50) ---
INSTANCE MENUITEM_OPT_UNION_MINDAMAGE_VAL(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    text[0]     = "Fester Mindestschaden";
    text[1]     = "Greift nur, wenn Schadens-Modus auf 'Fester Wert' steht.";
    posX        = 1000;
    posY        = 8000;
    dimX        = 3000;
    dimY        = 750;
    fontName    = MENU_FONT_SMALL;
    flags       = IT_CHROMAKEYED | IT_TRANSPARENT | IT_SELECTABLE | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_OPT_UNION_MINDAMAGE_VAL_CHOICE(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    type        = MENU_ITEM_CHOICEBOX;
    // Weil der Text bei 0 anfaengt, ist Index = echter Wert! (Index 5 schreibt 5 in die INI)
    text[0]     = "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50";
    fontName    = MENU_FONT_SMALL;
    posX        = 5000;
    posY        = 8000; 
    dimX        = MENU_SLIDER_DX;
    dimY        = MENU_SLIDER_DY;
    
    onChgSetOption = "MinDamageValue";
    onChgSetOptionSection = "UNION_MINIMUM_DAMAGE";
    
    flags       = IT_CHROMAKEYED | IT_TRANSPARENT | IT_TXT_CENTER;
};

// --- EINTRAG INS GOTHIC MENÜ ---
INSTANCE MENU_OPT_GAME(C_MENU_DEF)
{
    items[19] = "MENUITEM_OPT_UNION_MINDAMAGE_MODE";
    items[20] = "MENUITEM_OPT_UNION_MINDAMAGE_MODE_CHOICE";
    items[21] = "MENUITEM_OPT_UNION_MINDAMAGE_VAL";
    items[22] = "MENUITEM_OPT_UNION_MINDAMAGE_VAL_CHOICE";
};