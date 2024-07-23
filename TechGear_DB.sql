CREATE DATABASE techgear;

USE techgear;

CREATE TABLE inventory (
    id VARCHAR (50) NOT NULL,
    category VARCHAR(50) NOT NULL,
    item_name VARCHAR(50) NOT NULL,
    specs TEXT,
    price DECIMAL(10, 2) NOT NULL,
    quantity INT NOT NULL,
    cost_price DECIMAL(10, 2) NOT NULL
);

INSERT INTO inventory (id, category, item_name, specs, price, quantity, cost_price)
VALUES
("L1", ("Laptop1", "Laptops"), ("Brand", "Dell"), (("RAM", "16GB"), ("Storage", "512GB SSD")), 999.99, 10, 800.0),
("L2", ("Laptop2", "Laptops"), ("Brand", "Apple"), (("RAM", "16GB"), ("Storage", "256GB SSD")), 1299.99, 5, 1000.0),
("L3", ("Laptop3", "Laptops"), ("Brand", "HP"), (("RAM", "8GB"), ("Storage", "1TB HDD")), 699.99, 8, 500.0),
("L4", ("Laptop4", "Laptops"), ("Brand", "Lenovo"), (("RAM", "16GB"), ("Storage", "1TB SSD")), 1199.99, 6, 900.0),
("L5", ("Laptop5", "Laptops"), ("Brand", "Asus"), (("RAM", "32GB"), ("Storage", "2TB SSD")), 1899.99, 3, 1500.0),
("L6", ("Laptop6", "Laptops"), ("Brand", "Acer"), (("RAM", "8GB"), ("Storage", "512GB SSD")), 799.99, 12, 600.0);

CREATE TABLE purchase_history (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    item_name VARCHAR(50) NOT NULL,
    cost DECIMAL(10, 2) NOT NULL,
    purchase_date DATETIME NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);
