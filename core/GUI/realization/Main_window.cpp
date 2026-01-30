#include "../declaration/Main_window.hpp"

#include <filesystem>

#include "FL/Fl_Button.H"
#include "FL/Fl_File_Browser.H"

/**
 * Initialize elements in main window
 */
void Check_runner::GUI::Main_window::window_init() {
    Fl_Button *btn;
    Fl_File_Browser *file_browser;
    //Create entities:
    {
        btn = new Fl_Button(10, 20, 100, 50, "Exit");
        file_browser = new Fl_File_Browser(70, 80, 200, 150);
    }
    //Align block:
    {
        btn->align(FL_ALIGN_LEFT_BOTTOM);
        file_browser->align(FL_ALIGN_CENTER);
    }
    file_browser->load(reinterpret_cast<const char *>(std::filesystem::current_path().c_str()));
    btn->callback([](Fl_Widget *) {
        //access to main window
        printf("Hello World\n");
    });

    child_count = this->children();
    end(); //end creating main window
}

Fl_Window *Check_runner::GUI::Main_window::get_self() const {
    return top_window();
}

Check_runner::GUI::Main_window::Main_window(): Fl_Window(600, 600) {
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int x, int y, int w, int h): Fl_Window(x, y, w, h, "Check runner") {
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int w, int h, const char *window_name): Fl_Window(w, h, window_name) {
    this->window_name = window_name;
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int x, int y, int w, int h, const char *window_name): Fl_Window(x, y, w, h, window_name) {
    this->window_name = window_name;
    window_init();
}

Check_runner::GUI::Main_window::~Main_window() = default;

/**
 * Simply show window
 */
void Check_runner::GUI::Main_window::show_window() {
    show();
}
