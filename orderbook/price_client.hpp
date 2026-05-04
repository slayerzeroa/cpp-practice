// price_client.hpp
#pragma once
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

using json = nlohmann::json;

// 기본: 테스트넷. 실계정은 https://api.binance.com 로 바꾸세요.
inline std::string BINANCE_BASE = "https://testnet.binance.vision"; // Spot Testnet REST base. :contentReference[oaicite:1]{index=1}

// 간단 헬퍼: GET + JSON 파싱
inline json http_get_json(const std::string& url_path) {
    httplib::Client cli(BINANCE_BASE.c_str());
    cli.set_connection_timeout(5);  // sec
    cli.set_read_timeout(5);        // sec

    auto res = cli.Get(url_path.c_str());
    if (!res) throw std::runtime_error("HTTP request failed (no response)");
    if (res->status != 200) throw std::runtime_error("HTTP " + std::to_string(res->status) + ": " + res->body);
    return json::parse(res->body);
}

// 단일 심볼 현재가
inline double get_last_price(const std::string& symbol) {
    // ex) /api/v3/ticker/price?symbol=BTCUSDT
    auto j = http_get_json("/api/v3/ticker/price?symbol=" + symbol);
    // 응답 예: { "symbol": "BTCUSDT", "price": "68000.12" }
    return std::stod(j.at("price").get<std::string>());
}

// 최우선 매수/매도 호가(bid/ask) 가져오기
struct BookTicker { double bid; double ask; };

inline BookTicker get_book_ticker(const std::string& symbol) {
    // ex) /api/v3/ticker/bookTicker?symbol=BTCUSDT
    auto j = http_get_json("/api/v3/ticker/bookTicker?symbol=" + symbol);
    // 응답 예: { "symbol":"BTCUSDT","bidPrice":"67990.1","bidQty":"0.5","askPrice":"67990.2","askQty":"0.4" }
    BookTicker bt{
        std::stod(j.at("bidPrice").get<std::string>()),
        std::stod(j.at("askPrice").get<std::string>())
    };
    return bt;
}

// OHLCV 구조
struct Kline {
    long long openTime;
    double open;
    double high;
    double low;
    double close;
    double volume;
    long long closeTime;
};

// 최근 N개 klines 가져오기 (interval: "1m","5m","1h","1d" 등)
#include <vector>
inline std::vector<Kline> get_klines(const std::string& symbol,
                                     const std::string& interval,
                                     int limit = 100) {
    // ex) /api/v3/klines?symbol=BTCUSDT&interval=1m&limit=100
    auto path = "/api/v3/klines?symbol=" + symbol + "&interval=" + interval + "&limit=" + std::to_string(limit);
    auto j = http_get_json(path);

    std::vector<Kline> out;
    out.reserve(j.size());
    for (const auto& arr : j) {
        // 배열 형태 응답: [ openTime, "open","high","low","close","volume", closeTime, ... ]
        Kline k;
        k.openTime  = arr[0].get<long long>();
        k.open      = std::stod(arr[1].get<std::string>());
        k.high      = std::stod(arr[2].get<std::string>());
        k.low       = std::stod(arr[3].get<std::string>());
        k.close     = std::stod(arr[4].get<std::string>());
        k.volume    = std::stod(arr[5].get<std::string>());
        k.closeTime = arr[6].get<long long>();
        out.push_back(k);
    }
    return out;
}
