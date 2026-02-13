#include "FabCore.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <windows.h>

namespace Fabricator {

    // Внутренняя реализация FabCore
    struct FabCore::Impl {
        std::vector<std::shared_ptr<ISystemComponent>> components;
        SystemState currentState = SystemState::Idle;
        bool isRunning = false;
        std::unique_ptr<Logger> logger;
        std::unique_ptr<NetworkManager> network;
        std::unique_ptr<ScreenManager> screen;
        std::unique_ptr<IOManager> io;

        Impl()
            : logger(std::make_unique<Logger>())
            , network(std::make_unique<NetworkManager>())
            , screen(std::make_unique<ScreenManager>())
            , io(std::make_unique<IOManager>()) {
        }
    };

    FabCore::FabCore() : pImpl(std::make_unique<Impl>()) {
        // Регистрируем компоненты
        pImpl->components.push_back(std::move(pImpl->logger));
        pImpl->components.push_back(std::move(pImpl->network));
        pImpl->components.push_back(std::move(pImpl->screen));
        pImpl->components.push_back(std::move(pImpl->io));
    }

    FabCore::~FabCore() {
        Shutdown();
    }

    bool FabCore::Initialize() {
        pImpl->currentState = SystemState::Initializing;

        // Устанавливаем кодировку для консоли
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        std::cout << "[Fabricator] Initializing..." << std::endl;

        InitializeComponents();

        pImpl->currentState = SystemState::Running;
        pImpl->isRunning = true;

        return true;
    }

    void FabCore::Shutdown() {
        if (pImpl->currentState != SystemState::ShuttingDown) {
            pImpl->currentState = SystemState::ShuttingDown;
            pImpl->isRunning = false;

            for (auto& component : pImpl->components) {
                component->Shutdown();
            }

            std::cout << "\n[Fabricator] Shutting down..." << std::endl;
        }
    }

    void FabCore::Run() {
        if (!pImpl->isRunning) {
            return;
        }

        PrintBanner();

        while (pImpl->isRunning && pImpl->currentState == SystemState::Running) {
            ProcessCommands();
            UpdateComponents();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void FabCore::RegisterComponent(std::shared_ptr<ISystemComponent> component) {
        pImpl->components.push_back(component);
    }

    void FabCore::UnregisterComponent(const std::string& componentName) {
        auto it = std::remove_if(pImpl->components.begin(), pImpl->components.end(),
            [&componentName](const auto& comp) {
                return comp->GetName() == componentName;
            });
        pImpl->components.erase(it, pImpl->components.end());
    }

    SystemState FabCore::GetState() const {
        return pImpl->currentState;
    }

    std::string FabCore::GetVersion() const {
        return "Fabricator 1.0.0f201 Pre-release";
    }

    void FabCore::PrintBanner() const {
        std::cout << "[Fabricator] " << GetVersion() << std::endl;
        std::cout << std::endl;
    }

    void FabCore::InitializeComponents() {
        for (auto& component : pImpl->components) {
            if (component->Initialize()) {
                std::cout << component->GetName() << std::endl;
            }
        }
    }

    void FabCore::ProcessCommands() {
        std::cout << "<fabricatorguest>> ";
        std::string input;
        std::getline(std::cin, input);

        if (!input.empty()) {
            auto cmdType = pImpl->io->ParseCommand(input);

            switch (cmdType) {
            case CommandType::Exit:
                Shutdown();
                break;
            case CommandType::Clear:
                system("cls");
                PrintBanner();
                break;
            case CommandType::Help:
                std::cout << "Available commands: help, clear, exit, info, network" << std::endl;
                break;
            case CommandType::SystemInfo:
                std::cout << "Fabricator ver. 1.0.0f201" << std::endl;
                std::cout << "Components: " << pImpl->components.size() << " loaded" << std::endl;
                break;
            case CommandType::NetworkStatus:
                pImpl->network->SimulateNetworkActivity();
                break;
            default:
                std::cout << "Unknown command: " << input << std::endl;
                break;
            }
        }
    }

    void FabCore::UpdateComponents() {
        for (auto& component : pImpl->components) {
            component->Update();
        }
    }

    // Реализация Logger
    Logger::Logger() : m_status(ComponentStatus::Uninitialized) {}

    bool Logger::Initialize() {
        m_status = ComponentStatus::Initializing;
        // Инициализация логгера
        m_status = ComponentStatus::Ready;
        return true;
    }

    void Logger::Shutdown() {
        m_status = ComponentStatus::Disabled;
    }

    void Logger::Log(const std::string& message, LogLevel level) {
        std::string prefix;
        switch (level) {
        case LogLevel::Info: prefix = "[INFO]"; break;
        case LogLevel::Warning: prefix = "[WARN]"; break;
        case LogLevel::Error: prefix = "[ERROR]"; break;
        case LogLevel::Debug: prefix = "[DEBUG]"; break;
        case LogLevel::Network: prefix = "[NET]"; break;
        case LogLevel::System: prefix = "[SYS]"; break;
        }

        std::string logMessage = prefix + " " + message;
        m_logHistory.push_back(logMessage);
        std::cout << logMessage << std::endl;
    }

    void Logger::SetColor(ConsoleColor color) const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (color) {
        case ConsoleColor::Green: SetConsoleTextAttribute(hConsole, 10); break;
        case ConsoleColor::Yellow: SetConsoleTextAttribute(hConsole, 14); break;
        case ConsoleColor::Red: SetConsoleTextAttribute(hConsole, 12); break;
        case ConsoleColor::Blue: SetConsoleTextAttribute(hConsole, 9); break;
        case ConsoleColor::Cyan: SetConsoleTextAttribute(hConsole, 11); break;
        case ConsoleColor::White: SetConsoleTextAttribute(hConsole, 15); break;
        default: SetConsoleTextAttribute(hConsole, 7); break;
        }
    }

    void Logger::ResetColor() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);
    }

    // Реализация NetworkManager
    NetworkManager::NetworkManager() : m_status(ComponentStatus::Uninitialized) {}

    bool NetworkManager::Initialize() {
        m_status = ComponentStatus::Initializing;
        // Инициализация сети
        m_status = ComponentStatus::Ready;
        return true;
    }

    void NetworkManager::Shutdown() {
        m_status = ComponentStatus::Disabled;
    }

    void NetworkManager::Update() {
        // Периодические сетевые операции
    }

    void NetworkManager::SimulateNetworkActivity() {
        std::cout << "[NETWORK] Scanning interfaces..." << std::endl;
        std::cout << "[NETWORK] eth0: 192.168.1.100" << std::endl;
        std::cout << "[NETWORK] wlan0: 10.0.0.5" << std::endl;
        std::cout << "[NETWORK] Status: Connected" << std::endl;
    }

    // Реализация ScreenManager
    ScreenManager::ScreenManager() : m_status(ComponentStatus::Uninitialized) {}

    bool ScreenManager::Initialize() {
        m_status = ComponentStatus::Initializing;
        // Инициализация экрана
        m_status = ComponentStatus::Ready;
        return true;
    }

    void ScreenManager::Shutdown() {
        m_status = ComponentStatus::Disabled;
    }

    void ScreenManager::Clear() {
        system("cls");
    }

    void ScreenManager::DrawPrompt() const {
        std::cout << "<fabricatorguest>> ";
    }

    // Реализация IOManager
    IOManager::IOManager() : m_status(ComponentStatus::Uninitialized) {}

    bool IOManager::Initialize() {
        m_status = ComponentStatus::Initializing;
        // Инициализация ввода/вывода
        m_status = ComponentStatus::Ready;
        return true;
    }

    void IOManager::Shutdown() {
        m_status = ComponentStatus::Disabled;
    }

    void IOManager::Update() {
        // Обработка ввода/вывода
    }

    std::string IOManager::ReadInput() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    CommandType IOManager::ParseCommand(const std::string& input) const {
        std::string cmd = input;
        // Преобразуем в нижний регистр для сравнения
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "exit" || cmd == "quit") return CommandType::Exit;
        if (cmd == "help" || cmd == "?") return CommandType::Help;
        if (cmd == "clear" || cmd == "cls") return CommandType::Clear;
        if (cmd == "info" || cmd == "system") return CommandType::SystemInfo;
        if (cmd == "network" || cmd == "net") return CommandType::NetworkStatus;

        return CommandType::Unknown;
    }

    void IOManager::ExecuteCommand(const std::string& input) {
        // Базовая обработка команд
        auto cmdType = ParseCommand(input);
        switch (cmdType) {
        case CommandType::Help:
            std::cout << "Available commands: help, clear, exit, info, network" << std::endl;
            break;
        default:
            std::cout << "Command executed: " << input << std::endl;
            break;
        }
    }

}