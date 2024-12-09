#include <vector>
#include <string>
#include <stdint.h>

//RREQの雛形
struct RREQ {
    uint8_t type; // RREQタイプ
    uint8_t flags; //J,R,G,D,Uのどれかが設定される。それぞれの意味はrfcを要参照
    uint8_t hop_count; //送信元から経由ノードのホップカウント数
    uint32_t rreq_id; //送信元ノードに関連づけられたときに特定のRREQを識別する一意のID   
    uint32_t dest_ip; //宛先ノードのIPアドレス
    uint32_t dest_seq_num; //宛先へ向かう経路について送信元が受信した最新のシーケンス番号
    uint32_t origin_ip; //送信元ノードのIPアドレス
    uint32_t origin_seq_num; //送信元ノードを指すルートエントリで使われるシーケンス番号
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
RREQ generateRREQ(uint8_t flags,uint8_t hop_count, uint32_t origin_ip, uint32_t dest_ip, uint32_t origin_seq_num, uint32_t rreq_id) {
    RREQ rreq;
    rreq.type=1; //typeの設定
    rreq.flags=flags; //フラグの設定
    rreq.hop_count= hop_count; //ホップカウントの設定
    rreq.origin_ip = origin_ip; //送信元のIPアドレスの設定
    rreq.dest_ip = dest_ip; //宛先のIPアドレスの設定
    rreq.origin_seq_num = origin_seq_num; //送信元ノードを指すルートエントリで使われるシーケンス番号の設定
    rreq.rreq_id = rreq_id; //送信元ノードに関連づけられたときに特定のRREQを識別する一意のIDの設定
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