#ifndef __OUTPUT_TERMINAL_HPP__
#define __OUTPUT_TERMINAL_HPP__

#include <string>
#include <vector>

#include <screen_buffer.hpp>

#undef OT_TEST

class output_terminal
{
public:

    output_terminal(void) = delete;
    output_terminal(int width, int height);

    void render(screen_buffer &sb);
    void set_preview_mode(void) { mode_ = terminal_mode::PREVIEW; }
    void set_permanent_mode(void) { mode_ = terminal_mode::PERMANENT; }
    bool is_permanent_mode(void) const { return (mode_ == terminal_mode::PERMANENT); }
    bool is_preview_mode(void) const { return (mode_ == terminal_mode::PREVIEW); }

    //
    // Output stream operators for the most common types.
    //

    output_terminal &operator<<(const std::string &s) { put_data(s); return *this; }
    output_terminal &operator<<(const char *s) { put_data(std::string(s)); return *this; }
    output_terminal &operator<<(bool b) { if (b) put_data("true"); else put_data("false"); return *this; }
    output_terminal &operator<<(int value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(long value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(long long value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(unsigned value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(unsigned long value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(unsigned long long value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(float value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(double value) { put_data(std::to_string(value)); return *this; }
    output_terminal &operator<<(long double value) { put_data(std::to_string(value)); return *this; }

    #ifdef OT_TEST
    void display(void) const;
    #endif

private:

    enum class terminal_mode
    {
        PREVIEW,
        PERMANENT
    };

    typedef std::vector<std::string> terminal_data;

    const terminal_data &get_terminal_data(void) const { return data_; }
    unsigned long get_last_update_(void) const { return last_update_; }


    void new_line(void);
    void put_data(const std::string &s);

    terminal_data data_;
    const int line_constraint_;
    unsigned long last_update_;
    terminal_mode mode_;
};

#endif /* __OUTPUT_TERMINAL_HPP__ */
