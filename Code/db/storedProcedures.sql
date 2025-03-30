DELIMITER $$

CREATE PROCEDURE GetAllProducts()
BEGIN
    -- Realizamos el SELECT para obtener todos los productos
    SELECT p.product_id,
           p.name,
           p.cost,
           p.price,
           p.stock,
           p.product_family_id,
           pf.name AS family_name   
    FROM Product p
    LEFT JOIN Product_Family pf ON p.product_family_id = pf.product_family_id;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE deleteProductById(IN p_product_id VARCHAR(50))
BEGIN
    -- Verificar si el producto existe antes de eliminarlo
    IF EXISTS (SELECT 1 FROM Product WHERE product_id = p_product_id) THEN
        DELETE FROM Product WHERE product_id = p_product_id;
        SELECT CONCAT('Producto con ID ', p_product_id, ' eliminado exitosamente.') AS mensaje;
    ELSE
        SELECT 'Error: El producto no existe.' AS mensaje;
    END IF;
END $$

DELIMITER ;



------------------------------------------
DELIMITER $$
CREATE PROCEDURE searchProduct(IN p_search_value VARCHAR(50))
BEGIN
    DECLARE v_count INT;

    -- Intentar buscar por ID del producto
    SELECT COUNT(*) INTO v_count 
    FROM Product 
    WHERE product_id = p_search_value;

    IF v_count > 0 THEN
        SELECT 
            p.product_id, 
            p.name,
            pf.name AS family, 
            p.cost, 
            p.price, 
            p.stock
        FROM Product p
        LEFT JOIN Product_Family pf ON p.product_family_id = pf.product_family_id
        WHERE p.product_id = p_search_value;
    ELSE
        -- Si no se encontró por ID, buscar por nombre
        SELECT 
            p.product_id, 
            p.name,
            pf.name AS family, 
            p.cost, 
            p.price, 
            p.stock
        FROM Product p
        LEFT JOIN Product_Family pf ON p.product_family_id = pf.product_family_id
        WHERE p.name = p_search_value;
    END IF;
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE getLastQuotId()
BEGIN
    SELECT MAX(quotation_id)
    FROM Quotation p;
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE filterProductsByFamily(IN p_family VARCHAR(100))
BEGIN
    SELECT p.product_id,
           p.name,
           p.cost,
           p.price,
           p.stock,
           p.product_family_id,
           pf.name AS family_name  
    FROM Product p
    LEFT JOIN Product_Family pf ON p.product_family_id = pf.product_family_id
    WHERE pf.name LIKE CONCAT('%', p_family, '%');
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE createQuotation
(
	IN p_sub_total FLOAT,
    IN p_total_taxes FLOAT
)
BEGIN
	INSERT INTO Quotation 
		(sub_total, total_taxes, state)
    VALUES
		(p_sub_total, p_total_taxes, 'Pendiente de Pago');
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE addLineToQuotation
(
    IN p_quotation_id INT,
    IN p_product_name VARCHAR(100),
    IN p_qty INT,
    IN p_line_sub_total FLOAT,
    IN p_line_total_taxes FLOAT
)
BEGIN
    DECLARE p_product_id VARCHAR(50);
    
    -- Obtener el ID del producto
    SELECT product_id INTO p_product_id FROM Product WHERE name = p_product_name LIMIT 1;
    
    -- Validar si el producto existe
    IF p_product_id IS NULL THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Error: Producto no encontrado';
    ELSE
        -- Insertar la línea en la cotización
        INSERT INTO Quotation_Lines (quotation_id, product_id, quantity, line_sub_total, line_total_taxes)
        VALUES (p_quotation_id, p_product_id, p_qty, p_line_sub_total, p_line_total_taxes);
    END IF;
END $$
DELIMITER ;