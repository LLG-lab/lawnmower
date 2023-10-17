#include <output_terminal.hpp>
#include <fonts.hpp>
#include <aux_functions.hpp>

#ifdef OT_TEST
#include <iostream>
#endif

output_terminal::output_terminal(int width, int height)
    : data_ { static_cast<long unsigned int>(height) },
      line_constraint_ { width },
      last_update_ { 0ul },
      mode_ { terminal_mode::PREVIEW }
{
}

void output_terminal::render(screen_buffer &sb)
{
    if (mode_ == terminal_mode::PREVIEW)
    {
        auto now = aux::get_current_timestamp();

        if (now - last_update_ > 4000)
        {
            return;
        }
    }

    int cur_y = 10;
    pixel pix;
    pix.r = 255;
    pix.g = 255;
    pix.b = 255;

    for (int row = 0; row < get_terminal_data().size(); row++)
    {
        int cur_x = 30;
        const std::string &s = get_terminal_data().at(row);
        for (int charnum = 0; charnum < s.length(); charnum++)
        {
            unsigned char c = s[charnum];

            for (int gy = 0; gy < 16; gy++)
            {
                for (int gx = 0; gx < 8; gx++)
                {
                    if (glyph_pixel(c, gx, gy))
                    {
                        sb.put_pixel(cur_x+gx, cur_y+gy, pix);
                    }
                }
            }

            cur_x += 9;
        }

        cur_y += 20;
    }
}

#ifdef OT_TEST
void output_terminal::display(void) const
{
    for (auto it = data_.rbegin(); it != data_.rend(); it++)
    {
        std::cout << (*it) << std::endl;
    }
}
#endif /* OT_TEST */

void output_terminal::new_line(void)
{
    for (int i = data_.size() - 2; i >= 0; i--)
    {
        data_[i+1] = data_[i];
    }

    data_[0].clear();
}

void output_terminal::put_data(const std::string &s)
{
    size_t len = s.length();

    for (int i = 0; i < len; i++)
    {
        char c = s[i];

        if (data_[0].length() == line_constraint_)
        {
            new_line();
        }

        if (c == '\n')
        {
            new_line();
        }
        else
        {
            data_[0].push_back(c);
        }
    }

    last_update_ = aux::get_current_timestamp();
}
