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

DELIMITER $$

CREATE PROCEDURE GetProductFamilyID(
    IN p_name VARCHAR(100),
    OUT p_id VARCHAR(50)
)
BEGIN
    SELECT product_family_id INTO p_id
    FROM Product_Family
    WHERE name = p_name;
END $$

DELIMITER ;