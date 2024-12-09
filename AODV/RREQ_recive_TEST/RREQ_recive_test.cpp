#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../data_struct.h"

RREQ deserializeRREQ(const std::vector<uint8_t>& buffer) {
    RREQ rreq;

    if (buffer.size() < 19) {
        throw std::runtime_error("Buffer size too small for RREQ");
    }

    rreq.type = buffer[0];
    rreq.flags = buffer[1];
    rreq.hop_count = buffer[2];

    rreq.rreq_id = (buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
    rreq.dest_ip = (buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10];
    rreq.dest_seq_num = (buffer[11] << 24) | (buffer[12] << 16) | (buffer[13] << 8) | buffer[14];
    rreq.origin_ip = (buffer[15] << 24) | (buffer[16] << 16) | (buffer[17] << 8) | buffer[18];
    rreq.origin_seq_num = (buffer[19] << 24) | (buffer[20] << 16) | (buffer[21] << 8) | buffer[22];

    return rreq;
}

void receiveRREQ(uint16_t port) {
    // ソケットの作成
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // ソケットアドレスの設定
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // ソケットをポートにバインド
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(sock);
        return;
    }

    std::cout << "Listening for RREQ on port " << port << "..." << std::endl;

    while (true) {
        // データの受信
        std::vector<uint8_t> buffer(1024); // 十分なサイズのバッファ
        sockaddr_in senderAddr{};
        socklen_t senderAddrLen = sizeof(senderAddr);

        ssize_t recvLen = recvfrom(sock, buffer.data(), buffer.size(), 0, (sockaddr*)&senderAddr, &senderAddrLen);
        if (recvLen < 0) {
            perror("Receive failed");
            break;
        }

        buffer.resize(recvLen);

        // 受信したデータをデシリアライズ
        try {
            RREQ rreq = deserializeRREQ(buffer);

            // RREQ内容の出力
            std::cout << "Received RREQ:" << std::endl;
            std::cout << "Type: " << static_cast<int>(rreq.type) << std::endl;
            std::cout << "Flags: " << static_cast<int>(rreq.flags) << std::endl;
            std::cout << "Hop Count: " << static_cast<int>(rreq.hop_count) << std::endl;
            std::cout << "RREQ ID: " << rreq.rreq_id << std::endl;
            std::cout << "Destination IP: " << rreq.dest_ip << std::endl;
            std::cout << "Destination Seq Num: " << rreq.dest_seq_num << std::endl;
            std::cout << "Origin IP: " << rreq.origin_ip << std::endl;
            std::cout << "Origin Seq Num: " << rreq.origin_seq_num << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Failed to deserialize RREQ: " << e.what() << std::endl;
        }
    }

    // ソケットのクローズ
    close(sock);
}

int main() {
    uint16_t port = 654; // RFC 3561で指定されたポート
    receiveRREQ(port);
    return 0;
}