#include <stdint.h>
#include "../data_struct.h"
#include <iostream>

int main() {
    // テストデータ
    uint8_t type = 2;                // RREPタイプ
    uint8_t reserved = 0;            // 予約フィールド
    uint8_t prefix_size = 24;        // プレフィックスサイズ
    uint32_t origin_ip = 0xC0A80101; // 192.168.1.1
    uint32_t dest_ip = 0xC0A80102;   // 192.168.1.2
    uint32_t dest_seq_num = 42;      // 宛先シーケンス番号
    uint8_t hop_count = 3;           // ホップ数
    uint64_t lifetime = 10000;       // ライフタイム（ミリ秒）

    // RREP生成
    RREP rrep = generateRREP(type, reserved, prefix_size, origin_ip, dest_ip, dest_seq_num, hop_count, lifetime);

    // 結果を出力
    std::cout << "Generated RREP:" << std::endl;
    std::cout << "Type: " << static_cast<int>(rrep.type) << std::endl;
    std::cout << "Reserved: " << static_cast<int>(rrep.reserved) << std::endl;
    std::cout << "Prefix Size: " << static_cast<int>(rrep.Prefix_Size) << std::endl;
    std::cout << "Origin IP: " << rrep.origin_ip << std::endl;
    std::cout << "Destination IP: " << rrep.dest_ip << std::endl;
    std::cout << "Destination Seq Num: " << rrep.dest_seq_num << std::endl;
    std::cout << "Hop Count: " << static_cast<int>(rrep.hop_count) << std::endl;
    std::cout << "Lifetime: " << rrep.lifetime << std::endl;

    return 0;
}