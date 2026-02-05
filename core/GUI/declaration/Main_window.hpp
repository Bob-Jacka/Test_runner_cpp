#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Return_Button.H>
#include "Gui_vars.hpp"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Menu_Bar.H"

import Libio;

namespace Check_runner::GUI {
    class Main_window final : public Fl_Window {
        void window_init();

    public:
        Main_window();

        Main_window(int x, int y, int w, int h);

        Main_window(int w, int h, const char *window_name);

        Main_window(int x, int y, int w, int h, const char *window_name);

        ~Main_window() override;

        Fl_Window *m_replace_dlg = nullptr;
        Fl_Input *m_replace_find = nullptr;
        Fl_Input *m_replace_with = nullptr;
        Fl_Button *m_replace_all = nullptr;
        Fl_Return_Button *m_replace_next = nullptr;
        Fl_Button *m_replace_cancel = nullptr;

        Fl_Text_Editor *m_editor = nullptr;
        char search[256] = {};
    };
}
#endif
