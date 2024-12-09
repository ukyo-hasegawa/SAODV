#include <iostream>
#include <unordered_map>
#include <vector>
#include "../data_struct.h"
#include <random>

int main() {
    uint32_t origin_ip = 0xC0A80101; // 192.168.1.1
    uint32_t dest_ip = 0xC0A80102;   // 192.168.1.2
    uint32_t rreq_id = 100;
    uint32_t origin_seq_num = 42;
    uint8_t hop_count = 0;
    uint8_t flags = 10;

    RREQ rreq = generateRREQ(flags,hop_count,origin_ip,dest_ip,origin_seq_num,rreq_id);

    // 結果を出力
    std::cout << "Generated RREQ:" << std::endl;
    std::cout << "Type: " << static_cast<int>(rreq.type) << std::endl;
    std::cout << "Flags: " << static_cast<int>(rreq.flags) << std::endl;
    std::cout << "Hop Count: " << static_cast<int>(rreq.hop_count) << std::endl;
    std::cout << "RREQ ID: " << rreq.rreq_id << std::endl;
    std::cout << "Destination IP: " << rreq.dest_ip << std::endl;
    std::cout << "Destination Seq Num: " << rreq.dest_seq_num << std::endl;
    std::cout << "Originator IP: " << rreq.origin_ip << std::endl;
    std::cout << "Originator Seq Num: " << rreq.origin_seq_num << std::endl;

    return 0;
}