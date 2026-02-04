#include "../declaration/Main_window.hpp"
#include "../declaration/Gui_vars.hpp"

char *filename;
int changed;
int loading;
auto textbuffer = new Fl_Text_Buffer();

template<int element_pos, typename Reinterpret_type, int child_count = 6>
    requires (element_pos >= 0 && ((child_count > element_pos) || (element_pos <= child_count - 1)))
static auto SELF_GET_CHILD = [](Fl_Widget *widget) {
    return reinterpret_cast<Reinterpret_type>(widget->top_window()->as_window()->child(element_pos));
};

static auto SELF = [](Fl_Widget *widget) -> auto {
    return widget->top_window()->as_window();
};

void load_file(char *newfile_name, const int ipos) {
    loading = 1;
    const int insert = (ipos != -1);
    changed = insert;
    if (!insert) {
        strcpy(filename, "");
    }
    int result;
    if (!insert) {
        result = textbuffer->loadfile(newfile_name);
    } else {
        result = textbuffer->insertfile(newfile_name, ipos);
    }
    if (result) {
        fl_alert("Error reading from file \'%s\':\n%s.", newfile_name, strerror(errno));
    } else if (!insert) {
        strcpy(filename, newfile_name);
    }
    loading = 0;
    textbuffer->call_modify_callbacks();
}

// void paste_callback(Fl_Widget *widget, void *v) {
//     const auto *window = static_cast<Check_runner::GUI::Main_window *>(v);
//     Fl_Text_Editor::kf_paste(0, window->get_text_editor());
// }
//
// void copy_callback(Fl_Widget *widget, void *v) {
//     const auto *window = static_cast<Check_runner::GUI::Main_window *>(v);
//     Fl_Text_Editor::kf_copy(0, window->get_text_editor());
// }
//
// void cut_callback(Fl_Widget *widget, void *v) {
//     const auto *window = static_cast<Check_runner::GUI::Main_window *>(v);
//     Fl_Text_Editor::kf_cut(0, window->get_text_editor());
// }

void delete_callback(Fl_Widget *widget, void *v) {
    textbuffer->remove_selection();
}

void save_file(char *newfile) {
    if (textbuffer->savefile(newfile)) {
        fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    } else {
        strcpy(filename, newfile);
    }
    changed = 0;
    textbuffer->call_modify_callbacks();
}

void save_as_callback(Fl_Widget *, void *) {
    char *newfile = fl_file_chooser("Save File As?", "*", filename);
    if (newfile != nullptr)
        save_file(newfile);
}

void save_callback(Fl_Widget *, void *) {
    if (filename[0] == '\0') {
        save_as_callback(nullptr, nullptr);
        return;
    }
    save_file(filename);
}

int check_save() {
    if (!changed) {
        return 1;
    }
    const int result = fl_choice("The current file has not been saved.\n"
                                 "Would you like to save it now?",
                                 "Cancel", "Save", "Discard");

    if (result == 1) {
        save_callback(nullptr, nullptr);
        return !changed;
    }

    return (result == 2) ? 1 : 0;
}

void open_callback(Fl_Widget *widget, void *v) {
    if (!check_save())
        return;

    char *newfile = fl_file_chooser("Open File?", "*", filename);
    if (newfile != nullptr)
        load_file(newfile, -1);
}

/**
 * Exit from utility
 * @param w this
 * @param v view
 */
static void quit_tf(Fl_Widget *w, void *v) {
    exit(0);
}

void newCallback(Fl_Widget *widget, void *data) {
    if (!check_save())
        return;

    filename[0] = '\0';
    textbuffer->select(0, textbuffer->length());
    textbuffer->remove_selection();
    changed = 0;
    textbuffer->call_modify_callbacks();
}

/**
 * Initialize elements in main window and position them
 */
void Check_runner::GUI::Main_window::window_init() {
    //Create entities block:
    {
        //Main window entities
        m_btn_load = new Fl_Button(30, 25, BTN_WIDTH, BTN_HEIGHT, "Load files"); //0
        m_btn_cur_dir = new Fl_Button(150, 25, BTN_WIDTH + 15, BTN_HEIGHT, "Current directory"); //add 15, because text is too width
        m_file_browser = new Fl_File_Browser(40, 75, 400, 600, "Files"); //2

        m_menu_bar = new Fl_Menu_Bar(0, 0, 100, 20); {
            //3
            m_menu_bar->add("&File/&Open", FL_COMMAND + 'o', open_callback, this);
            m_menu_bar->add("&File/&Save", FL_COMMAND + 's', save_callback, this);
            m_menu_bar->add("&File/&Quit", FL_COMMAND + 'q', quit_tf);
        }
    }
    //Other actions with widgets:
    {
        m_file_browser->hide();
    }
    //Align block:
    {
        m_file_browser->align(FL_ALIGN_CENTER);
    }
    //Set actions:
    {
        m_btn_load->callback([](Fl_Widget *widget) {
            char *file = fl_file_chooser("Pick your file", "*.txt", std::filesystem::current_path().generic_string().c_str(), 0);
            if (file != nullptr) {
                filename = file; //attach filename to file variable
#ifdef DEBUG_GUI
                printf("Selected file: %s\n", file);
            }
            else {
                printf("User cancelled\n");
            }
#else
            }
#endif
            //TODO create separate class for text input
            const auto tx_win = new Fl_Double_Window(640, 480, "Edit file");
            const auto tx_editor = new Fl_Text_Editor(20, 20, DEFAULT_WINDOW_WIDTH - 40, DEFAULT_WINDOW_HEIGHT - 40);
            tx_editor->buffer(textbuffer); // attach the text buffer to our editor widget
            tx_win->resizable(tx_editor);
            tx_win->show();
            auto file_lines = File_controller::readlines(file); //TODO can transfer file controller from main to main window
            for (auto &line: file_lines) {
                textbuffer->append((line + "\n").c_str());
            }

            //TODO add as a function
            // const auto result = textbuffer->savefile(filename);
            // switch (result) {
            //     case -1:
            //         printf("Save file failed with error\n");
            //         break;
            //     case 1:
            //         printf("Save file failed\n");
            //         break;
            //     case 0:
            //         printf("Save file successful\n");
            //         break;
            //     default: break;
            // }
        });

        m_btn_cur_dir->callback([](Fl_Widget *widget) {
            auto *f_browser = SELF_GET_CHILD<FILE_BROWSER_NUM, Fl_File_Browser *>(widget);
            f_browser->load(std::filesystem::current_path().generic_string().c_str());
            f_browser->show();
        });
    }
    end(); //end creating main window
}

Fl_Window *Check_runner::GUI::Main_window::get_self() const {
    return top_window();
}

Check_runner::GUI::Main_window::Main_window(): Fl_Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
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
