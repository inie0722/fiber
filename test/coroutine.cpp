#include <iostream>
#include <memory>
#include <stddef.h>

#include "coroutine.hpp"

using namespace std;
using namespace mio;

int main(void)
{
    size_t max = 999;
    coroutine<1024> co([&](mio::coroutine<1024> &arg) {
        for (size_t i = 0; i < max;)
        {
            i += co.yeid<size_t, size_t>(i);
        }
    });

    co.resume();

    for (size_t i = 0; i < max - 1; i++)
    {
        cout << co.resume<size_t, size_t>(1) << endl;
    }
    return 0;
}
