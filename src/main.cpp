#include "app.hpp"
#include <cstdlib>

int main() {
    auto maybe_app = App::make();
    if (!maybe_app.has_value()) {
        return EXIT_FAILURE;
    }
    maybe_app.value().run();
}