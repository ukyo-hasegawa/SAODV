#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../data_struct.h"

/*RREQを送信するソースコード*/

// RREPメッセージをシリアライズする関数
std::vector<uint8_t> serializeRREP(const RREP& rrep) {
    std::vector<uint8_t> buffer;

    buffer.push_back(rrep.type);
    buffer.push_back(rrep.flags);
    buffer.push_back(rrep.hop_count);

    uint32_t fields[] = {rrep.dest_ip, rrep.dest_seq_num, rrep.origin_ip, rrep.lifetime};
    for (uint32_t field : fields) {
        buffer.push_back((field >> 24) & 0xFF);
        buffer.push_back((field >> 16) & 0xFF);
        buffer.push_back((field >> 8) & 0xFF);
        buffer.push_back(field & 0xFF);
    }

    return buffer;
}

// RREPメッセージをユニキャストで送信する関数
void sendRREP(const RREP& rrep, const std::string& destination_ip, uint16_t port) {
    // ソケットの作成
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // 宛先アドレスの設定
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, destination_ip.c_str(), &destAddr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return;
    }

    // RREPのシリアライズ
    std::vector<uint8_t> data = serializeRREP(rrep);

    // データの送信
    if (sendto(sock, data.data(), data.size(), 0, (sockaddr*)&destAddr, sizeof(destAddr)) < 0) {
        perror("Failed to send RREP");
    } else {
        std::cout << "RREP sent to " << destination_ip << ":" << port << std::endl;
    }

    // ソケットのクローズ
    close(sock);
}

int main() {
    // RREPの生成（適切な値を設定）
    RREP rrep = generateRREP(2, 0, 0, 0xC0A80102, 0xC0A80101, 100, 1, 5000);

    // 宛先IPアドレスとポート（ユニキャスト先のIPアドレスを指定）
    std::string destination_ip = "10.0.0.1";
    uint16_t port = 2000;

    // RREPの送信
    sendRREP(rrep, destination_ip, port);

    return 0;
}