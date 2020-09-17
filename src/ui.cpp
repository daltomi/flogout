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

#include "ui.hpp"


ActionWidget::ActionWidget(const int w, const int h) :
    Fl_Box(0,0,w,h)
{
    align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
}

void ActionWidget::Action_CB(CONFIG::ACTION ca)
{
#ifdef DEBUG
    std::cerr << "Action_CB!: " << ca << std::endl;
#else
    (ca != CONFIG::ACTION::NL ) ? exit( system(CONFIG::dbus[ca].c_str() ) ) : exit(0);
#endif


}


void ActionWidget::drawActionEnter(CONFIG::ACTION ca)
{
    Fl_PNG_Image* ptr_b = static_cast<Fl_PNG_Image*>(imgElements[CONFIG::ACTION::Z]);
    Fl_PNG_Image* ptr_f = static_cast<Fl_PNG_Image*>(imgElements[ca]);

    const vector<short>& e = CONFIG::DataElements[ca];

    ptr_b->draw(e[0], e[1], ptr_f->w(), ptr_f->h(), e[0], e[1]);
    ptr_f->draw(e[0], e[1], ptr_f->w(), ptr_f->h(), ptr_f->w() / 2,0);
}


void ActionWidget::drawActionLeave(CONFIG::ACTION ca)
{
    Fl_PNG_Image* ptr_b = static_cast<Fl_PNG_Image*>(imgElements[CONFIG::ACTION::Z]);
    Fl_PNG_Image* ptr_f = static_cast<Fl_PNG_Image*>(imgElements[ca]);

    const vector<short>& e = CONFIG::DataElements[ca];

    ptr_b->draw(e[0], e[1], ptr_f->w(), ptr_f->h(), e[0], e[1]);
    ptr_f->draw(e[0], e[1], ptr_f->w() / 2, ptr_f->h(), 0 ,0);
}



int ActionWidget::handle(int event)
{
    static CONFIG::ACTION action = CONFIG::ACTION::NL;
    static CONFIG::ACTION pushAction = CONFIG::ACTION::NL;

    if( 0 == imgElements.size() ) { return  0; }

    switch(event)
    {
        case FL_PUSH:
            if( (pushAction = EventInside() ) != CONFIG::ACTION::NL)
            {
                Action_CB( pushAction );
            }
            break;

        case FL_ENTER:
            if( action == CONFIG::ACTION::NL )
            {
                if( (action = EventInside() ) != CONFIG::ACTION::NL )
                {
                    drawActionEnter(action);
                }
            }
            else /*LEAVE*/
            {
                CONFIG::ACTION tmp;
                if( ( tmp = EventInside()) == CONFIG::ACTION::NL)
                {

                    drawActionLeave(action);
                    action = tmp;
                }
            }
            break;
   }

   return Fl_Widget::handle(event);
}



ActionWidget::~ActionWidget()
{
    for( auto e : imgElements )
    {
        delete static_cast<Fl_PNG_Image*>(e.second);
    }
}


CONFIG::ACTION ActionWidget::EventInside()
{
    for( auto t : CONFIG::DataElements )
    {
        Fl_PNG_Image* ptr = static_cast<Fl_PNG_Image*>(imgElements[t.first]);

        if( ptr != nullptr )
        {
            const vector<short>& v = t.second;

            if( Fl::event_inside(v[0], v[1], ptr->w() / 2, ptr->h() ) )
            {
                return t.first;
            }
        }
    }

    return CONFIG::ACTION::NL; //No encontrado
}


void ActionWidget::setBackgroundImage(const string& file_name)
{
    imgElements[CONFIG::ACTION::Z] = new Fl_PNG_Image(file_name.c_str() );
}


void ActionWidget::newElementImage(const string& file_name, CONFIG::ACTION ca)
{
    imgElements[ca] = new Fl_PNG_Image(file_name.c_str() );
}



void ActionWidget::draw()
{
    if( 0 == imgElements.size() )
    {
        return;
    }

    auto itb = imgElements.begin();
    auto ite = imgElements.end();
    Fl_PNG_Image* ptr = static_cast<Fl_PNG_Image*>( itb->second);

    //Dibujar fondo primero
    ptr->draw(0,0);

    while( ++itb != ite )
    {
        const vector<short>& e = CONFIG::DataElements[ itb->first ];

        ptr = static_cast<Fl_PNG_Image*>( itb->second);

        ptr->draw(e[0], e[1], ptr->w() / 2, ptr->h(), 0, 0);
    }
}


////////////////////



LogoutWindow::LogoutWindow(const int w, const int h) : Fl_Double_Window(w, h, "logout_dbus-send")
{
    string path("imagenes/");

    if (not FileAccessOk(path.c_str(), false))
    {
       path = "/usr/share/flogout/imagenes/";
    
		if (not FileAccessOk(path.c_str(), true))
		{
			exit(1);
		}
    }

    fl_register_images();

    aw =  new ActionWidget( CONFIG::ActionWidgetW, CONFIG::ActionWidgetH );
    aw->setBackgroundImage(path + "fondo.png");
    aw->newElementImage(path + "apagar.png", CONFIG::ACTION::A);
    aw->newElementImage(path + "reiniciar.png", CONFIG::ACTION::R);
    aw->newElementImage(path + "suspender.png", CONFIG::ACTION::S);
    aw->newElementImage(path + "cancelar.png", CONFIG::ACTION::C);
}



int LogoutWindow::Show()
{
    clear_border();
    end();
    set_modal();
    show();

    return Fl::run();
}

