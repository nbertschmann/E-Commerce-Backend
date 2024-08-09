INSERT INTO USER_INFORMATION (username, password, email)
VALUES ('Miguel22', 'MiguelIsCool76', 'miguel22@gmail.com');

INSERT INTO PRODUCT_INFORMATION (product_name, price, stock_quantity)
VALUES ('Kyles Killer Lemonade', 22.06, 5);

INSERT INTO USER_INFORMATION (username, password, email)
VALUES ('Bob76', 'bobbyboy', 'bob@hotmail.com');

INSERT INTO PRODUCT_INFORMATION (product_name, price, stock_quantity)
VALUES ('Milk', 5.00, 20);

INSERT INTO USER_CART (fk_user_id)
VALUES ((SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Bob76'));

INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity)
VALUES ((SELECT CART_ID FROM USER_CART WHERE fk_user_id = (SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Bob76')),
        (SELECT PRODUCT_ID FROM PRODUCT_INFORMATION WHERE product_name = 'Milk'), 
        4);

INSERT INTO USER_CART (fk_user_id)
VALUES ((SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Miguel22'));

INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity)
VALUES ((SELECT CART_ID FROM USER_CART WHERE fk_user_id = (SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Miguel22')),
        (SELECT PRODUCT_ID FROM PRODUCT_INFORMATION WHERE product_name = 'Kyles Killer Lemonade'), 
        9);

INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity)
VALUES ((SELECT CART_ID FROM USER_CART WHERE fk_user_id = (SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Bob76')),
        (SELECT PRODUCT_ID FROM PRODUCT_INFORMATION WHERE product_name = 'Kyles Killer Lemonade'), 
        4);