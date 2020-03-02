/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelFunc */
#define  PANEL_GRAPH                      2       /* control type: graph, callback function: graphFunc */
#define  PANEL_ANGLE                      3       /* control type: numeric, callback function: (none) */
#define  PANEL_INITIAL_SPEED              4       /* control type: numeric, callback function: (none) */
#define  PANEL_SHOW_TIME                  5       /* control type: numeric, callback function: (none) */
#define  PANEL_FIRE_BUTTON                6       /* control type: textButton, callback function: setFunc */
#define  PANEL_LOAD_BUTTON                7       /* control type: command, callback function: loadFunc */
#define  PANEL_RESET                      8       /* control type: command, callback function: resetFunc */
#define  PANEL_LINK_BUTTON                9       /* control type: command, callback function: linkFunc */
#define  PANEL_CLEAR_GRAPH                10      /* control type: command, callback function: clearGraph */
#define  PANEL_RESTORE                    11      /* control type: command, callback function: restoreFunc */
#define  PANEL_ZOOM                       12      /* control type: command, callback function: zoomFunc */
#define  PANEL_YELLOWCURSOR_DISTANCE      13      /* control type: numeric, callback function: (none) */
#define  PANEL_YELLOWCURSOR_HEIGHT        14      /* control type: numeric, callback function: (none) */
#define  PANEL_WHITECURSOR_DISTANCE       15      /* control type: numeric, callback function: (none) */
#define  PANEL_WHITECURSOR_HEIGHT         16      /* control type: numeric, callback function: (none) */
#define  PANEL_COLORNUM_2                 17      /* control type: color, callback function: (none) */
#define  PANEL_COLORNUM                   18      /* control type: color, callback function: (none) */
#define  PANEL_SPLITTER                   19      /* control type: splitter, callback function: (none) */
#define  PANEL_DECORATION_2               20      /* control type: deco, callback function: (none) */
#define  PANEL_SET_TITLE                  21      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_3               22      /* control type: deco, callback function: (none) */
#define  PANEL_CONTROL_MUSIC              23      /* control type: binary, callback function: controlMusicFunc */
#define  PANEL_TIMER                      24      /* control type: timer, callback function: runFunc */
#define  PANEL_ZOOM_TITLE                 25      /* control type: textMsg, callback function: (none) */
#define  PANEL_VOLUME                     26      /* control type: scale, callback function: volumeFunc */
#define  PANEL_VIDEO_TITLE                27      /* control type: textMsg, callback function: (none) */
#define  PANEL_PICTURE                    28      /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK clearGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK controlMusicFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK graphFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK linkFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK loadFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK restoreFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK runFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK setFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK volumeFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK zoomFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
