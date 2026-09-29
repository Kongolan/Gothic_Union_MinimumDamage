META
{
    Parser = Menu;
    MergeMode = true;
};

INSTANCE MENUITEM_OPT_MINDAMAGE(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    text[0]     = "Minimaler Schaden";
    text[1]     = "Berechnung des Mindestschadens wählen.";
    posX        = 1000;
    posY        = 6000;
    dimX        = 3000;
    dimY        = 750;
    fontName    = MENU_FONT_SMALL;
    flags       = flags | IT_EFFECTS_NEXT;
};

INSTANCE MENUITEM_OPT_MINDAMAGE_CHOICE(C_MENU_ITEM_DEF)
{
    backpic     = MENU_ITEM_BACK_PIC;
    type        = MENU_ITEM_CHOICEBOX;
    text[0]     = "Dynamisch|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50";
    fontName    = MENU_FONT_SMALL;
    posX        = 5000;
    posY        = 6000; 
    dimX        = MENU_SLIDER_DX;
    dimY        = MENU_SLIDER_DY;
    onChgSetOption = "MinDamageValue";
    onChgSetOptionSection = "ZMODMINDAMAGE";
    flags       = flags & ~IT_SELECTABLE;
    flags       = flags | IT_TXT_CENTER;
};

INSTANCE MENU_OPT_GAME(C_MENU_DEF)
{
    items[60] = "MENUITEM_OPT_MINDAMAGE";
    items[61] = "MENUITEM_OPT_MINDAMAGE_CHOICE";
};
