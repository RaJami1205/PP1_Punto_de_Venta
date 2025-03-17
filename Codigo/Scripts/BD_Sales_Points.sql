DROP DATABASE IF EXISTS Sales_Points;

CREATE DATABASE Sales_Points;

USE Sales_Points;

-- DEFINICION DE TABLAS

CREATE TABLE Sales_Point (
    sales_point_id  INT             NOT NULL AUTO_INCREMENT,
    name            VARCHAR(100)    NOT NULL UNIQUE,
    legal_entity_id VARCHAR(100)    NOT NULL UNIQUE,
    phone_number    VARCHAR(50)     NOT NULL,
    PRIMARY KEY (sales_point_id)
);

CREATE TABLE Users (
    user_id     INT            NOT NULL AUTO_INCREMENT,
    username    VARCHAR(50)    NOT NULL UNIQUE,
    password    VARBINARY(255)   NOT NULL,
    PRIMARY KEY (user_id)
);

CREATE TABLE Product_Family (
    product_family_id   INT NOT NULL AUTO_INCREMENT,
    name                VARCHAR(100) NOT NULL UNIQUE,
    PRIMARY KEY (product_family_id)
);

CREATE TABLE Product (
    product_id          INT NOT NULL  AUTO_INCREMENT,
    name                VARCHAR(100)  NOT NULL UNIQUE,
    cost                DECIMAL(10,2) NOT NULL,
    price               DECIMAL(10,2) NOT NULL,
    stock               INT NOT NULL,
    product_family_id   INT NOT NULL,
    PRIMARY KEY (product_id),
    FOREIGN KEY (product_family_id) REFERENCES Product_Family(product_family_id)
);

CREATE TABLE Quotation (
    quotation_id    INT NOT NULL AUTO_INCREMENT,
    sub_total       DECIMAL(15,2) NOT NULL,
    total_taxes     DECIMAL(15,2) NOT NULL,
    total           DECIMAL(15,2) AS (sub_total + total_taxes) STORED,
    state           VARCHAR(100)  NOT NULL,
    PRIMARY KEY (quotation_id)
);

CREATE TABLE Quotation_Lines (
    quotation_line_id   INT NOT NULL AUTO_INCREMENT,
    quotation_id        INT NOT NULL,
    product_id          INT NOT NULL,
    quantity            INT NOT NULL,
    line_sub_total      DECIMAL(15,2) NOT NULL CHECK (line_sub_total >= 0),
    line_total_taxes    DECIMAL(15,2) NOT NULL CHECK (line_total_taxes >= 0),
    PRIMARY KEY (quotation_line_id),
    FOREIGN KEY (quotation_id)  REFERENCES Quotation(quotation_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (product_id)    REFERENCES Product(product_id)
);

CREATE TABLE Invoice (
    invoice_id              INT  NOT NULL AUTO_INCREMENT,
    quotation_reference_id  INT,
    date                    DATE NOT NULL,
    sub_total               DECIMAL(15,2) NOT NULL ,
    total_taxes             DECIMAL(15,2) NOT NULL,
    total                   DECIMAL(15,2) AS (sub_total + total_taxes) STORED,
    PRIMARY KEY (invoice_id),
    FOREIGN KEY (quotation_reference_id ) REFERENCES Quotation(quotation_id)
        ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE Invoice_Lines (
    invoice_line_id   INT NOT NULL AUTO_INCREMENT,
    invoice_id        INT NOT NULL,
    product_id        INT NOT NULL,
    quantity          INT NOT NULL,
    line_sub_total    DECIMAL(15,2) NOT NULL CHECK (line_sub_total >= 0),
    line_total_taxes  DECIMAL(15,2) NOT NULL CHECK (line_total_taxes >= 0),
    PRIMARY KEY (invoice_line_id),
    FOREIGN KEY (invoice_id) REFERENCES Invoice(invoice_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (product_id) REFERENCES Product(product_id)
);