#ifndef __SDLPINGPROBE_H__
#define __SDLPINGPROBE_H__

#include <SDL_net.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>

class SDLPingProbe {
private:
    std::thread* m_Thread = nullptr;
    std::atomic<bool> m_bRunning = { false };

    UDPsocket m_Socket = nullptr;
    UDPpacket* m_Packet = nullptr;
    IPaddress m_ServerIP{};

    // 【修改點 1】每個物件自己有一個原子變數儲存 Ping 值
    std::atomic<int> m_CurrentPing = { 0 };

public:
    SDLPingProbe() = default;
    ~SDLPingProbe() { Stop(); }

    // 【修改點 2】提供一個 Get 方法讓外部讀取
    int GetPing() const {
        return m_CurrentPing.load();
    }

    bool Start(const std::string& ip, Uint16 port) {
        if (SDLNet_ResolveHost(&m_ServerIP, ip.c_str(), port) == -1) return false;

        // 傳入 0 讓系統自動分配一個隨機的閒置 UDP Port
        // 這樣兩個線程就會拿到不同的 Port，互不衝突
        m_Socket = SDLNet_UDP_Open(0);
        if (!m_Socket) return false;

        m_Packet = SDLNet_AllocPacket(64);
        if (!m_Packet) return false;

        m_bRunning = true;
        m_Thread = new std::thread(&SDLPingProbe::Loop, this);
        return true;
    }

    void Stop() {
        m_bRunning = false;
        if (m_Thread && m_Thread->joinable()) {
            m_Thread->join();
            delete m_Thread;
            m_Thread = nullptr;
        }
        if (m_Packet) { SDLNet_FreePacket(m_Packet); m_Packet = nullptr; }
        if (m_Socket) { SDLNet_UDP_Close(m_Socket); m_Socket = nullptr; }
    }

private:
    void Loop() {
        while (m_bRunning) {
            // --- 發送 ---
            m_Packet->address = m_ServerIP;
            auto start = std::chrono::high_resolution_clock::now();
            long long sendTime = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
            memcpy(m_Packet->data, &sendTime, sizeof(sendTime));
            m_Packet->len = sizeof(sendTime);
            SDLNet_UDP_Send(m_Socket, -1, m_Packet);

            // --- 接收 (帶 Timeout) ---
            bool received = false;
            int elapsedMs = 0;
            while (elapsedMs < 1000 && m_bRunning) {
                if (SDLNet_UDP_Recv(m_Socket, m_Packet)) {
                    received = true;
                    break;
                }
                SDL_Delay(10);
                elapsedMs += 10;
            }

            // --- 計算並更新自己的 Ping 值 ---
            if (received) {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

                // 【修改點 3】更新成員變數
                m_CurrentPing.store((int)duration);
            }
            else {
                // 超時可以設為 -1 或 999
                m_CurrentPing.store(-1);
            }

            // 休閒時間 (2秒)
            for (int i = 0; i < 500 && m_bRunning; ++i) SDL_Delay(10);
        }
    }
};
#endif