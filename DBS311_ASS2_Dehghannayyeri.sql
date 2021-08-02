--***********************
-- Name: Sanaz Dehghannayyeri

--***********************

--This procedure has an input parameter to receive the customer ID and an output parameter named found.
--This procedure looks for the given customer ID in the database. If the customer exists, it sets the variable found to 1. Otherwise, the found variable is set to 0.
--To check if your query in the find_customer() procedure returns a row, you can check the no_data_found exception in the EXCEPTION block.
CREATE OR REPLACE PROCEDURE find_customer (
    in_customer_id  IN   NUMBER,
    found           OUT  NUMBER
) AS
    count_rows NUMBER := 0;
BEGIN
    BEGIN
        SELECT
            COUNT(*)
        INTO count_rows
        FROM
            customers
        WHERE
            customer_id = in_customer_id;

        dbms_output.put_line('count_rows :' || count_rows);
        IF ( count_rows <> 0 ) THEN
            found := 1;
        ELSE
            found := 0;
        END IF;

    EXCEPTION
        WHEN no_data_found THEN
            found := 0;
    END;
END find_customer;
/

SELECT
    *
FROM
    customers
WHERE
    customer_id = 0;
/

DECLARE
    out_temp NUMBER;
BEGIN
    find_customer(0, out_temp);
    dbms_output.put_line('result :' || out_temp);
END;
/


--This procedure has an input parameter to receive the product ID and an output parameter named price.
--This procedure looks for the given product ID in the database. If the product exists, it stores the product’s list_price in the variable price. Otherwise, the price variable is set to 0.
CREATE OR REPLACE PROCEDURE find_product (
    in_product_id  IN   NUMBER,
    price          OUT  products.list_price%TYPE
) AS
    temp_price products.list_price%TYPE;
BEGIN
    BEGIN
        SELECT
            list_price
        INTO temp_price
        FROM
            products
        WHERE
            product_id = in_product_id;

        IF ( temp_price = 0 ) THEN
            price := 0;
        ELSE
            price := temp_price;
        END IF;

    EXCEPTION
        WHEN no_data_found THEN
            price := 0;
    END;
END find_product;
/

SELECT
    *
FROM
    products
WHERE
    product_id = 0;
/

DECLARE
    out_temp NUMBER;
BEGIN
    find_product(0, out_temp);
    dbms_output.put_line('result :' || out_temp);
END;
/


--This procedure has an input parameter to receive the customer ID and an output parameter named new_order_id.
--To add a new order for the given customer ID, you need to generate the new order Id. 
--To calculate the new order Id, find the maximum order ID in the orders table and increase it by 1.
CREATE OR REPLACE PROCEDURE add_order (
    in_customer_id  IN   NUMBER,
    new_order_id    OUT  NUMBER
) AS
    temp_order_id NUMBER;
BEGIN
    SELECT
        MAX(order_id) + 1
    INTO temp_order_id
    FROM
        orders;

    INSERT INTO orders (
        order_id,
        customer_id,
        status,
        salesman_id,
        order_date
    ) VALUES (
        temp_order_id,
        in_customer_id,
        'Shipped',
        56,
        sysdate
    );

    new_order_id := temp_order_id;
END add_order;
/

DECLARE
    out_temp NUMBER;
BEGIN
    add_order(1, out_temp);
    dbms_output.put_line('result :' || out_temp);
END;
/


--This procedure has five IN parameters. It stores the values of these parameters to 
--the table order_items. 
CREATE OR REPLACE PROCEDURE add_order_item (
    orderid    IN  order_items.order_id%TYPE,
    itemid     IN  order_items.item_id%TYPE,
    productid  IN  order_items.product_id%TYPE,
    in_quantity   IN  order_items.quantity%TYPE,
    price      IN  order_items.unit_price%TYPE
) AS

BEGIN
    INSERT INTO order_items (
        order_id,
        item_id,
        product_id,
        quantity,
        unit_price
    ) VALUES (
        orderid,
        itemid,
        productid,
        in_quantity,
       price
    );
END add_order_item;
/

   EXECUTE add_order_item(9,9,238,50,23);
   /
   select * from order_items where  order_id = 9 and item_id = 9 and product_id = 238;
    
