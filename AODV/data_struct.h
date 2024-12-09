#include <vector>
#include <string>
#include <stdint.h>

//RREQの雛形
struct RREQ {
    uint8_t type = 1; // RREQタイプ
    uint8_t flags = 0;
    uint8_t hop_count = 0;
    uint32_t rreq_id;
    uint32_t dest_ip;
    uint32_t dest_seq_num;
    uint32_t origin_ip;
    uint32_t origin_seq_num;
};
//RREPの雛形
struct RREP {
    uint8_t type = 2; // RREPタイプ
    uint8_t flags = 0;
    uint8_t hop_count = 0;
    uint32_t dest_ip;
    uint32_t dest_seq_num;
    uint32_t origin_ip;
    uint32_t lifetime;
};
//RERRの雛形
struct RERR {
    uint8_t type = 3; // RERRタイプ
    uint8_t flags = 0;
    uint8_t dest_count;
    std::vector<uint32_t> unreachable_dest_ips;
    std::vector<uint32_t> unreachable_dest_seq_nums;
};
//RREQの生成
RREQ generateRREQ(uint32_t origin_ip, uint32_t dest_ip, uint32_t origin_seq_num, uint32_t rreq_id) {
    RREQ rreq;
    rreq.origin_ip = origin_ip;
    rreq.dest_ip = dest_ip;
    rreq.origin_seq_num = origin_seq_num;
    rreq.rreq_id = rreq_id;
    return rreq;
}
//RREPの生成
RREP generateRREP(uint32_t origin_ip, uint32_t dest_ip, uint32_t dest_seq_num, uint8_t hop_count, uint64_t lifetime) {
    RREP rrep;
    rrep.origin_ip = origin_ip;
    rrep.dest_ip = dest_ip;
    rrep.dest_seq_num = dest_seq_num;
    rrep.hop_count = hop_count;
    rrep.lifetime = lifetime;
    return rrep;
}