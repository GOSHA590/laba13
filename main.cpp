#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cctype>
#include <sstream>

using namespace std;

bool isDigitsOnly(const string& str) {
    for (char c : str) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool isDouble(const string& str) {
    bool dotFound = false;
    for (char c : str) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            if (c == '.' && !dotFound) {
                dotFound = true;
            } else if (c == ',' && !dotFound) {
                dotFound = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool isValidStationName(const string& name) {
    if (name.empty()) return false;

    const string forbiddenChars = "!@#$%^&*()+=[]{}|;:,.<>?/\\\"'~`";
    
    for (char c : name) {
        if (forbiddenChars.find(c) != string::npos) {
            return false;
        }
    }
    return true;
}

bool isInteger(const string& str) {
    for (char c : str) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

int getIntInput(const string& prompt) {
    string input;
    int value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым!" << endl;
            continue;
        }

        if (!isDigitsOnly(input)) {
            cout << "Ошибка: Введите целое положительное число без букв и символов!" << endl;
            continue;
        }
        
        try {
            value = stoi(input);
            if (value <= 0) {
                cout << "Ошибка: Число должно быть положительным!" << endl;
                continue;
            }
            break;
        } catch (const exception& e) {
            cout << "Ошибка: Недопустимое число! Попробуйте снова." << endl;
        }
    }
    
    return value;
}

int getPercentInput(const string& prompt) {
    string input;
    int value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым!" << endl;
            continue;
        }

        if (!isInteger(input)) {
            cout << "Ошибка: Введите целое число без букв, символов и дробной части!" << endl;
            continue;
        }
        
        try {
            value = stoi(input);
            if (value < 0 || value > 100) {
                cout << "Ошибка: Процент должен быть от 0 до 100!" << endl;
                continue;
            }
            break;
        } catch (const exception& e) {
            cout << "Ошибка: Недопустимое число! Попробуйте снова." << endl;
        }
    }
    
    return value;
}

double getDoubleInput(const string& prompt) {
    string input;
    double value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым!" << endl;
            continue;
        }

        replace(input.begin(), input.end(), ',', '.');

        if (!isDouble(input)) {
            cout << "Ошибка: Введите число в формате 12345 без букв и лишних символов!" << endl;
            continue;
        }
        
        try {
            value = stod(input);
            if (value <= 0) {
                cout << "Ошибка: Число должно быть положительным!" << endl;
                continue;
            }
            break;
        } catch (const exception& e) {
            cout << "Ошибка: Недопустимое число! Попробуйте снова." << endl;
        }
    }
    
    return value;
}

string getStringInput(const string& prompt) {
    string input;
    
    while (true) {
        cout << prompt;
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым!" << endl;
            continue;
        }

        if (!isValidStationName(input)) {
            cout << "Ошибка: Название содержит запрещенные символы! Разрешены только буквы, цифры, пробелы, дефисы и подчеркивания." << endl;
            continue;
        }
        
        break;
    }
    
    return input;
}

class ITariff {
public:
    virtual ~ITariff() = default;
    virtual double calculatePrice() const = 0;
    virtual string getDestination() const = 0;
    virtual string getInfo() const = 0;
};

class TariffStrategy : public ITariff {
protected:
    string destination;
    double basePrice;

public:
    TariffStrategy(const string& dest, double price)
        : destination(dest), basePrice(price) {

        if (dest.empty()) {
            throw invalid_argument("Направление не может быть пустым");
        }
        if (price <= 0) {
            throw invalid_argument("Цена должна быть положительной");
        }
        if (price > 1000000) {
            throw invalid_argument("Цена не может превышать 1 000 000 рублей");
        }
    }

    string getDestination() const override {
        return destination;
    }

    double getBasePrice() const {
        return basePrice;
    }
};

class NoDiscount : public TariffStrategy {
public:
    NoDiscount(const string& dest, double price)
        : TariffStrategy(dest, price) {}

    double calculatePrice() const override {
        return basePrice;
    }

    string getInfo() const override {
        return "Направление: " + destination + ", Цена: " +
               to_string(static_cast<int>(basePrice)) + " руб. (Без скидки)";
    }
};

class WithDiscount : public TariffStrategy {
private:
    int discountPercent;

public:
    WithDiscount(const string& dest, double price, int discount)
        : TariffStrategy(dest, price), discountPercent(discount) {

        if (discount < 0 || discount > 100) {
            throw out_of_range("Скидка должна быть от 0 до 100 процентов");
        }
    }

    double calculatePrice() const override {
        return basePrice * (1 - discountPercent / 100.0);
    }

    int getDiscountPercent() const {
        return discountPercent;
    }

    string getInfo() const override {
        double finalPrice = calculatePrice();
        return "Направление: " + destination + ", Базовая цена: " +
               to_string(static_cast<int>(basePrice)) + " руб., Скидка: " +
               to_string(discountPercent) + "%, Итоговая цена: " +
               to_string(static_cast<int>(finalPrice)) + " руб.";
    }
};

class RailwayStation {
private:
    vector<unique_ptr<ITariff>> tariffs;

public:
    void addTariff(const string& destination, double price) {
        try {
            tariffs.push_back(make_unique<NoDiscount>(destination, price));
            cout << "✓ Тариф добавлен: " << destination << " - " << price << " руб." << endl;
        } catch (const exception& e) {
            cerr << "Ошибка при добавлении тарифа: " << e.what() << endl;
            throw;
        }
    }

    void addTariffWithDiscount(const string& destination, double price, int discount) {
        try {
            tariffs.push_back(make_unique<WithDiscount>(destination, price, discount));
            cout << "✓ Тариф со скидкой добавлен: " << destination << " - "
                      << price << " руб. (" << discount << "% скидка)" << endl;
        } catch (const exception& e) {
            cerr << "Ошибка при добавлении тарифа со скидкой: " << e.what() << endl;
            throw;
        }
    }

    string findCheapestDestination() const {
        if (tariffs.empty()) {
            throw runtime_error("Нет доступных тарифов");
        }

        auto minTariff = min_element(tariffs.begin(), tariffs.end(),
            [](const unique_ptr<ITariff>& a, const unique_ptr<ITariff>& b) {
                return a->calculatePrice() < b->calculatePrice();
            });

        return (*minTariff)->getDestination() + " (Цена: " +
               to_string(static_cast<int>((*minTariff)->calculatePrice())) + " руб.)";
    }

    void displayAllTariffs() const {
        if (tariffs.empty()) {
            cout << "Нет доступных тарифов." << endl;
            return;
        }

        cout << "\n=== Все тарифы ===" << endl;
        for (size_t i = 0; i < tariffs.size(); ++i) {
            cout << i + 1 << ". " << tariffs[i]->getInfo() << endl;
        }
        cout << "===================" << endl;
    }

    size_t getTariffCount() const {
        return tariffs.size();
    }

    void searchByDestination(const string& destination) const {
        bool found = false;
        cout << "\nРезультаты поиска по направлению '" << destination << "':" << endl;
        
        for (const auto& tariff : tariffs) {
            if (tariff->getDestination() == destination) {
                cout << "✓ " << tariff->getInfo() << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "Тарифы по направлению '" << destination << "' не найдены." << endl;
        }
    }
};

void displayMenu() {
    cout << "\n=== Система управления вокзалом ===" << endl;
    cout << "1. Добавить тариф без скидки" << endl;
    cout << "2. Добавить тариф со скидкой" << endl;
    cout << "3. Показать все тарифы" << endl;
    cout << "4. Найти самое дешевое направление" << endl;
    cout << "5. Поиск тарифов по направлению" << endl;
    cout << "6. Количество тарифов" << endl;
    cout << "7. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    RailwayStation station;
    int choice = 0;
    
    cout << "=== Система управления железнодорожным вокзалом ===" << endl;

    do {
        displayMenu();
        choice = getIntInput("");
        
        try {
            switch (choice) {
                case 1: {
                    cout << "\n--- Добавление тарифа без скидки ---" << endl;
                    string destination = getStringInput("Введите направление: ");
                    double price = getDoubleInput("Введите цену (руб.): ");
                    
                    station.addTariff(destination, price);
                    break;
                }
                
                case 2: {
                    cout << "\n--- Добавление тарифа со скидкой ---" << endl;
                    string destination = getStringInput("Введите направление: ");
                    double price = getDoubleInput("Введите базовую цену (руб.): ");
                    int discount = getPercentInput("Введите размер скидки (%): ");
                    
                    station.addTariffWithDiscount(destination, price, discount);
                    break;
                }
                
                case 3: {
                    cout << "\n--- Список всех тарифов ---" << endl;
                    station.displayAllTariffs();
                    break;
                }
                
                case 4: {
                    try {
                        string cheapest = station.findCheapestDestination();
                        cout << "\n--- Самое дешевое направление ---" << endl;
                        cout << "✓ " << cheapest << endl;
                    } catch (const exception& e) {
                        cout << "Ошибка: " << e.what() << endl;
                    }
                    break;
                }
                
                case 5: {
                    cout << "\n--- Поиск тарифов по направлению ---" << endl;
                    string destination = getStringInput("Введите направление для поиска: ");
                    station.searchByDestination(destination);
                    break;
                }
                
                case 6: {
                    cout << "\n--- Статистика ---" << endl;
                    cout << "Количество тарифов в системе: " << station.getTariffCount() << endl;
                    break;
                }
                
                case 7: {
                    cout << "Выход из программы. До свидания!" << endl;
                    return 0;  
                }
                
                default: {
                    cout << "Неверный выбор! Пожалуйста, выберите действие от 1 до 7." << endl;
                    break;
                }
            }
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
        
    } while (true);

    return 0;
}
