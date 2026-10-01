# **Tài Liệu Báo Cáo: Phân Tích Trade-Off Bộ Nhớ & Tái Cấu Trúc Struct Cho IoT Gateway**

## **1. Phân Tích Bài Toán & Đề Xuất Giải Pháp Tối Ưu Bộ Nhớ**

### **Phân Tích Input / Output**
* **Dữ liệu đầu vào (Input):**
  * `id` (int): Mã thiết bị.
  * `status` (int): Trạng thái hoạt động (0: Tắt, 1: Bật).
  * `voltage` (float): Điện áp $U$ (Volt).
  * `current` (float): Dòng điện $I$ (Ampere).
  * `power_measured` (float): Công suất đo thực tế $P$ (Watt).
  * `active_minutes` (int): Thời gian hoạt động $t$ (Phút).
* **Dữ liệu đầu ra (Output):**
  * Dung lượng bộ nhớ (`sizeof`) trước và sau khi tối ưu.
  * Các cảnh báo: Sai lệch cảm biến ($>5\%$), Quá tải nguy hiểm ($I > 30\text{A}$ hoặc $P > 6600\text{W}$), Rò rỉ điện năng ($status = 0$ và $I > 0.05\text{A}$).
  * Điện năng tiêu thụ thực tế ($kWh$).

---

### **Đề Xuất Các Giải Pháp Tái Cấu Trúc Struct**

#### **Cấu trúc ban đầu (Chưa tối ưu - Phát sinh Padding Bytes):**
```c
struct Device_Bad {
    char status;           // 1 byte  (+3 bytes padding)
    double power_measured; // 8 bytes
    char id;               // 1 byte  (+3 bytes padding)
    float voltage;         // 4 bytes
    char is_overloaded;    // 1 byte  (+3 bytes padding)
    float current;         // 4 bytes
    int active_minutes;    // 4 bytes (+4 bytes padding cuoi)
}; // Off-by-default padding => Kích thước lên tới 32 bytes!