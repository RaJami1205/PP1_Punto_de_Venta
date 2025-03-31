USE Sales_Points;

DELIMITER $$

-- Procedimiento para obtener todos los productos
CREATE PROCEDURE GetAllProducts()
BEGIN
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

-- Procedimiento para eliminar producto por ID
CREATE PROCEDURE deleteProductById(IN p_product_id VARCHAR(50))
BEGIN
    IF EXISTS (SELECT 1 FROM Product WHERE product_id = p_product_id) THEN
        DELETE FROM Product WHERE product_id = p_product_id;
        SELECT CONCAT('Producto con ID ', p_product_id, ' eliminado exitosamente.') AS mensaje;
    ELSE
        SELECT 'Error: El producto no existe.' AS mensaje;
    END IF;
END $$

-- Procedimiento para buscar productos por valor de búsqueda
CREATE PROCEDURE searchProduct(IN p_search_value VARCHAR(50))
BEGIN
    DECLARE v_count INT;

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

-- Procedimiento para obtener el último ID de cotización
CREATE PROCEDURE getLastQuotId()
BEGIN
    SELECT MAX(quotation_id)
    FROM Quotation p;
END $$

-- Procedimiento para filtrar productos por familia
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

-- Procedimiento para crear una cotización
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

-- Procedimiento para añadir una línea a la cotización
CREATE PROCEDURE addLineToQuotation
(
    IN p_quot_line_id INT,
    IN p_quotation_id INT,
    IN p_product_name VARCHAR(100),
    IN p_qty INT,
    IN p_line_sub_total FLOAT,
    IN p_line_total_taxes FLOAT
)
BEGIN
    DECLARE p_product_id VARCHAR(50);
    
    SELECT product_id INTO p_product_id FROM Product WHERE name = p_product_name LIMIT 1;
    
    IF p_product_id IS NULL THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Error: Producto no encontrado';
    ELSE
        INSERT INTO Quotation_Lines (quotation_line_id, quotation_id, product_id, quantity, line_sub_total, line_total_taxes)
        VALUES (p_quot_line_id, p_quotation_id, p_product_id, p_qty, p_line_sub_total, p_line_total_taxes);
    END IF;
END $$

-- Procedimiento para obtener cotizaciones
CREATE PROCEDURE getQuotations
(
    IN show_all BOOL
)
BEGIN
    IF show_all THEN
        SELECT quotation_id, sub_total, total_taxes, total, state
        FROM Quotation;
    ELSE
        SELECT quotation_id, sub_total, total_taxes, total, state
        FROM Quotation
        WHERE state = 'Pendiente de Pago';
    END IF;
END $$

-- Procedimiento para buscar una cotización por ID
CREATE PROCEDURE searchQuotation
(
    IN p_quota_id INT
)
BEGIN
    SELECT quotation_id, sub_total, total_taxes, total, state
    FROM Quotation
    WHERE p_quota_id = quotation_id;
END $$

-- Procedimiento para buscar líneas de cotización por ID de cotización
CREATE PROCEDURE searchQuotationLines
(
    IN p_quota_id INT
)
BEGIN
    SELECT ql.quotation_line_id, ql.quotation_id, p.name, ql.quantity, ql.line_sub_total, ql.line_total_taxes
    FROM Quotation_Lines ql
    LEFT JOIN Product p ON p.product_id = ql.product_id
    WHERE p_quota_id = quotation_id;
END $$

-- Procedimiento para actualizar una cotización
CREATE PROCEDURE updateQuotation
(
    IN p_quot_id INT,
    IN p_sub_total FLOAT,
    IN p_total_taxes FLOAT
)
BEGIN
    UPDATE Quotation
    SET sub_total = p_sub_total, total_taxes = p_total_taxes
    WHERE quotation_id = p_quot_id;
END $$

-- Procedimiento para actualizar una línea de cotización
CREATE PROCEDURE updateQuotationLine
(
    IN p_quot_line_id INT,
    IN p_quot_id INT,
    IN p_product_name VARCHAR(100),
    IN p_qty INT,
    IN p_line_sub_total FLOAT,
    IN p_line_total_taxes FLOAT
)
BEGIN
    DECLARE p_product_id VARCHAR(50);
    
    SELECT product_id INTO p_product_id FROM Product WHERE name = p_product_name LIMIT 1;
    
    UPDATE Quotation_Lines
    SET 
        product_id = p_product_id, 
        quantity = p_qty, 
        line_sub_total = p_line_sub_total, 
        line_total_taxes = p_line_total_taxes
    WHERE quotation_line_id = p_quot_line_id AND quotation_id = p_quot_id;
END $$

-- Procedimiento para eliminar líneas de cotización a partir de un ID específico
CREATE PROCEDURE deleteQuotationLinesFromId
(
    IN p_quot_id INT,
    IN p_id_from INT
)
BEGIN
    DELETE FROM Quotation_Lines
    WHERE quotation_id = p_quot_id AND quotation_line_id > p_id_from;
END $$

DELIMITER ;
