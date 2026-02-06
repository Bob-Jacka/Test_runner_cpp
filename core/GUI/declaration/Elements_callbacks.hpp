#ifndef ELEMENTS_CALLBACKS_HPP
#define ELEMENTS_CALLBACKS_HPP

#include <FL/Fl_Widget.H>
#include <string>

void find_сallback(Fl_Widget *widget, void *v);

void find_2_сallback(Fl_Widget *widget, void *v);

//Special callbacks:
void changed_сallback(int, int nInserted, int nDeleted, int, const char *, void *v);

extern "C" void colorize_callback(int pos, int nInserted, int nColorized, int nRestyled, const char *deletedText, void *v);

void replace_сallback(Fl_Widget *widget, void *v);

void replace_2_сallback(Fl_Widget *widget, void *v);

void repl_all_сallback(Fl_Widget *widget, void *v);

void repl_can_сallback(Fl_Widget *widget, void *v);

void exit_сallback(Fl_Widget *widget);

void paste_сallback(Fl_Widget *widget, void *v);

void copy_сallback(Fl_Widget *widget, void *v);

void cut_сallback(Fl_Widget *widget, void *v);

void delete_сallback(Fl_Widget *widget);

void undo_сallback(Fl_Widget *widget);

void save_as_сallback(Fl_Widget *widget);

void save_callback(Fl_Widget *widget);

void open_callback(Fl_Widget *widget);

void new_callback(Fl_Widget *widget);

#endif
