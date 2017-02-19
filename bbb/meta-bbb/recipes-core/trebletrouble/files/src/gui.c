/*
 *  Copyright (C) 2016-2017  Emilie Cobbold
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

/******************************************************************************/
/* This file has been generated by the uGFX-Studio                            */
/*                                                                            */
/* http://ugfx.org                                                            */
/******************************************************************************/

#include "colors.h"
#include "widgetstyles.h"

#include "gui.h"
#include "menu.h"

#include "src/gdisp/gdisp.h"
#include "src/gdisp/gdisp_pixmap.h"

#include "src/gwin/gwin_label.h"

// GListeners
GListener glistener;

// GHandles
GHandle ghContainerPage0;
GHandle ghContainer1;
GHandle ghButton1_2;
GHandle ghButton1_1;
GHandle ghButton1;
GHandle ghLabel1;

// Fonts
font_t dejavu_sans_16;
font_t dejavu_sans_16_anti_aliased;
font_t dejavu_sans_32;

static void createPagePage0(void)
{
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);


	// create container widget: ghContainerPage0
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 800;
	wi.g.height = 400;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainerPage0 = gwinContainerCreate(0, &wi, 0);

	// create container widget: ghContainer1
	wi.g.show = TRUE;
	wi.g.x = 90;
	wi.g.y = 40;
	wi.g.width = 620;
	wi.g.height = 360;
	wi.g.parent = ghContainerPage0;
	wi.text = "Container1";
	wi.customDraw = gwinContainerDraw_Std;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer1 = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

	// create button widget: ghButton1_2
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 260;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	wi.text = "Settings";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1_2 = gwinButtonCreate(0, &wi);
	gwinSetFont(ghButton1_2, dejavu_sans_16_anti_aliased);
	gwinRedraw(ghButton1_2);

	// create button widget: ghButton1_1
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 150;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	/* wi.text = "Compose Mode"; */
	wi.text = "Metronome";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1_1 = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 40;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	wi.text = "Practice Mode";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1 = gwinButtonCreate(0, &wi);

	// Create label widget: ghLabel1
	/* wi.g.show = TRUE; */
	/* wi.g.x = 280; */
	/* wi.g.y = 290; */
	/* wi.g.width = 120; */
	/* wi.g.height = 20; */
	/* wi.g.parent = ghButton1_2; */
	/* wi.text = "Settings"; */
	/* wi.customDraw = gwinLabelDrawJustifiedLeft; */
	/* wi.customParam = 0; */
	/* wi.customStyle = 0; */
	/* ghLabel1 = gwinLabelCreate(0, &wi); */

}

void guiShowPage(unsigned pageIndex)
{
	// Hide all pages
	gwinHide(ghContainerPage0);

	// Show page selected page
	switch (pageIndex) {
	case 0:
		gwinShow(ghContainerPage0);
		break;

	default:
		break;
	}
}

void guiCreate(void)
{
	GWidgetInit wi;

	// Prepare fonts
	dejavu_sans_16 = gdispOpenFont("DejaVuSans16");
	dejavu_sans_16_anti_aliased = gdispOpenFont("DejaVuSans16_aa");
	dejavu_sans_32 = gdispOpenFont("DejaVuSans32");

	// Prepare images

	// GWIN settings
	gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&white, FALSE);
	gwinSetDefaultColor(black_studio);
	gwinSetDefaultBgColor(magenta_studio);

	// Create all the display pages
	createPagePage0();

	// Select the default display page
	guiShowPage(0);

}

void guiEventLoop(void)
{
	GEvent* pe;

	while (1) {
		// Get an event
		pe = geventEventWait(&glistener, 0);
		if (!pe)
			continue;
		printf("Valid PE\n");
		switch (pe->type) {
		case GEVENT_GWIN_BUTTON:
			if (((GEventGWinButton*)pe)->gwin == ghButton1) {
				printf("Button 1\n");
				play_song_menu(gdispPixmapGetBits(gdispGetDisplay(0)), NULL);
			} else if (((GEventGWinButton*)pe)->gwin == ghButton1_1) {
				printf("Button 1 1\n");
				metronome_menu(gdispPixmapGetBits(gdispGetDisplay(0)), NULL);
			}
			break;
		default:
			break;
		}

	}
}
