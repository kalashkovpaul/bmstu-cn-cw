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