# Cafeteria System C++
A console-based C++ project that simulates a cafeteria management system 
with separate Admin and Customer interfaces.

## Features

### Admin Panel (password protected)
- Add, delete, and display menu items
- Update item price and stock
- View all orders and total sales
- Cancel entire orders or specific items from an order

### Customer Panel
- View available menu items
- Place orders and receive a printed bill with order number

## Data Persistence
- All inventory and order data is saved to files (items.txt, orders.txt)
- Data is automatically loaded on startup

## How to Run
Compile using g++:
g++ main.cpp -o cafeteria
./cafeteria

## Built With
- C++ (Standard Library)
- File handling using fstream
- Structs and arrays for data management
