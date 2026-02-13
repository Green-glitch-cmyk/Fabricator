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
    // Добавляем больше цветов
    enum class ConsoleColor {
        // Основные
        Default,
        Black,
        White,

        // Стандартные
        Green,
        Yellow,
        Red,
        Blue,
        Cyan,
        Magenta,      // новый

        // Яркие варианты
        BrightGreen,   // новый
        BrightYellow,  // новый
        BrightRed,     // новый
        BrightBlue,    // новый
        BrightCyan,    // новый
        BrightMagenta, // новый

        // Фоновые (для особых случаев)
        BgBlack,       // новый
        BgWhite,       // новый
        BgRed,         // новый
        BgGreen,       // новый
        BgBlue         // новый
    };
}

#endif // FABENUMS_H