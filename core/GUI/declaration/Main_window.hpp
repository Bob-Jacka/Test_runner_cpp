#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>
#include "FL/Fl_Window.H"

//TODO сделать через builder создание окна
namespace Check_runner::GUI {
    class Main_window final : Fl_Window {
        std::string window_name;

        int child_count = 0;

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
