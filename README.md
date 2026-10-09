<div align="center">

# 🗂️ MYLS

### UNIX File Listing Utility · System Programming Midterm

**Trình mô phỏng lệnh `ls` bằng C trên NetBSD**

[![Language](https://img.shields.io/badge/Language-C11-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Platform](https://img.shields.io/badge/Platform-NetBSD-EAB92D?style=for-the-badge&logo=netbsd&logoColor=black)](https://www.netbsd.org/)
[![Standard](https://img.shields.io/badge/API-POSIX-38618C?style=for-the-badge)](https://pubs.opengroup.org/onlinepubs/9799919799/)
[![Build](https://img.shields.io/badge/Build-BSD%20Make-238636?style=for-the-badge)](https://man.netbsd.org/make.1)

![Editor](https://img.shields.io/badge/Editor-VS%20Code-007ACC?style=flat-square&logo=visualstudiocode&logoColor=white)
![Virtualization](https://img.shields.io/badge/VM-VirtualBox-183A61?style=flat-square&logo=virtualbox&logoColor=white)
![File transfer](https://img.shields.io/badge/Transfer-WinSCP-1874CD?style=flat-square)
![Options](https://img.shields.io/badge/Supported%20options-19-orange?style=flat-square)

**[📚 Tài liệu](#-tổng-quan) · [⚙️ Biên dịch](#️-biên-dịch-trên-netbsd) · [📥 Cài lệnh](#-cài-đặt-tùy-chọn-chạy-myls-không-cần-) · [🚀 Sử dụng](#-hướng-dẫn-sử-dụng) · [🧪 Kiểm thử](#-kiểm-thử)**

<sub>Dự án giữa kỳ · Lập trình hệ thống UNIX · Mã sinh viên: 24IT015</sub>

</div>

---

## 🧭 Mục lục

| | | |
|---|---|---|
| [📖 Tổng quan](#-tổng-quan) | [✨ Tính năng](#-tính-năng) | [🧰 Công nghệ](#-công-nghệ--môi-trường) |
| [📁 Cấu trúc](#-cấu-trúc-dự-án) | [🔌 Kết nối NetBSD](#-kết-nối-windows--netbsd) | [⚙️ Biên dịch](#️-biên-dịch-trên-netbsd) |
| [🚀 Cách sử dụng](#-hướng-dẫn-sử-dụng) | [🧪 Kiểm thử](#-kiểm-thử) | [🔄 Quy trình](#-quy-trình-phát-triển) |
| [📥 Cài lệnh `myls`](#-cài-đặt-tùy-chọn-chạy-myls-không-cần-) | [🛠️ Xử lý sự cố](#️-xử-lý-sự-cố) | [🔗 Kho mã nguồn](#-kho-mã-nguồn) |

---

## 📖 Tổng quan

**MYLS** là chương trình dòng lệnh mô phỏng một phần chức năng của `ls` trên UNIX, được phát triển **từ đầu bằng ngôn ngữ C**. Dự án tập trung vào việc thao tác trực tiếp với hệ thống tập tin thông qua các API như `opendir()`, `readdir()`, `lstat()`, `stat()` và `readlink()`; **không gọi lệnh `ls` có sẵn** để tạo kết quả.

Mã nguồn được chia thành nhiều mô-đun, có Makefile để biên dịch, bộ kiểm thử tự động và báo cáo dự án. Phạm vi chức năng dựa trên **tài liệu `ls(1)` NetBSD 10.1 được giảng viên cung cấp**; dự án không nhằm triển khai toàn bộ GNU `ls`.

**Cú pháp:**

```sh
./myls [-AacdFfhiklnqRrSstuw] [file ...]
```

- Không truyền đường dẫn: liệt kê thư mục hiện tại.
- Truyền một hoặc nhiều tập tin: in các tập tin tương ứng.
- Truyền thư mục: liệt kê nội dung thư mục, trừ khi dùng `-d`.
- Mặc định: hiển thị một mục trên mỗi dòng.
- Trả mã thoát `0` nếu thành công, khác `0` nếu có lỗi.

> **Trạng thái kiểm thử:** Mã đã được biên dịch và kiểm thử cơ bản trên Linux trong quá trình tạo dự án. Người sử dụng cần chạy `make` và `make test` trên NetBSD thực tế, đồng thời đối chiếu với `ls` gốc trước khi nộp bài. `make` thành công chỉ xác nhận việc biên dịch, không đảm bảo mọi hành vi đều chính xác.

## ✨ Tính năng

Mã nguồn bao gồm xử lý cho **19 tùy chọn** trong tài liệu:

| Tùy chọn | Ý nghĩa |
|:--:|---|
| `-a` | Hiển thị tất cả mục, kể cả `.` và `..`. |
| `-A` | Hiển thị mục ẩn nhưng loại trừ `.` và `..`. |
| `-c` | Dùng thời điểm thay đổi trạng thái tập tin trong chế độ thời gian. |
| `-d` | Hiển thị chính thư mục, không liệt kê nội dung. |
| `-F` | Thêm ký hiệu phân loại (`/`, `*`, `@`, `=`, `\|` …). |
| `-f` | Không sắp xếp danh sách. |
| `-h` | Định dạng kích thước/số block dễ đọc khi đi với `-l` hoặc `-s`. |
| `-i` | Hiển thị số inode. |
| `-k` | Dùng đơn vị kilobyte cho thông tin block khi đi với `-s`. |
| `-l` | Hiển thị danh sách chi tiết. |
| `-n` | Tương tự `-l`, hiển thị UID/GID dưới dạng số. |
| `-q` | Thay byte không in được bằng `?`. |
| `-R` | Duyệt đệ quy các thư mục con. |
| `-r` | Đảo ngược thứ tự sắp xếp. |
| `-S` | Sắp xếp theo kích thước giảm dần. |
| `-s` | Hiển thị số block đã sử dụng. |
| `-t` | Sắp xếp theo thời gian, mới nhất trước. |
| `-u` | Dùng thời điểm truy cập trong chế độ thời gian. |
| `-w` | In nguyên dạng tên tập tin, kể cả ký tự không in được. |

Các tùy chọn có thể kết hợp, chẳng hạn `-la`, `-ltr`, `-Rla`. Chương trình có xử lý những nhóm tùy chọn mà lựa chọn phía sau ghi đè lựa chọn phía trước, như `-d/-R`, `-l/-n`, `-c/-u` và `-q/-w`.

## 🧰 Công nghệ & môi trường

| Thành phần | Vai trò |
|---|---|
| **Visual Studio Code (Windows)** | Viết, đọc và chỉnh sửa mã nguồn. |
| **VirtualBox + NetBSD** | Môi trường UNIX để biên dịch và chạy chương trình. |
| **WinSCP (SFTP)** | Chuyển mã nguồn từ Windows sang NetBSD. |
| **`cc`** | Trình biên dịch C trên NetBSD (GCC hoặc Clang, tùy máy). |
| **BSD `make`** | Tự động biên dịch và thực hiện kiểm thử. |
| **POSIX / C11** | API và chuẩn ngôn ngữ dùng trong dự án. |

### ✅ Yêu cầu trước khi bắt đầu

1. NetBSD đã được cài và khởi động thành công trong VirtualBox.
2. Có tài khoản người dùng thông thường trên NetBSD và biết mật khẩu.
3. NetBSD có trình biên dịch `cc` và tiện ích `make` (bộ công cụ phát triển).
4. Có WinSCP trên Windows; máy ảo đã bật dịch vụ SSH/SFTP và có kết nối mạng phù hợp.
5. Có VS Code để mở dự án. VS Code **không bắt buộc** phải cài trên NetBSD.

Kiểm tra nhanh trong Terminal NetBSD:

```sh
uname -a
command -v cc
command -v make
```

Nếu `cc` không được tìm thấy, cần cài bộ công cụ phát triển tương ứng với bản NetBSD đang dùng (ví dụ bộ **comp**). Nếu `make` không có, kiểm tra lại bản cài đặt NetBSD.

## 📁 Cấu trúc dự án

```text
unix-ls/
├── include/
│   └── ls.h             # Khai báo cấu trúc dữ liệu và API chung
├── src/
│   ├── main.c           # Điểm vào chương trình, mã thoát
│   ├── options.c        # Phân tích tùy chọn dòng lệnh
│   ├── entries.c        # Danh sách động, xử lý đường dẫn
│   ├── sort.c           # Các chế độ sắp xếp
│   ├── directory.c      # Đọc thư mục, xử lý đối số, đệ quy
│   └── display.c        # Định dạng và in thông tin tập tin
├── tests/
│   └── test.sh          # Bộ kiểm thử tự động
├── Makefile             # Biên dịch / kiểm thử / dọn dẹp
├── .gitignore           # Loại trừ binary và object files
├── .gitattributes       # Giữ định dạng LF khi dùng Git
└── README.md            # Hướng dẫn sử dụng và báo cáo
```

Khi chạy `make`, thư mục dự án sẽ có thêm chương trình thực thi `myls` và các file `.o`. Đây là sản phẩm biên dịch, không phải mã nguồn cần sửa trực tiếp.

## 🔌 Kết nối Windows ↔ NetBSD

Có thể bỏ qua mục này nếu **WinSCP của bạn đã kết nối thành công** tới máy ảo NetBSD.

### 5.1 · 🌐 Cấu hình NAT trong VirtualBox

Tắt máy ảo trước khi điều chỉnh cấu hình. Trong VirtualBox, chọn máy ảo NetBSD → **Settings → Network → Adapter 1**:

1. Bật **Enable Network Adapter**.
2. Đặt **Attached to: NAT**.
3. Mở **Advanced → Port Forwarding**.
4. Thêm quy tắc:

| Trường | Giá trị ví dụ |
|---|---|
| Name | `SSH` |
| Protocol | `TCP` |
| Host IP | `127.0.0.1` |
| Host Port | `2222` |
| Guest IP | Để trống |
| Guest Port | `22` |

Cổng `2222` chỉ là ví dụ, có thể thay nếu cổng đã bị sử dụng. Nếu bạn đang dùng **Bridged Adapter** và kết nối trực tiếp qua IP của NetBSD, hãy dùng địa chỉ IP và cổng SSH thực tế thay cho `127.0.0.1:2222`.

### 5.2 · 🔐 Bật SSH trên NetBSD

Đăng nhập NetBSD và chuyển quyền quản trị:

```sh
su -
```

Mở `/etc/rc.conf`:

```sh
vi /etc/rc.conf
```

Đảm bảo có cấu hình:

```sh
sshd=YES
```

Lưu file và khởi động dịch vụ (nếu chưa chạy):

```sh
/etc/rc.d/sshd start
```

Nếu dịch vụ đã chạy, bạn không cần khởi động lại. Thoát tài khoản root:

```sh
exit
```

**An toàn:** Sử dụng tài khoản người dùng bình thường để truyền file và thực hiện dự án; không cần bật đăng nhập SSH bằng root.

### 5.3 · 📤 Kết nối bằng WinSCP

Mở WinSCP → **New Site** và điền:

| Trường | Ví dụ với NAT Port Forwarding |
|---|---|
| File protocol | `SFTP` |
| Host name | `127.0.0.1` |
| Port number | `2222` |
| User name | `<ten-tai-khoan-NetBSD>` |
| Password | Mật khẩu của tài khoản đó |

Nhấn **Login**. Lần đầu kết nối, kiểm tra fingerprint máy chủ trước khi chấp nhận. Sau khi đăng nhập, giao diện WinSCP sẽ hiển thị file Windows ở một bên và file NetBSD ở bên còn lại.

### 5.4 · 📦 Chuyển dự án vào máy ảo

1. Giải nén file ZIP của dự án trên Windows.
2. Trong VS Code, mở thư mục `unix-ls` để xem hoặc chỉnh sửa code.
3. Mở WinSCP, duyệt tới thư mục home của tài khoản NetBSD (`~`).
4. Kéo **cả thư mục `unix-ls`** từ Windows sang NetBSD.
5. Kiểm tra NetBSD có file `~/unix-ls/Makefile` và `~/unix-ls/src/main.c`.

> **Tránh lỗi thư mục lồng nhau:** Đường dẫn đúng phải là `~/unix-ls/Makefile`, không phải `~/unix-ls/unix-ls/Makefile`. Không nên chuyển riêng từng file `.c` mà bỏ quên `include/`, `Makefile` hay `tests/`.

## ⚙️ Biên dịch trên NetBSD

Tại Terminal NetBSD, đăng nhập bằng tài khoản đã nhận source và chạy:

```sh
cd ~/unix-ls
make
```

Nếu thành công, chương trình thực thi tên `myls` được tạo ở thư mục gốc dự án. Kiểm tra:

```sh
ls -l ./myls
```

Chạy thử:

```sh
./myls
```

**Vì sao dùng `./myls` thay vì `myls`?** Dấu `./` yêu cầu shell chạy file thực thi trong thư mục hiện tại; thư mục hiện tại thường không nằm trong biến môi trường `PATH`.

Các lệnh Makefile:

| Lệnh | Tác dụng |
|---|---|
| `make` | Biên dịch chương trình `myls`. |
| `make test` | Chạy bộ kiểm thử tự động trong `tests/test.sh`. |
| `make clean` | Xóa `myls` và các file `.o` đã biên dịch. |
| `make clean && make` | Biên dịch sạch lại từ đầu. |
| `make install` | Biên dịch nếu cần và cài `myls` vào `/usr/local/bin` (cần quyền ghi). |
| `make uninstall` | Gỡ bản `myls` đã cài khỏi `/usr/local/bin` (cần quyền ghi). |

Không cần sử dụng `gcc` riêng lẻ vì Makefile đã quản lý việc biên dịch tất cả các module.

## 📥 Cài đặt tùy chọn: chạy `myls` không cần `./`

> **Không bắt buộc.** Để làm và kiểm thử bài giữa kỳ, chỉ cần `make` rồi chạy `./myls`. Phần này dành cho người muốn sử dụng `myls` như một lệnh thông thường từ mọi thư mục. Chỉ thực hiện nếu Makefile của phiên bản dự án đã có hai mục tiêu `install` và `uninstall`.

### 📌 Cài vào `/usr/local/bin`

Sau khi tải mã nguồn về NetBSD, đăng nhập bằng tài khoản thường (ví dụ `bach`) rồi chạy:

```sh
cd ~/unix-ls
make test
su
make install
exit
```

- Lệnh `su` yêu cầu **mật khẩu root** nhưng giữ nguyên thư mục hiện tại khi chuyển quyền trong cách dùng thông thường; xác nhận đang ở thư mục dự án bằng `pwd` nếu cần.
- `make install` tự gọi bước biên dịch nếu chương trình chưa được tạo hoặc mã nguồn mới hơn; không nhất thiết phải chạy `make` riêng.
- Lệnh này sao chép chương trình sang `/usr/local/bin/myls`, **không thay thế** `/bin/ls` của NetBSD.
- Cần quyền root vì tài khoản thường có thể gặp lỗi `install: /usr/local: mkdir: Permission denied`.

Kiểm tra kết quả:

```sh
ls -l /usr/local/bin/myls
/usr/local/bin/myls -la
command -v myls
```

Nếu `command -v myls` trả về `/usr/local/bin/myls` thì có thể chạy trực tiếp:

```sh
myls
myls -la
myls -R /tmp
```

### 🧭 Nếu đã cài nhưng `myls: command not found`

Nguyên nhân thường là `/usr/local/bin` chưa có trong `PATH`. Kiểm tra:

```sh
echo "$PATH"
```

Với **Bash** (ví dụ dấu nhắc `bach@...$`), thêm tạm thời cho phiên hiện tại:

```sh
export PATH="/usr/local/bin:$PATH"
```

Muốn áp dụng lâu dài cho Bash của tài khoản hiện tại:

```sh
printf '%s\n' 'export PATH="/usr/local/bin:$PATH"' >> ~/.bashrc
. ~/.bashrc
```

Với **csh/tcsh**, dùng cú pháp khác:

```csh
set path = ( /usr/local/bin $path )
rehash
```

Để thiết lập lâu dài cho csh/tcsh, chỉnh file `~/.cshrc` tương ứng. **Không thêm dấu `./` vào PATH và không đổi tên bản `myls` thành `ls`.**

### ♻️ Cập nhật hoặc gỡ cài đặt

Sau khi sửa code, biên dịch và kiểm thử lại; sau đó cài lại phiên bản mới:

```sh
cd ~/unix-ls
make
make test
su
make install
exit
```

Nếu không muốn dùng lệnh `myls` đã cài nữa:

```sh
cd ~/unix-ls
su
make uninstall
exit
```

Lệnh gỡ cài đặt chỉ xóa `/usr/local/bin/myls` theo Makefile, **không xóa source code** và không ảnh hưởng lệnh `ls` gốc.

---

## 🚀 Hướng dẫn sử dụng

Các ví dụ dưới đây dùng `./myls` để có thể chạy ngay sau khi `make` trong thư mục `~/unix-ls`. Nếu đã thực hiện phần **cài đặt tùy chọn** và `PATH` được cấu hình đúng, bạn có thể thay `./myls` bằng `myls` ở tất cả ví dụ.

### 7.1 · Liệt kê thư mục hiện tại

```sh
./myls
```

### 7.2 · Liệt kê một đường dẫn cụ thể

```sh
./myls /etc
./myls /tmp
./myls src/main.c
```

### 7.3 · Hiển thị tập tin ẩn

```sh
./myls -a .     # Bao gồm . và ..
./myls -A .     # Bỏ qua . và ..
```

### 7.4 · Hiển thị thông tin chi tiết

```sh
./myls -l .
./myls -la .
./myls -n .     # UID/GID dạng số
./myls -lh .    # Kích thước dễ đọc
```

Chế độ `-l` hiển thị các trường như loại và quyền tập tin, số liên kết, chủ sở hữu, nhóm, kích thước, thời gian và tên; với symbolic link có thể kèm `->` và đích liên kết.

### 7.5 · Sắp xếp và duyệt đệ quy

```sh
./myls -S .       # Kích thước giảm dần
./myls -t .       # Thời gian mới nhất trước
./myls -tr .      # Thời gian cũ nhất trước
./myls -r .       # Đảo thứ tự
./myls -R .       # Bao gồm các thư mục con
./myls -Rla .     # Đệ quy, long format, hiện file ẩn
```

Cẩn thận với `-R` trên thư mục rất lớn, vì lượng kết quả có thể nhiều.

### 7.6 · In inode, block và phân loại tập tin

```sh
./myls -i .
./myls -s .
./myls -sk .
./myls -sh .
./myls -F .
./myls -lF /etc
```

### 7.7 · Nhiều đường dẫn và tên bắt đầu bằng dấu `-`

```sh
./myls /etc /tmp
./myls -- -ten-file
```

`--` đánh dấu kết thúc danh sách tùy chọn: các đối số phía sau được hiểu là đường dẫn.

## 🧪 Kiểm thử

### 8.1 · Kiểm thử tự động

```sh
cd ~/unix-ls
make test
```

Script `tests/test.sh` tạo thư mục thử nghiệm tạm, kiểm tra các tình huống cơ bản (tập tin ẩn, symbolic link, quyền thực thi, một số cách sắp xếp, long format, đệ quy, tùy chọn sai, đường dẫn không tồn tại...) rồi dọn dữ liệu tạm. Khi vượt qua tất cả phép kiểm tra, script in:

```text
PASS: myls smoke tests
```

Đây là **smoke test**, không phải bằng chứng rằng mọi tổ hợp của 19 tùy chọn đều đã được kiểm thử đầy đủ. Nếu test thất bại, xem dòng lỗi trong Terminal và kiểm tra lại code, môi trường, khác biệt giữa BSD và GNU utilities.

### 8.2 · So sánh với `ls` của NetBSD

Chương trình này mặc định in **mỗi tập tin một dòng** theo tài liệu được giao. Khi so sánh bản mặc định, lệnh `ls` gốc có thể hiển thị nhiều cột tùy môi trường; để tránh so sánh sai do định dạng terminal, hãy dùng cùng một kiểu đầu ra có thể đối chiếu, chẳng hạn khi ghi ra file:

```sh
ls -a /tmp > /tmp/ls-original.txt
./myls -a /tmp > /tmp/ls-myls.txt
diff -u /tmp/ls-original.txt /tmp/ls-myls.txt
```

Nếu `diff` không in gì, **hai file đầu ra giống nhau** trong trường hợp đang kiểm tra. Lặp lại phép so sánh với thư mục thử nghiệm nhỏ và các tùy chọn khác. Với `-l`, `-s`, `-h`, cần xem cả quy tắc định dạng và đơn vị block, không chỉ so sánh chuỗi máy móc.

### 8.3 · Kiểm thử các trường hợp lỗi

```sh
./myls /duong-dan-khong-ton-tai
./myls -z
./myls /etc /duong-dan-khong-ton-tai
```

Kiểm tra chương trình thông báo lỗi, tiếp tục xử lý các đường dẫn hợp lệ khi có thể, và trả mã thoát khác `0` nếu có lỗi:

```sh
./myls /duong-dan-khong-ton-tai
echo $?
```

Ngoài ra nên kiểm thử thư mục rỗng, file tên có khoảng trắng, liên kết tượng trưng bị hỏng và thư mục không có quyền truy cập.

## 🔄 Quy trình phát triển

Mỗi lần thay đổi mã nguồn:

1. **Windows / VS Code:** sửa các file `.c`, `.h` và nhấn **Ctrl + S**.
2. **WinSCP:** tải lên NetBSD **những file đã thay đổi**; chọn ghi đè file cũ khi cần.
3. **NetBSD:** chạy `cd ~/unix-ls && make`.
4. **NetBSD:** chạy `make test` và kiểm tra thêm lệnh `./myls` liên quan tới phần vừa sửa.
5. **Nếu có lỗi:** đọc thông báo trình biên dịch hoặc kết quả test, sửa trên VS Code rồi đồng bộ lại.

**Không chỉnh sửa file `.o` hoặc chương trình `myls` bằng tay.** Các file này được tạo tự động từ mã nguồn.

## 🛠️ Xử lý sự cố

| Hiện tượng | Nguyên nhân có thể | Hướng xử lý |
|---|---|---|
| WinSCP báo `Connection refused` | SSH chưa chạy hoặc NAT forward chưa đúng | Kiểm tra `sshd`, địa chỉ IP và cổng SSH. |
| WinSCP báo `Authentication failed` | Sai username/mật khẩu | Dùng tài khoản NetBSD bình thường, kiểm tra thông tin đăng nhập. |
| `make: not found` | Thiếu công cụ phát triển hoặc PATH | Kiểm tra `command -v make` và bộ cài NetBSD. |
| `cc: not found` | Thiếu trình biên dịch | Kiểm tra `command -v cc`, cài bộ công cụ phát triển phù hợp. |
| `don't know how to make ...` | Sai thư mục hoặc mục tiêu Makefile | Chạy `pwd`, `ls -l Makefile`, kiểm tra tên target. |
| `./myls: not found` | Chưa biên dịch hoặc đứng sai thư mục | Vào `~/unix-ls`, chạy `make`, kiểm tra `ls -l myls`. |
| `Permission denied` khi liệt kê | Không có quyền đọc đường dẫn hoặc chạy file | Kiểm tra quyền tập tin bằng `ls -l`; không tự ý dùng root để đọc dữ liệu. |
| `make install` báo `mkdir: Permission denied` | Không có quyền ghi vào `/usr/local/bin` | Chuyển sang root bằng `su`, rồi chạy `make install` trong thư mục dự án. |
| `myls: command not found` sau khi cài | `/usr/local/bin` không có trong `PATH` | Kiểm tra `command -v myls`; thêm `/usr/local/bin` vào `PATH` như hướng dẫn ở trên. |
| `make test` thất bại | Lỗi hành vi hoặc khác biệt công cụ môi trường | Xem lỗi đầu tiên, đối chiếu bằng test đơn lẻ và `ls` gốc. |
| Sửa code mà kết quả không đổi | Chưa tải file mới lên máy ảo | Kiểm tra WinSCP đã ghi đè đúng file trong `~/unix-ls/src/`. |

---

## 🔗 Kho mã nguồn

**GitHub:** [LeSyBach/LESYBACH_24IT015_midterm](https://github.com/LeSyBach/LESYBACH_24IT015_midterm)

<div align="center">

---

**🗂️ MYLS · UNIX System Programming Midterm**

<sub>Written in C · Built with BSD Make · Runs on NetBSD</sub>

</div>
