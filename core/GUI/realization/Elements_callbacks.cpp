#include "../declaration/Elements_callbacks.hpp"
#include <cstdlib> // exit

#include "../declaration/Main_window.hpp"
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Editor.H>

//Local flags:
int changed;
int loading;
char filename[256];
std::string title;

/**
 * Resolve child by its parent
 * @tparam element_pos position of the element
 * @tparam Reinterpret_type type to reinterpret
 * @tparam child_count count of child in parent
 */
template<int element_pos, typename Reinterpret_type, int child_count = 8>
    requires (element_pos >= 0 && ((child_count > element_pos) || (element_pos >= child_count - 1)))
auto SELF_GET_CHILD = [](Fl_Widget *widget) {
    return reinterpret_cast<Reinterpret_type>(widget->top_window()->as_window()->child(element_pos));
};

/**
 * Injects buffer from top window text editor
 */
auto GET_BUFFER_FROM = [](Fl_Widget *widget) {
    return reinterpret_cast<Fl_Text_Editor *>(widget->top_window()->child(1))->buffer();
};

/**
 * Cast void section into Main window;
 * @param view unknown memory address
 */
auto GET_SELF_FROM_VOID = [](void *view) {
    return static_cast<Check_runner::GUI::Main_window *>(view);
};

void set_title(Fl_Window *widget) {
    if (filename[0] == '\0') {
        title.assign("Untitled");
    } else {
        const char *slash = strrchr(filename, '/');
#ifdef WIN32
        if (slash == nullptr) {
            slash = strrchr(filename, '\\');
        }
#endif
        if (slash != nullptr) {
            title.assign(slash + 1);
        } else {
            title.assign(filename);
        }
    }

    if (changed) {
        title.append(" (modified)");
    }
    widget->label(title.c_str());
}

void load_file(char *newfile, const int ipos, Fl_Widget *widget) {
    auto textbuffer = GET_BUFFER_FROM(widget);
    loading = 1;
    const int insert = ipos != -1;
    changed = insert;
    if (!insert) {
        strcpy(filename, "");
    }
    int result;
    if (!insert) {
        result = textbuffer->loadfile(newfile);
    } else {
        result = textbuffer->insertfile(newfile, ipos);
    }

    if (result) {
        fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
    } else if (!insert) {
        strcpy(filename, newfile);
    }
    loading = 0;
    textbuffer->call_modify_callbacks();
}

void save_file(char *newfile, Fl_Widget *widget) {
    auto textbuffer = GET_BUFFER_FROM(widget);
    if (textbuffer->savefile(newfile)) {
        fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    } else {
        strcpy(filename, newfile);
    }

    changed = 0;
    textbuffer->call_modify_callbacks();
}

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
    const auto window = GET_SELF_FROM_VOID(v);
    auto textbuffer = GET_BUFFER_FROM(widget);
    if (window->search[0] == '\0') {
        find_сallback(widget, v);
        return;
    }

    window->m_editor->insert_position(0);

    int pos = window->m_editor->insert_position();
    int found = textbuffer->search_forward(pos, window->search, &pos);

    if (found) {
        textbuffer->select(pos, pos + strlen(window->search));
        window->m_editor->insert_position(pos + strlen(window->search));
        window->m_editor->show_insert_position();
    } else {
        fl_alert("No occurrences of \'%s\' found!", window->search);
    }
}

void find_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    const char *value = fl_input("Search String: ", window->search);

    if (value != nullptr) {
        strcpy(window->search, value);
        find_2_сallback(widget, v);
    }
}

void changed_сallback(int, const int nInserted, const int nDeleted, int, const char *, void *v) {
    if ((nInserted || nDeleted) && !loading) {
        changed = 1;
    }
    const auto window = GET_SELF_FROM_VOID(v);
    if (loading) {
        window->m_editor->show_insert_position();
    }
}

void replace_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    window->m_replace_dlg->show();
}

void replace_2_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    auto textbuffer = GET_BUFFER_FROM(widget);
    const char *find = window->m_replace_find->value();
    const char *replace = window->m_replace_with->value();

    if (find[0] == '\0') {
        window->m_replace_dlg->show();
        return;
    }

    window->m_replace_dlg->hide();
    window->m_editor->insert_position(0);

    int pos = window->m_editor->insert_position();
    int found = textbuffer->search_forward(pos, find, &pos);

    if (found) {
        textbuffer->select(pos, pos + strlen(find));
        textbuffer->remove_selection();
        textbuffer->insert(pos, replace);
        textbuffer->select(pos, pos + strlen(replace));

        window->m_editor->insert_position(pos + strlen(replace));
        window->m_editor->show_insert_position();
    } else
        fl_alert("No occurrences of \'%s\' found!", find);
}

/**
 * Callback for all replace
 * @param widget current widget
 * @param v current view
 */
void repl_all_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    auto textbuffer = GET_BUFFER_FROM(widget);
    const char *find = window->m_replace_find->value();
    const char *replace = window->m_replace_with->value();

    find = window->m_replace_find->value();

    if (find[0] == '\0') {
        window->m_replace_dlg->show();
        return;
    }

    window->m_replace_dlg->hide();
    window->m_editor->insert_position(0);
    int times = 0;

    for (int found = 1; found;) {
        int pos = window->m_editor->insert_position();
        found = textbuffer->search_forward(pos, find, &pos);

        if (found) {
            textbuffer->select(pos, pos + strlen(find));
            textbuffer->remove_selection();
            textbuffer->insert(pos, replace);

            window->m_editor->insert_position(pos + strlen(replace));
            window->m_editor->show_insert_position();
            times++;
        }
    }

    if (times) {
        fl_message("Replaced %d occurrences.", times);
    } else {
        fl_alert("No occurrences of \'%s\' found!", find);
    }
}

void repl_can_сallback(Fl_Widget *widget, void *v) {
    const auto window = GET_SELF_FROM_VOID(v);
    window->m_replace_dlg->hide();
}

void exit_сallback(Fl_Widget *widget) {
    exit(0);
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
    auto textbuffer = GET_BUFFER_FROM(widget);
    textbuffer->remove_selection();
}

void undo_сallback(Fl_Widget *widget) {
    auto textbuffer = GET_BUFFER_FROM(widget);
    textbuffer->undo();
}

void save_as_сallback(Fl_Widget *widget) {
    char *newfile = fl_file_chooser("Save File As?", "*", filename); //new file handler
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

    char *newfile = fl_file_chooser("Open File?", "*", filename);
    if (newfile != nullptr) {
        load_file(newfile, -1, widget);
    }
}

void new_callback(Fl_Widget *widget) {
    auto textbuffer = GET_BUFFER_FROM(widget);
    if (!check_save(widget)) {
        return;
    }
    filename[0] = '\0';
    textbuffer->select(0, textbuffer->length());
    textbuffer->remove_selection();
    changed = 0;
    textbuffer->call_modify_callbacks();
}
