CREATE TABLE patients (
    patient_id SERIAL NOT NULL PRIMARY KEY,
    address_id INT NOT NULL,
    passport INT NOT NULL,
    phone VARCHAR(10) NOT NULL,
    home_phone VARCHAR(10),
    email VARCHAR(20)
);

CREATE TABLE staff (
    staff_id SERIAL NOT NULL PRIMARY KEY,
    passport INT NOT NULL,
    post INT NOT NULL,
    employment_date DATE NOT NULL,
    dismissal_date DATE
);

CREATE TABLE posts (
    post_id SERIAL NOT NULL PRIMARY KEY,
    title VARCHAR(30) NOT NULL,
    salary INT NOT NULL
);

CREATE TABLE sessions (
    session_id SERIAL NOT NULL PRIMARY KEY,
    doctor_id INT NOT NULL,
    patient_id INT NOT NULL,
    record_id INT NOT NULL,
    state_id INT NOT NULL,
    session_date DATE NOT NULL,
    dynamics VARCHAR(256),
    prescription VARCHAR(256),
    note VARCHAR(256)
);

CREATE TABLE schedules (
    schedule_id SERIAL NOT NULL PRIMARY KEY,
    staff_id INT NOT NULL,
    workstart TIMESTAMP NOT NULL,
    workend TIMESTAMP NOT NULL,
    week_day week_day_t NOT NULL,
    office INT
);

CREATE TABLE records (
    record_id SERIAL NOT NULL PRIMARY KEY,
    agreement_id INT NOT NULL,
    registration_date DATE NOT NULL
);

CREATE TABLE states (
    state_id SERIAL NOT NULL PRIMARY KEY,
    general_condition VARCHAR(256),
    height INT,
    patient_weight INT,
    pulse INT,
    pressure VARCHAR(50),
    temperature REAL,
    other VARCHAR(256)
);

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

CREATE TABLE accesses (
    access_id SERIAL NOT NULL PRIMARY KEY,
    staff_id INT NOT NULL,
    username VARCHAR(50) NOT NULL,
    passwordhash BYTEA NOT NULL,
    access_level access_level_t NOT NULL
);