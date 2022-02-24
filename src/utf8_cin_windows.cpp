#include <iostream>
#ifdef WIN32
#include <streambuf>
#include <Windows.h>

constexpr unsigned default_win_utf8_cin_buf_size = 2048;

class win_utf8_streambuf : public std::streambuf
{
public:
    win_utf8_streambuf(unsigned read_buf_size = default_win_utf8_cin_buf_size, bool handle_console_eof = false);

protected:
    int_type underflow() override;

private:
    unsigned _read_buf_size, _u8_buf_size, _wide_buf_size;
    std::unique_ptr<char[]> _buffer;
    std::unique_ptr<wchar_t[]> _wide_buffer;

    bool _handle_eof, _eof = false;
};

win_utf8_streambuf::win_utf8_streambuf(unsigned read_buf_size, bool handle_console_eof)
    : _read_buf_size(read_buf_size), _u8_buf_size(read_buf_size * 6), _wide_buf_size(read_buf_size * 2),
      _buffer(std::unique_ptr<char[]>(new char[_u8_buf_size])),
      _wide_buffer(std::unique_ptr<wchar_t[]>(new wchar_t[read_buf_size * 2])),
      _handle_eof(handle_console_eof)
{
}

std::streambuf::int_type win_utf8_streambuf::underflow()
{
    if (_eof)
    {
        setg(nullptr, nullptr, nullptr);
        return traits_type::eof();
    }

    if (gptr() < egptr())
        return traits_type::to_int_type(*this->gptr());

    unsigned long read_len;
    auto ret = ReadConsoleW(
        GetStdHandle(STD_INPUT_HANDLE),
        _wide_buffer.get(),
        _wide_buf_size,
        &read_len, NULL);

    if (!ret || read_len == 0)
    {
        _eof = true;
        setg(nullptr, nullptr, nullptr);
        return traits_type::eof();
    }

    if (_handle_eof)
    {
        auto buf = _wide_buffer.get();
        for (unsigned long i = 0; i < read_len; i++)
        {
            // ReadConsole will read Ctrl+Z as-is, so special handling is needed here
            if (buf[i] == 0x001A)
            {
                read_len = i;
                _eof = true;
            }
        }
    }

    if (read_len == 0)
    {
        _eof = true;
        setg(nullptr, nullptr, nullptr);
        return traits_type::eof();
    }

    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        _wide_buffer.get(),
        read_len,
        _buffer.get(),
        _u8_buf_size,
        NULL, NULL);
    setg(_buffer.get(), _buffer.get(), _buffer.get() + size);
    if (size == 0)
        return traits_type::eof();
    return traits_type::to_int_type(*this->gptr());
}

namespace
{
std::unique_ptr<win_utf8_streambuf> global_win_utf8_streambuf;
}

void initialize_utf8_console_windows()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    if (!global_win_utf8_streambuf)
        global_win_utf8_streambuf = std::make_unique<win_utf8_streambuf>(2048, true);
    std::cin.rdbuf(global_win_utf8_streambuf.get());
}

#endif