#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>

#include <filesystem>
#include "../../Entities/File_controller.hpp"
#include "FL/Fl_Button.H"
#include "FL/Fl_File_Chooser.H"
#include "FL/Fl_Menu_Bar.H"
#include "FL/Fl_Text_Editor.H"
#include "FL/Fl_Window.H"

import Libio;

namespace Check_runner::GUI {
    class Main_window final : Fl_Window {
        std::string window_name;

        //Main window:
        Fl_Button *m_btn_load = nullptr; //0
        Fl_Button *m_btn_cur_dir = nullptr; //1
        Fl_File_Browser *m_file_browser = nullptr; //2
        Fl_Menu_Bar *m_menu_bar = nullptr; //3

        void window_init();

        [[nodiscard]] Fl_Window *get_self() const;

    public:
        Main_window();

        Main_window(int x, int y, int w, int h);

        Main_window(int w, int h, const char *window_name);

        Main_window(int x, int y, int w, int h, const char *window_name);

        ~Main_window() override;

        void show_window();
    };
}

#endif
