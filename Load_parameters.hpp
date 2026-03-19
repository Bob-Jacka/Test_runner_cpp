#ifndef LOAD_PARAMETERS_HPP
#define LOAD_PARAMETERS_HPP

#define BITSET_FLAGS_UPDATE //flag for turn on bit flags

#ifdef BITSET_FLAGS_UPDATE
#include <bitset>
#include <stdexcept>
#endif

namespace Check_runner {
    /**
     * Namespace for load parameters in utility
     */
    namespace LP {
        /**
         * Utility load parameters struct.
         * Simply stores load compiler data and provide it outside.
         */
        struct Load_parameters {
            private:
                //main utility parameters, part of it is required:
                std::string entry_point;          ///entry point of utility use.
                std::string devices_entry_point;  ///file with devices to test on.
                std::string parameters;           ///main parameters in utility.
                int         check_device_count{}; ///count of devices for test on.

#ifdef BITSET_FLAGS_UPDATE
#pragma message("Using bit set flags functionality")

                //Getters
#define GET_GUI_FLAG() get_flag_value(0)
#define GET_IS_COLORED_OUT() get_flag_value(1)
#define GET_IS_COMMENTS() get_flag_value(2)
#define GET_IS_TIME_REC() get_flag_value(3)
#define GET_IS_FILE_WRITE() get_flag_value(4)

#define GET_RANDOM_STRAT() get_flag_value(5)
#define GET_PARALLEL_STRAT() get_flag_value(6)
#define GET_HIGH_PRIOR_STRAT() get_flag_value(7)
#define GET_EVERYTHING_NOW_STRAT() get_flag_value(8)
#define GET_USUAL_STRAT() get_flag_value(9)
#define GET_CHOOSE_STRAT() get_flag_value(10)

                //Setters
#define SET_GUI_FLAG() set_flag_value(0)
#define SET_IS_COLORED_OUT() set_flag_value(1)
#define SET_IS_COMMENTS() set_flag_value(2)
#define SET_IS_TIME_REC() set_flag_value(3)
#define SET_IS_FILE_WRITE() set_flag_value(4)

#define SET_RANDOM_STRAT() set_flag_value(5)
#define SET_PARALLEL_STRAT() set_flag_value(6)
#define SET_HIGH_PRIOR_STRAT() set_flag_value(7)
#define SET_EVERYTHING_NOW_STRAT() set_flag_value(8)
#define SET_USUAL_STRAT() set_flag_value(9)
#define SET_CHOOSE_STRAT() set_flag_value(10)

                /**
                  * 0 bit - is gui turn on
                  * 1 bit - is colored output
                  * 2 bit - is comments
                  * 3 bit - is time record
                  * 4 bit - is need for file write (test results)
                  *
                  * 5 bit - is random run strategy
                  * 6 bit - is parallel strategy
                  * 7 bit - is high prior strategy
                  * 8 bit - is everything now strategy
                  * 9 bit - is usual strategy
                  * 10 bit - is choose strategy
                  *
                  * 11 bit - reserved for future operations.
                  * 12 bit - reserved for future operations.
                  * 13 bit - reserved for future operations.
                  * 14 bit - reserved for future operations.
                  * 15 bit - reserved for future operations.
                */
                std::bitset<16> flags;

#elifndef BITSET_FLAGS_UPDATE
#pragma message("Using usual flag functionality")

                bool        is_gui{};             ///is need to run in graphics user interface. true - 'gui', false - 'console' (default).

                //other utility parameters:
                bool is_colored{};     ///is need for colored output.
                bool is_comments{};    ///is need for comments in utility.
                bool is_time_record{}; ///for each test case to record time.
                bool is_file_write{};  ///is need for file write at the end of test run.
                //strategies to run:
                bool is_random_run{};     ///randomly run test cases.
                bool is_parallel_run{};   ///strategy for parallel adding tests.
                bool is_high_priority{};  ///runs only high priority test cases.
                bool is_everything_now{}; ///usually prints all tests in console or anything.
                bool is_usual_run{};      ///simple get test cases without actions.
                bool is_choose_run{};     ///choose severity of test cases.
#endif

            public:
                Load_parameters() = default;

                ~Load_parameters() = default;

                ////////////////////////getter
                [[nodiscard]] std::string get_entry_point() const;

                [[nodiscard]] int get_check_device_count() const;

                [[nodiscard]] std::string get_devices() const;

                [[nodiscard]] std::string get_parameters() const;

                [[nodiscard]] std::string get_devices_entry_point() const;

                void set_entry_point(const std::string &);

                void set_devices_entry_point(const std::string &);

                void set_check_device_count(int);

                void set_parameters(const std::string &);

#ifdef BITSET_FLAGS_UPDATE
                [[nodiscard]] bool get_flag_value(int position);

                void set_flag_value(int position);

#elifndef BITSET_FLAGS_UPDATE

                [[nodiscard]] bool get_is_colored() const;

                [[nodiscard]] bool get_is_comments() const;

                [[nodiscard]] bool get_is_time_record() const;

                [[nodiscard]] bool get_is_random() const;

                [[nodiscard]] bool get_is_high_priority() const;

                [[nodiscard]] bool get_is_everything_now() const;

                [[nodiscard]] bool get_is_usual_run() const;

                [[nodiscard]] bool get_is_choose_run() const;

                [[nodiscard]] bool get_is_file_write() const;

                [[nodiscard]] bool get_gui() const;

                ////////////////////////setters

                void set_is_colored(bool);

                void set_is_comments(bool);

                void set_is_time_record(bool);

                void set_is_random_run(bool);

                void set_is_high_priority(bool);

                void set_is_everything_now(bool);

                void set_is_usual_strat(bool);

                void set_is_choose_strat(bool);

                void set_is_file_write(bool);

                void set_is_gui(bool);
#endif
        };

        ////////////////////////realizations

#ifdef BITSET_FLAGS_UPDATE

        /**
         * Get flag value from bitset, start position from left
         * @return bool value
         */
        inline bool Load_parameters::get_flag_value(const int position) {
            flags.flip();
            const bool res = flags.test(position);
            flags.flip();
            return res;
        }

        /**
         * Set a bit at position to true, start position from left
         * @param position flag position
         * @throw out_of_range if position is wrong
         */
        inline void Load_parameters::set_flag_value(const int position) {
            if ((position <= 15) && (position >= 0)) {
                flags.flip();
                flags.set(position, true);
                flags.flip();
            } else {
                throw std::out_of_range("Position value cannot be - " + std::to_string(position));
            }
        }

#elifndef BITSET_FLAGS_UPDATE
        inline int Load_parameters::get_check_device_count() const {
            return check_device_count;
        }

        inline std::string Load_parameters::get_entry_point() const {
            return entry_point;
        }

        inline bool Load_parameters::get_is_colored() const {
            return is_colored;
        }

        inline bool Load_parameters::get_is_comments() const {
            return is_comments;
        }

        inline bool Load_parameters::get_is_time_record() const {
            return is_time_record;
        }

        inline bool Load_parameters::get_is_random() const {
            return is_random_run;
        }

        inline bool Load_parameters::get_is_high_priority() const {
            return is_high_priority;
        }

        inline bool Load_parameters::get_is_everything_now() const {
            return is_everything_now;
        }

        inline bool Load_parameters::get_is_usual_run() const {
            return is_usual_run;
        }

        inline bool Load_parameters::get_is_choose_run() const {
            return is_choose_run;
        }

        inline std::string Load_parameters::get_devices_entry_point() const {
            return devices_entry_point;
        }

        inline bool Load_parameters::get_is_file_write() const {
            return is_file_write;
        }

        inline std::string Load_parameters::get_parameters() const {
            return parameters;
        }

        inline bool Load_parameters::get_gui() const {
            return is_gui;
        }

        inline void Load_parameters::set_entry_point(const std::string &new_entry_point) {
            this->entry_point = new_entry_point;
        }

        inline void Load_parameters::set_check_device_count(const int new_check_device_count) {
            this->check_device_count = new_check_device_count;
        }

        inline void Load_parameters::set_devices_entry_point(const std::string &devices_entry_point) {
            this->devices_entry_point = devices_entry_point;
        }

        inline void Load_parameters::set_is_colored(const bool colored) {
            this->is_colored = colored;
        }

        inline void Load_parameters::set_is_comments(const bool comments) {
            this->is_comments = comments;
        }

        inline void Load_parameters::set_is_time_record(const bool time_record) {
            this->is_time_record = time_record;
        }

        inline void Load_parameters::set_is_random_run(const bool random_run) {
            this->is_random_run = random_run;
        }

        inline void Load_parameters::set_is_high_priority(const bool high_prior) {
            this->is_high_priority = high_prior;
        }

        inline void Load_parameters::set_is_everything_now(const bool everything_now) {
            this->is_everything_now = everything_now;
        }

        inline void Load_parameters::set_is_usual_strat(const bool usual) {
            this->is_usual_run = usual;
        }

        inline void Load_parameters::set_is_choose_strat(const bool choose) {
            this->is_usual_run = choose;
        }

        inline void Load_parameters::set_is_file_write(const bool get_devices) {
            this->is_file_write = get_devices;
        }

        inline void Load_parameters::set_parameters(const std::string &parameters) {
            this->parameters = parameters;
        }

        inline void Load_parameters::set_is_gui(const bool gui) {
            this->is_gui = gui;
        }

#endif
        /**
         * Static names of utility parameters.
         * Ex. --suit=<filename> or --strategy=high_prior
         * Ex. --suit=<filename> --devices=<devices file name>
         */
        struct Static_load_parameters_names {
            // main options
            static constexpr std::string entry   = "suit";      ///usually string value, include test suit (required)
            static constexpr std::string devices = "devices";   ///devices on which start utility (optional)
            static constexpr std::string strat   = "strategy";  ///for use strategy in utility
            static constexpr std::string inter   = "interface"; ///for interface - can be 'gui' or 'console'

            // special options, can be not specified (optional)
            static constexpr std::string time_check = "time_record";
            static constexpr std::string comments   = "comment";
            static constexpr std::string colored    = "colored";
            static constexpr std::string parameters = "parameters";

            // strategies, cannot be None (required)
            static constexpr std::string high_prior_strat = "high_prior";
            static constexpr std::string random_strat     = "random";
            static constexpr std::string parallel_strat   = "parallel";
            static constexpr std::string everything_now   = "everything_now"; ///not presented in main check cli
            static constexpr std::string usual_strat      = "usual";
            static constexpr std::string choose_strat     = "choose";

            //other
            static constexpr std::string help = "--help";
        };
    }
}

#endif
