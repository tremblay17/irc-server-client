package main

import (
	"fmt"
	"log"
	"net"
	"errors"
)

var returnCode int
var senderUname string
var receiverUname string

func main() {
	returnCode = 0
	// Listen for incoming connections
	listener, err := net.Listen("tcp", "localhost:8080")
	if err != nil {
		log.Fatal(err)
	}
	defer listener.Close()

	fmt.Println("Server started. Listening on localhost:8080")

	for {
		// Accept new connection
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal(err)
		}

		// Handle the connection in a new goroutine
		go handleHandshake(conn)
	}
}

func banIP() {
	// Ban the IP address
	//		done by adding the IP address to a blacklist table
}

func isFirstConnection() {
	// Check if this is the first connection from IP
	//		done by checking the connection history table
	// if yes, perform key exchange
	// add entry to database
	// if no, skip key exchange
}

func keyExchange() {
	// Generate a public-private key pair
	// The client will generate a public-private key pair
	// The server will receive a public key from the client
	// Receive shared secret from client
	// Server will decrypt the shared secret using its public key
}

func handleHandshake(conn net.Conn) {
	defer conn.Close()

	// Read handshake from client
	buffer := make([]byte, 5)
	n, err := conn.Read(buffer)
	if err != nil {
		log.Println(err)
		returnCode = 100
	}

	// Process handshake
	handshake := string(buffer[:n])
	if(handshake != "CYFTM") {
		err = errors.New("Unrecognized handshake")
		//send response to client
		_, err = conn.Write([]byte("EURH"))
		returnCode = 101
	} else {
		fmt.Println("Received handshake:", handshake)

		// Send response to client
		response := "TMWG"
		_, err = conn.Write([]byte(response))
		if err != nil {
			log.Println(err)
			returnCode = 102
		}

		fmt.Println("Sent response to client:", response)

		//Wait for client to send sender and receiver details
		n, err = conn.Read(buffer)
		if err != nil {
			log.Println(err)
			returnCode = 100
		}

		//use the senderIp to check if this is the first connection from the IP
		senderUname = string(buffer[:2])
		senderIp := conn.RemoteAddr().String()
		//isFirstConnection(senderIp)

		//use the username to check for receiver ip
		receiverUname = string(buffer[3:5])
		receiverIp := conn.RemoteAddr().String()
		//send receiverIp to client


		fmt.Println("Sender:", senderUname, senderIp)
		fmt.Println("Receiver:", receiverUname, receiverIp)
		returnCode = 111
	}

	switch returnCode {
	case 100:
		fmt.Println("Failed to read client handshake")
	case 101:
		fmt.Println("Unrecognized server response")
	case 102:
		fmt.Println("Failed to send response to client")
	default:
		fmt.Println("Handshake successful")
	}
}

func recvMessage() {
	// Receive message from client
	// ...
}

func storeMessage() {
	// Store message in database
	// ...
}

func sendMessage() {
	// Send message to client from database
	// ...
}