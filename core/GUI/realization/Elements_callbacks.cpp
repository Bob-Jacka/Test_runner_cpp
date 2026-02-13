#include "../declaration/Elements_callbacks.hpp"

#include "../declaration/Main_window.hpp"
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Editor.H>

//Local flags:
bool changed;
bool loading; //TODO maybe replace bool values with bitset
char filename[256];

//Global parse data:
auto           compare_keywords_func = [](const void *current, const void *in_structures) -> int { return 0; }; //func for compare keywords
constexpr char code_types[]          = {"has"};                                                                 //For F color type
constexpr char code_keywords[]       = {"|"};                                                                   //For G color type

/**
 * Injects buffer from top window text editor:
 * @param widget insert widget
 * @return text buffer
 */
auto GET_TXT_BUFFER_FROM = [](const Fl_Widget *widget) {
    return reinterpret_cast<Fl_Text_Editor *>(widget->top_window()->child(2))->buffer();
};

/**
 * Injects style buffer from top window text editor:
 * @param widget insert widget
 * @return style buffer object
 */
auto GET_STYLE_BUFFER_FROM = [](const Fl_Widget *widget) {
    return reinterpret_cast<Fl_Text_Editor *>(widget->top_window()->child(2))->style_buffer();
};

/**
 * Cast void section into Main window;
 * @param view unknown memory address
 * @return Self cast from unknown memory
 */
auto GET_SELF_FROM_VOID = [](void *view) {
    return static_cast<Check_runner::GUI::Main_window *>(view);
};

/**
 * Cast void section into FL window;
 * @param view unknown memory address
 * @return Window casted from unknown memory
 */
auto GET_WINDOW_FROM_VOID = [](void *view) {
    return static_cast<Fl_Window *>(view);
};

/**
 * Pointer to self window
 */
auto SELF = [](const Fl_Widget *widget) -> auto {
    return widget->top_window()->as_window();
};

/**
 * Set window title
 * @param widget
 * @param new_title
 */
void set_title(const Fl_Widget *widget, const char *new_title) {
    const auto  main_window = SELF(widget);
    std::string changed_title(new_title);
    if (changed) {
        changed_title.append(" (modified)");
    }
    main_window->label(changed_title.c_str());
}

/**
 * Load file into text buffer
 * @param newfile name of the file
 * @param ipos position
 * @param widget injected widget
 */
void load_file(char *newfile, const int ipos, Fl_Widget *widget) {
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    loading                = true;
    const bool insert      = ipos != -1;
    changed                = insert;

    if (!insert) {
        strcpy(filename, "");
    }
    try {
        if (!insert) {
            text_buffer->loadfile(newfile);
            set_title(widget, newfile);
        } else {
            text_buffer->insertfile(newfile, ipos);
            set_title(widget, newfile);
        }
    } catch (...) {
        fl_alert("Error loading file \'%s\':\n%s.", newfile, strerror(errno));
    }

    if (!insert) {
        strcpy(filename, newfile);
    }
    loading = false;
    text_buffer->call_modify_callbacks();
}

/**
 * Save file in text buffer
 * @param newfile file name
 * @param widget injected widget
 */
void save_file(char *newfile, const Fl_Widget *widget) {
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    try {
        if (text_buffer->savefile(newfile)) {
            fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
        } else {
            strcpy(filename, newfile);
        }
    } catch (...) {
        //
    }

    changed = false;
    text_buffer->call_modify_callbacks();
}

/**
 * Check is file changed
 * @param widget
 * @return 1 - true, 0 for false
 */
int check_save(Fl_Widget *widget) {
    if (!changed) {
        return 1;
    }

    const int result = fl_choice("The current file has not been saved.\n"
                                 "Would you like to save it now?",
                                 "Cancel", "Save", "Discard");

    if (result == 1) {
        save_callback(widget);
        return !changed;
    }

    return result == 2 ? 1 : 0;
}

void find_2_сallback(Fl_Widget *widget, void *v) {
    const auto window      = GET_SELF_FROM_VOID(v);
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    if (window->search[0] == '\0') {
        find_сallback(widget, v);
        return;
    }

    window->m_editor->insert_position(0);

    int pos   = window->m_editor->insert_position();
    int found = text_buffer->search_forward(pos, window->search, &pos);

    if (found) {
        text_buffer->select(pos, pos + strlen(window->search));
        window->m_editor->insert_position(pos + strlen(window->search));
        window->m_editor->show_insert_position();
    } else {
        fl_alert("No occurrences of \'%s\' found!", window->search);
    }
}

void find_сallback(Fl_Widget *widget, void *v) {
    const auto  window = GET_SELF_FROM_VOID(v);
    const char *value  = fl_input("Search String: ", window->search);

    if (value != nullptr) {
        strcpy(window->search, value);
        find_2_сallback(widget, v);
    }
}

/**
 * Parse style in colorize callback
 * @param text given text symbol
 * @param style given style
 * @param length given len of text
 */
extern "C" void style_parser(const char *text, char *style, int length) {
    char        current;
    int         col;
    int         last;
    char        buf[255],
            *   buf_p;
    const char *temp;

    /**
     * col - column number
     * last - is it last symbol
     * length - len of the line of the text
     * current - current style of the section of text
     */
    for (current = *style, col = 0, last = 0; length > 0; --length, ++text) {
        if (current == 'A') {
            if (col >= 0 && *text == '#') {
                current = 'E'; //directive
            } else if (strncmp(text, "*", 1) == 0) {
                current = 'B'; //block line comment
            } else if (strncmp(text, "/*", 2) == 0) {
                current = 'C'; //multiline comment start
            } else if (strncmp(text, "\\\"", 2) == 0) {
                // Quoted quote...
                *style++ = current;
                *style++ = current;
                ++text;
                --length;
                col += 2;
                continue;
            } else if (*text == '\"') {
                current = 'D';
            } else if (!last && islower(*text)) {
                // Might be a keyword...
                for (temp = text, buf_p = buf;
                     islower(*temp) && buf_p < buf + sizeof(buf) - 1;
                     *buf_p++ = *temp++) {
                    //Empty braces
                }

                if (!islower(*temp)) {
                    *buf_p = '\0';

                    buf_p = buf;

                    if (bsearch(&buf_p, code_types,
                                std::size(code_types),
                                sizeof(code_types[0]), compare_keywords_func)) {
                        while (text < temp) {
                            *style++ = 'F';
                            ++text;
                            --length;
                            ++col;
                        }

                        --text;
                        ++length;
                        last = 1;
                        continue;
                    }
                    if (bsearch(&buf_p, code_keywords,
                                std::size(code_keywords),
                                sizeof(code_keywords[0]), compare_keywords_func)) {
                        while (text < temp) {
                            *style++ = 'G';
                            ++text;
                            --length;
                            ++col;
                        }

                        --text;
                        ++length;
                        last = 1;
                        continue;
                    }
                }
            }
        } else if (current == 'C' && strncmp(text, "*/", 2) == 0) {
            // Close a C comment...
            *style++ = current;
            *style++ = current;
            ++text;
            --length;
            current = 'A';
            col += 2;
            continue;
        } else if (current == 'D') {
            // Continuing in string...
            if (strncmp(text, "\\\"", 2) == 0) {
                // Quoted end quote...
                *style++ = current;
                *style++ = current;
                ++text;
                --length;
                col += 2;
                continue;
            }
            if (*text == '\"') {
                // End quote...
                *style++ = current;
                ++col;
                current = 'A';
                continue;
            }
        }

        // Copy style info...
        if (current == 'A' && (*text == '{' || *text == '}')) {
            *style++ = 'G';
        } else {
            *style++ = current;
        }
        ++col;

        last = isalnum(*text) || *text == '.';

        if (*text == '\n') {
            // Reset column and possibly reset the style
            col = 0;
            if (current == 'B' || current == 'E') {
                current = 'A';
            }
        }
    }
}

void changed_сallback(int, const int nInserted, const int nDeleted, int, const char *, void *v) {
    if ((nInserted || nDeleted) && !loading) {
        changed = true;
    }
    const auto window = GET_SELF_FROM_VOID(v);
    if (loading) {
        window->m_editor->show_insert_position();
    }
}

extern "C" void colorize_callback(const int   pos,         // I - Position of update
                                  const int   nInserted,   // I - Number of inserted chars
                                  const int   nColorized,  // I - Number of deleted chars
                                  int         nRestyled,   // I - Number of restyled chars
                                  const char *deletedText, // I - Text that was deleted
                                  void *      v            // I - Callback data
) {
    int      start; // Start of text
    int      end;   // End of text
    char     last,  // Last style on line
            *style, // Style data
            *text;  // Text data

    const auto text_buffer  = GET_TXT_BUFFER_FROM(GET_WINDOW_FROM_VOID(v)); //inject text buffer
    const auto style_buffer = GET_STYLE_BUFFER_FROM(GET_WINDOW_FROM_VOID(v));

    // If this is just a selection change, just unselect the style buffer...
    if (nInserted == 0 && nColorized == 0) {
        style_buffer->unselect();
        return;
    }

    // Track changes in the text buffer...
    if (nInserted > 0) {
        // Insert characters into the style buffer...
        style = new char[nInserted + 1];
        memset(style, 'A', nInserted);
        style[nInserted] = '\0';

        style_buffer->replace(pos, pos + nColorized, style);
        delete[] style;
    } else {
        // Just delete characters in the style buffer...
        style_buffer->remove(pos, pos + nColorized);
    }
    style_buffer->select(pos, pos + nInserted - nColorized);

    start = text_buffer->line_start(pos);
    end   = text_buffer->line_end(pos + nInserted - nColorized);

    text  = text_buffer->text_range(start, end);
    style = style_buffer->text_range(start, end);
    last  = style[end - start - 1];

    style_parser(text, style, end - start);

    style_buffer->replace(start, end, style);
    static_cast<Fl_Text_Editor *>(v)->redisplay_range(start, end);

    if (last != style[end - start - 1]) {
        free(text);
        free(style);

        end   = text_buffer->length();
        text  = text_buffer->text_range(start, end);
        style = style_buffer->text_range(start, end);

        style_parser(text, style, end - start);

        style_buffer->replace(start, end, style);
        static_cast<Fl_Text_Editor *>(v)->redisplay_range(start, end);
    }

    free(text);
    free(style);
}

/**
 * Callback for replace
 * @param widget
 * @param v view
 */
void replace_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    window->m_replace_dlg->show();
    window->m_editor->deactivate(); //deactivate window for changing focus
}

void replace_2_сallback(Fl_Widget *widget, void *v) {
    const auto  window      = GET_SELF_FROM_VOID(v);
    const auto  text_buffer = GET_TXT_BUFFER_FROM(widget);
    const char *find        = window->m_replace_find->value();
    const char *replace     = window->m_replace_with->value();

    if (find[0] == '\0') {
        window->m_replace_dlg->show();
        return;
    }

    window->m_replace_dlg->hide();
    window->m_editor->insert_position(0);

    int       pos   = window->m_editor->insert_position();
    const int found = text_buffer->search_forward(pos, find, &pos);
    window->m_editor->activate();
    if (found) {
        text_buffer->select(pos, pos + strlen(find));
        text_buffer->remove_selection();
        text_buffer->insert(pos, replace);
        text_buffer->select(pos, pos + strlen(replace));

        window->m_editor->insert_position(pos + strlen(replace));
        window->m_editor->show_insert_position();
    } else {
        fl_alert("No occurrences of \'%s\' found!", find);
    }
}

/**
 * Callback for all replace
 * @param widget current widget
 * @param v current view
 */
void repl_all_сallback(Fl_Widget *widget, void *v) {
    const auto  window      = GET_SELF_FROM_VOID(v);
    const auto  text_buffer = GET_TXT_BUFFER_FROM(widget);
    const char *find        = window->m_replace_find->value();
    const char *replace     = window->m_replace_with->value();

    if (find[0] == '\0') {
        window->m_replace_dlg->show();
        return;
    }

    window->m_replace_dlg->hide();
    window->m_editor->insert_position(0);
    int times = 0;

    for (int found = 1; found;) {
        int pos = window->m_editor->insert_position();
        found   = text_buffer->search_forward(pos, find, &pos);

        if (found) {
            text_buffer->select(pos, pos + strlen(find));
            text_buffer->remove_selection();
            text_buffer->insert(pos, replace);

            window->m_editor->insert_position(pos + strlen(replace));
            window->m_editor->show_insert_position();
            ++times;
        }
    }

    if (times) {
        fl_message("Replaced %d occurrences.", times);
    } else {
        fl_alert("No occurrences of \'%s\' found!", find);
    }
    window->m_editor->activate();
}

void repl_can_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    window->m_replace_dlg->hide();
}

void exit_сallback(Fl_Widget *widget) {
    check_save(widget);
    std::exit(0);
}

void paste_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    Fl_Text_Editor::kf_paste(0, window->m_editor);
}

void copy_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    Fl_Text_Editor::kf_copy(0, window->m_editor);
}

void cut_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    Fl_Text_Editor::kf_cut(0, window->m_editor);
}

void delete_сallback(Fl_Widget *widget) {
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    text_buffer->remove_selection();
}

void undo_сallback(Fl_Widget *widget) {
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    text_buffer->undo();
}

void save_as_сallback(Fl_Widget *widget) {
    char *newfile = fl_file_chooser("Save File As?", "*.txt", filename); //new file handler
    if (newfile != nullptr) {
        save_file(newfile, widget);
    }
}

void save_callback(Fl_Widget *widget) {
    if (filename[0] == '\0') {
        save_as_сallback(widget);
    } else {
        save_file(filename, widget);
    }
}

void open_callback(Fl_Widget *widget) {
    if (!check_save(widget)) {
        return;
    }

    char *newfile = fl_file_chooser("Open File?", "*.txt", filename); //Show only txt file and directories
    if (newfile != nullptr) {
        load_file(newfile, -1, widget);
    }
}

void new_callback(Fl_Widget *widget) {
    const auto text_buffer = GET_TXT_BUFFER_FROM(widget);
    if (!check_save(widget)) {
        return;
    }
    filename[0] = '\0';
    text_buffer->select(0, text_buffer->length());
    text_buffer->remove_selection();
    changed = false;
    text_buffer->call_modify_callbacks();
}

/**
 * About modal window
 * @param widget widget
 * @param v view
 */
void about_callback(Fl_Widget *widget, void *v) {
    const auto about_window = new Fl_Double_Window(600, 600, "About");
    about_window->begin();
    auto *box = new Fl_Box(0, 0, 590, 590);
    box->label("About page:\n"
        "1\n"
        "2\n"
        "3\n"
        "Utility version - 2.2.0\n"
    );
    box->labelsize(16);
    box->labelfont(FL_BOLD);

    about_window->end();
    about_window->show();
}

void increase_font_callback(Fl_Widget *widget, void *v) {
    const auto self         = GET_SELF_FROM_VOID(v);
    const int  current_font = self->m_editor->textsize();
    self->m_editor->textsize(current_font + 2);
#ifdef DEBUG
    printf("Current font size - %d\n", self->m_editor->textsize());
#endif
    self->m_editor->redraw();
}

void decrease_font_callback(Fl_Widget *widget, void *v) {
    const auto self         = GET_SELF_FROM_VOID(v);
    const int  current_font = self->m_editor->textsize();
    self->m_editor->textsize(current_font - 2);
#ifdef DEBUG
    printf("Current font size - %d\n", self->m_editor->textsize());
#endif
    self->m_editor->redraw();
}

void AI_chat_callback(Fl_Widget *widget, void *v) {
    auto       buffer      = Fl_Text_Buffer();
    const auto chat_window = new Fl_Double_Window(600, 600, "AI chat");
    chat_window->begin();

    const auto txt_input = new Fl_Text_Editor(20, 550, 500, 50);
    txt_input->buffer(buffer);
    txt_input->textsize(15);

#ifdef DEBUG
    printf("User sent message to ai");
#endif
    chat_window->end();
    chat_window->show();
}

void AI_agent_callback(Fl_Widget *widget, void *v) {
    const auto agent_window = new Fl_Double_Window(600, 600, "AI agent menu");
    agent_window->begin();
    agent_window->end();
    agent_window->show();
}
