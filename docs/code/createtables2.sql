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