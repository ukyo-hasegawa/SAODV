#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "../data_struct.h"

std::vector<uint8_t> serializeRREQ(const RREQ& rreq) {
    std::vector<uint8_t> buffer;

    buffer.push_back(rreq.type);
    buffer.push_back(rreq.flags);
    buffer.push_back(rreq.hop_count);

    // 32ビット整数をバイト列に変換
    uint32_t fields[] = {rreq.rreq_id, rreq.dest_ip, rreq.dest_seq_num, rreq.origin_ip, rreq.origin_seq_num};
    for (uint32_t field : fields) {
        buffer.push_back((field >> 24) & 0xFF);
        buffer.push_back((field >> 16) & 0xFF);
        buffer.push_back((field >> 8) & 0xFF);
        buffer.push_back(field & 0xFF);
    }

    return buffer;
}

void broadcastRREQ(const RREQ& rreq, const std::string& broadcast_ip, uint16_t port) {
    // ソケットの作成
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // ブロードキャストオプションの有効化
    int broadcastEnable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        perror("Failed to enable broadcast");
        close(sock);
        return;
    }

    // 宛先アドレスの設定
    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port);
    inet_pton(AF_INET, broadcast_ip.c_str(), &broadcastAddr.sin_addr);

    // RREQのシリアライズ
    std::vector<uint8_t> data = serializeRREQ(rreq);

    // データの送信
    if (sendto(sock, data.data(), data.size(), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        perror("Failed to send RREQ");
    } else {
        std::cout << "RREQ broadcasted to " << broadcast_ip << ":" << port << std::endl;
    }

    // ソケットのクローズ
    close(sock);
}

int main() {
    // RREQの生成
    RREQ rreq = generateRREQ(0, 0, 0xC0A80101, 0xC0A80102, 42, 1); // 適切な値を設定

    // ブロードキャスト設定
    std::string broadcast_ip = "255.255.255.255"; // ブロードキャストアドレス
    uint16_t port = 654; // RFC 3561のデフォルトポート

    // RREQのブロードキャスト
    broadcastRREQ(rreq, broadcast_ip, port);

    return 0;
}