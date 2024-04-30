package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"net/http"
	"os"
)

var returnCode int
var senderUname string
var receiverUname string
var listenAddr string
var isHandshakeSuccessful bool

type IP struct {
	Query string
}

func getip2() string {
	req, err := http.Get("http://ip-api.com/json/")
	if err != nil {
		return err.Error()
	}
	defer req.Body.Close()

	body, err := io.ReadAll(req.Body)
	if err != nil {
		return err.Error()
	}

	var ip IP
	json.Unmarshal(body, &ip)

	return ip.Query
}

func main() {
	returnCode = 0
	isHandshakeSuccessful = false
	// Listen for incoming connections
	args := os.Args[1:]
	if len(args[1]) == 0 {
		listenAddr = getip2() + ":" + args[0]
	} else {
		listenAddr = args[1] + ":" + args[0]
	}
	fmt.Println("Server IP:", listenAddr)
	listener, err := net.Listen("tcp", listenAddr)
	if err != nil {
		log.Fatal(err)
	}
	defer listener.Close()

	fmt.Println("Server started. Listening on ", listenAddr)

	// Accept new connection
	conn, err := listener.Accept()
	if err != nil {
		log.Fatal(err)
	}

	// Handle the connection in a new goroutine
	for {
		defer conn.Close()
		if isHandshakeSuccessful == false {
			handleHandshake(conn)
		}
		if returnCode != 111 {
			break
		}
		// Receive message from client until a message is sent
		for i := 0; i < -1; i++ {
			message, err := recvMessage(conn)
			if err != nil {
				log.Println(err)
				returnCode = 103
			}
			fmt.Println("Received message from client:", message)

			// Send message to client
			msgToSend := "Message received"
			sendMessage(conn, msgToSend)
			if err != nil {
				log.Println(err)
				returnCode = 104
			}
			fmt.Println("Sent response to client")
			//if message receive is "/exit" break
			if message == "/exit" {
				break
			}
		}
		break
	}
	conn.Close()
}

func handleHandshake(conn net.Conn) {
	// Read handshake from client
	buffer := make([]byte, 5)
	n, err := conn.Read(buffer)
	if err != nil {
		log.Println(err)
		returnCode = 100
	}

	// Process handshake
	handshake := string(buffer[:n])
	if handshake != "CYFTM" {
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

func recvMessage(conn net.Conn) (string, error) {
	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	if err != nil {
		return "", err
	}
	message := string(buffer[:n])
	return message, nil
}

func storeMessage() {
	// Store message in database
	// ...
}

func sendMessage(conn net.Conn, message string) {
	// Send message to client
	_, err := conn.Write([]byte(message))
	if err != nil {
		log.Println("Error sending message:", err)
		return
	}

	log.Println("Message sent successfully")
}
