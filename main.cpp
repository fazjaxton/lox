#include <iostream>

#include "scanner.h"

using namespace std;

int main()
{
    string line;
    Scanner scanner{};

    while (getline(cin, line)) {
        scanner.scan(line);
        scanner.incLine();
    }
    auto tokens = scanner.complete();
    for (auto const & token : tokens) {
        cout << token.toString() << endl;
    }

    return 0;
}
