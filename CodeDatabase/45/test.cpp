#include <iostream>
#include <string>

bool is_digits(const std::string &str) {
    return !str.empty() && str.find_first_not_of("0123456789") == std::string::npos;
}

int main() {
    std::string str1 = "Hello world";
    std::string str2 = "123";
    std::string str3 = "";  // пустая строка
    
    std::cout << (is_digits(str1) ? "Yes" : "No") << "\n";  // No
    std::cout << (is_digits(str2) ? "Yes" : "No") << "\n";  // Yes
    std::cout << (is_digits(str3) ? "Yes" : "No") << "\n";  // No
    
    return 0;
}