CREATE TABLE USER_INFORMATION (
    USER_ID        INT     NOT NULL,
    username       TEXT    NOT NULL,
    password       TEXT    NOT NULL,
    email          TEXT,
    PRIMARY KEY (USER_ID)
);

CREATE TABLE PRODUCT_INFORMATION (
    PRODUCT_ID     INT     NOT NULL,
    product_name   TEXT    NOT NULL,
    price          REAL    NOT NULL,
    stock_quantity INT     NOT NULL,
    PRIMARY KEY (PRODUCT_ID)
);

CREATE TABLE USER_CART (
    CART_ID        INT     NOT NULL,
    fk_user_id     INT     NOT NULL UNIQUE,
    PRIMARY KEY (CART_ID),
    FOREIGN KEY (fk_user_id) REFERENCES USER_INFORMATION(USER_ID)
);

CREATE TABLE CART_ITEMS (
    ITEM_ID        INT     NOT NULL,
    fk_cart_id     INT     NOT NULL,
    fk_product_id  INT     NOT NULL,
    quantity       INT     NOT NULL,
    PRIMARY KEY (ITEM_ID),
    FOREIGN KEY (fk_cart_id) REFERENCES USER_CART(CART_ID),
    FOREIGN KEY (fk_product_id) REFERENCES PRODUCT_INFORMATION(PRODUCT_ID)
);
