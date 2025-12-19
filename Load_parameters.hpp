#ifndef LOAD_PARAMETERS_HPP
#define LOAD_PARAMETERS_HPP
#include <string>

/**
 * Namespace for load parameters
 */
namespace LP {
    /**
     * Utility load parameters struct.
     * Simply stores load compiler data and provide it outside.
     */
    struct Load_parameters {
    private:
        //main utility parameters, part of it is required:
        std::string entry_point; ///entry point of utility use.
        std::string devices_entry_point; ///file with devices to test on.
        std::string parameters; ///main parameters in utility.
        int check_device_count{}; ///count of devices for test on.
        bool is_gui{}; ///is need to run in graphics user interface. true - 'gui', false - 'console' (default).

        //other utility parameters:
        bool is_colored{}; ///is need for colored output.
        bool is_comments{}; ///is need for comments in utility.
        bool is_time_record{}; ///for each test case to record time.
        bool is_file_write{}; ///is need for file write at the end of test run.

        //strategies to run:
        bool is_random_run{}; ///randomly run test cases.
        bool is_parallel_run{}; ///strategy for parallel adding tests.
        bool is_high_priority{}; ///runs only high priority test cases.
        bool is_everything_now{}; ///usually prints all tests in console or anything.
        bool is_usual_run{}; ///simple get test cases without actions.
        bool is_choose_run{}; ///choose severity of test cases.

    public:
        Load_parameters() = default;

        ~Load_parameters() = default;

        ////////////////////////getter

        [[nodiscard]] std::string get_entry_point() const;

        [[nodiscard]] int get_check_device_count() const;

        [[nodiscard]] bool get_is_colored() const;

        [[nodiscard]] bool get_is_comments() const;

        [[nodiscard]] bool get_is_time_record() const;

        [[nodiscard]] bool get_is_random() const;

        [[nodiscard]] bool get_is_high_priority() const;

        [[nodiscard]] bool get_is_everything_now() const;

        [[nodiscard]] bool get_is_usual_run() const;

        [[nodiscard]] bool get_is_choose_run() const;

        [[nodiscard]] std::string get_devices_entry_point() const;

        [[nodiscard]] bool get_is_file_write() const;

        [[nodiscard]] std::string get_devices() const;

        [[nodiscard]] std::vector<std::string> get_cli_parameters() const;

        [[nodiscard]] std::string get_parameters() const;

        [[nodiscard]] bool get_gui() const;

        ////////////////////////setters

        void set_entry_point(const std::string &);

        void set_devices_entry_point(const std::string &);

        void set_check_device_count(int);

        void set_is_colored(bool);

        void set_is_comments(bool);

        void set_is_time_record(bool);

        void set_is_random_run(bool);

        void set_is_high_priority(bool);

        void set_is_everything_now(bool);

        void set_is_usual_strat(bool);

        void set_is_choose_strat(bool);

        void set_is_file_write(bool);

        void set_parameters(const std::string &);

        void set_is_gui(bool);
    };

    ////////////////////////realizations

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

    inline void Load_parameters::set_is_gui(const bool new_gui) {
        this->is_gui = new_gui;
    }

    /**
     * Static names of utility parameters.
     * Ex. --suit=<filename> or --strategy=high_prior
     * Ex. --suit=<filename> --devices=<devices file name>
     */
    struct Static_load_parameters_names {
        // main options
        static constexpr std::string entry = "suit"; ///usually string value, include test suit (required)
        static constexpr std::string devices = "devices"; ///devices on which start utility (optional)
        static constexpr std::string strat = "strategy"; ///for use strategy in utility
        static constexpr std::string inter = "interface"; ///for interface - can be 'gui' or 'console'

        // special options, can be not specified (optional)
        static constexpr std::string time_check = "time_record";
        static constexpr std::string comments = "comment";
        static constexpr std::string colored = "colored";
        static constexpr std::string parameters = "parameters";

        // strategies, cannot be None (required)
        static constexpr std::string high_prior_strat = "high_prior";
        static constexpr std::string random_strat = "random";
        static constexpr std::string parallel_strat = "parallel";
        static constexpr std::string everything_now = "everything_now"; ///not presented in main check cli
        static constexpr std::string usual_strat = "usual";
        static constexpr std::string choose_strat = "choose";
    };
}

#endif
