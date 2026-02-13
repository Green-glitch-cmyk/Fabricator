#pragma once
#ifndef FABCORE_H
#define FABCORE_H

#include "FabEnums.h"
#include <string>
#include <memory>
#include <vector>

namespace Fabricator {

    // Интерфейс компонента системы
    class ISystemComponent {
    public:
        virtual ~ISystemComponent() = default;
        virtual bool Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual std::string GetName() const = 0;
        virtual ComponentStatus GetStatus() const = 0;
        virtual void Update() = 0;
    };

    // Основное ядро системы
    class FabCore {
    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        FabCore();
        ~FabCore();

        bool Initialize();
        void Shutdown();
        void Run();

        // Управление компонентами
        void RegisterComponent(std::shared_ptr<ISystemComponent> component);
        void UnregisterComponent(const std::string& componentName);

        // Системные функции
        SystemState GetState() const;
        std::string GetVersion() const;
        void PrintBanner() const;

    private:
        void InitializeComponents();
        void ProcessCommands();
        void UpdateComponents();
    };

    // Логгер системы
    class Logger : public ISystemComponent {
    private:
        ComponentStatus m_status;
        std::vector<std::string> m_logHistory;

    public:
        Logger();
        bool Initialize() override;
        void Shutdown() override;
        std::string GetName() const override { return "LOGGER"; }
        ComponentStatus GetStatus() const override { return m_status; }
        void Update() override {}

        void Log(const std::string& message, LogLevel level = LogLevel::Info);
        void SetColor(ConsoleColor color) const;
        void ResetColor() const;
    };

    // Сетевой менеджер
    class NetworkManager : public ISystemComponent {
    private:
        ComponentStatus m_status;

    public:
        NetworkManager();
        bool Initialize() override;
        void Shutdown() override;
        std::string GetName() const override { return "NETWORK"; }
        ComponentStatus GetStatus() const override { return m_status; }
        void Update() override;

        void SimulateNetworkActivity();
    };

    // Экранный менеджер
    class ScreenManager : public ISystemComponent {
    private:
        ComponentStatus m_status;

    public:
        ScreenManager();
        bool Initialize() override;
        void Shutdown() override;
        std::string GetName() const override { return "SCREEN"; }
        ComponentStatus GetStatus() const override { return m_status; }
        void Update() override {}

        void Clear();
        void DrawPrompt() const;
    };

    // IO Stream менеджер
    class IOManager : public ISystemComponent {
    private:
        ComponentStatus m_status;

    public:
        IOManager();
        bool Initialize() override;
        void Shutdown() override;
        std::string GetName() const override { return "IOSTREAM"; }
        ComponentStatus GetStatus() const override { return m_status; }
        void Update() override;

        std::string ReadInput();
        CommandType ParseCommand(const std::string& input) const;
        void ExecuteCommand(const std::string& input);
    };

}

#endif // FABCORE_H