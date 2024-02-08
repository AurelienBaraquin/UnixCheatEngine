#pragma once

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

namespace CheatEngine::Memory
{
    class Tracer
    {
    public:
        Tracer(pid_t pid);
        ~Tracer();

        bool attach();
        bool detach();

        template<typename T>
        T read(long address) {
            if (!isAttached())
                throw std::runtime_error("Not attached to process");

            T value;
            long data = ptrace(PTRACE_PEEKDATA, m_pid, address, nullptr);

            if (data == -1 && errno != 0)
                throw std::runtime_error("Failed to read memory");

            if (std::memcpy(&value, &data, sizeof(T)) == nullptr)
                throw std::runtime_error("Failed to copy memory");

            return value;
        }

        template<typename T>
        void write(long address, T value) {
            if (!isAttached())
                throw std::runtime_error("Not attached to process");

            long data;

            if (std::memcpy(&data, &value, sizeof(T)) == nullptr)
                throw std::runtime_error("Failed to copy memory");

            if (ptrace(PTRACE_POKEDATA, m_pid, address, data) == -1)
                throw std::runtime_error("Failed to write memory");
        }

        bool isAttached() const;

    private:
        pid_t m_pid;
        bool m_attached = false;
    };
}
