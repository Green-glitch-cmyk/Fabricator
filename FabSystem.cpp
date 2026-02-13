#include "FabCore.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // Создаем ядро системы
        auto fabricator = std::make_unique<Fabricator::FabCore>();

        // Инициализируем систему
        if (!fabricator->Initialize()) {
            std::cerr << "Failed to initialize Fabricator system!" << std::endl;
            return 1;
        }

        // Запускаем главный цикл
        fabricator->Run();

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}