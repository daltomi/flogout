/*
	Copyright © 2020 daltomi <daltomi@disroot.org>

	This file is part of flogout.

	flogout is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	flogout is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with flogout.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ui_hpp
#define ui_hpp

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_PNG_Image.H>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "system.hpp"

using std::map;
using std::string;
using std::vector;

//TODO: configuracion por archivo ASCII

namespace CONFIG
{
// NL = NULL
// Z = Reservado, background
// S uspender
// C ancelar
// A pagar
// R einiciar
enum ACTION{NL=0, Z=1, S=2, C=3, A=4, R=5};

static map<ACTION,const string> dbus{
   // {R,"sudo dbus-send --system --print-reply --dest=\"org.freedesktop.ConsoleKit\" /org/freedesktop/ConsoleKit/Manager org.freedesktop.ConsoleKit.Manager.Restart"},
   // {A,"sudo dbus-send --system --print-reply --dest=\"org.freedesktop.ConsoleKit\" /org/freedesktop/ConsoleKit/Manager org.freedesktop.ConsoleKit.Manager.Stop"},
   // {S,"sudo dbus-send --system --print-reply --dest=\"org.freedesktop.UPower\" /org/freedesktop/UPower org.freedesktop.UPower.Suspend"}
    {R,"sudo shutdown -r now"},
    {A,"sudo shutdown -h now"},
    {S,"sudo dbus-send --system --print-reply --dest=\"org.freedesktop.UPower\" /org/freedesktop/UPower org.freedesktop.UPower.Suspend"}
}; //CONFIG

static map<ACTION, vector<short> > DataElements{
//      action, x, y
        {R,{ 30, 90} },
        {S,{ 210, 8} },
        {C,{ 235, 118} },
        {A,{ 95, 15} }
};


const short ActionWidgetW = 340;
const short ActionWidgetH = 190;


};//////FIN CONFIGURACION //////


class ActionWidget : public Fl_Box
{
    public:
        ActionWidget(const int w, const int h);

        virtual ~ActionWidget();

        void setBackgroundImage(const string& file_name);
        void newElementImage(const string& file_name, CONFIG::ACTION);

        int handle(int event); //virtual


    protected:

        map<CONFIG::ACTION, Fl_PNG_Image*> imgElements;

        CONFIG::ACTION EventInside();

        static void Action_CB(CONFIG::ACTION ca );

        void draw(); //virtual

        void drawActionLeave(CONFIG::ACTION ca);
        void drawActionEnter(CONFIG::ACTION ca);

};

class LogoutWindow : public Fl_Double_Window
{
    public:
        LogoutWindow(const int w, const int h);
        virtual ~LogoutWindow() {delete aw; }
        int Show();

    protected:

    ActionWidget* aw;
};

#endif

