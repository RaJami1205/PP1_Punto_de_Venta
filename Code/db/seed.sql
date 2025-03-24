-- INSERCION DE DATOS

USE Sales_Points;

INSERT INTO Sales_Point 
    (name, legal_entity_id, phone_number)
VALUES
    ('Mini Super El Exito', '11023456785', '+506 8764 3456');

INSERT INTO Users (username, password)
VALUES ('admin', SHA2('1234', 256));


INSERT INTO Product_Family (name) VALUES 
('Electrodomésticos'),
('Muebles'),
('Tecnología'),
('Juguetes'),
('Ropa y Moda'),
('Hogar y Jardín'),
('Salud y Belleza'),
('Alimentos y Bebidas');

-- Electrodomésticos
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Refrigerador LG 400L', 500.00, 750.00, 15, 1),
('Microondas Samsung 20L', 80.00, 120.00, 30, 1),
('Licuadora Oster 3 Velocidades', 30.00, 50.00, 40, 1);

-- Muebles
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Sofá de Cuero 3 Plazas', 300.00, 500.00, 10, 2),
('Mesa de Comedor 6 Personas', 200.00, 350.00, 12, 2),
('Silla de Oficina Ergonómica', 50.00, 100.00, 25, 2);

-- Tecnología
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Laptop Dell i7 16GB RAM', 800.00, 1200.00, 8, 3),
('Smartphone Samsung Galaxy S23', 600.00, 900.00, 20, 3),
('Smart TV 55" 4K UHD', 450.00, 700.00, 10, 3);

-- Juguetes
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('LEGO Star Wars Millennium Falcon', 100.00, 180.00, 15, 4),
('Muñeca Barbie Edición Especial', 20.00, 40.00, 30, 4),
('Carro de Control Remoto', 35.00, 60.00, 25, 4);

-- Ropa y Moda
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Jeans Levis 501', 25.00, 50.00, 50, 5),
('Chaqueta de Cuero Negra', 60.00, 120.00, 20, 5),
('Zapatillas Nike Air Max', 70.00, 140.00, 30, 5);

-- Hogar y Jardín
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Set de Herramientas 50 Piezas', 40.00, 80.00, 25, 6),
('Manguera de Jardín 15m', 15.00, 30.00, 40, 6),
('Maceta Decorativa Grande', 20.00, 45.00, 35, 6);

-- Salud y Belleza
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Perfume Calvin Klein 100ml', 50.00, 100.00, 20, 7),
('Plancha para el Cabello Remington', 30.00, 60.00, 25, 7),
('Crema Hidratante Facial', 15.00, 35.00, 30, 7);

-- Alimentos y Bebidas
INSERT INTO Product (name, cost, price, stock, product_family_id) VALUES 
('Café Molido 500g', 5.00, 12.00, 100, 8),
('Vino Tinto Reserva 750ml', 20.00, 50.00, 40, 8),
('Chocolate en Barra 200g', 3.00, 8.00, 80, 8);