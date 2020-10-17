#pragma once

#include <stddef.h>
#include <string.h>

#include <array>
#include <memory>
#include <tuple>
#include <any>

extern "C"
{
#include "context.h"
}

namespace mio
{
    template<size_t SIZE_>
    class coroutine
    {
    private:
        struct
        {
            std::array<char, SIZE_> base;
            char * end;
            char * top;
        }stack_;

        char *caller_context_;

        std::any arg_swap_;

    public:
        template <typename Callable, typename... Args>
        coroutine( Callable callable, Args... args)
        {
            //初始化栈
            this->stack_.end = &this->stack_.base[SIZE_];
            this->stack_.top = this->stack_.end;

            using tuple_args_t = decltype(std::tuple(callable, std::forward_as_tuple(*this, args...)));
            tuple_args_t tuple_args(callable, std::forward_as_tuple(*this, args...));

            create_context(&this->stack_.top, [](void *arg) {
                tuple_args_t tuple_args = *(tuple_args_t *)arg;

                Callable &callable = std::get<0>(tuple_args);
                auto &args = std::get<1>(tuple_args);
                std::get<0>(args). template yeid();
                std::apply(callable, args);       
            });

            swap_context(&caller_context_, this->stack_.top, &tuple_args);
        }

        void resume()
        {
            swap_context(&caller_context_, this->stack_.top, nullptr);
        }

        template <typename T_1>
        T_1 resume()
        {
            swap_context(&caller_context_, this->stack_.top, nullptr);
            return std::any_cast<T_1>(arg_swap_);
        }

        template <typename T_1, typename T_2>
        T_1 resume(const T_2 & arg)
        {
            arg_swap_ = arg;
            swap_context(&caller_context_, this->stack_.top, nullptr);
            return std::any_cast<T_1>(arg_swap_);
        }
        
        void yeid()
        {
            swap_context(&this->stack_.top, this->caller_context_, nullptr);
        }

        template <typename T_1>
        T_1 yeid()
        {
            swap_context(&this->stack_.top, this->caller_context_, nullptr);
            return std::any_cast<T_1>(arg_swap_);
        }

        template <typename T_1, typename T_2>
        T_1 yeid(const T_2 & arg)
        {
            arg_swap_ = arg;
            swap_context(&this->stack_.top, this->caller_context_, nullptr);
            return std::any_cast<T_1>(arg_swap_);
        }
    };
} // namespace mio