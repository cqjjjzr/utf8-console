#include <string_view>
#include <iomanip>
#include "utf8.h"

extern "C" int u8c_wcwidth(int ucs);

class wcswidth_iterator
{
private:
    size_t _result = 0;

public:
    [[nodiscard]] size_t result() const { return _result; }
    void reset() { _result = 0; }

    wcswidth_iterator& operator=(int value)
    {
        auto len = u8c_wcwidth(value);
        if (len >= 0)
            _result += len;
        return *this;
    }

    wcswidth_iterator& operator*() { return *this; }
    wcswidth_iterator& operator++() { return *this; }
    wcswidth_iterator& operator++(int) { return *this; }
};

size_t utf8_cswidth(std::string_view str)
{
    return utf8::utf8to32(str.begin(), str.end(), wcswidth_iterator()).result();
}

decltype(std::setw(0)) setw_u8(int w, std::string_view u8s)
{
    return std::setw(w + u8s.length() - utf8_cswidth(u8s));
}