#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../data_struct.h"

/*RREPを受信するソースコード*/

RREP deserializeRREP(const std::vector<uint8_t>& buffer) {
    RREP rrep;

    if (buffer.size() < 19) {
        throw std::runtime_error("Buffer size too small for RREP");
    }

    rrep.type = buffer[0];
    rrep.flags = buffer[1];
    rrep.hop_count = buffer[2];

    //rrep.rrep_id = (buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
    rrep.dest_ip = (buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
    rrep.dest_seq_num = (buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10];
    rrep.origin_ip = (buffer[11] << 24) | (buffer[12] << 16) | (buffer[13] << 8) | buffer[14];
    //rrep.dest_seq_num = (buffer[19] << 24) | (buffer[20] << 16) | (buffer[21] << 8) | buffer[22];

    return rrep;
}

void receiveRREP(uint16_t port) {
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
    } else {
        std::cout << "Bind Success" << std::endl;
    }

    std::cout << "Listening for RREP on port " << port << "..." << std::endl;

    while (true) {
        // データの受信
        std::vector<uint8_t> buffer(1024); // 十分なサイズのバッファ
        sockaddr_in senderAddr{};
        socklen_t senderAddrLen = sizeof(senderAddr);

        ssize_t recvLen = recvfrom(sock, buffer.data(), buffer.size(), 0, (sockaddr*)&senderAddr, &senderAddrLen);
        if (recvLen < 0) {
            perror("Receive failed");
            break;
        } else {
            std::cout << "receive success" << std::endl;
        }

        buffer.resize(recvLen);

        // 受信したデータをデシリアライズ
        try {
            RREP rrep = deserializeRREP(buffer);

            // RREP内容の出力
            std::cout << "Received RREP:" << std::endl;
            std::cout << "Type: " << static_cast<int>(rrep.type) << std::endl;
            std::cout << "Flags: " << static_cast<int>(rrep.flags) << std::endl;
            std::cout << "Hop Count: " << static_cast<int>(rrep.hop_count) << std::endl;
            //std::cout << "RREP ID: " << rrep.rrep_id << std::endl;
            std::cout << "Destination IP: " << rrep.dest_ip << std::endl;
            std::cout << "Destination Seq Num: " << rrep.dest_seq_num << std::endl;
            std::cout << "Origin IP: " << rrep.origin_ip << std::endl;
            std::cout << "dest Seq Num: " << rrep.dest_seq_num << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Failed to deserialize RREP: " << e.what() << std::endl;
        }
    }

    // ソケットのクローズ
    close(sock);
}

int main() {
    uint16_t port = 2000; // RFC 3561で指定されたポート
    receiveRREP(port);
    return 0;
}