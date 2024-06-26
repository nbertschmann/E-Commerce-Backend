INSERT INTO USER_INFORMATION (USER_ID, username, password, email)
VALUES (12465, 'Miguel22', 'killer76', 'miguel22@gmail.com');

INSERT INTO PRODUCT_INFORMATION (PRODUCT_ID, product_name, price, stock_quantity)
VALUES (122, 'Kyles Killer Lemonade', 22.06, 5);

INSERT INTO USER_CART (CART_ID, fk_user_id)
VALUES (1776, 12465);

INSERT INTO CART_ITEMS (ITEM_ID, fk_cart_id, fk_product_id, quantity)
VALUES (1199, 1776, 122, 9);