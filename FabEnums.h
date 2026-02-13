#pragma once
#ifndef FABENUMS_H
#define FABENUMS_H

namespace Fabricator {
    // Состояния системы
    enum class SystemState {
        Initializing,
        Running,
        ShuttingDown,
        Error,
        Idle
    };

    // Типы сообщений логгера
    enum class LogLevel {
        Info,
        Warning,
        Error,
        Debug,
        Network,
        System
    };

    // Статусы компонентов
    enum class ComponentStatus {
        Uninitialized,
        Initializing,
        Ready,
        Failed,
        Disabled
    };

    // Типы команд
    enum class CommandType {
        Unknown,
        Help,
        Clear,
        Exit,
        SystemInfo,
        NetworkStatus,
        Custom
    };

    // Цветовые коды (для эмуляции)
    enum class ConsoleColor {
        Default,
        Green,
        Yellow,
        Red,
        Blue,
        Cyan,
        White
    };
}

#endif // FABENUMS_H