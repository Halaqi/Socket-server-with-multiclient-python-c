import socket
import threading

PORT = 6000
HOST = "0.0.0.0"
BUFFER_SIZE = 1024

# Medicine inventory
inventory = {
    "aspirin": 10,
    "paracetamol": 5,
    "ibuprofen": 0,
}

# Lock for thread safety
inventory_lock = threading.Lock()


def process_request(client_socket, address):
    print(f"Client connected: {address}")
    try:
        while True:
            # Receive client request
            data = client_socket.recv(BUFFER_SIZE).decode().strip()

            if not data:
                break

            # Check for "exit" command
            if data.lower() == "exit":
                print(f"Client disconnected: {address}")
                break

            response = ""
            with inventory_lock:
                if data.lower() in inventory:
                    if inventory[data.lower()] > 0:
                        inventory[data.lower()] -= 1
                        response = f"{data} is available. You bought one. {inventory[data.lower()]} left."
                    else:
                        response = f"Sorry, {data} is out of stock."
                else:
                    response = f"The medicine '{data}' is not available in the inventory."

            # Send response back to client
            client_socket.send(response.encode())
    finally:
        client_socket.close()


def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print(f"Server listening on {HOST}:{PORT}...")

    while True:
        client_socket, client_address = server_socket.accept()
        # Handle each client in a new thread
        client_thread = threading.Thread(target=process_request, args=(client_socket, client_address))
        client_thread.daemon = True
        client_thread.start()


if __name__ == "__main__":
    try:
        start_server()
    except KeyboardInterrupt:
        print("Server shutting down.")
