-- Vista que devuelve la cantidad de cotizaciones pendientes
CREATE VIEW Pending_Quotations AS
SELECT 
    COUNT(q.quotation_id) AS pending_quotations_quantity
FROM 
    Quotation q
LEFT JOIN 
    Invoice i ON q.quotation_id = i.quotation_reference_id
WHERE 
    i.invoice_id IS NULL;

-- Vista que devuelva la cantidad de cotizaciones facturadas
CREATE VIEW Quotation_With_Invoice AS
SELECT 
    COUNT(q.quotation_id) AS total_quotations_with_invoice
FROM 
    Quotation q
JOIN 
    Invoice i ON q.quotation_id = i.quotation_reference_id;

-- Vista que devuelve el promedio de compras totales
CREATE VIEW Average_Invoice_Total AS
SELECT 
    AVG(i.total) AS average_purchase_total
FROM 
    Invoice i;

-- Vista que devuelve los 5 productos más vendidos
CREATE VIEW Top_5_Selling_Products AS
SELECT 
    p.name AS product_name,
    SUM(il.quantity) AS total_sold
FROM 
    Invoice_Lines il
JOIN 
    Product p ON il.product_id = p.product_id
GROUP BY 
    p.product_id
ORDER BY 
    total_sold DESC
LIMIT 5;

-- Vista que devuelve los productos más vendidos por cada familia de productos
CREATE VIEW Top_Product_Per_Family AS
SELECT 
    pf.name AS product_family_name,
    sp.product_name AS top_product_name,
    sp.total_sales
FROM 
    (SELECT 
        p.product_family_id,
        p.name AS product_name,
        SUM(il.quantity) AS total_sales,
        ROW_NUMBER() OVER (PARTITION BY p.product_family_id ORDER BY SUM(il.quantity) DESC) AS row_num
    FROM 
        Invoice_Lines il
    JOIN 
        Product p ON il.product_id = p.product_id
    GROUP BY 
        p.product_family_id, p.product_id) AS sp
JOIN 
    Product_Family pf ON sp.product_family_id = pf.product_family_id
WHERE 
    sp.row_num = 1;

-- Vista que me devuelve el monto total vendido por familia
CREATE VIEW Total_Sales_Per_Family AS
SELECT 
    pf.name AS product_family_name,
    SUM(il.quantity * p.price) AS total_sales
FROM 
    Invoice_Lines il
JOIN 
    Product p ON il.product_id = p.product_id
JOIN 
    Product_Family pf ON p.product_family_id = pf.product_family_id
GROUP BY 
    pf.product_family_id;
    