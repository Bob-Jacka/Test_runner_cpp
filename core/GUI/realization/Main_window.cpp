#include "../declaration/Main_window.hpp"
#include "../declaration/Elements_callbacks.hpp"

void Check_runner::GUI::Main_window::window_init() {
    m_text_buffer  = new Fl_Text_Buffer();
    m_style_buffer = new Fl_Text_Buffer();

    m_replace_dlg = new Fl_Window(40, 50, DEFAULT_DIALOG_WIDTH, DEFAULT_DIALOG_HEIGHT, "Replace"); {
        m_replace_find = new Fl_Input(80, 10, BTN_WIDTH, BTN_HEIGHT, "Find:");
        m_replace_with = new Fl_Input(80, 40, BTN_WIDTH, BTN_HEIGHT, "Replace:");
        m_replace_all  = new Fl_Button(10, 70, 80, BTN_HEIGHT, "Replace All");
        m_replace_all->callback(repl_all_сallback, this);

        m_replace_next = new Fl_Return_Button(105, 70, 120, BTN_HEIGHT, "Replace Next");
        m_replace_next->callback(replace_2_сallback, this);

        m_replace_cancel = new Fl_Button(230, 70, 60, BTN_HEIGHT, "Cancel");
        m_replace_cancel->callback(repl_can_сallback, this);
        m_replace_dlg->end(); //end creating inner window
    }

    m_editor = nullptr;
    *search  = static_cast<char>(0);
    this->end(); //end creating mini window
}

Check_runner::GUI::Main_window::Main_window() : Fl_Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int x, int y, int w, int h) : Fl_Window(x, y, w, h, "Check runner") {
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int w, int h, const char *window_name) : Fl_Window(w, h, window_name) {
    window_init();
}

Check_runner::GUI::Main_window::Main_window(int x, int y, int w, int h, const char *window_name) : Fl_Window(x, y, w, h, window_name) {
    window_init();
}

Check_runner::GUI::Main_window::~Main_window() {
    delete m_replace_dlg;
}
