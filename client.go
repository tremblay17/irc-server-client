package main

import (
	"fmt"
	"net"
)

func main() {
	// Connect to the server
	conn, err := net.Dial("tcp", "localhost:8080")
	if err != nil {
		fmt.Println("Failed to connect to server:", err)
		return
	}
	defer conn.Close()

	// Send a handshake message to the server
	_, err = conn.Write([]byte("Hello, server!"))
	if err != nil {
		fmt.Println("Failed to send handshake message:", err)
		return
	}

	// Read the response from the server
	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	if err != nil {
		fmt.Println("Failed to read server response:", err)
		return
	}

	// Print the server's response
	fmt.Println("Server response:", string(buffer[:n]))

	// Perform additional actions after handshake
	// ...

	// Close the connection
	conn.Close()
	return
}
