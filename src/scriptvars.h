#pragma once

namespace Impacto {

enum ScriptVars {
  LR_DATE = 1029,
  SW_MESWINDOW0ALPHA = 2105,
  SW_MESWINDOW1ALPHA = 2106,
  SW_MESWINDOW2ALPHA = 2107,
  SW_TITLEMODE = 2115,
  SW_TITLEMOVIECT = 2116,
  SW_TITLEDISPCT = 2119,
  SW_SYSMENUALPHA = 2143,
  SW_OPTIONCHAALPHA = 2172,
  SW_OPTIONCHANO = 2173,
  SW_BG1POSX_OFS = 2500,
  SW_BG1POSY_OFS = 2501,
  SW_BG1SX_OFS = 2502,
  SW_BG1SY_OFS = 2503,
  SW_BG1SIZE_OFS = 2504,
  SW_BG1LX_OFS = 2505,
  SW_BG1LY_OFS = 2506,
  SW_MASK1ALPHA_OFS = 2786,
  SW_MASK2ALPHA_OFS = 2787,
  SW_MASK3ALPHA_OFS = 2788,
  SW_SAVEERRORCODE = 3333,
  SW_TITLECUR1 = 3341,
  SW_TITLECUR2 = 3342,
  // C;H LCC
  // SW_TITLECUR1 = 1741,
  // SW_TITLECUR2 = 1742,
  // MO6TW
  // SW_TITLECUR1 = 1740,
  // SW_TITLECUR2 = 1741,
  SW_SYSMESALPHA = 3370,
  SW_SYSMESANIMCTCUR = 3371,
  SW_SYSMESANIMCTF = 3372,
  SW_BG1SURF = 3400,
  SW_SCRIPTNO0 = 4340,
  SW_SCRIPTNO1 = 4341,
  SW_SCRIPTNO2 = 4342,
  SW_SCRIPTNO3 = 4343,
  SW_SCRIPTNO4 = 4344,
  SW_SCRIPTNO5 = 4345,
  SW_SCRIPTNO6 = 4346,
  SW_SCRIPTNO7 = 4347,
  SW_SCRIPTNO8 = 4348,
  SW_SCRIPTNO9 = 4349,
  SW_SCRIPTNO10 = 4350,
  SW_SCRIPTNO11 = 4351,
  SW_SCRIPTNO12 = 4352,
  SW_SCRIPTNO13 = 4353,
  SW_SCRIPTNO14 = 4354,
  SW_SCRIPTNO15 = 4355,
  SW_MASK1COLOR = 4450,
  SW_MASK1ALPHA = 4451,
  SW_MASK1PRI = 4452,
  SW_MASK1POSX = 4453,
  SW_MASK1POSY = 4454,
  SW_MASK1SIZEX = 4455,
  SW_MASK1SIZEY = 4456,
  // Darling
  // SW_MASK1COLOR = 4400,
  // SW_MASK1ALPHA = 4401,
  // SW_MASK1PRI = 4402,
  // SW_MASK1POSX = 4403,
  // SW_MASK1POSY = 4404,
  // SW_MASK1SIZEX = 4405,
  // SW_MASK1SIZEY = 4406,
  // C;H LCC
  // SW_MASK1COLOR = 2340,
  // SW_MASK1ALPHA = 2341,
  // SW_MASK1PRI = 2342,
  // SW_MASK1POSX = 2343,
  // SW_MASK1POSY = 2344,
  // SW_MASK1SIZEX = 2345,
  // SW_MASK1SIZEY = 2346,
  // MO6TW
  // SW_MASK1COLOR = 2329,
  // SW_MASK1ALPHA = 2330,
  // SW_MASK1PRI = 2331,
  // SW_MASK1POSX = 2332,
  // SW_MASK1POSY = 2333,
  // SW_MASK1SIZEX = 2334,
  // SW_MASK1SIZEY = 2335,
  SW_MESWIN0POSX = 4420,
  SW_MESWIN0POSY = 4421,
  SW_MESWIN0TYPE = 4422,
  SW_MESMODE0 = 4423,
  SW_BG1POSX = 4500,
  SW_BG1POSY = 4501,
  SW_BG1SX = 4502,
  SW_BG1SY = 4503,
  SW_BG1SIZE = 4504,
  SW_BG1LX = 4505,
  SW_BG1LY = 4506,
  SW_BG1NO = 4507,
  SW_BG1PRI = 4508,
  SW_CHA1POSX = 5100,
  SW_CHA1POSY = 5101,
  SW_CHA1POSZ = 5102,
  SW_CHA1ROTX = 5103,
  SW_CHA1ROTY = 5104,
  SW_CHA1ROTZ = 5105,
  SW_CHA1NO = 5109,
  SW_CHA1YCENTER = 5111,
  SW_CHA1POSE = 5112,
  SW_CHA1EX = 5113,
  SW_IRUOCAMERAPOSX = 5400,
  SW_IRUOCAMERAPOSY = 5401,
  SW_IRUOCAMERAPOSZ = 5402,
  SW_IRUOCAMERAROTY = 5403,
  SW_IRUOCAMERAROTX = 5404,
  SW_IRUOCAMERAHFOV = 5408,
  SW_MAINCAMERAPOSX = 5420,
  SW_MAINCAMERAPOSY = 5421,
  SW_MAINCAMERAPOSZ = 5422,
  SW_MAINCAMERAROTY = 5423,
  SW_MAINCAMERAROTX = 5424,
  SW_MAINCAMERAHFOV = 5428,
  SW_MAINLIGHTCOLOR = 5460,
  SW_MAINLIGHTPOSX = 5461,
  SW_MAINLIGHTPOSY = 5462,
  SW_MAINLIGHTPOSZ = 5463,
  SW_MAINLIGHTWEIGHT = 5464,
  SW_MAINLIGHTDARKMODE = 5465,

};

enum FlagVars {
  SF_MESALLSKIP = 1234,
  SF_TITLEMODE = 1240,
  SF_LOADING = 1264,
  SF_LOADINGFROMSAVE = 1835,
  SF_BG1DISP = 2400,
  SF_CHA1DISP = 2410,
  SF_DATEDISPLAY = 2640,
  SF_IRUOENABLE = 2800,
  SF_IRUO = 2816
};

}  // namespace Impacto