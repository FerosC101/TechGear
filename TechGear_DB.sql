CREATE DATABASE techgear;

USE techgear;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(50) NOT NULL,
    money DECIMAL(10, 2) NOT NULL DEFAULT 0
);

CREATE TABLE inventory (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    category VARCHAR(50) NOT NULL,
    specs TEXT,
    price DECIMAL(10, 2) NOT NULL,
    quantity INT NOT NULL,
    cost_price DECIMAL(10, 2) NOT NULL
);

CREATE TABLE purchase_history (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    item_name VARCHAR(50) NOT NULL,
    cost DECIMAL(10, 2) NOT NULL,
    purchase_date DATETIME NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);