import socket

def start_client():
    HOST = '127.0.0.1'  
    PORT = 21123       

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))

    message = "Hej, serwer! To jest klient."
    client_socket.sendall(message.encode('utf-8'))

    data = client_socket.recv(1024).decode('utf-8')
    print("Odpowiedz serwera:", data)

    client_socket.close()

if __name__ == "__main__":
    start_client()