#ifndef RING_MENU_H
#define RING_MENU_H

#include <inttypes.h>

// menus
#include "menus/GlobalBrightness.h"
#include "menus/FactoryReset.h"
#include "menus/ModeSharing.h"
#include "menus/ColorSelect.h"
#include "menus/PatternSelect.h"
#include "menus/Randomizer.h"

class Menu;

class Menus
{
public:
  // opting for static class here because there should only ever be one
  // Menu control object and I don't like singletons
  static bool init();

  // Run the ringmenu and any menus it contains
  // returns true if the menu remains open, false if closed
  static bool run();
  
  // whether any menus are open
  static bool shouldRun();

  // the number of menus in the ring menu
  static uint32_t numMenus();

private:
  // run the currently open menu
  static bool runCurMenu();
  // run the ring filling logic
  static bool runRingFill();
  // helper to calculate the relative hold time for the current menu
  static LedPos calcLedPos();

  // private structure for menu entry menu => color
  struct MenuEntry
  {
    MenuEntry(Menu *m, RGBColor c) :
      menu(m), color(c)
    {
    }
    Menu *menu;
    RGBColor color;
  };

  // ======================
  //  Menus
  static Randomizer m_randomizer;
  static ColorSelect m_colorSelect;
  static PatternSelect m_patternSelect;
  static GlobalBrightness m_globalBrightness;
  static FactoryReset m_factoryReset;
  static ModeSharing m_modeSharing;

  // list of menu entries above with chosen colors
  static const MenuEntry m_menuList[];

  // =====================
  //  private members

  // the ring menu section
  static uint32_t m_selection;

  // whether the ring menu is open
  static bool m_isOpen;

  // the current sub menu that is open
  static Menu *m_pCurMenu;
};

#endif
