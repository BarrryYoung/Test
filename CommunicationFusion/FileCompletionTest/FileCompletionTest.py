import socket

def send_file(filename, target_ip, target_port):
    # 创建UDP套接字
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 打开要发送的文件
    with open(filename, 'rb') as file:
        data = file.read(1024)  # 每次读取1024字节数据
        while data:
            # 发送数据
            udp_socket.sendto(data, (target_ip, target_port))
            data = file.read(1024)

    # 关闭套接字
    udp_socket.close()

def receive_file(save_as_filename, listen_ip, listen_port):
    # 创建UDP套接字
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 绑定IP和端口
    udp_socket.bind((listen_ip, listen_port))

    print(f"Listening on {listen_ip}:{listen_port}")

    # 打开要保存的文件
    with open(save_as_filename, 'wb') as file:
        while True:
            # 接收数据
            data, addr = udp_socket.recvfrom(1024)
            
            # 数据为空表示传输结束
            if not data:
                break
            
            # 写入数据到文件
            file.write(data)

    print("File received successfully.")

    # 关闭套接字
    udp_socket.close()

if __name__ == "__main__":
    choice=int(input("输入数字，发送方还是接收方：\n1：接收方\n2：发送方"))
    
    if(choice==1):
        file_to_save = "recvdtestdata"
        listen_ip_address = "0.0.0.0"  # 监听所有网络接口
        listen_port = 12345  # 监听的端口号
        receive_file(file_to_save, listen_ip_address, listen_port)
    elif(choice==2):
        file_to_send = "testdata"
        target_ip_address = input("输入对方ip：")
        target_ip_address = "192.168.3.2"  # 修改为接收方的IP地址
        target_port = 12345  # 修改为接收方的端口号

        send_file(file_to_send, target_ip_address, target_port)
    else: print("invalid choice")