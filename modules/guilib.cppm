module;

/**
 * Module for simple GUI (graphical user interface)
 * Wrapper for your favourite (not QT) library
 * Version - 0.0.3
 */

/*
 * Your includes here
 */
#include <string>

#include "FL/Fl_Text_Editor.H"
#include "FL/Fl_Window.H"

export module Guilib;

// #define DEBUG_GUI //uncomment this to debug

/**
 * Global namespace for interface library
 */
namespace Guilib {
    /**
     * Namespace for useful actions
     */
    namespace Utility {
        /**
         * Get current window child by its position.
         * widget - element of the main window
         * @tparam position position of the widget on window from 0 to n.
         * @return child by given position
         */
        export template<int element_pos, typename Reinterpret_type, int child_count = 6>
            requires (element_pos >= 0 && ((child_count > element_pos) && (element_pos >= child_count - 1)))
        auto SELF_GET_CHILD = [](Fl_Widget *widget) {
#ifdef DEBUG_GUI
            printf("Get position - %d", position);
#endif
            return reinterpret_cast<Reinterpret_type>(widget->top_window()->as_window()->child(position));
        };

        /**
         * Get current entity window.
         * widget - element of the main window
         * @return self
         */
        export auto SELF = [](Fl_Widget *widget) -> auto {
            return widget->top_window()->as_window();
        };
    }

    /**
     * Namespace for gui elements
     */
    export namespace Elements {
        /**
         * Simple button wrapper.
         */
        class FLTK_button {
            bool state = false; //button state

        public:
            FLTK_button();

            FLTK_button(const FLTK_button &) = delete;

            ~FLTK_button();

            //methods:
            [[nodiscard]] auto get_state() const -> bool {
                return state;
            }
        };

        class FLTK_file_browser {
            std::string title;
            std::string filename;

        public:
            FLTK_file_browser();

            FLTK_file_browser(const FLTK_file_browser &) = delete;

            ~FLTK_file_browser();
        };

        /**
         * Simple text input wrapper
         */
        class FLTK_text_input {
            Fl_Text_Editor *editor;
            Fl_Text_Buffer *buffer;

        public:
            FLTK_text_input();

            FLTK_text_input(const FLTK_text_input &) = delete;

            ~FLTK_text_input();
        };
    }
}
