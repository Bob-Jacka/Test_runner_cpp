#ifndef LOAD_PARAMETERS_HPP
#define LOAD_PARAMETERS_HPP
#include <string>

/**
 * Namespace for load parameters
 */
namespace LP {
    /**
     * Utility load parameters struct.
     * Simply stores load compiler data.
     */
    struct Load_parameters {
    private:
        std::string entry_point; ///entry point of utility use.
        std::string devices_entry_point; //file with devices
        int check_device_count{}; ///count of devices for test on.

        bool is_colored{};
        bool is_comments{};
        bool is_time_record{}; ///for each test case to record time
        bool is_file_write{}; ///is need for file write at the end of test run

        //strategies to run
        bool is_random_run{}; ///randomly run test cases
        bool is_parallel_run{}; ///
        bool is_high_priority{}; ///runs only high priority test cases

    public:
        Load_parameters();

        [[nodiscard]] std::string get_entry_point() const;

        [[nodiscard]] int get_check_device_count() const;

        [[nodiscard]] bool get_is_colored() const;

        [[nodiscard]] bool get_is_comments() const;

        [[nodiscard]] bool get_is_time_record() const;

        [[nodiscard]] bool get_is_random() const;

        [[nodiscard]] bool get_is_high_priority() const;

        [[nodiscard]] std::string get_devices_entry_point() const;

        [[nodiscard]] bool get_is_file_write() const;

        void set_entry_point(const std::string &);

        void set_devices_entry_point(const std::string& devices_entry_point);

        void set_check_device_count(int);

        void set_is_colored(bool);

        void set_is_comments(bool);

        void set_is_time_record(bool);

        void set_is_random_run(bool);

        void set_is_high_priority(bool);

        void set_is_file_write(bool);
    };

    inline Load_parameters::Load_parameters() {
    }

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

    inline std::string Load_parameters::get_devices_entry_point() const {
        return devices_entry_point;
    }

    inline bool Load_parameters::get_is_file_write() const {
        return is_file_write;
    }

    inline void Load_parameters::set_entry_point(const std::string &) {
        this->entry_point = entry_point;
    }

    inline void Load_parameters::set_check_device_count(const int) {
        this->check_device_count = check_device_count;
    }

    inline void Load_parameters::set_devices_entry_point(const std::string& devices_entry_point) {
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

    inline void Load_parameters::set_is_file_write(const bool get_devices) {
        this->is_file_write = get_devices;
    }

    /**
     * Static names of utility parameters.
     * Ex. --suit=<filename> or --strategy=high_prior
     * Ex. --suit=<filename> --devices=<devices list>
     */
    struct Static_load_parameters_names {
        // main options
        static constexpr std::string entry = "suit"; ///usually string value, include test suit (required)
        static constexpr std::string devices = "devices"; ///devices on which start utility (optional)
        static constexpr std::string strat = "strategy"; ///for use strategy

        // special options, can be not specified (optional)
        static constexpr std::string time_check = "time_record";
        static constexpr std::string comments = "comment";
        static constexpr std::string colored = "colored";

        // strategies, cannot be None (required)
        static constexpr std::string high_prior_strat = "high_prior";
        static constexpr std::string random_strat = "random";
        static constexpr std::string parallel_strat = "parallel";
    };
}

#endif
