CREATE TABLE patients (
    patient_id SERIAL NOT NULL PRIMARY KEY,
    address_id INT NOT NULL,
    passport INT NOT NULL,
    phone VARCHAR(10) NOT NULL,
    home_phone VARCHAR(10),
    email VARCHAR(20)
);