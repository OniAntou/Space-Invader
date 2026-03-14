# 🚀 Space Invader

Một trò chơi bắn quái vật không gian cổ điển được viết bằng **C++** và **OpenGL**.

![Gameplay](https://img.shields.io/badge/Platform-Windows-blue) ![License](https://img.shields.io/badge/License-MIT-green) ![OpenGL](https://img.shields.io/badge/OpenGL-3.0+-orange)

## 🎮 Cách chơi

| Phím | Hành động |
|------|-----------|
| `A` / `D` | Di chuyển trái / phải |
| `Space` | Bắn đạn (ở Menu: bắt đầu chơi) |
| `R` | Chơi lại (khi thua hoặc thắng) |
| `Esc` | Thoát game |

- **3 màn chơi** với độ khó tăng dần (quái vật nhanh hơn, nhiều hơn).
- Tiêu diệt hết quái vật trước khi chúng rơi xuống đáy màn hình!

---

## 🛠️ Yêu cầu hệ thống

- **Hệ điều hành:** Windows 10/11
- **MSYS2** (bao gồm MinGW-w64 UCRT64 toolchain)

## 📦 Cài đặt & Build

### Bước 1: Cài đặt MSYS2

Tải và cài đặt MSYS2 tại: [https://www.msys2.org/](https://www.msys2.org/)

### Bước 2: Cài đặt các thư viện cần thiết

Mở terminal **MSYS2 UCRT64** và chạy lệnh sau:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-glm make
```

> [!IMPORTANT]
> Phải mở đúng terminal **UCRT64** (biểu tượng màu xanh lục), **không phải** MSYS2 MSYS hay MINGW64.

### Bước 3: Thêm MSYS2 vào PATH

Đảm bảo thư mục `C:\msys64\ucrt64\bin` đã được thêm vào biến môi trường `PATH` của Windows:

1. Tìm kiếm **"Environment Variables"** trong Start Menu.
2. Chỉnh sửa biến `Path` trong **System variables**.
3. Thêm `C:\msys64\ucrt64\bin` vào danh sách.

### Bước 4: Build project

Mở terminal trong thư mục project và chạy:

```bash
make
```

### Bước 5: Chạy game

```bash
./main.exe
```

---

## 📁 Cấu trúc project

```
OpenGL_Project/
├── main.cpp              # Entry point, khởi tạo cửa sổ GLUT
├── game.h / game.cpp     # Logic game chính (trạng thái, cập nhật, render)
├── shader.h / shader.cpp # Quản lý GLSL shaders
├── model_renderer.h/cpp  # Vẽ các đối tượng (tàu, quái, đạn)
├── resourceManager.h/cpp # Quản lý tải tài nguyên (shaders)
├── model.vs / model.fs   # Vertex & Fragment shaders
├── Makefile              # Script build
├── freeglut.dll          # DLL runtime cho FreeGLUT
├── glew32.dll            # DLL runtime cho GLEW
└── glm-1.0.3/            # Thư viện toán GLM (header-only)
```

## 📚 Thư viện sử dụng

| Thư viện | Phiên bản | Mục đích |
|----------|-----------|----------|
| [FreeGLUT](https://freeglut.sourceforge.net/) | 3.x | Quản lý cửa sổ, sự kiện bàn phím |
| [GLEW](https://glew.sourceforge.net/) | 2.x | Nạp các hàm mở rộng OpenGL |
| [GLM](https://github.com/g-truc/glm) | 1.0.3 | Toán học đồ họa (vector, matrix) |
| **OpenGL** | 3.0+ | API đồ họa |

## 📝 License

Dự án này sử dụng giấy phép [MIT](LICENSE).
