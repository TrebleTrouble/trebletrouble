/******************************************************************************/
/* This file has been generated by the uGFX-Studio                            */
/*                                                                            */
/* http://ugfx.org                                                            */
/******************************************************************************/

#include "colors.h"
#include "widgetstyles.h"
#include "gui.h"

#include "gwin/gwin_textedit.h"

// GListeners
GListener glistener;

// GHandles
GHandle ghContainerPage0;
GHandle ghContainer1;
GHandle ghButton1_2;
GHandle ghButton1_1;
GHandle ghButton1;
GHandle ghTextedit1;

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
	wi.text = "Compose Mode";
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

	// Create textedit widget: ghTextedit1
	/* wi.g.show = TRUE; */
	/* wi.g.x = 180; */
	/* wi.g.y = 10; */
	/* wi.g.width = 450; */
	/* wi.g.height = 50; */
	/* wi.g.parent = ghContainerPage0; */
	/* wi.text = "Welcome to Treble Trouble "; */
	/* wi.customDraw = gwinTexteditDefaultDraw; */
	/* wi.customParam = 0; */
	/* wi.customStyle = 0; */
	/* ghTextedit1 = gwinTexteditCreate(0, &wi, 0); */
	/* gwinSetFont(ghTextedit1, dejavu_sans_32); */
	/* gwinRedraw(ghTextedit1); */
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
	/* dejavu_sans_16 = gdispOpenFont("DejaVuSans16"); */
	/* dejavu_sans_16_anti_aliased = gdispOpenFont("DejaVuSans16_aa"); */
	/* dejavu_sans_32 = gdispOpenFont("DejaVuSans32"); */

	// Prepare images

	// GWIN settings
	gwinWidgetClearInit(&wi);
	/* gwinSetDefaultFont(dejavu_sans_16); */
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
		switch (pe->type) {
		}

	}
}

