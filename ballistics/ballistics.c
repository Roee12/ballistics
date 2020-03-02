#include <cvintwrk.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "ballistics.h"
#include "bass.h" 

#define M_PI 3.14159265358979323846 
 
HSTREAM Fire_SND;
HSTREAM Music_SND;

static int panelHandle;

double t=0;
double vox,voy,vy,t,g=9.81;
double xright,xleft,ybottom,ytop; // for the restore function
double x1,y1,x2,y2; 
int hitTheGround=0;							
double vol; //for the volume value
FILE *fp;

int main (int argc, char *argv[])
{
	char filename[300]="panelData";
	BASS_Init( -1,44100, 0,0,NULL);
	Fire_SND = BASS_StreamCreateFile(FALSE,"Cannon+3.mp3",0,0,0); 
	Music_SND = BASS_StreamCreateFile(FALSE,"02.mp3",0,0,0);
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ballistics.uir", PANEL)) < 0)
		return -1;
	BASS_ChannelPlay(Music_SND,TRUE);
	DisplayPanel (panelHandle);
	RecallPanelState (panelHandle, filename, 0);
	SetCtrlVal(panelHandle,PANEL_VOLUME,100.00);
	SetCtrlVal(panelHandle,PANEL_CONTROL_MUSIC,1);
	GetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, &xleft, &xright);
	GetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, &ybottom, &ytop);
	RunUserInterface ();
	BASS_StreamFree(Fire_SND);
	BASS_StreamFree(Music_SND);
	DiscardPanel (panelHandle);
	
	return 0;
}

int CVICALLBACK panelFunc (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{	
	char filename[300]="panelData";
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			SavePanelState (panelHandle, filename, 0);
			QuitUserInterface (0);
			

			break;
	}
	return 0;
}

int CVICALLBACK setFunc (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	char filename[300];
	double angle,initialSpeed,radAngle;
	int state,selection;;
	double x,y;
	char height[10]="Height";
	char distance[10] = "Distance";
	switch (event)
	{
		case EVENT_COMMIT:

			x=0.00;
			y=0.00;
			GetCtrlVal(panelHandle,PANEL_INITIAL_SPEED,&initialSpeed);
			GetCtrlVal(panelHandle,PANEL_ANGLE,&angle);
			radAngle = 2*M_PI*angle/360.00;
			vox=initialSpeed*cos (radAngle);
			voy=initialSpeed*sin (radAngle);
			SetCtrlAttribute (panelHandle, PANEL_INITIAL_SPEED, ATTR_CTRL_MODE, VAL_INDICATOR);
			SetCtrlAttribute (panelHandle, PANEL_ANGLE, ATTR_CTRL_MODE, VAL_INDICATOR);
			if (initialSpeed > 0.00 && angle > 0.00)
			{
				selection = FileSelectPopup ("", "firedata.csv", "", "Please Select a File", VAL_SAVE_BUTTON, 0, 0, 1, 0, filename);
			    if (selection !=0)
			    {
			        fp=fopen(filename,"w");
		            GetCtrlVal(panelHandle,PANEL_FIRE_BUTTON,&state);
					PlotPoint (panelHandle, PANEL_GRAPH, x, y, VAL_SOLID_CIRCLE, VAL_RED);
					fprintf(fp,"%s,%s\n",distance,height);
					fprintf(fp,"%.2lf,%.2lf\n",x,y);
			        SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED,state);
					BASS_ChannelPlay(Fire_SND,TRUE);

			    }
				else
				{
					 SetCtrlVal(panelHandle,PANEL_FIRE_BUTTON,0);
				}
			}
			else
			{
				SetCtrlVal(panelHandle,PANEL_FIRE_BUTTON,0);
				MessagePopup("Error","Select a valid intial speed and angle values!");
				SetCtrlAttribute (panelHandle, PANEL_INITIAL_SPEED, ATTR_CTRL_MODE, VAL_HOT);
				SetCtrlAttribute (panelHandle, PANEL_ANGLE, ATTR_CTRL_MODE, VAL_HOT);

			}
			break;
	}
	return 0;
}

int CVICALLBACK runFunc (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	double dt=0.01;
	double x,y;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			if (hitTheGround == 0)
			{
				
				t+=dt;
			    SetCtrlVal(panelHandle,PANEL_SHOW_TIME,t);
				x=vox*t;
				y=(voy*t)-(0.5*g*pow(t,2));
				vy=voy-(g*t);
				fprintf(fp,"%.2lf,%.2lf\n",x,y); 
				if (y > 0.01)
				{
					PlotPoint (panelHandle, PANEL_GRAPH, x, y, VAL_SOLID_CIRCLE, VAL_RED);
				}
				if (y < 0.01)
				{
					hitTheGround =1;
					GetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, &xleft, &xright);
					GetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, &ybottom, &ytop);
					MessagePopup ("Message", "The ball has hit the ground!");
					SetCtrlAttribute (panelHandle, PANEL_INITIAL_SPEED, ATTR_CTRL_MODE, VAL_HOT);
				    SetCtrlAttribute (panelHandle, PANEL_ANGLE, ATTR_CTRL_MODE, VAL_HOT);
					
				}
			}
			else
			{
			    SetCtrlVal(panelHandle,PANEL_FIRE_BUTTON,0);
				SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED,0);
				fclose(fp);
				hitTheGround=0; 
				t=0;
			}
			break;
	}
	return 0;
}



int CVICALLBACK loadFunc (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	FILE *fp2;
	char filename[300],line[50];
	int selection;
	double x,y;

	switch (event)
	{
		case EVENT_COMMIT:

			selection =FileSelectPopup ("", ".csv", "", "Please Select a File", VAL_LOAD_BUTTON, 0, 0, 1, 0, filename);
			if (selection != 0)
			{
				fp2 = fopen(filename,"r");
				fgets(line,49,fp2);
				while(fgets(line,49,fp2) != NULL)
				{	
					sscanf(line,"%lf,%lf",&x,&y);
					if (y > 0.01)
						PlotPoint (panelHandle, PANEL_GRAPH, x, y, VAL_SOLID_CIRCLE, VAL_GREEN);
					else
						PlotPoint (panelHandle, PANEL_GRAPH, x, 0.00, VAL_SOLID_CIRCLE, VAL_GREEN);  
				}
				fclose(fp2);   
			}

			break;
	}
	return 0;
}

int CVICALLBACK resetFunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			t=0;
			hitTheGround =0;
			SetCtrlVal(panelHandle,PANEL_INITIAL_SPEED,0.00);
			SetCtrlVal(panelHandle,PANEL_ANGLE,0.00);
			SetCtrlVal(panelHandle,PANEL_SHOW_TIME,0.00);
			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			SetCtrlAttribute (panelHandle, PANEL_INITIAL_SPEED, ATTR_CTRL_MODE, VAL_HOT);
			SetCtrlAttribute (panelHandle, PANEL_ANGLE, ATTR_CTRL_MODE, VAL_HOT);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.00, 100.00);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, 0.00, 100.00);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_AUTOSCALE, xleft, xright);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_AUTOSCALE, ybottom, ytop);
			
			break;
	}
	return 0;
}

int CVICALLBACK linkFunc (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				InetLaunchDefaultWebBrowser ("https://www.youtube.com/watch?v=OpffzW9P36I");
			break;
	}
	return 0;
}

void swap(double *a, double *b)
{
    double temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

int CVICALLBACK zoomFunc (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	double xMin, xMax, yMin, yMax;
	switch (event)
	{
		case EVENT_COMMIT:
			GetGraphCursor (panelHandle, PANEL_GRAPH, 1, &xMin, &yMin);
            GetGraphCursor (panelHandle, PANEL_GRAPH, 2, &xMax, &yMax);
            if (xMin > xMax) 
			{
				swap(&xMin, &xMax);
			}
            if (yMin > yMax) 
			{
				swap(&yMin, &yMax);
			}
            if (xMin == xMax) 
			{
				xMax += 0.001;
			}
            if (yMin == yMax) 
			{
				yMax += 0.001;
			}
            SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, xMin, xMax);
            SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, yMin, yMax);
			GetGraphCursor (panelHandle, PANEL_GRAPH, 1, &x1, &y1);
			SetCtrlVal(panelHandle,PANEL_WHITECURSOR_DISTANCE,x1);
			SetCtrlVal(panelHandle,PANEL_WHITECURSOR_HEIGHT,y1);
			GetGraphCursor (panelHandle, PANEL_GRAPH, 2, &x2, &y2);
			SetCtrlVal(panelHandle,PANEL_YELLOWCURSOR_DISTANCE,x2);
			SetCtrlVal(panelHandle,PANEL_YELLOWCURSOR_HEIGHT,y2);
			break;
	}
	return 0;
}



int CVICALLBACK graphFunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetGraphCursor (panelHandle, PANEL_GRAPH, 1, &x1, &y1);
			SetCtrlVal(panelHandle,PANEL_WHITECURSOR_DISTANCE,x1);
			SetCtrlVal(panelHandle,PANEL_WHITECURSOR_HEIGHT,y1);
			GetGraphCursor (panelHandle, PANEL_GRAPH, 2, &x2, &y2);
			SetCtrlVal(panelHandle,PANEL_YELLOWCURSOR_DISTANCE,x2);
			SetCtrlVal(panelHandle,PANEL_YELLOWCURSOR_HEIGHT,y2);
			break;
	}
	return 0;
}


int CVICALLBACK controlMusicFunc (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int state;
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal(panelHandle,PANEL_CONTROL_MUSIC,&state);
			if (state == 0)
			{
				BASS_ChannelSetAttribute(Music_SND, BASS_ATTRIB_VOL, 0.00);
				SetCtrlVal(panelHandle,PANEL_VOLUME,0.00);
			}
			else 
			{
				SetCtrlVal(panelHandle,PANEL_VOLUME,100.00);
				BASS_ChannelSetAttribute(Music_SND, BASS_ATTRIB_VOL, 1.00);

			}
			break;
	}
	return 0;
}

int CVICALLBACK volumeFunc (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:

			GetCtrlVal(panelHandle,PANEL_VOLUME,&vol);
			if (vol == 0.00)
			{
				SetCtrlVal(panelHandle,PANEL_CONTROL_MUSIC,0);
			}
			else
			{
				SetCtrlVal(panelHandle,PANEL_CONTROL_MUSIC,1);
			}
			vol /= 100.00;
			BASS_ChannelSetAttribute(Music_SND, BASS_ATTRIB_VOL, vol);
			break;
	}
	return 0;
}

int CVICALLBACK restoreFunc (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, xleft, xright);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, ybottom, ytop);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_AUTOSCALE, xleft, xright);
			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_AUTOSCALE, ybottom, ytop);
			break;
	}
	return 0;
}

int CVICALLBACK clearGraph (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);  
			break;
	}
	return 0;
}
