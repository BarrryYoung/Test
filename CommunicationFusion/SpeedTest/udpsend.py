import socket

# 发送端配置
server_ip = '127.0.0.1'  # 接收端IP地址
server_port = 12345      # 接收端端口号
total_data_size = 10240  # 10KB的数据
packet_size = 42       # 每个数据包的大小（小于典型的MTU）

# 创建UDP套接字
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 创建10KB的数据
data = b'x' * total_data_size

# 将数据分割成多个包并发送
for i in range(0, total_data_size, packet_size):
    end = min(i + packet_size, total_data_size)
    client_socket.sendto(data[i:end], (server_ip, server_port))

# 关闭套接字
client_socket.close()

