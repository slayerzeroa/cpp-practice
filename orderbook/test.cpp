// main.cpp
#include "price_client.hpp"
#include <iostream>

int main() {
    try {
        // 실계정으로 테스트하려면 다음 한 줄만 변경:
        // BINANCE_BASE = "https://api.binance.com"; // 공식 REST base들 중 하나. :contentReference[oaicite:5]{index=5}

        auto last = get_last_price("SOLUSDT");
        std::cout << "Last price: " << last << "\n";

        auto bt = get_book_ticker("SOLUSDT");
        std::cout << "Bid: " << bt.bid << " / Ask: " << bt.ask << "\n";

        auto ks = get_klines("SOLUSDT", "1m", 5);
        for (auto& k : ks) {
            std::cout << k.openTime << " O:" << k.open << " H:" << k.high
                      << " L:" << k.low << " C:" << k.close
                      << " V:" << k.volume << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "ERR: " << e.what() << "\n";
    }
}
