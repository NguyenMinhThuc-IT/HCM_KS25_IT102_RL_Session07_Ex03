#include <stdio.h>
#include <math.h>

// 1. Struct chua toi uu (Lãng phí Memory Padding)
struct Device_Bad {
    char status;
    double power_measured;
    char id;
    float voltage;
    char is_overloaded;
    float current;
    int active_minutes;
};

// 2. Struct da toi uu (Flat Struct Optimization - Sap xep giam dan)
struct Device_Optimized {
    double power_measured; // 8 bytes
    float voltage;         // 4 bytes
    float current;         // 4 bytes
    int active_minutes;    // 4 bytes
    int id;                // 4 bytes
    int status;            // 4 bytes (0: Tat, 1: Bat)
};

int main() {
    // In so sanh dung luong bo nho
    printf("=== DUNG LUONG BO NHO (SIZEOF) ===\n");
    printf("Kich thuoc Struct CHUA toi uu : %lu bytes\n", sizeof(struct Device_Bad));
    printf("Kich thuoc Struct DA toi uu   : %lu bytes\n", sizeof(struct Device_Optimized));
    printf("-----------------------------------\n\n");

    struct Device_Optimized dev;

    // Gia su nhap du lieu
    dev.id = 101;
    dev.status = 1;             // 1: BAT, 0: TAT
    dev.voltage = 220.0f;       // Điện áp (V)
    dev.current = 32.0f;        // Dòng điện (A)
    dev.power_measured = 7040.0;// Công suất đo được (W)
    dev.active_minutes = 120;   // Thời gian chạy (Phút)

    // A. Kiem tra tinh hop le cua du lieu
    if (dev.voltage <= 0.0f || dev.current < 0.0f || dev.active_minutes < 0) {
        printf("[LOI DU LIEU] Thong so cam bien khong hop le (U <= 0, I < 0 hoac t < 0)!\n");
        return 1;
    }

    if (dev.status != 0 && dev.status != 1) {
        printf("[LOI DU LIEU] Trang thai thiet bi khong hop le (Chỉ nhan 0 hoac 1)!\n");
        return 1;
    }

    printf("=== HE THONG GIAM SAT AN TOAN DIEN IOT GATEWAY ===\n");
    printf("Ma thiet bi: %d | Trang thai: %s\n", dev.id, dev.status ? "BAT" : "TAT");
    printf("Dien ap: %.1fV | Dong dien: %.1fA | Cong suat do: %.1fW\n", 
            dev.voltage, dev.current, dev.power_measured);
    printf("--------------------------------------------------\n");

    // B. Kiem tra canh bao ro ri dien khi thiet bi TAT
    if (dev.status == 0 && dev.current > 0.05f) {
        printf("[CANH BAO] RO RI DIEN HOAC TAI AN! (Dong dien ro: %.2fA)\n", dev.current);
    }

    // C. Kiem tra sai lech cam bien cong suat (> 5%)
    float P_calc = dev.voltage * dev.current;
    float diff = dev.power_measured - P_calc;
    if (diff < 0) diff = -diff; // Lay gia tri tuyet doi

    if ((diff / P_calc) > 0.05f) {
        printf("[CANH BAO] Cam bien cong suat bao sai lech! (P_tinh: %.1fW vs P_do: %.1fW)\n", 
                P_calc, dev.power_measured);
    }

    // D. Kiem tra canh bao qua tai nguy hiem (I > 30A hoac P > 6600W)
    if (dev.current > 30.0f || dev.power_measured > 6600.0) {
        printf("[CANH BAO NGUY HIEM] QUA TAI NGUY HIEM! (I > 30A hoac P > 6600W)\n");
    }

    // E. Tinh toan dien nang tieu thu (kWh)
    // Ep kieu chia so thuc (60.0f) va (1000.0f) de tranh loi chia so nguyen
    double kWh = (dev.power_measured * (dev.active_minutes / 60.0)) / 1000.0;

    printf("--------------------------------------------------\n");
    printf("Thoi gian hoat dong : %d phut (%.2f gio)\n", dev.active_minutes, dev.active_minutes / 60.0f);
    printf("Dien nang tieu thu  : %.4f kWh\n", kWh);
    printf("==================================================\n");

    return 0;
}