CREATE TABLE agreements (
    agreement_id SERIAL NOT NULL PRIMARY KEY,
    patient_id INT NOT NULL,
    code VARCHAR(50) NOT NULL,
    conclusion_date DATE NOT NULL,
    expiration_date DATE NOT NULL,
    renewal_date DATE NOT NULL
);

CREATE TABLE addresses (
    address_id SERIAL NOT NULL PRIMARY KEY,
    country VARCHAR(50) NOT NULL,
    city VARCHAR(50) NOT NULL,
    street VARCHAR(50) NOT NULL,
    house VARCHAR(50) NOT NULL,
    flat VARCHAR(50)
);

CREATE TABLE passports (
    passport_id SERIAL NOT NULL PRIMARY KEY,
    surname VARCHAR(50) NOT NULL,
    middlename VARCHAR(50) NOT NULL,
    lastname VARCHAR(50),
    birth_date DATE NOT NULL,
    gender gender_t NOT NULL,
    series VARCHAR(10) NOT NULL,
    num VARCHAR(10) NOT NULL,
    issue_date DATE NOT NULL,
    issue_location VARCHAR(128) NOT NULL
);